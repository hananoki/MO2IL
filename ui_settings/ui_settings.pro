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

INCLUDEPATH += src ../Qtlib/src ../Qtlib/src/ui ../Qtlib/include

RESOURCES += \
  ../Main/src/res.qrc

SOURCES += \
    src/ComboBoxFileHandler.cpp \
    src/UISettings.cpp \
    src/UISettingsFO4.cpp \
    src/UISettingsFONV.cpp \
    src/UISettingsFileHandler.cpp \
    src/UISettingsSSE.cpp \
    src/UISettingsTool.cpp

HEADERS += \
    src/ComboBoxFileHandler.h \
    src/UISettings.h \
    src/UISettingsFO4.h \
    src/UISettingsFONV.h \
    src/UISettingsFileHandler.h \
    src/UISettingsSSE.h \
    src/UISettingsTool.h

FORMS += \
    src/UISettings.ui \
    src/UISettingsFO4.ui \
    src/UISettingsFONV.ui \
    src/UISettingsFileHandler.ui \
    src/UISettingsSSE.ui \
    src/UISettingsTool.ui

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
