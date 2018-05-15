#include "VxDispCm.h"
#include "DiShdr.h"

#define VBO_AT_VERTEX       "VxVertex"
#define VBO_AT_NORMAL       "VxNormal"

#define VBO_UN_PROJMAT      "VxProjMat"
#define VBO_UN_VIEWMAT      "VxViewMat"
#define VBO_UN_FRONTCOLOR   "VxColor"
#define VBO_UN_BACKCOLOR    "VxBackColor"
#define VBO_UN_SPECULAR     "VxSpecular"
#define VBO_UN_DIFFUSE      "VxDiffuse"
#define VBO_UN_SHININESS    "VxShininess"
#define VBO_UN_AMBIENT      "VxAmbient"
#define VBO_UN_LIGHTCOLOR   "VxLightColor"
#define VBO_UN_LIGHTDIR     "VxLightDir"



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
    d->m_shdrFile[DiShdrFace].m_frag.push_back(":/shader/light.frag");

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

void DiShdr::bindSurfVBO()
{
    int stride = sizeof(QVector3D) + sizeof(QVector3D);
    int offset = 0;

    int vertexLocation = d->m_progAct->attributeLocation(VBO_AT_VERTEX);
    d->m_progAct->enableAttributeArray(vertexLocation);
    d->m_progAct->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, stride);

    offset += sizeof(QVector3D);

    int normalLocation = d->m_progAct->attributeLocation(VBO_AT_NORMAL);
    d->m_progAct->enableAttributeArray(normalLocation);
    d->m_progAct->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, stride);
}

void DiShdr::bindWireVBO()
{
    int stride = sizeof(QVector3D);
    int offset = 0;

    int vertexLocation = d->m_progAct->attributeLocation(VBO_AT_VERTEX);
    d->m_progAct->enableAttributeArray(vertexLocation);
    d->m_progAct->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, stride);
}

void DiShdr::setLightAt(const DiLightAt& lightAt)
{
    d->m_progAct->setUniformValue(VBO_UN_AMBIENT,    lightAt.ambient);
    d->m_progAct->setUniformValue(VBO_UN_LIGHTCOLOR, lightAt.color);
    d->m_progAct->setUniformValue(VBO_UN_LIGHTDIR,   lightAt.direction);
}

void DiShdr::setSurfAt(const DiSurfAt& surfAt)
{
    d->m_progAct->setUniformValue(VBO_UN_FRONTCOLOR, surfAt.front_color);
    d->m_progAct->setUniformValue(VBO_UN_BACKCOLOR,  surfAt.back_color);
    d->m_progAct->setUniformValue(VBO_UN_DIFFUSE,    surfAt.diffuse);
    d->m_progAct->setUniformValue(VBO_UN_SHININESS,  surfAt.shininess);
    d->m_progAct->setUniformValue(VBO_UN_SPECULAR,   surfAt.specular);
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
