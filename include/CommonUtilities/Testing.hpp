/*
 This file is part of vedgTools/CommonUtilities.
 Copyright (C) 2014, 2015 Igor Kushnir <igorkuo AT Google mail>

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

# ifndef COMMON_UTILITIES_TESTING_HPP
# define COMMON_UTILITIES_TESTING_HPP

# include <cstddef>
# include <array>
# include <string>
# include <iostream>
# include <typeinfo>


namespace CommonUtilities
{
namespace Testing
{
constexpr const char * missingExceptionMessage() noexcept {
    return "Exception must have been thrown!";
}

template <typename T>
void printForType(bool upperCaseStart, const std::string & ending = ": ")
{
    std::cout << (upperCaseStart ? 'F' : 'f');
    std::cout << "or type \"" << typeid(T).name() << '"' << ending;
}

template <typename T>
void compactPrint(const std::string & name, const T & value)
{
    std::cout << name << "=\"" << value << '"';
}

template <typename T>
void printNotLast(const std::string & name, const T & value)
{
    compactPrint(name, value);
    std::cout << ", ";
}

template <typename T>
void print(const std::string & name, const T & value)
{
    compactPrint(name, value);
    std::cout << '.' << std::endl;
}

template <typename T>
void print(
    const std::array<std::string, 0> &, const std::array<T, 0> &) noexcept
{}

template <typename T, std::size_t size>
void print(const std::array<std::string, size> & names,
           const std::array<T, size> & values)
{
    for (std::size_t i = 0; i < size - 1; ++i)
        printNotLast(names[i], values[i]);
    print(names.back(), values.back());
}

template <typename T, typename U>
void print(const std::string & name1, const T & value1,
           const std::string & name2, const U & value2)
{
    printNotLast(name1, value1);
    print(name2, value2);
}

template <typename T, typename U, typename V>
void print(const std::string & name1, const T & value1,
           const std::string & name2, const U & value2,
           const std::string & name3, const V & value3)
{
    printNotLast(name1, value1);
    print(name2, value2, name3, value3);
}

} // END namespace Testing
} // END namespace CommonUtilities

# endif // COMMON_UTILITIES_TESTING_HPP
