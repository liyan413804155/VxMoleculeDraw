#ifndef VX_DISPCM_H
#define VX_DISPCM_H

#include <QtCore/qglobal.h>

#include <QMatrix4x4>
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

enum VeShdrType
{
    DiShdrFace = 0,
    DiShdrWire = 1,
    DiShdrLast
};

struct DiSurfAt
{
    QVector3D front_color;
    QVector3D back_color;
};

struct DiWireAt
{
    QVector3D color;
};

#endif

