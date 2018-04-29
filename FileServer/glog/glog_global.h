#ifndef GLOG_GLOBAL_H
#define GLOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GLOG_LIBRARY)
#  define GLOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GLOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GLOG_GLOBAL_H
