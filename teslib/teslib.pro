QT       += core gui concurrent xml
win32:QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src ../zlib/zlib-1.2.12  ../Qtlib/src ../Qtlib/src/ui ../Qtlib/include

SOURCES += \
    src/TESField.cpp \
    src/TESFieldReader.cpp \
    src/TESFormAllocator.cpp \
    src/TESGroup.cpp \
    src/TESGroupNode.cpp \
    src/TESStringTable.cpp \
    src/TES.cpp \
    src/TESFile.cpp \
    src/TESForm.cpp \
    src/TESRecord.cpp \
    src/TESRecordTES4.cpp \
    src/TESStrings.cpp \
    src/TESSystem.cpp \
    src/TESType.cpp \
    src/VFileSys.cpp

HEADERS += \
    src/TESField.h \
    src/TESFieldReader.h \
    src/TESFormAllocator.h \
    src/TESGroup.h \
    src/TESGroupNode.h \
    src/TESStringTable.h \
    src/TESRecordTES4.h \
    src/VFileSys.h \
    src/generate_signature.h \
    src/TESSignature.h \
    src/TES.h \
    src/TESFile.h \
    src/TESForm.h \
    src/TESRecord.h \
    src/TESStrings.h \
    src/TESSystem.h \
    src/TESType.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
