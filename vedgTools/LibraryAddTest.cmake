# Brief: adds test "${TargetName}_${targetNameSuffix}" with specified
# ${sources}. Links it to Qt Core, Test modules and to ${libraries}.
# Optional arguments after sources: additional required Qt5 modules,
# then period, then additional required Qt4 components.
# For example: libraryAddTest("StringTest" test/StringTest.cpp ${Target_Name})
# OR libraryAddTest("ShortcutsTest" test/ShortcutsTest.cpp ${Target_Name}
#                                               Xml . ${QT_QTXML_LIBRARY})
# OR libraryAddTest("ReserveTest" test/ReserveTest.cpp "")
function(libraryAddTest targetNameSuffix sources libraries)
    set(LAT_TARGET_NAME "${Target_Name}_${targetNameSuffix}")
    add_executable(${LAT_TARGET_NAME} ${sources})

    include(vedgTools/ParseQtModules)
    parseQtModules(${ARGN})
    set(PQM_MODULES_5 ${PQM_MODULES_5} Core Test)
    set(PQM_COMPONENTS_4 ${PQM_COMPONENTS_4}
                            ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})
    include(vedgTools/LinkQtParsed)
    linkQtParsed(${LAT_TARGET_NAME})

    target_link_libraries(${LAT_TARGET_NAME} ${libraries})
    add_test(${LAT_TARGET_NAME} ${LAT_TARGET_NAME})
endfunction()
