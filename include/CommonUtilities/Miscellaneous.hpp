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

# ifndef COMMON_UTILITIES_MISCELLANEOUS_HPP
# define COMMON_UTILITIES_MISCELLANEOUS_HPP


namespace CommonUtilities
{
constexpr int safeCtypeCast(unsigned char c) noexcept { return c; }

template<int (& F)(int)>
constexpr int safeCtype(unsigned char c) { return F(c); }
}

# endif // COMMON_UTILITIES_MISCELLANEOUS_HPP
