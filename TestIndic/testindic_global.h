#ifndef TESTINDIC_GLOBAL_H
#define TESTINDIC_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef TESTINDIC_LIB
# define TESTINDIC_EXPORT Q_DECL_EXPORT
#else
# define TESTINDIC_EXPORT Q_DECL_IMPORT
#endif

#endif // TESTINDIC_GLOBAL_H
