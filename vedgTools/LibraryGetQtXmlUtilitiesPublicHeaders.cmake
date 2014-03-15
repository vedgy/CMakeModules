include(vedgTools/LibraryGetPublicHeadersProperty)
if(NOT DEFINED PATH_TO_QT_XML_UTILITIES)
    set(PATH_TO_QT_XML_UTILITIES QtXmlUtilities)
endif()
libraryGetPublicHeadersProperty(QtXmlUtilities ${PATH_TO_QT_XML_UTILITIES}
                                    QT_XML_UTILITIES_WAS_SET)
