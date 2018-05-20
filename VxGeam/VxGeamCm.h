#ifndef VX_GEAMCM_H
#define VX_GEAMCM_H

#include <QtCore/qglobal.h>
#include <QtCore/qmath.h>

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>

#include <functional>

#ifndef BUILD_STATIC
# if defined(VXGEAM_BUILD)
#  define VXGEAM_EXPORT Q_DECL_EXPORT
# else
#  define VXGEAM_EXPORT Q_DECL_IMPORT
# endif
#else
# define VXGEAM_EXPORT
#endif

void GeMeshTri(int xStep,int yStep,QVector<QVector3D>& faceVertex,QVector<short>& faceIndex,std::function<QVector3D(int i, int j)> funcVertex,std::function<QVector3D(int i, int j)> funcNormal);

class VXGEAM_EXPORT VxGeam
{
public:
    VxGeam();
    virtual ~VxGeam();

public:
    virtual void triFace(QVector<QVector3D>& vertexFace, QVector<short>& indexFace, int xStep, int yStep) = 0;

public:
    virtual QMatrix4x4 getXform()const = 0;
    virtual bool isect(const QVector3D& org, const QVector3D& dir, float& param) const = 0;
};


#endif