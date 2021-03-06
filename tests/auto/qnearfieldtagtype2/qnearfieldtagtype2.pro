SOURCES += tst_qnearfieldtagtype2.cpp
TARGET = tst_qnearfieldtagtype2
CONFIG += testcase

QT = core

INCLUDEPATH += ../../../src/connectivity/nfc
DEPENDPATH += ../../../src/connectivity/nfc

HEADERS += \
    qnearfieldmanagervirtualbase_p.h \
    qnearfieldtarget_emulator_p.h \
    qnearfieldmanager_emulator_p.h \
    targetemulator_p.h

SOURCES += \
    qnearfieldmanagervirtualbase.cpp \
    qnearfieldtarget_emulator.cpp \
    qnearfieldmanager_emulator.cpp \
    targetemulator.cpp


include(../../../common.pri)

CONFIG += mobility
MOBILITY = connectivity

maemo* {
    testfiles.sources = *.nfc
    testfiles.path = nfcdata
    DEPLOYMENT += testfiles
    DEFINES += SRCDIR=\\\"$$MAEMO_TEST_INSTALL_ROOT/nfcdata\\\"
} else {
    DEFINES += SRCDIR=\\\"$$PWD\\\"
}

maemo*:CONFIG += insignificant_test
