#include "VxDispCm.h"
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

void DiVBO::renderSurf(DiShdr& shdr)
{
    d->m_vbo.bind();
    shdr.bindSurfVBO();
    glDrawArrays(GL_TRIANGLES, 0, d->m_count);
    d->m_vbo.release();
}

void DiVBO::renderWire(DiShdr& shdr)
{
    d->m_vbo.bind();
    shdr.bindWireVBO();
    glDrawArrays(GL_LINES, 0, d->m_count);
    d->m_vbo.release();
}