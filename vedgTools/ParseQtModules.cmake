# Brief: parses Qt5 modules and Qt4 components;
# returns them in PQM_MODULES_5 and PQM_COMPONENTS_4 variables in parent scope.
# Arguments: required Qt5 modules, then period, then required Qt4 components.
# For example: parseQtModules(Core Xml Widgets .
#               ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY})
# OR           parseQtModules(Qt5Core Qt5Xml Qt5Widgets . QTCORE QTXML QTGUI)
function(parseQtModules)
    set(PQM_4 FALSE)
    unset(PQM_MODULES_5)
    unset(PQM_COMPONENTS_4)
    foreach(PQM_ARG ${ARGN})
        if(PQM_4)
            set(PQM_COMPONENTS_4 ${PQM_COMPONENTS_4} ${PQM_ARG})
        else()
            if(PQM_ARG STREQUAL .)
                set(PQM_4 TRUE)
            else()
                set(PQM_MODULES_5 ${PQM_MODULES_5} ${PQM_ARG})
            endif()
        endif()
    endforeach()

    set(PQM_MODULES_5 ${PQM_MODULES_5} PARENT_SCOPE)
    set(PQM_COMPONENTS_4 ${PQM_COMPONENTS_4} PARENT_SCOPE)
endfunction()
