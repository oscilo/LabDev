#ifndef SELECTIVEMICROVOLTMETER_GLOBAL_H
#define SELECTIVEMICROVOLTMETER_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef SELECTIVEMICROVOLTMETER_LIB
# define SELECTIVEMICROVOLTMETER_EXPORT Q_DECL_EXPORT
#else
# define SELECTIVEMICROVOLTMETER_EXPORT Q_DECL_IMPORT
#endif

#endif // SELECTIVEMICROVOLTMETER_GLOBAL_H
