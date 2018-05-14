#include "VxDispCm.h"
#include "DiShdr.h"

#define VBO_AT_VERTEX       "VxVertex"

#define VBO_UN_PROJMAT      "VxProjMat"
#define VBO_UN_VIEWMAT      "VxViewMat"
#define VBO_UN_FRONTCOLOR   "VxColor"
#define VBO_UN_BACKCOLOR    "VxBackColor"

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

    d->m_shdrFile[DiShdrWire].m_vert.push_back(":/shader/wire.vert");
    d->m_shdrFile[DiShdrWire].m_frag.push_back(":/shader/wire.frag");
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

void DiShdr::setSurfAt(const DiSurfAt& surfAt)
{
    d->m_progAct->setUniformValue(VBO_UN_FRONTCOLOR, surfAt.front_color);
    d->m_progAct->setUniformValue(VBO_UN_BACKCOLOR,  surfAt.back_color);
}

void DiShdr::setWireAt(const DiWireAt& wireAt)
{
    d->m_progAct->setUniformValue(VBO_UN_FRONTCOLOR, wireAt.color);
}

void DiShdr::setProjMat(const QMatrix4x4& mat)
{
    d->m_progAct->setUniformValue(VBO_UN_PROJMAT, mat);
}

void DiShdr::setViewMat(const QMatrix4x4& mat)
{
    d->m_progAct->setUniformValue(VBO_UN_VIEWMAT, mat);
}
