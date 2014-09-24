function(libraryLinkQtCoreUtilities libraryName)
    # MinGW requires linking each time -> " OR WIN32".
    if(NOT QT_CORE_UTILITIES_WAS_SET OR WIN32)
        target_link_libraries(${libraryName} QtCoreUtilities)
    endif()
endfunction()
