#ifndef NARROWBANDFILTER_GLOBAL_H
#define NARROWBANDFILTER_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef NARROWBANDFILTER_LIB
# define NARROWBANDFILTER_EXPORT Q_DECL_EXPORT
#else
# define NARROWBANDFILTER_EXPORT Q_DECL_IMPORT
#endif

#endif // NARROWBANDFILTER_GLOBAL_H
