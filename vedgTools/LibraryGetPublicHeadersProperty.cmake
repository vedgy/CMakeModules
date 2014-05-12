# Returns: public headers of specified library in
# ${libraryName}_PublicHeaders_Path. If there is a third argument, stores in it
# true if ${libraryName}_PublicHeaders_Path was preset and false otherwise.
function(libraryGetPublicHeadersProperty libraryName pathToLibrary)
    set(publicHeadersName ${libraryName}_PublicHeaders_Path)
    get_property(GET_P_H_P_WAS_SET GLOBAL PROPERTY ${publicHeadersName} SET)

    if(NOT GET_P_H_P_WAS_SET)
        add_subdirectory(${pathToLibrary})
    endif()
    get_property(${publicHeadersName} GLOBAL PROPERTY ${publicHeadersName})

    if(GET_P_H_P_WAS_SET)
        message("${publicHeadersName} = " ${${publicHeadersName}})
    endif()
    set(${publicHeadersName} ${${publicHeadersName}} PARENT_SCOPE)

    if(${ARGC} GREATER 2)
        set(${ARGV2} ${GET_P_H_P_WAS_SET} PARENT_SCOPE)
    endif()
endfunction()
