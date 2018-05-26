#ifndef GE_CYLINDER_H
#define GE_CYLINDER_H

class VXGEAM_EXPORT GeCylinder : public VxGeam
{
public:
    GeCylinder();
    GeCylinder(const QMatrix4x4& plane, float r, float h);
    virtual ~GeCylinder();

public:
    virtual void triFace(QVector<QVector3D>& vertexFace, QVector<short>& indexFace, int xStep, int yStep) const override;

public:
    virtual QMatrix4x4 getXform()const override;
    virtual bool isect(const QVector3D& org, const QVector3D& dir, float& param) const override;

private:
    friend class GeCylinderImpl;
    GeCylinderImpl *d;
};

#endif