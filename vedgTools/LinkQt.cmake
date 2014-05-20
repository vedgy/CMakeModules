# Brief: links ${targetName} to Qt5 or Qt4 depending on ${USE_QT5}.
# Arguments after targetName: required Qt5 modules, then period, then
# required Qt4 components.
# For example: linkQt(myTarget Core Xml .
#                     ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
# If ${USE_QT5} is true and there are no Qt5 arguments OR
# if ${USE_QT5} is false and there are no Qt4 arguments,
# function call has no effect.
function(linkQt targetName)
    include(vedgTools/ParseQtModules)
    parseQtModules(${ARGN})
    include(vedgTools/LinkQtParsed)
    linkQtParsed(${targetName})
endfunction()
