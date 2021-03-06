#ifndef DI_SHDR_H
#define DI_SHDR_H

class VXDISP_EXPORT DiShdr
{
public:
    DiShdr();
    ~DiShdr();

    void active(VeShdrType shdrType);
    void bindSurfVBO();
    void bindWireVBO();
    void setLightAt(const DiLightAt& lightAt);
    void setSurfAt(const DiSurfAt& surfAt);
    void setWireAt(const DiWireAt& wireAt);
    void setProjMat(const QMatrix4x4& mat);
    void setViewMat(const QMatrix4x4& mat);
    void setModelMat(const QMatrix4x4& mat);

private:
    friend class DiShdrImpl;
    DiShdrImpl *d;
};

#endif

