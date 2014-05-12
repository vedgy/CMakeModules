# Brief: links ${targetName} to Qt5 or Qt4 depending on ${USE_QT5}.
# Arguments after targetName: required Qt5 modules, then period, then
# required Qt4 components.
# For example: linkQt(myTarget Core Xml .
#                     ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
# If ${USE_QT5} is true and there are no Qt5 arguments OR
# if ${USE_QT5} is false and there are no Qt4 arguments,
# function call has no effect.
function(linkQt targetName)
    set(L_Q_4 FALSE)
    unset(L_Q_MODULES5)
    unset(L_Q_COMPONENTS4)
    foreach(L_Q_ARG ${ARGN})
        if(L_Q_4)
            set(L_Q_COMPONENTS4 ${L_Q_COMPONENTS4} ${L_Q_ARG})
        else()
            if(L_Q_ARG STREQUAL .)
                set(L_Q_4 TRUE)
            else()
                set(L_Q_MODULES5 ${L_Q_MODULES5} ${L_Q_ARG})
            endif()
        endif()
    endforeach()

    if(USE_QT5)
        if(DEFINED L_Q_MODULES5)
            qt5_use_modules(${targetName} ${L_Q_MODULES5})
        endif()
    else()
        if(DEFINED L_Q_COMPONENTS4)
            target_link_libraries(${targetName} ${L_Q_COMPONENTS4})
        endif()
    endif()
endfunction()
