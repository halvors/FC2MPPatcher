#ifndef MPPATCH_GLOBAL_H
#define MPPATCH_GLOBAL_H

#include <QtGlobal>

#include <winsock2.h>
#include <iphlpapi.h>

#if defined(MPPATCH_LIBRARY)
#  define MPPATCHSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MPPATCHSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MPPATCH_GLOBAL_H
