#ifndef GE_SPHERE_H
#define GE_SPHERE_H

class VXGEAM_EXPORT GeSphere : public VxGeam
{
public:
    GeSphere(const QVector3D& org, float r);
    ~GeSphere();

public:
    virtual void triFace(QVector<QVector3D>& vertexFace, QVector<short>& indexFace, int xStep, int yStep) override;

public:
    virtual QMatrix4x4 getXform()const override;
    virtual bool isect(const QVector3D& org, const QVector3D& dir, float& param) const override;

private:
    friend class GeSphereImpl;
    GeSphereImpl *d;
};

#endif