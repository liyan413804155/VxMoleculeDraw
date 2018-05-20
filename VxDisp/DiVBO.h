#ifndef DI_VBO_H
#define DI_VBO_H

#include "DiShdr.h"

class VXDISP_EXPORT DiVBO
{
public:
    DiVBO(QVector<QVector3D>& vertex, QVector<short>& index);
    ~DiVBO();

    void renderSurf(DiShdr& shdr);
    void renderWire(DiShdr& shdr);

private:
    friend class DiVBOImpl;
    DiVBOImpl *d;
};

#endif

