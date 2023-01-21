QT       += core gui concurrent xml multimedia
win32:QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = ../Main/src/pch.h

INCLUDEPATH += src ../Main/src ../Main/src/ui ../teslib/src ../Qtlib/src ../Qtlib/src/ui ../Qtlib/include

RESOURCES += \
  ../Main/src/res.qrc

SOURCES += \
    src/MO2FileTreeView.cpp \
    src/TreeViewBuild.cpp \
    src/UIMO2View.cpp \
    src/UIMO2ViewBuild.cpp \
    src/UIMO2ViewFOJP.cpp \
    src/UIMO2ViewFile.cpp \
    src/UIMO2ViewMerge.cpp

HEADERS += \
    src/MO2FileTreeView.h \
    src/TreeViewBuild.h \
    src/UIMO2View.h \
    src/UIMO2ViewBuild.h \
    src/UIMO2ViewFOJP.h \
    src/UIMO2ViewFile.h \
    src/UIMO2ViewMerge.h

FORMS += \
    src/UIMO2View.ui \
    src/UIMO2ViewBuild.ui \
    src/UIMO2ViewFOJP.ui \
    src/UIMO2ViewFile.ui \
    src/UIMO2ViewMerge.ui

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
