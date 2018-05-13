#include "VxDispCm.h"
#include "DiShdr.h"

#define VBO_AT_VERTEX   "VxVertex"

struct ShdrFile
{
    QVector<QString> m_vert;
    QVector<QString> m_frag;
};

class DiShdrImpl
{
public:
    QOpenGLShaderProgram m_progAll[DiShdrLast];
    QOpenGLShaderProgram *m_progAct;
    ShdrFile m_shdrFile[DiShdrLast];
};

DiShdr::DiShdr()
{
    d = new DiShdrImpl();

    d->m_progAct = nullptr;

    d->m_shdrFile[DiShdrFace].m_vert.push_back(":/shader/face.vert");
    d->m_shdrFile[DiShdrFace].m_frag.push_back(":/shader/face.frag");
}

DiShdr::~DiShdr()
{
    delete d;
}

void DiShdr::active(VeShdrType shdrType)
{
    if (!d->m_progAll[shdrType].isLinked())
    {
        bool ret = true;

        for (QString& file : d->m_shdrFile[shdrType].m_vert)
        {
            ret = d->m_progAll[shdrType].addShaderFromSourceFile(QOpenGLShader::Vertex, file);
            assert(ret);
        }

        for (QString& file : d->m_shdrFile[shdrType].m_frag)
        {
            ret = d->m_progAll[shdrType].addShaderFromSourceFile(QOpenGLShader::Fragment, file);
            assert(ret);
        }

        ret = d->m_progAll[shdrType].link();
        assert(ret);
    }

    d->m_progAct = &d->m_progAll[shdrType];
    d->m_progAct->bind();
}

void DiShdr::bindVBO()
{
    int vertexLocation = d->m_progAct->attributeLocation(VBO_AT_VERTEX);
    d->m_progAct->enableAttributeArray(vertexLocation);
    d->m_progAct->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
}

void DiShdr::setSurfAt(DiSurfAt& surfAt)
{

}

DiShdr VgDiShdr;
