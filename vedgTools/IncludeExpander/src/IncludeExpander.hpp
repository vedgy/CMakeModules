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

# ifndef INCLUDE_EXPANDER_HPP
# define INCLUDE_EXPANDER_HPP

# include <string>
# include <memory>


class IncludeExpander
{
public:
    static const std::string libraryCollection, libraryPrefix, thisLibrary,
           startCommand, startSeparator, endSeparator;

    explicit IncludeExpander();
    ~IncludeExpander();

    /// @return Exit code suitable to return from main().
    int operator()(const std::string & inputFile,
                   const std::string & outputFile,
                   const std::string & modulesDir);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

# endif // INCLUDE_EXPANDER_HPP
