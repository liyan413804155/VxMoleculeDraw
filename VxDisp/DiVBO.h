#ifndef DI_VBO_H
#define DI_VBO_H

class VXDISP_EXPORT DiVBO
{
public:
    DiVBO(QVector<QVector3D>& vertex);
    ~DiVBO();

    void renderSurf(DiSurfAt& surfAt);

private:
    friend class DiVBOImpl;
    DiVBOImpl *d;
};

#endif

