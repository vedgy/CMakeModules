cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../..")
include(vedgTools/CMakeLicense)

set(TCLAP_INCLUDE_PATH "" CACHE PATH
    "Path to tclap/include directory or empty string if tclap is in system include directory.")
option(DEBUG_INCLUDE_EXPANDER
        "Print details of internal workflow to stdout." OFF)


project(IncludeExpander)
include(vedgTools/SetCxxFlags)


set(Executable_Name include_expander)

if(NOT (TCLAP_INCLUDE_PATH STREQUAL ""))
    message("TCLAP_INCLUDE_PATH = " ${TCLAP_INCLUDE_PATH})
    include_directories(${TCLAP_INCLUDE_PATH})
else()
    message("TCLAP_INCLUDE_PATH was not specified."
            " Assume that tclap is in system include directory.")
endif()

if(DEBUG_INCLUDE_EXPANDER)
    add_definitions(-DDEBUG_INCLUDE_EXPANDER)
endif()

add_definitions(-DEXECUTABLE_NAME="${Executable_Name}")


set(Sources_Path src)
set(Sources
    ${Sources_Path}/PatternUtilities.cpp ${Sources_Path}/IncludeExpander.cpp
    ${Sources_Path}/main.cpp
)

add_executable(${Executable_Name} ${Sources})
