# Brief: Checks if ${stringName} is defined, not empty, and ${${stringName}}
# does not end with '/'. If yes, appends '/' to the end of ${${stringName}} and
# sets ${stringName} to updated ${${stringName}} in PARENT_SCOPE. Otherwise has
# no effect.
function(stringAppendSlashIfAbsent stringName)
    if((DEFINED ${stringName}) AND (NOT (${stringName} STREQUAL "")))
        string(LENGTH ${${stringName}} SASIA_STR_LEN)
        math(EXPR SASIA_STR_LEN "${SASIA_STR_LEN}-1")
        string(SUBSTRING ${${stringName}} ${SASIA_STR_LEN} 1 SASIA_LAST_SYMBOL)
        if(NOT (SASIA_LAST_SYMBOL STREQUAL "/"))
            set(${stringName} "${${stringName}}/" PARENT_SCOPE)
        endif()
    endif()
endfunction()
