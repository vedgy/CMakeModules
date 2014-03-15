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

# include "PatternUtilities.hpp"

# include <cctype>
# include <stdexcept>


namespace PatternUtilities
{
bool Whitespace::match(const std::string & source, std::size_t & index)
{
    if (index < source.size() && std::isspace(source[index])) {
        ++index;
        return true;
    }
    return false;
}


bool Param::match(const std::string & source, std::size_t & index)
{
    discarder_(source, index);
    std::size_t end = index;
    skipIf(source, end, [](char c) { return !(c == ')' || std::isspace(c)); });
    if (end == index)
        return false;
    param_ = source.substr(index, end - index);
    index = end;
    return true;
}


bool SearchSymbol::match(const std::string & source, std::size_t & index)
{
    index = source.find(symbol_, index);
    if (index == std::string::npos)
        return false;
    symbolPosition_ = index;
    ++index;
    return true;
}


bool SearchLine::match(const std::string & source, std::size_t & index)
{
    while (findStr(source, index), index != std::string::npos) {
        lineBeginning_ = patternBeginning_ = index;
        while (lineBeginning_ != 0) {
            --lineBeginning_;
            if (source[lineBeginning_] == '\n') {
                ++lineBeginning_;
                break;
            }
            if (! std::isspace(source[lineBeginning_])) {
                lineBeginning_ = std::string::npos;
                break;
            }
        }
        if (lineBeginning_ == std::string::npos) {
            index = source.find('\n', index);
            if (index == std::string::npos)
                break;
            ++index;
        }
        else {
            index += size();
            return true;
        }
    }
    return false;
}


SearchCiStringLine::SearchCiStringLine(const std::string & lowerStr)
    : lowerStrWithoutFirstSymbol_(
        lowerStr.empty() ? std::string() : lowerStr.substr(1))
{
    if (lowerStr.empty()) {
        throw std::runtime_error(
            "Don't pass empty string to SearchCiStringLine constructor.");
    }
    const char lower = lowerStr.front();
    const char upper = std::toupper(lower);
    if (lower == upper)
        firstSymbol_.assign(1, lower);
    else
        firstSymbol_.assign( { lower, upper });
}

void SearchCiStringLine::findStr(const std::string & source,
                                 std::size_t & index)
{
    while ((index = source.find_first_of(firstSymbol_, index))
            != std::string::npos) {
        ++index;
        if (CiString(lowerStrWithoutFirstSymbol_).match(source, index)) {
            index -= size();
            break;
        }
        index = source.find('\n', index);
        if (index == std::string::npos)
            break;
        ++index;
    }
}



bool PatternMatcher::match(const std::string & source, std::size_t & index)
{
    while (patternId_ < patternSequence_.size()) {
        if (patternSequence_[patternId_]->match(source, index))
            ++patternId_;
        else
            return false;
    }
    return true;
}

}
