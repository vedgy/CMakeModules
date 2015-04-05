/*
 This file is part of vedgTools/IncludeExpander.
 Copyright (C) 2014, 2015 Igor Kushnir <igorkuo AT Google mail>

 vedgTools/IncludeExpander is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 vedgTools/IncludeExpander is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 vedgTools/IncludeExpander.  If not, see <http://www.gnu.org/licenses/>.
*/

# include "IncludeExpander.hpp"

# include "PatternUtilities.hpp"

# include <CommonUtilities/String.hpp>
# include <CommonUtilities/Streams.hpp>

# include <cstddef>
# include <utility>
# include <array>
# include <map>
# include <string>
# include <stdexcept>
# include <iostream>
# include <fstream>


namespace
{
class Error : public std::runtime_error
{
public:
    explicit Error() : std::runtime_error(std::string()) {}
    explicit Error(const std::string & sWhat) : std::runtime_error(sWhat) {}
};

/// @throw Error In case of filesystem error.
void checkError(const std::ifstream & ifs, const std::string & filename)
{
    if (! CommonUtilities::isStreamFine(ifs)) {
        std::cerr << "Reading file " << filename << " failed."
                  << std::endl;
        throw Error();
    }
}

/// @throw Error In case of filesystem error.
void checkError(const std::ofstream & ofs, const std::string & filename)
{
    if (! CommonUtilities::isStreamFine(ofs)) {
        std::cerr << "Writing to file " << filename << " failed."
                  << std::endl;
        throw Error();
    }
}


/// @return Contents of the file (filename in dir).
std::string getText(const std::string & dir, const std::string & filename)
{
    const std::string absoluteName = dir + filename;

# ifdef DEBUG_INCLUDE_EXPANDER
    std::cout << "Getting text of " << absoluteName << std::endl;
# endif

    std::ifstream source(absoluteName);
    std::string result = CommonUtilities::getFileContents(source);
    checkError(source, absoluteName);
    return result;
}


using namespace PatternUtilities;

}


class IncludeExpander::Impl
{
public:
    explicit Impl();

    /// @brief Expands source and returns result.
    /// @param modulesDir Directory that contains cmake modules.
    std::string expand(const std::string & source, std::string modulesDir);

private:
    /// @return Comment suitable for placing before included contents of module
    /// moduleName.
    std::string getIncludeOpeningComment(std::string moduleName);
    /// @return Comment suitable for placing after included contents of module
    /// moduleName.
    std::string getIncludeClosingComment(std::string moduleName);

    /// @brief Reads module's text, expands all includes recursively and
    /// returns result.
    std::string getContents(const std::string & moduleName);

    /// @return (<expanded source>, true) if include patterns are present in
    /// source; (std::string(), false) otherwise.
    std::pair<std::string, bool> expandIncludes(const std::string & source);


    Whitespace whitespace_;

    SearchCiStringLine startInclude_ { startCommand() };
    String startSeparator_ { startSeparator(), Str::skipWs };
    String libraryPrefix_ { libraryPrefix(), Str::skipWs };
    Param filename_ { Str::noSkip };
    String endSeparator_ { endSeparator(), Str::skipWs };


    SearchStringLine startBoilerplate_ { "##" };
    std::array<String, 3> directiveBoilerplate_ {{
            String("vedgTools/CMakeModules", Str::skipBlank),
            String("path", Str::skipBlank),
            String("boilerplate", Str::skipBlank)
        }
    };
    SearchSymbol searchEndOfLine_ { '\n' };

    CiString includeCommand_ { startCommand(), Str::skipWs };
    std::array<String, 2> includeBoilerplate_ {{
            String("OPTIONAL", Str::skipWs),
            String("RESULT_VARIABLE", Str::skipWs)
        }
    };
    Param wasIncluded_ { Str::skipWs };

    CiString if_ { "if", Str::skipWs };
    ParamCopy wasIncludedCopy_ { wasIncluded_, Str::skipWs };
    std::array<String, 2> ifVars {{
            String("STREQUAL", Str::skipWs), String("NOTFOUND", Str::skipWs)
        }
    };

    SearchCiStringLine endif_ { "endif" };
    SearchSymbol searchEndSeparator_ { endSeparator().back() };


    const PatternMatcher::PatternSequence includeSequence_;
    /// NOTE: non-const only because of verbose initialization in constructor.
    /// May not be changed after constructor.
    PatternMatcher::PatternSequence boilerplateSequence_;

    /// Holds current modulesDir.
    std::string modulesDir_;

    /// (moduleName, moduleContents)
    std::map<std::string, std::string> modules_;
};


IncludeExpander::Impl::Impl()
    : includeSequence_ {
    & startInclude_, & startSeparator_,
    & libraryPrefix_, & filename_, & endSeparator_
}, boilerplateSequence_(1, & startBoilerplate_)
{
    for (auto & p : directiveBoilerplate_)
        boilerplateSequence_.push_back(& p);
    boilerplateSequence_.push_back(& searchEndOfLine_);

    boilerplateSequence_.push_back(& includeCommand_);
    boilerplateSequence_.push_back(& startSeparator_);
    boilerplateSequence_.push_back(& libraryPrefix_);
    boilerplateSequence_.push_back(& filename_);
    for (auto & p : includeBoilerplate_)
        boilerplateSequence_.push_back(& p);
    boilerplateSequence_.push_back(& wasIncluded_);
    boilerplateSequence_.push_back(& endSeparator_);

    boilerplateSequence_.push_back(& if_);
    boilerplateSequence_.push_back(& startSeparator_);
    boilerplateSequence_.push_back(& wasIncludedCopy_);
    for (auto & p : ifVars)
        boilerplateSequence_.push_back(& p);
    boilerplateSequence_.push_back(& endSeparator_);

    boilerplateSequence_.push_back(& endif_);
    boilerplateSequence_.push_back(& searchEndSeparator_);
}

std::string IncludeExpander::Impl::expand(
    const std::string & source, std::string modulesDir)
{
    modulesDir_ = std::move(modulesDir);

    PatternMatcher boilerplateMatcher(boilerplateSequence_);
    std::size_t index = 0;
    bool matchedDirective, matched;

    while (true) {
        matched = boilerplateMatcher.match(source, index);
        matchedDirective = boilerplateMatcher.currentPatternIndex() >
                           directiveBoilerplate_.size();
        if (matched || matchedDirective ||
                boilerplateMatcher.currentPatternIndex() == 0) {
            break;
        }
        boilerplateMatcher.reset();
    }

    if (matched || matchedDirective) {
        const bool eofFound = boilerplateMatcher.currentPatternIndex() >
                              directiveBoilerplate_.size() + 1;
        const std::size_t posAfterComment =
            eofFound ?
            searchEndOfLine_.getSymbolPosition() + 1 : source.size();
        std::string result = source.substr(0, posAfterComment);
        if (matched) {
            result +=
                "## Boilerplate code that searches CMakeModules in "
                "${CMAKE_MODULE_PATH} and adds it if missing was omitted.\n" +
                getContents(filename_.getParam()) +
                source.substr(index);
        }
        else {
            if (! eofFound)
                result += '\n';
            result +=
                "## WARNING: boilerplate directive was detected, but "
                "following lines don't match prefedined patterns.\n"
                "## Boilerplate substitution was not executed.\n" +
                source.substr(posAfterComment);
        }
        auto expanded = expandIncludes(result);
        if (expanded.second)
            return std::move(expanded.first);
        return result;
    }
    else {
        auto expanded = expandIncludes(source);
        if (expanded.second)
            return std::move(expanded.first);
        return source;
    }
}


std::string IncludeExpander::Impl::getIncludeOpeningComment(
    std::string moduleName)
{
    return "# {!!! " + std::move(moduleName) + '\n';
}

std::string IncludeExpander::Impl::getIncludeClosingComment(
    std::string moduleName)
{
    return "# !!!} " + std::move(moduleName) + '\n';
}

std::string IncludeExpander::Impl::getContents(const std::string & moduleName)
{
# ifdef DEBUG_INCLUDE_EXPANDER
    std::cout << "Getting contents of " << moduleName << std::endl;
# endif
    auto p = modules_.equal_range(moduleName);
    if (p.first == p.second) {
        std::string contents = getText(modulesDir_, moduleName + ".cmake");
        p.first = modules_.
# if GCC_EARLIER_THAN_4_8
                  insert(p.first,
                         std::make_pair(moduleName, std::move(contents)));
# else
                  emplace_hint(p.first, moduleName, std::move(contents));
# endif
        while (true) {
            auto expanded = expandIncludes(p.first->second);
            if (expanded.second)
                p.first->second = std::move(expanded.first);
            else
                break;
        }
    }
    return p.first->second;
}

std::pair<std::string, bool> IncludeExpander::Impl::expandIncludes(
    const std::string & source)
{
    std::string result;
    bool expanded = false;
    PatternMatcher includeMatcher(includeSequence_);

    std::size_t index = 0, prevIndex = 0;
    while (true) {
        if (includeMatcher.match(source, index)) {
            expanded = true;
            const std::size_t lineBeginning = startInclude_.getLineBeginning();
            result += source.substr(prevIndex, lineBeginning - prevIndex);

            std::string indent =
                source.substr(
                    lineBeginning,
                    startInclude_.getPatternBeginning() - lineBeginning);
            // Use indent of include-line + 2 spaces for all expanded lines.
            const std::string biggerIndent = indent + "  ";

            std::string moduleName = filename_.getParam();
            result += indent + getIncludeOpeningComment(moduleName);

            std::size_t indentPos = result.size();
            /// WARNING: be careful with reordering statements because
            /// getContents() can modify startInclude_, filename_.
            result += getContents(moduleName);
            while (true) {
                result.insert(indentPos, biggerIndent);
                indentPos = result.find('\n', indentPos);
                if (indentPos == std::string::npos)
                    break;
                if (++indentPos == result.size())
                    break;
            }

            result += std::move(indent) +
                      getIncludeClosingComment(std::move(moduleName));

            prevIndex = index;
        }
        else if (includeMatcher.currentPatternIndex() == 0)
            break;
        includeMatcher.reset();
    }
    if (expanded)
        result += source.substr(prevIndex);

    return { std::move(result), expanded };
}


IncludeExpander::IncludeExpander() : impl_(new Impl)
{
}

IncludeExpander::~IncludeExpander() noexcept = default;

int IncludeExpander::operator()(const std::string & inputFile,
                                const std::string & outputFile,
                                const std::string & modulesDir)
{
    std::ifstream input(inputFile);
    const std::string source = CommonUtilities::getFileContents(input);
    try {
        checkError(input, inputFile);
    }
    catch (const Error &) {
        return 3;
    }

    std::string result;
    try {
        std::string dir = modulesDir;
        if (! dir.empty() && dir.back() != '/')
            dir += '/';
        result = impl_->expand(source, std::move(dir));
    }
    catch (const Error &) {
        return 4;
    }

    std::ofstream output(outputFile);
    output << result;
    try {
        checkError(output, outputFile);
    }
    catch (const Error &) {
        return 5;
    }

    return 0;
}
