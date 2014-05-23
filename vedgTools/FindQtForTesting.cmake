# Brief: handles finding Qt for tests.
# Should be used for project that does not itself depend on Qt.
option(ENABLE_TESTING
        "Build library tests. Requires Qt4 or Qt5 (Core and Test modules)." OFF)
if(ENABLE_TESTING)
    include(vedgTools/ExecutableQtStart)
    include(vedgTools/LibraryFindQt)
    libraryFindQt(Qt5Core Qt5Test . QTCORE QTTEST)
endif()
