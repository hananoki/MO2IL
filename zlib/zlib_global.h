#pragma once

#include <QtCore/qglobal.h>

#if defined(ZLIB_LIBRARY)
#  define ZLIB_EXPORT Q_DECL_EXPORT
#else
#  define ZLIB_EXPORT Q_DECL_IMPORT
#endif
