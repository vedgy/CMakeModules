function(libraryLinkQtCoreUtilities libraryName)
    if(NOT QT_CORE_UTILITIES_WAS_SET)
        target_link_libraries(${libraryName} QtCoreUtilities)
    endif()
endfunction()