#include "VxDispCm.h"
#include "DiVBO.h"

class DiVBOFaceImpl
{
public:
    DiVBOFaceImpl()
        : m_index(QOpenGLBuffer::IndexBuffer)
    {

    }

    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_index;
    int m_count;
};

DiVBOFace::DiVBOFace(QVector<QVector3D>& vertex, QVector<short>& index)
{
    d = new DiVBOFaceImpl();

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

DiVBOFace::~DiVBOFace()
{
    delete d;
}

void DiVBOFace::render(DiShdr& shdr)
{
    d->m_vertex.bind();
    d->m_index.bind();
    shdr.bindSurfVBO();
    glDrawElements(GL_TRIANGLES, d->m_count, GL_UNSIGNED_SHORT, 0);
    d->m_vertex.release();
    d->m_index.release();
}

class DiVBOWireImpl
{
public:
    QVector<QOpenGLBuffer>  m_vertex;
    QVector<int>            m_count;
};

DiVBOWire::DiVBOWire(QVector<QVector<QVector3D>>& vertex)
{
    d = new DiVBOWireImpl;
    d->m_vertex.resize(vertex.size());
    d->m_count.resize(vertex.size());

    for (int i = 0; i < vertex.size(); i++)
    {
        d->m_vertex[i].create();
        d->m_vertex[i].bind();
        d->m_vertex[i].allocate(&vertex[i][0], vertex[i].size() * sizeof(QVector3D));
        d->m_vertex[i].release();

        d->m_count[i] = vertex[i].size();
    }
}

DiVBOWire::~DiVBOWire()
{
    delete d;
}

void DiVBOWire::render(DiShdr& shdr)
{
    for (int i = 0; i < d->m_vertex.size(); i++)
    {
        d->m_vertex[i].bind();
        shdr.bindWireVBO();
        glDrawArrays(GL_LINE_STRIP, 0, d->m_count[i]);
        d->m_vertex[i].release();
    }
}