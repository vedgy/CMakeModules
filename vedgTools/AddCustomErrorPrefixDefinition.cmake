function(addCustomErrorPrefixDefinition targetName capTargetName parentScope)
    set(Target_Error_Prefix \"**\ ${targetName}\ CRITICAL\ **\ \")
    add_definitions(
        -D${capTargetName}_ERROR_PREFIX=${Target_Error_Prefix}
    )
    if (${parentScope})
        set(Target_Error_Prefix ${Target_Error_Prefix} PARENT_SCOPE)
    endif()
endfunction()
