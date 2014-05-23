if(NOT DEFINED CMAKE_CXX_FLAGS_WERE_SET)
    set(CMAKE_CXX_FLAGS_WERE_SET TRUE)

    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers -pedantic")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wcast-qual -Wredundant-decls -Winvalid-pch")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wdisabled-optimization")

    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Wdouble-promotion -Wlogical-op")
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Wvector-operation-performance")

        if(NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
            set(CMAKE_CXX_FLAGS
                "${CMAKE_CXX_FLAGS} -Wconditionally-supported -Wdate-time")
        endif()
    endif()
endif()
