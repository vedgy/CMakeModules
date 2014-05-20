# Brief: links ${targetName} to Qt5 or Qt4 depending on ${USE_QT5}.
# Necessary Qt modules must be present in
# ${PQM_MODULES_5} and ${PQM_COMPONENTS_4} variables.
function(linkQtParsed targetName)
    if(USE_QT5)
        if(DEFINED PQM_MODULES_5)
            qt5_use_modules(${targetName} ${PQM_MODULES_5})
        endif()
    else()
        if(DEFINED PQM_COMPONENTS_4)
            target_link_libraries(${targetName} ${PQM_COMPONENTS_4})
        endif()
    endif()
endfunction()
