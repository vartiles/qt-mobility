TEMPLATE = app
CONFIG+=testcase
TARGET=tst_qgeoareamonitor

include (../../../common.pri)

INCLUDEPATH += ../../../src/location

# Input 
HEADERS += ../qgeocoordinate/qlocationtestutils_p.h
SOURCES += tst_qgeoareamonitor.cpp \
           ../qgeocoordinate/qlocationtestutils.cpp

CONFIG += mobility
MOBILITY = location

symbian {
        TARGET.CAPABILITY = ALL -TCB
}

