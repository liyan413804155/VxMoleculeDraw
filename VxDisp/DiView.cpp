#include "VxDispCm.h"
#include "DiView.h"

class DiViewImpl
{
public:
    float m_wDev, m_hDev;
    float m_xView, m_yView;
    float m_extent;
    float m_near, m_far;
    QMatrix4x4 m_viewMat;
};

DiView::DiView()
{
    d = new DiViewImpl;
    d->m_hDev = 100;
    d->m_wDev = 100;
    d->m_xView = 0;
    d->m_yView = 0;
    d->m_extent = 1;
    d->m_near = -500;
    d->m_far = 500;
}

DiView::~DiView()
{
    delete d;
}

void DiView::reisze(float w, float h)
{
    d->m_wDev = w;
    d->m_hDev = h;
}

void DiView::zoom(float x, float y, float r)
{
    QVector4D pntView = getProjMat().inverted() 
        * QVector4D(2 * x / d->m_wDev - 1.0f, 1.0f - 2 * y / d->m_hDev, 0.0, 1.0f);

    QMatrix4x4 viewMat;
    viewMat.translate(pntView.x(), pntView.y(), 0.0f);
    viewMat.scale(r);
    viewMat.translate(-pntView.x(), -pntView.y(), 0.0f);
    QMatrix4x4 projMat =  getProjMat() * viewMat;

    float width = 2.0f / projMat(0, 0);
    d->m_xView = -projMat(0, 3) * width / 2.0f;
    float height = 2.0f / projMat(1, 1);
    d->m_yView = -projMat(1, 3) * height / 2.0f;

    d->m_extent /= r;
}

QMatrix4x4 DiView::getProjMat()const
{
    float w_h = d->m_wDev / d->m_hDev;
    float x = 1.0f;
    float y = 1.0f;
    if (w_h > 1.0f)
    {
        x = d->m_extent * w_h;
        y = d->m_extent;
    }
    else
    {
        x = d->m_extent;
        y = d->m_extent / w_h;
    }

    QMatrix4x4 proj;
    proj.ortho(-x + d->m_xView, x + d->m_xView, -y + d->m_yView, y + d->m_yView, d->m_near, d->m_far);

    return proj;
}

QMatrix4x4 DiView::getViewMat()const
{
    return d->m_viewMat;
}