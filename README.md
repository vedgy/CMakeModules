## Introduction

<b>vedgTools/CMakeModules</b> is a collection of cmake files, which are used by
other vedgTools libraries as well as separate projects.

<b>vedgTools/IncludeExpander</b> is part of this library.
It allows to create expanded version of CMakeLists.txt file that includes
`vedgTools/*.cmake` modules. More specifically, include_expander replaces all
vedgTools includes with corresponding cmake-files' contents.
vedgTools/IncludeExpander depends on free library TCLAP.
Bash shell scripts for downloading TCLAP and building IncludeExpander
are provided.

## License

Copyright (C) 2014 Igor Kushnir <igorkuo AT Google mail>

vedgTools/CMakeModules is licensed under the <b>GNU GPLv3+</b> license,
a copy of which can be found in the `vedgTools/COPYING` file.

vedgTools/CMakeModules is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

vedgTools/CMakeModules is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
vedgTools/CMakeModules.  If not, see <http://www.gnu.org/licenses/>.
