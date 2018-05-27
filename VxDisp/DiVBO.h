#ifndef DI_VBO_H
#define DI_VBO_H

#include "DiShdr.h"

class VXDISP_EXPORT DiVBOFace
{
public:
    DiVBOFace(QVector<QVector3D>& vertex, QVector<short>& index);
    ~DiVBOFace();

    void render(DiShdr& shdr);

private:
    friend class DiVBOFaceImpl;
    DiVBOFaceImpl *d;
};

class VXDISP_EXPORT DiVBOWire
{
public:
    DiVBOWire(QVector<QVector<QVector3D>>& vertex);
    ~DiVBOWire();

    void render(DiShdr& shdr);

private:
    friend class DiVBOWireImpl;
    DiVBOWireImpl *d;
};


#endif

