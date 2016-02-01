CONFIG += testcase
TARGET = tst_qsqldatabase
SOURCES  += tst_qsqldatabase.cpp

QT += sql testlib

win32: {
   !wince*: LIBS += -lws2_32
   else: LIBS += -lws2
}
