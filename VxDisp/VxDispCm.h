#ifndef VX_DISPCM_H
#define VX_DISPCM_H

#include <QtCore/qglobal.h>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QVector3D>

#include <assert.h>

#ifndef BUILD_STATIC
# if defined(VXDISP_BUILD)
#  define VXDISP_EXPORT Q_DECL_EXPORT
# else
#  define VXDISP_EXPORT Q_DECL_IMPORT
# endif
#else
# define VXDISP_EXPORT
#endif

struct DiSurfAt
{
    QVector3D front_color;
    QVector3D back_color;
};

#endif

