# Brief: chooses Qt5 or Qt4 and finds specified modules.
# Arguments: required Qt5 modules, then period, then required Qt4 components.
# For example: executableFindQt(Qt5Core Qt5Widgets . QTCORE QTGUI)
macro(executableFindQt)
    if(FORCE_QT4)
        set(USE_QT5 FALSE)
    else()
        set(USE_QT5 TRUE)
        foreach(E_F_Q_MODULE ${ARGV})
            if(E_F_Q_MODULE STREQUAL .) # Qt4 modules follow after period.
                break()
            endif()

            find_package(${E_F_Q_MODULE})
            if(NOT ${E_F_Q_MODULE}_FOUND)
                set(USE_QT5 FALSE)
            endif()
        endforeach()
    endif()
    set_property(GLOBAL PROPERTY USE_QT5 ${USE_QT5})

    if(USE_QT5)
        message("Using Qt5.")
    else()
        set(E_F_Q_5_PASSED FALSE)
        unset(E_F_Q_4)
        foreach(E_F_Q_MODULE ${ARGV})
            if(E_F_Q_5_PASSED)
                # Append (Qt4 component) to ${E_F_Q_4}.
                set(E_F_Q_4 ${E_F_Q_4} ${E_F_Q_MODULE})
            else()
                if(E_F_Q_MODULE STREQUAL .) # Qt4 modules follow
                    set(E_F_Q_5_PASSED TRUE)   # after period.
                endif()
            endif()
        endforeach()
        unset(E_F_Q_5_PASSED)

        find_package(Qt4 COMPONENTS ${E_F_Q_4} REQUIRED)
        unset(E_F_Q_4)
        include_directories(${QT_INCLUDES})
        message("Using Qt4.")
    endif()

    add_definitions(-DQT_USE_QSTRINGBUILDER)
endmacro()
