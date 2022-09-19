#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(LIBGIZMOQT_LIB)
#  define LIBGIZMOQT_EXPORT Q_DECL_EXPORT
# else
#  define LIBGIZMOQT_EXPORT Q_DECL_IMPORT
# endif
#else
# define LIBGIZMOQT_EXPORT
#endif
