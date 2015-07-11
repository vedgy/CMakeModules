/*
 This file is part of vedgTools/CommonUtilities.
 Copyright (C) 2015 Igor Kushnir <igorkuo AT Google mail>

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

# ifndef COMMON_UTILITIES_FUNCTION_CONSTANT_HPP
# define COMMON_UTILITIES_FUNCTION_CONSTANT_HPP


# define PRIVATE_FC_KEYWORDED_FUNCTION_CONSTANT(keyword, Type, name, value) \
keyword const Type & name()                                                 \
{                                                                           \
    static const Type variable = value;                                     \
    return variable;                                                        \
}


# define NAMESPACE_FUNCTION_CONSTANT(Type, name, value) \
    PRIVATE_FC_KEYWORDED_FUNCTION_CONSTANT(inline, Type, name, value)

# define CLASS_FUNCTION_CONSTANT(Type, name, value) \
    PRIVATE_FC_KEYWORDED_FUNCTION_CONSTANT(static, Type, name, value)

# endif // COMMON_UTILITIES_FUNCTION_CONSTANT_HPP
