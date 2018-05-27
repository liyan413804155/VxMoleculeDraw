#ifndef GE_CONE_H
#define GE_CONE_H

class VXGEAM_EXPORT GeCone : public VxGeam
{
public:
    GeCone();
    GeCone(const QMatrix4x4& plane, float r, float h);
    ~GeCone();

public:
    virtual void triFace(QVector<QVector3D>& vertexFace, QVector<short>& indexFace, int xStep, int yStep) const override;

public:
    virtual QMatrix4x4 getXform()const override;
    virtual bool isect(const QVector3D& org, const QVector3D& dir, float& param) const override;

private:
    friend class GeConeImpl;
    GeConeImpl *d;
};


#endif