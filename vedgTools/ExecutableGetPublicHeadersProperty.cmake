macro(executableGetPublicHeadersProperty libraryName)
    get_property(${libraryName}_PublicHeaders_Path
                    GLOBAL PROPERTY ${libraryName}_PublicHeaders_Path)
endmacro()
