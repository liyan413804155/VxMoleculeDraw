#include "VxDispCm.h"
#include "DiView.h"

class DiViewImpl
{
public:
    float m_w, m_h;
    float m_x, m_y;
    float m_extent;
    float m_near, m_far;
    QMatrix4x4 m_viewMat;
};

DiView::DiView()
{
    d = new DiViewImpl;
    d->m_h = 100;
    d->m_w = 100;
    d->m_x = 0;
    d->m_y = 0;
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
    d->m_w = w;
    d->m_h = h;
}

QMatrix4x4 DiView::getProjMat()const
{
    float r = d->m_w / d->m_h;
    float x = 1.0f;
    float y = 1.0f;
    if (r > 1.0f)
    {
        x = d->m_extent * r;
        y = d->m_extent;
    }
    else
    {
        x = d->m_extent;
        y = d->m_extent / r;
    }

    QMatrix4x4 proj;
    proj.ortho(-x + d->m_x, x + d->m_x, -y + d->m_y, y + d->m_y, d->m_near, d->m_far);

    return proj;
}

QMatrix4x4 DiView::getViewMat()const
{
    return d->m_viewMat;
}