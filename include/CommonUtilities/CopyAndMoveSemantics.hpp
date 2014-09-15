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

# ifndef COMMON_UTILITIES_COPY_AND_MOVE_SEMANTICS_HPP
# define COMMON_UTILITIES_COPY_AND_MOVE_SEMANTICS_HPP


# define PRIVATE_CAMSM_COPYABLE(Class) \
    Class(const Class &) = default;    \
    Class & operator=(const Class &) = default;

# define PRIVATE_CAMSM_NON_COPYABLE(Class) \
    Class(const Class &) = delete;         \
    Class & operator=(const Class &) = delete;

# define PRIVATE_CAMSM_MOVABLE(Class) \
    Class(Class &&) = default;        \
    Class & operator=(Class &&) = default;

# define PRIVATE_CAMSM_NON_MOVABLE(Class) \
    Class(Class &&) = delete;             \
    Class & operator=(Class &&) = delete;


/// Defaults copy/move constructors/assignment operators in Class.
# define COPYABLE_AND_MOVABLE(Class) \
    PRIVATE_CAMSM_COPYABLE(Class)    \
    PRIVATE_CAMSM_MOVABLE(Class)

/// Deletes copy constructor/assignment operator in Class;
/// defaults move constructor/assignment operator in Class.
# define NON_COPYABLE_BUT_MOVABLE(Class) \
    PRIVATE_CAMSM_NON_COPYABLE(Class)    \
    PRIVATE_CAMSM_MOVABLE(Class)

/// Deletes copy/move constructors/assignment operators in Class.
# define NEITHER_COPYABLE_NOR_MOVABLE(Class) \
    PRIVATE_CAMSM_NON_COPYABLE(Class)        \
    PRIVATE_CAMSM_NON_MOVABLE(Class)

# endif // COMMON_UTILITIES_COPY_AND_MOVE_SEMANTICS_HPP
