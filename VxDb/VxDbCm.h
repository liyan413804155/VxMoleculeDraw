#ifndef VX_DBCM_H
#define VX_DBCM_H

#include <QtCore/qglobal.h>
#include <QQueue>
#include <QString>

#include <assert.h>

#ifndef BUILD_STATIC
# if defined(VXDB_BUILD)
#  define VXDB_EXPORT Q_DECL_EXPORT
# else
#  define VXDB_EXPORT Q_DECL_IMPORT
# endif
#else
# define VXDB_EXPORT
#endif

typedef char OmName[32];

#endif


