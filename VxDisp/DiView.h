class VXDISP_EXPORT DiView
{
public:
    DiView();
    ~DiView();

public:
    void reisze(float wDev, float hDev);
    void zoom(float xDev, float yDev, float factor);
    void rotate(const QMatrix4x4& viewMat, const QPointF& srcDev, const QPointF& desDev);

public:
    QMatrix4x4 getViewMat()const;
    QMatrix4x4 getProjMat()const;
    float getHeight()const;
    float getWidth()const;

private:
    friend class DiViewImpl;
    DiViewImpl *d;
};