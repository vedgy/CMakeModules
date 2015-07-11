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

# ifndef INCLUDE_EXPANDER_HPP
# define INCLUDE_EXPANDER_HPP

# include <CommonUtilities/FunctionConstant.hpp>
# include <CommonUtilities/CopyAndMoveSemantics.hpp>

# include <string>
# include <memory>


class IncludeExpander
{
public:
# define INCLUDE_EXPANDER_string_constant(name, value) \
    CLASS_FUNCTION_CONSTANT(std::string, name, value)

    INCLUDE_EXPANDER_string_constant(libraryCollection, "vedgTools")
    INCLUDE_EXPANDER_string_constant(libraryPrefix, libraryCollection() + '/')
    INCLUDE_EXPANDER_string_constant(thisLibrary, "CMakeModules")
    INCLUDE_EXPANDER_string_constant(startCommand, "include")
    INCLUDE_EXPANDER_string_constant(startSeparator, "(")
    INCLUDE_EXPANDER_string_constant(endSeparator, ")")
# undef INCLUDE_EXPANDER_string_constant

    explicit IncludeExpander();
    NON_COPYABLE_BUT_MOVABLE(IncludeExpander)
    ~IncludeExpander() noexcept;

    /// @return Exit code suitable to return from main().
    int operator()(const std::string & inputFile,
                   const std::string & outputFile,
                   const std::string & modulesDir);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

# endif // INCLUDE_EXPANDER_HPP
