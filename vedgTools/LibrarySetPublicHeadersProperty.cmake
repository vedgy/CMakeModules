# Brief: makes path "include" absolute, assigns it to
# ${libraryName}_PublicHeaders_Path, propagates this variable to PARENT_SCOPE
# and stores it as a GLOBAL PROPERTY.
function(librarySetPublicHeadersProperty libraryName)
    set(publicHeadersName ${libraryName}_PublicHeaders_Path)
    get_filename_component(${publicHeadersName} include ABSOLUTE)
    set_property(GLOBAL PROPERTY ${publicHeadersName}
                    ${${publicHeadersName}})
    set(${publicHeadersName} ${${publicHeadersName}} PARENT_SCOPE)
endfunction()
