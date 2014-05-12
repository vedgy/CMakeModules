# Brief: if ${propertyName} property is not set or is equal to empty string,
# sets it to ${defaultValue}.
# Sets ${propertyName} variable to (${propertyName} property value)
# in PARENT_SCOPE.
function(getGlobalProperty propertyName defaultValue)
    get_property(${propertyName} GLOBAL PROPERTY ${propertyName})
    if((NOT DEFINED ${propertyName}) OR (${propertyName} STREQUAL ""))
        set(${propertyName} ${defaultValue})
        set_property(GLOBAL PROPERTY ${propertyName} ${${propertyName}})
    endif()
    set(${propertyName} ${${propertyName}} PARENT_SCOPE)
endfunction()
