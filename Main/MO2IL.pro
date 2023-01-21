QT       += core gui concurrent xml multimedia
win32:QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = src/pch.h

INCLUDEPATH += src src/ui ../Qtlib/src ../Qtlib/src/ui ../Qtlib/include ../zlib/zlib-1.2.12 ../teslib/src ../ui_settings/src ../ui_mo2/src

CONFIG( debug, debug|release ) {
    # debug
    win32:LIBS += -L../Qtlib/debug -L../Qtlib/QtLibSound/debug -L../zlib/debug -L../teslib/debug -L../ui_settings/debug -L../ui_mo2/debug -L../libs/Debug
} else {
    # release
    win32:LIBS += -L../Qtlib/release -L../Qtlib/QtLibSound/release -L../zlib/release -L../teslib/release -L../ui_settings/release -L../ui_mo2/release -L../libs/Release
}

LIBS += Qtlib.lib zlib.lib teslib.lib QDdsPlugin.lib ui_settings.lib ui_mo2.lib

RESOURCES += \
  src/res.qrc

RC_ICONS = src/icon/play_store_256.ico

SOURCES += \
    src/ActionFile.cpp \
    src/ActionFolder.cpp \
    src/App.cpp \
    src/Config.cpp \
    src/ContextMenu.cpp \
    src/FojpUtility.cpp \
    src/ItemFile.cpp \
    src/ItemFolder.cpp \
    src/ItemMO2View.cpp \
    src/ItemModFile.cpp \
    src/HLogView.cpp \
    src/steam.cpp \
    src/ui/UIMainSearch.cpp \
    src/ui/UIMainWindow.cpp \
    src/ui/UIModFileView.cpp \
    src/ui/UIModListView.cpp \
    src/ui/UIOverrideCheck.cpp \
    src/ui/UIToolBar.cpp \
    src/ui/UITranslator.cpp \
    src/ui/UIPanelInfo.cpp \
    src/utils.cpp

HEADERS += \
    src/ActionFile.h \
    src/ActionFolder.h \
    src/App.h \
    src/Config.h \
    src/ContextMenu.h \
    src/Enums.h \
    src/FojpUtility.h \
    src/ItemFile.h \
    src/ItemFolder.h \
    src/ItemMO2View.h \
    src/ItemModFile.h \
    src/HLogView.h \
    src/steam.h \
    src/ui/UIMainSearch.h \
    src/ui/UIMainWindow.h \
    src/ui/UIModFileView.h \
    src/ui/UIModListView.h \
    src/ui/UIOverrideCheck.h \
    src/ui/UIToolBar.h \
    src/ui/UITranslator.h \
    src/ui/UIPanelInfo.h \
    src/utils.h

FORMS += \
    src/ui/UIMainSearch.ui \
    src/ui/UIMainWindow.ui \
    src/ui/UIModFileView.ui \
    src/ui/UIModListView.ui \
    src/ui/UIOverrideCheck.ui \
    src/ui/UIToolBar.ui \
    src/ui/UITranslator.ui \
    src/ui/UIPanelInfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
