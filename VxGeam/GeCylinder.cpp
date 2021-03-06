#include "VxGeamCm.h"
#include "GeCylinder.h"

class GeCylinderImpl
{
public:
    float m_radius;     /* cone cylinder */
    float m_height;     /* cone cylinder */
    QMatrix4x4 m_xform; /* cone transform for unit cylinder */
};

GeCylinder::GeCylinder() 
    : GeCylinder(QMatrix4x4(), 1.0f, 1.0f)
{

}

GeCylinder::GeCylinder(const QMatrix4x4& plane, float r, float h)
{
    d = new GeCylinderImpl;
    d->m_radius = r;
    d->m_height = h;

    QMatrix4x4 scale;
    scale.scale(r, r, h);
    d->m_xform = plane * scale;
}

GeCylinder::~GeCylinder()
{
    delete d;
}

void GeCylinder::triFace
(
QVector<QVector3D>& vertexFace, /* o: vertex data */
QVector<short>& indexFace,      /* o: vertex index */
int xStep,  /* i :triangulate x step */
int yStep   /* i: triangulate y step */
) const
{
    float heightStep = 1.0f / yStep;
    float alphaStep = 2 * M_PI / xStep;

    QVector3D org = d->m_xform.column(3).toVector3D();
    QVector3D dir = d->m_xform.column(2).toVector3D().normalized();

    /* [1] side face */
    GeMeshTri(xStep, yStep, vertexFace, indexFace,
        [&](int i, int j)->QVector3D
    {
        QVector3D vertex(std::cos(j * alphaStep), std::sin(j * alphaStep), i * heightStep);
        return (d->m_xform * QVector4D(vertex, 1.0f)).toVector3D();
    },
        [&](int i, int j)->QVector3D
    {
        QVector3D normal(std::cos(j * alphaStep), std::sin(j * alphaStep), 0.0f);
        return (d->m_xform * QVector4D(normal, 0.0f)).toVector3D().normalized();
    });

    /* [2] bottom face */
    for (int i = 0; i <= xStep; i++)
    {
        QVector3D vertex(std::cos(i * alphaStep), std::sin(i * alphaStep), 0.0f);
        vertexFace.push_back((d->m_xform * QVector4D(vertex, 1.0f)).toVector3D());

        QVector3D normal(0.0f, 0.0f, -1.0f);
        vertexFace.push_back((d->m_xform * QVector4D(normal, 0.0f)).toVector3D().normalized());
    }

    vertexFace.push_back(org);
    vertexFace.push_back(-dir);

    int bottomEdgeIndex = (xStep + 1) * (yStep + 1);
    int bottomCenterIndex = bottomEdgeIndex + xStep + 1;

    for (int i = 0; i < xStep; i++)
    {
        indexFace.push_back(bottomCenterIndex);
        indexFace.push_back(bottomEdgeIndex + i + 1);
        indexFace.push_back(bottomEdgeIndex + i);
    }

    /* [3] top face */
    for (int i = 0; i <= xStep; i++)
    {
        QVector3D vertex(std::cos(i * alphaStep), std::sin(i * alphaStep), 1.0f);
        vertexFace.push_back((d->m_xform * QVector4D(vertex, 1.0f)).toVector3D());

        QVector3D normal(0.0f, 0.0f, 1.0f);
        vertexFace.push_back((d->m_xform * QVector4D(normal, 0.0f)).toVector3D().normalized());
    }

    vertexFace.push_back(org + dir * d->m_height);
    vertexFace.push_back(dir);

    int topEdgeIndex = bottomCenterIndex + 1;
    int topCenterIndex = topEdgeIndex + xStep + 1;

    for (int i = 0; i < xStep; i++)
    {
        indexFace.push_back(topCenterIndex);
        indexFace.push_back(topEdgeIndex + i);
        indexFace.push_back(topEdgeIndex + i + 1);
    }
}

QMatrix4x4 GeCylinder::getXform() const
{
    return d->m_xform;
}

bool GeCylinder::isect
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
    QMatrix4x4 xformInv = d->m_xform.inverted();
    QVector3D mOrg = (xformInv * QVector4D(org, 1.0f)).toVector3D();
    QVector3D mDir = (xformInv * QVector4D(dir, 0.0f)).toVector3D();

    float a = mDir.x() * mDir.x() + mDir.y() * mDir.y();
    float b = 2 * (mOrg.x() * mDir.x() + mOrg.y() * mDir.y());
    float c = mOrg.x() * mOrg.x() + mOrg.y() * mOrg.y() - 1.0f;

    if (abs(a) < VxZero)
    {
        if (abs(b) < VxZero)
        {
            if (c > 0.0f)
                return false;

            float params[2] = {-mOrg.z() / mDir.z(), (1.0f - mOrg.z()) / mDir.z()};
            param = VxMin(params[0], params[1]);
            return true;
        }

        assert(0);
        return false;
    }

    float delta = b * b - 4 * a * c;
    if (delta < 0.0f)
        return false;

    float params[2] = { (-b - sqrt(delta)) / (2.0f * a), (-b + sqrt(delta)) / (2.0f * a) };
    QVector3D isectPnt[2] = {mOrg + mDir * params[0], mOrg + mDir * params[1]};

    if (isectPnt[0].z() > 1.0f && isectPnt[1].z() > 1.0f)
        return false;
    if (isectPnt[0].z() < 0.0f && isectPnt[1].z() < 0.0f)
        return false;
    if (isectPnt[0].z() < 1.0f && isectPnt[1].z() < 1.0f &&
        isectPnt[0].z() > 0.0f && isectPnt[1].z() > 0.0f)
    {
        param = VxMin(params[0], params[1]);
        return true;
    }

    if (isectPnt[0].z() < 1.0f && isectPnt[0].z() > 0.0f)
    {
        params[1] = -mOrg.z() / mDir.z();
        isectPnt[1] = mOrg + mDir * params[1];
        if (isectPnt[1].x() * isectPnt[1].x() + isectPnt[1].y() * isectPnt[1].y() < 1.0f)
        {
            param = VxMin(params[0], params[1]);
            return true;
        }

        params[1] = (1.0f - mOrg.z()) / mDir.z();
        isectPnt[1] = mOrg + mDir * params[1];
        if (isectPnt[1].x() * isectPnt[1].x() + isectPnt[1].y() * isectPnt[1].y() < 1.0f)
        {
            param = VxMin(params[0], params[1]);
            return true;
        }

        assert(0);
        param = params[0];
        return true;
    }

    if (isectPnt[1].z() < 1.0f && isectPnt[1].z() > 0.0f)
    {
        params[0] = -mOrg.z() / mDir.z();
        isectPnt[0] = mOrg + mDir * params[0];
        if (isectPnt[0].x() * isectPnt[0].x() + isectPnt[0].y() * isectPnt[0].y() < 1.0f)
        {
            param = VxMin(params[0], params[1]);
            return true;
        }

        params[0] = (1.0f - mOrg.z()) / mDir.z();
        isectPnt[0] = mOrg + mDir * params[0];
        if (isectPnt[0].x() * isectPnt[0].x() + isectPnt[0].y() * isectPnt[0].y() < 1.0f)
        {
            param = VxMin(params[0], params[1]);
            return true;
        }

        assert(0);
        param = params[1];
        return true;
    }

    params[0] = -mOrg.z() / mDir.z();
    isectPnt[0] = mOrg + mDir * params[0];
    assert(isectPnt[0].x() * isectPnt[0].x() + isectPnt[0].y() * isectPnt[0].y() < 1.0f);

    params[1] = (1.0f - mOrg.z()) / mDir.z();
    isectPnt[1] = mOrg + mDir * params[1];
    assert(isectPnt[1].x() * isectPnt[1].x() + isectPnt[1].y() * isectPnt[1].y() < 1.0f);

    param = VxMin(params[0], params[1]);
    return true;
}

VeGeamType GeCylinder::type()const
{
    return VeGeCylinder;
}