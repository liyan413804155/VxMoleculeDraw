#ifndef DI_SHDR_H
#define DI_SHDR_H

class DiShdr
{
public:
    DiShdr();
    ~DiShdr();

    void active(VeShdrType shdrType);
    void bindVBO();
    void setSurfAt(const DiSurfAt& surfAt);
    void setProjMat(const QMatrix4x4& mat);
    void setViewMat(const QMatrix4x4& mat);

private:
    friend class DiShdrImpl;
    DiShdrImpl *d;
};

extern DiShdr VgDiShdr;

#endif

