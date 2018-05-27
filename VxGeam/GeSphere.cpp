#include "VxGeamCm.h"
#include "GeSphere.h"

static QVector3D GeSpherePoint(float alpha, float beta);

class GeSphereImpl
{
public:
    QVector3D m_original;   /* sphere original */
    float m_raduis;         /* sphere radius */
    QMatrix4x4 m_xform;     /* transform for unit sphere */
};

GeSphere::GeSphere()
    : GeSphere(QVector3D(), 1.0f)
{

}

GeSphere::GeSphere
(
const QVector3D& org,   /* original */
float r /*  */
)
{
    d = new GeSphereImpl;
    d->m_original = org;
    d->m_raduis = r;

    d->m_xform.translate(org);
    d->m_xform.scale(r, r, r);
}

GeSphere::~GeSphere()
{
    delete d;
}

void GeSphere::triFace
(
QVector<QVector3D>& vertexFace, /* o: vertex data */
QVector<short>& indexFace,      /* o: vertex index */
int xStep,  /* i :triangulate x step */
int yStep   /* i: triangulate y step */
) const
{
    float alphaStep = M_PI / yStep;
    float betaStep = 2 * M_PI / xStep ;

    /* [1] side face */
    GeMeshTri(xStep, yStep - 2, vertexFace, indexFace,
    [&](int i, int j)->QVector3D
    {
        return d->m_original + d->m_raduis * GeSpherePoint((i + 1) * alphaStep, j * betaStep);
    },
    [&](int i, int j)->QVector3D
    {
        return GeSpherePoint((i + 1) * alphaStep, j * betaStep);
    });

    /* bottom point */
    vertexFace.push_back(d->m_original + d->m_raduis * GeSpherePoint(0.0f, 0.0f));
    vertexFace.push_back(GeSpherePoint(0.0f, 0.0f));

    /* top point */
    vertexFace.push_back(d->m_original + d->m_raduis * GeSpherePoint(M_PI, 0.0f));
    vertexFace.push_back(GeSpherePoint(M_PI, 0.0f));

    int bottomIndex = (xStep + 1) * (yStep - 1);
    int topIndex = (xStep + 1) * (yStep - 1) + 1;

    for (int i = 0; i < xStep; i++)
    {
        /* [2] bottom face */
        indexFace.push_back(bottomIndex);
        indexFace.push_back(i + 1);
        indexFace.push_back(i);

        /* [3] top face */
        indexFace.push_back(topIndex);
        indexFace.push_back((xStep + 1) * (yStep - 2) + i);
        indexFace.push_back((xStep + 1) * (yStep - 2) + i + 1);
    }
}

QMatrix4x4 GeSphere::getXform()const
{
    return d->m_xform;
}

bool GeSphere::isect
(
const QVector3D& org,   /* i: ray original */
const QVector3D& dir,   /* i: ray direction vector*/
float& param            /* o: intersection parameter */
) const
/*
Return value
true  : intersection
false : no intersection
*/
{
    QVector3D off = org - d->m_original;

    float a = dir.x() * dir.x() + dir.y() * dir.y() + dir.z() * dir.z();
    float b = 2 * (dir.x() * off.x() + dir.y() * off.y() + dir.z() * off.z());
    float c = off.x() * off.x() + off.y() * off.y() + off.z() * off.z() - d->m_raduis * d->m_raduis;

    float delta = b * b - 4 * a * c;

    if (delta < 0)
        return false;

    param = (-b + sqrt(delta)) / (2 * a);

    return true;
}

VeGeamType GeSphere::type()const
{
    return VeGeSphere;
}

static QVector3D GeSpherePoint
(
float alpha,
float beta
)
{
    return QVector3D(std::sin(alpha) * std::cos(beta), std::sin(alpha) * std::sin(beta), -std::cos(alpha));
}


