#include "VxDispCm.h"
#include "DiShdrUtil.h"
#include "DiVBO.h"

class DiVBOImpl
{
public:
    QOpenGLBuffer m_vbo;
    int m_count;
};

DiVBO::DiVBO(QVector<QVector3D>& vertex)
{
    d = new DiVBOImpl();

    d->m_vbo.create();
    d->m_vbo.bind();
    d->m_vbo.allocate(&vertex[0], vertex.size() * sizeof(QVector3D));
    d->m_vbo.release();

    d->m_count = vertex.size();
}

DiVBO::~DiVBO()
{
    delete d;
}

void DiVBO::renderSurf(DiSurfAt& surfAt)
{
    d->m_vbo.bind();

    diShdrBindVBO();
    diShdrSetSurfAt(surfAt);

    glDrawArrays(GL_TRIANGLES, 0, d->m_count);
    d->m_vbo.release();
}