QT -= gui

TEMPLATE = lib
DEFINES += ZLIB_LIBRARY

CONFIG += c++11
CONFIG += staticlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += zlib-1.2.12

SOURCES += \
    zlib-1.2.12/adler32.c \
    zlib-1.2.12/compress.c \
    zlib-1.2.12/crc32.c \
    zlib-1.2.12/deflate.c \
    zlib-1.2.12/gzclose.c \
    zlib-1.2.12/gzlib.c \
    zlib-1.2.12/gzread.c \
    zlib-1.2.12/gzwrite.c \
    zlib-1.2.12/infback.c \
    zlib-1.2.12/inffast.c \
    zlib-1.2.12/inflate.c \
    zlib-1.2.12/inftrees.c \
    zlib-1.2.12/trees.c \
    zlib-1.2.12/uncompr.c \
    zlib-1.2.12/zutil.c
    
#    zlib-1.2.12/contrib/minizip/ioapi.c \
#    zlib-1.2.12/contrib/minizip/iowin32.c \
#    zlib-1.2.12/contrib/minizip/miniunz.c \
#    zlib-1.2.12/contrib/minizip/minizip.c \
#    zlib-1.2.12/contrib/minizip/mztools.c \
#    zlib-1.2.12/contrib/minizip/unzip.c \
#    zlib-1.2.12/contrib/minizip/zip.c

HEADERS += \
    zlib-1.2.12/crc32.h \
    zlib-1.2.12/deflate.h \
    zlib-1.2.12/gzguts.h \
    zlib-1.2.12/inffast.h \
    zlib-1.2.12/inffixed.h \
    zlib-1.2.12/inflate.h \
    zlib-1.2.12/inftrees.h \
    zlib-1.2.12/trees.h \
    zlib-1.2.12/zconf.h \
    zlib-1.2.12/zutil.h \
    zlib-1.2.12/contrib/minizip/crypt.h \
    zlib-1.2.12/contrib/minizip/ioapi.h \
    zlib-1.2.12/contrib/minizip/iowin32.h \
    zlib-1.2.12/contrib/minizip/mztools.h \
    zlib-1.2.12/contrib/minizip/unzip.h \
    zlib-1.2.12/contrib/minizip/zip.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

