set(Target_Error_Prefix \"**\ ${Target_Name}\ CRITICAL\ **\ \")
add_definitions(
    -D${CAP_Target_Name}_ERROR_PREFIX=${Target_Error_Prefix}
)
