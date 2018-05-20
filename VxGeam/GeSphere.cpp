#include "VxGeamCm.h"
#include "GeSphere.h"

static QVector3D GeSpherePoint(float alpha, float beta);

class GeSphereImpl
{
public:
    QVector3D m_origal;
    float m_raduis;
    QMatrix4x4 m_xform;
};

GeSphere::GeSphere(const QVector3D& org, float r)
{
    d = new GeSphereImpl;
    d->m_origal = org;
    d->m_raduis = r;
}

GeSphere::~GeSphere()
{
    delete d;
}

void GeSphere::triFace(QVector<QVector3D>& vertexFace, QVector<short>& indexFace, int xStep, int yStep)
{
    float alphaStep = M_PI / yStep;
    float betaStep = 2 * M_PI / xStep ;

    GeMeshTri(xStep, yStep - 2, vertexFace, indexFace,
    [&](int i, int j)->QVector3D
    {
        return d->m_origal + d->m_raduis * GeSpherePoint((i + 1) * alphaStep, j * betaStep);
    },
    [&](int i, int j)->QVector3D
    {
        return GeSpherePoint((i + 1) * alphaStep, j * betaStep);
    });

    vertexFace.push_back(d->m_origal + d->m_raduis * GeSpherePoint(0.0f, 0.0f));
    vertexFace.push_back(GeSpherePoint(0.0f, 0.0f));

    vertexFace.push_back(d->m_origal + d->m_raduis * GeSpherePoint(M_PI, 0.0f));
    vertexFace.push_back(GeSpherePoint(M_PI, 0.0f));

    int bottomIndex = (xStep + 1) * (yStep - 1);
    int topIndex = (xStep + 1) * (yStep - 1) + 1;

    for (int i = 0; i < xStep; i++)
    {
        indexFace.push_back(bottomIndex);
        indexFace.push_back(i + 1);
        indexFace.push_back(i);

        indexFace.push_back(topIndex);
        indexFace.push_back((xStep + 1) * (yStep - 2) + i);
        indexFace.push_back((xStep + 1) * (yStep - 2) + i + 1);
    }
}

QMatrix4x4 GeSphere::getXform()const
{
    return QMatrix4x4();
}

bool GeSphere::isect(const QVector3D& org, const QVector3D& dir, float& param)const
{
    return false;
}

static QVector3D GeSpherePoint(float alpha, float beta)
{
    return QVector3D(std::sin(alpha) * std::cos(beta), std::sin(alpha) * std::sin(beta), -std::cos(alpha));
}
