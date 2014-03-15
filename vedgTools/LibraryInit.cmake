option(${CAP_Target_Name}_AS_SHARED_LIBRARY
            "Build ${Target_Name} as a shared library." OFF)

message("${CAP_Target_Name}_AS_SHARED_LIBRARY = "
            ${${CAP_Target_Name}_AS_SHARED_LIBRARY})

include(vedgTools/LibraryHeadersOnlyInit)
