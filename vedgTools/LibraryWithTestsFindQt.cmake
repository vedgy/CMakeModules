# Brief: handles finding Qt in library that contains Qt tests.
# Arguments: required Qt5 modules, then period, then required Qt4 components.
# For example: libraryWithTestsFindQt(Qt5Core Qt5Widgets . QTCORE QTGUI)
macro(libraryWithTestsFindQt)
    option(ENABLE_TESTING "Build library tests." OFF)

    set(LWTFQ_Required_Qt_Libraries ${ARGN})
    if(ENABLE_TESTING)
        set(LWTFQ_Required_Qt_Libraries
                Qt5Test ${LWTFQ_Required_Qt_Libraries} QTTEST)
    endif()

    include(vedgTools/LibraryFindQt)
    libraryFindQt(${LWTFQ_Required_Qt_Libraries})
endmacro()
