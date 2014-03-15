/*
 This file is part of vedgTools/IncludeExpander.
 Copyright (C) 2014 Igor Kushnir <igorkuo AT Google mail>

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

# include <cstddef>
# include <utility>
# include <iostream>
# include <fstream>
# include <array>
# include <string>
# include <map>
# include <sstream>
# include <stdexcept>


namespace
{
std::string readFile(const std::ifstream & fileStream)
{
    std::ostringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

class Error : public std::runtime_error
{
public:
    explicit Error() : std::runtime_error(std::string()) {}
    explicit Error(const std::string & sWhat) : std::runtime_error(sWhat) {}
};

/// @throw Error In case of filesystem error.
void checkError(const std::ifstream & ifs, const std::string & filename)
{
    if (! ifs.is_open() || ifs.bad()) {
        std::cerr << "Reading file " << filename << " failed."
                  << std::endl;
        throw Error();
    }
}

/// @throw Error In case of filesystem error.
void checkError(const std::ofstream & ofs, const std::string & filename)
{
    if (! ofs) {
        std::cerr << "Writing to file " << filename << " failed."
                  << std::endl;
        throw Error();
    }
}


/// @return Contents of the file dir[/]filename.
std::string getText(const std::string & dir, const std::string & filename)
{
    std::string absoluteName = dir;
    if (! absoluteName.empty() && absoluteName.back() != '/')
        absoluteName += '/';
    absoluteName += filename;

# ifdef DEBUG_INCLUDE_EXPANDER
    std::cout << "Getting text of " << absoluteName << std::endl;
# endif

    std::ifstream source(absoluteName);
    const std::string result = readFile(source);
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
    /// @param modulesDir Directory, which contains cmake modules.
    std::string expand(const std::string & source, std::string modulesDir);

private:
    /// @brief Reads module's text, expands all includes recursively and
    /// returns result.
    std::string getContents(const std::string & moduleName);
    /// @return (<expanded source>, true) if include patterns are present in
    /// source; (std::string(), false) otherwise.
    std::pair<std::string, bool> expandIncludes(const std::string & source);

    Whitespace whitespace_;

    SearchCiStringLine startInclude_ { startCommand };
    String startSeparator_ { startSeparator, skipWs };
    String libraryPrefix_ { libraryPrefix, skipWs };
    Param filename_ { noSkip };
    String endSeparator_ { endSeparator, skipWs };


    SearchStringLine startBoilerplate_ { "##" };
    std::array<String, 3> directiveBoilerplate_ {{
            String("vedgTools/CMakeModules", skipBlank),
            String("path", skipBlank), String("boilerplate", skipBlank)
        }
    };
    SearchSymbol searchEndOfLine_ { '\n' };

    CiString includeCommand_ { startCommand, skipWs };
    std::array<String, 2> includeBoilerplate_ {{
            String("OPTIONAL", skipWs), String("RESULT_VARIABLE", skipWs)
        }
    };
    Param wasIncluded_ { skipWs };

    CiString if_ { "if", skipWs };
    ParamCopy wasIncludedCopy_ { wasIncluded_, skipWs };
    std::array<String, 2> ifVars {{
            String("STREQUAL", skipWs), String("NOTFOUND", skipWs)
        }
    };

    SearchCiStringLine endif_ { "endif" };
    SearchSymbol searchEndSeparator_ { endSeparator.back() };


    const PatternMatcher::PatternSequence includeSequence_;
    /// NOTE: non-const only because of verbose initialization in constructor.
    /// Must not be changed after constructor.
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
                "## Boilerplate code, which searches CMakeModules in "
                "${CMAKE_MODULE_PATH} and adds it if missing, was omitted.\n" +
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
        const auto expanded = expandIncludes(result);
        return expanded.second ? expanded.first : result;
    }
    else {
        const auto expanded = expandIncludes(source);
        return expanded.second ? expanded.first : source;
    }
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
            const auto expanded = expandIncludes(p.first->second);
            if (expanded.second)
                p.first->second = expanded.first;
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

            // Use indent of include-line for all expanded lines.
            const std::string indent =
                source.substr(
                    lineBeginning,
                    startInclude_.getPatternBeginning() - lineBeginning);
            std::size_t indentPos = result.size();

            /// WARNING: be careful with reordering statements because
            /// getContents() can modify startInclude_, filename_.
            result += getContents(filename_.getParam());
            if (! indent.empty()) {
                while (true) {
                    result.insert(indentPos, indent);
                    indentPos = result.find('\n', indentPos);
                    if (indentPos == std::string::npos)
                        break;
                    ++indentPos;
                }
            }

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



const std::string
IncludeExpander::libraryCollection =
    "vedgTools",
    IncludeExpander::libraryPrefix =
        libraryCollection + '/',
        IncludeExpander::thisLibrary =
            "CMakeModules",
            IncludeExpander::startCommand =
                "include",
                IncludeExpander::startSeparator =
                    "(",
                    IncludeExpander::endSeparator = ")";


IncludeExpander::IncludeExpander() : impl_(new Impl)
{
}

IncludeExpander::~IncludeExpander() = default;

int IncludeExpander::operator()(const std::string & inputFile,
                                const std::string & outputFile,
                                const std::string & modulesDir)
{
    std::ifstream input(inputFile);
    const std::string source = readFile(input);
    try {
        checkError(input, inputFile);
    }
    catch (Error &) {
        return 3;
    }

    std::string result;
    try {
        result = impl_->expand(source, modulesDir);
    }
    catch (Error &) {
        return 4;
    }

    std::ofstream output(outputFile);
    output << result;
    try {
        checkError(output, outputFile);
    }
    catch (Error &) {
        return 5;
    }

    return 0;
}
