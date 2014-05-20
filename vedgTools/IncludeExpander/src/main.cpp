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

# include <tclap/CmdLine.h>

# include <string>
# include <iostream>


int main(int argc, char * argv[])
{
    try {
        TCLAP::CmdLine cmd(
            EXECUTABLE_NAME
            " - expands " + IncludeExpander::startCommand() + '(' +
            IncludeExpander::libraryPrefix() + "...) command in CMake file.",
            ' ', "1");

        const std::string stringTypeDesc = "string";

        TCLAP::ValueArg<std::string> inputArg(
            "i", "input", "CMake file to expand", false, "CMakeLists.txt",
            stringTypeDesc, cmd);

        TCLAP::ValueArg<std::string> outputArg(
            "o", "output", "Expanded CMake file", false,
            "CMakeLists_expanded.txt", stringTypeDesc, cmd);

        TCLAP::ValueArg<std::string> modulesDirArg(
            "m", "modules-dir",
            "Path to " + IncludeExpander::thisLibrary() + '/' +
            IncludeExpander::libraryCollection() + " directory",
            false, "../..", stringTypeDesc, cmd);

        cmd.parse(argc, argv);

        return IncludeExpander()(inputArg.getValue(), outputArg.getValue(),
                                 modulesDirArg.getValue());
    }
    catch (const TCLAP::ArgException & e) {
        std::cerr << "Error: " << e.error()
                  << " for argument " << e.argId() << std::endl;
        return 1;
    }
}
