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

# ifndef COMMON_UTILITIES_EXCEPTIONS_TO_STDERR_HPP
# define COMMON_UTILITIES_EXCEPTIONS_TO_STDERR_HPP

# include <exception>
# include <iostream>


namespace CommonUtilities
{
template <typename F>
bool exceptionsToStderr(F f, const char * errorPrefix)
{
    try {
        f();
        return true;
    }
    catch (const std::exception & e) {
        if (errorPrefix != nullptr)
            std::cerr << errorPrefix;
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        if (errorPrefix != nullptr)
            std::cerr << errorPrefix;
        std::cerr << "unknown exception!" << std::endl;
    }
    return false;
}

}

# endif // COMMON_UTILITIES_EXCEPTIONS_TO_STDERR_HPP
