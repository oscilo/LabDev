#ifndef TESTSFG_GLOBAL_H
#define TESTSFG_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef TESTSFG_LIB
# define TESTSFG_EXPORT Q_DECL_EXPORT
#else
# define TESTSFG_EXPORT Q_DECL_IMPORT
#endif

#endif // TESTSFG_GLOBAL_H
