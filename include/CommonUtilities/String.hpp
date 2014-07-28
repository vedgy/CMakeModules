/*
 This file is part of vedgTools/CommonUtilities.
 Copyright (C) 2014 Igor Kushnir <igorkuo AT Google mail>

 vedgTools/CommonUtilities is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 vedgTools/CommonUtilities is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 vedgTools/CommonUtilities.  If not, see <http://www.gnu.org/licenses/>.
*/

# ifndef COMMON_UTILITIES_STRING_HPP
# define COMMON_UTILITIES_STRING_HPP

# include <cstddef>
# include <cassert>
# include <cctype>
# include <utility>
# include <algorithm>
# include <string>


namespace CommonUtilities
{
/// safeCtype utilities prevent undefined behaviour of functions from <cctype>
/// header (the behavior of those functions is undefined if the value of
/// argument is not representable as unsigned char and is not equal to EOF).

constexpr int safeCtypeCast(unsigned char c) noexcept { return c; }

template<int (& F)(int)>
constexpr int safeCtype(unsigned char c) { return F(c); }

/// @brief Equivalent to safeCtype function but may improve performance if
/// passed to algorithm, because functor can be inlined more easily.
template<int (& F)(int)>
struct SafeCtype {
    constexpr int operator()(unsigned char c) const { return F(c); }
};


namespace String
{
/// @brief Removes characters for which discarder returns true
/// from the end of str.
template <typename F>
inline void trimRight(std::string & str, F discarder)
{
    str.erase(
        std::find_if_not(str.rbegin(), str.rend(), std::move(discarder)).base(),
        str.end());
}
/// @brief Removes whitespaces from the end of str.
inline void trimRight(std::string & str)
{
    trimRight(str, SafeCtype<std::isspace>());
}

/// @brief Removes characters for which discarder returns true
/// from the beginning of str.
template <typename F>
inline void trimLeft(std::string & str, F discarder)
{
    str.erase(str.begin(),
              std::find_if_not(str.begin(), str.end(), std::move(discarder)));
}
/// @brief Removes whitespaces from the beginning of str.
inline void trimLeft(std::string & str)
{
    trimLeft(str, SafeCtype<std::isspace>());
}

/// @brief Removes characters for which discarder returns true
/// from the beginning and the end of str.
template <typename F>
inline void trim(std::string & str, F discarder)
{
    trimRight(str, discarder);
    trimLeft(str, std::move(discarder));
}
/// @brief Removes whitespaces from the beginning and the end of str.
inline void trim(std::string & str)
{
    trim(str, SafeCtype<std::isspace>());
}


template <typename F>
inline void skipIf(const std::string & str, std::size_t & index, F discarder)
{
    index =
        std::find_if_not(str.begin() + index, str.end(), std::move(discarder)) -
        str.begin();
}

inline void skipWs(const std::string & str, std::size_t & index)
{
    skipIf(str, index, SafeCtype<std::isspace>());
}

inline void skipWsExceptEol(const std::string & str, std::size_t & index)
{
    skipIf(str, index, [](char c) {
        return safeCtype<std::isspace>(c) && c != '\n';
    });
}

inline void skipBlank(const std::string & str, std::size_t & index)
{
    skipIf(str, index, SafeCtype<std::isblank>());
}

inline void noSkip(const std::string &, std::size_t &) noexcept
{}


constexpr std::size_t npos() { return std::string::npos; }

/// @brief Searches pattern in str.substr(start, end - start).
/// @return Index of the beginning of pattern in str; npos() if pattern was not
/// found in the range.
inline std::size_t find(const std::string & str, std::size_t start,
                        std::size_t end, const std::string & pattern)
{
    assert(start <= end);
    const auto endIt = str.begin() + end;
    const auto it = std::search(str.begin() + start, endIt,
                                pattern.begin(), pattern.end());
    return it == endIt ? npos() : it - str.begin();
}


/// @return true if (lhs.substr(lhsPos, rhs.size()) == rhs).
/// NOTE: is safe to call if lhs.size() < lhsPos + rhs.size(). Returns false in
/// this case.
inline bool equalSubstr(const std::string & lhs, std::size_t lhsPos,
                        const std::string & rhs)
{
    return lhs.compare(lhsPos, rhs.size(), rhs) == 0;
}

namespace Backward
{
using RevIt = std::string::const_reverse_iterator;

/// @brief Locates the specified by locator entity in the
/// range = str.substr(start, end - start) backwards.
/// @tparam Locator Must take (RevIt begin, RevIt end) and return RevIt (the
/// matching position or end in case of no match).
/// @return Index of the last occurence of entity in str (within range) or
/// npos() if entity was not found in range.
template <typename Locator>
std::size_t locate(const std::string & str, std::size_t start, std::size_t end,
                   Locator locator)
{
    assert(start <= end);
    const RevIt rEnd = str.rend();
    const std::size_t result = rEnd - locator(rEnd - end, rEnd - start);
    return result == start ? npos() : result - 1;
}

/// @brief Searches character c in str.substr(start, end - start) backwards.
/// @return Index of the last character c in str (within range); npos() if c was
/// not found in the range.
inline std::size_t find(const std::string & str, std::size_t start,
                        std::size_t end, char c)
{
    return locate(str, start, end, [&](RevIt b, RevIt e) {
        return std::find(b, e, c);
    });
}

/// @brief Searches character for which p returns true in
/// str.substr(start, end - start) backwards.
/// @return Index of the last matching character in str (within range); npos()
/// if matching character was not found in the range.
template <typename Predicate>
std::size_t findIf(const std::string & str, std::size_t start, std::size_t end,
                   Predicate p)
{
    return locate(str, start, end, [&](RevIt b, RevIt e) {
        return std::find_if(b, e, std::move(p));
    });
}

/// @brief Searches character for which p returns false in
/// str.substr(start, end - start) backwards.
/// @return Index of the last matching character in str (within range); npos()
/// if matching character was not found in the range.
template <typename Predicate>
std::size_t findIfNot(const std::string & str, std::size_t start,
                      std::size_t end, Predicate p)
{
    return locate(str, start, end, [&](RevIt b, RevIt e) {
        return std::find_if_not(b, e, std::move(p));
    });
}

inline std::size_t findNonWs(const std::string & str, std::size_t start,
                             std::size_t end)
{
    return findIfNot(str, start, end, SafeCtype<std::isspace>());
}

inline std::size_t findEolOrNonWs(const std::string & str, std::size_t start,
                                  std::size_t end)
{
    return findIf(str, start, end, [](char c) {
        return c == '\n' || ! safeCtype<std::isspace>(c);
    });
}

}

}

}

# endif // COMMON_UTILITIES_STRING_HPP
