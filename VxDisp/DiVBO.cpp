#include "VxDispCm.h"
#include "DiVBO.h"

class DiVBOImpl
{
public:
    DiVBOImpl()
        : m_index(QOpenGLBuffer::IndexBuffer)
    {

    }

    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_index;
    int m_count;
};

DiVBO::DiVBO(QVector<QVector3D>& vertex, QVector<short>& index)
{
    d = new DiVBOImpl();

    d->m_vertex.create();
    d->m_vertex.bind();
    d->m_vertex.allocate(&vertex[0], vertex.size() * sizeof(QVector3D));

    d->m_index.create();
    d->m_index.bind();
    d->m_index.allocate(&index[0], index.size() * sizeof(short));

    d->m_vertex.release();
    d->m_index.release();

    d->m_count = index.size();
}

DiVBO::~DiVBO()
{
    delete d;
}

void DiVBO::renderSurf(DiShdr& shdr)
{
    d->m_vertex.bind();
    d->m_index.bind();
    shdr.bindSurfVBO();
    glDrawElements(GL_TRIANGLES, d->m_count, GL_UNSIGNED_SHORT, 0);
    d->m_vertex.release();
    d->m_index.release();
}

void DiVBO::renderWire(DiShdr& shdr)
{
    d->m_vertex.bind();
    d->m_index.bind();
    shdr.bindWireVBO();
    glDrawElements(GL_LINES, d->m_count, GL_UNSIGNED_SHORT, 0);
    d->m_vertex.release();
    d->m_index.release();
}