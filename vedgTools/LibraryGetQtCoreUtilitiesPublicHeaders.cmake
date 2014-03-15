include(vedgTools/LibraryGetPublicHeadersProperty)
if(NOT DEFINED PATH_TO_QT_CORE_UTILITIES)
    set(PATH_TO_QT_CORE_UTILITIES QtCoreUtilities)
endif()
libraryGetPublicHeadersProperty(QtCoreUtilities ${PATH_TO_QT_CORE_UTILITIES}
                                    QT_CORE_UTILITIES_WAS_SET)

if(QT_CORE_UTILITIES_WAS_SET)
    if(NOT QT_WAS_SET)
        message(FATAL_ERROR "USE_QT5 property was not set!")
    endif()
endif()
