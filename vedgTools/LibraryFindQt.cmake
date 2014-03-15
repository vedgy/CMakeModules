# Brief: handles finding Qt in library.
# Arguments: required Qt5 modules, then period, then required Qt4 components.
# For example: libraryFindQt(Qt5Core Qt5Widgets . QTCORE QTGUI)
macro(libraryFindQt)
    get_property(QT_WAS_SET GLOBAL PROPERTY USE_QT5 SET)
    if(QT_WAS_SET)
        get_property(USE_QT5 GLOBAL PROPERTY USE_QT5)
        message("USE_QT5 = " ${USE_QT5})
    else()
        include(vedgTools/ExecutableFindQt)
        executableFindQt(${ARGV})
    endif()
endmacro()
