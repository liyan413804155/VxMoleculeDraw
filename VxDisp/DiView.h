class VXDISP_EXPORT DiView
{
public:
    DiView();
    ~DiView();

public:
    void reisze(float w, float h);
    void zoom(float x, float y, float r);

public:
    QMatrix4x4 getViewMat()const;
    QMatrix4x4 getProjMat()const;

private:
    friend class DiViewImpl;
    DiViewImpl *d;
};