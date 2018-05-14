#include "VxDispCm.h"
#include "DiView.h"

class DiViewImpl
{
public:
    /* device size */
    float m_wDev;   /* width */
    float m_hDev;   /* height */

    /* coordinate that view coordinate system origin in projection coordinate system */
    float m_xViewOrig;  /* x coordinate */
    float m_yViewOrig;  /* y coordinate */

    float m_extent;     /* view extent factor */

    /* view z axis range */
    float m_zNear;  /* z axis near value */
    float m_zFar;   /* z axis farther value */

    QMatrix4x4 m_viewMat;   /* view matrix */
};

DiView::DiView()
{
    d = new DiViewImpl;
    d->m_hDev = 100;
    d->m_wDev = 100;
    d->m_xViewOrig = 0;
    d->m_yViewOrig = 0;
    d->m_extent = 1;
    d->m_zNear = -500;
    d->m_zFar = 500;
}

DiView::~DiView()
{
    delete d;
}

void DiView::reisze
(
float wDev, /* i: width */
float hDev  /* i: height  */
)
{
    d->m_wDev = wDev;
    d->m_hDev = hDev;
}

void DiView::zoom
(
float xDev,     /* i: zoom point x coordinate in device coordinate system */
float yDev,     /* i: zoom point y coordinate in device coordinate system */
float factor    /* i: zoom factor */
)
{
    /* zoom point in view coordinate system */
    QVector4D pntView = getProjMat().inverted() 
        * QVector4D(2 * xDev / d->m_wDev - 1.0f, 1.0f - 2 * yDev / d->m_hDev, 0.0, 1.0f);

    /* calculate new projection matrix */
    QMatrix4x4 viewMat;
    viewMat.translate(pntView.x(), pntView.y(), 0.0f);
    viewMat.scale(factor);
    viewMat.translate(-pntView.x(), -pntView.y(), 0.0f);
    QMatrix4x4 projMat =  getProjMat() * viewMat;

    /* update coordinate that view coordinate system origin in projection coordinate system*/
    float width = 2.0f / projMat(0, 0);
    d->m_xViewOrig = -projMat(0, 3) * width / 2.0f;
    float height = 2.0f / projMat(1, 1);
    d->m_yViewOrig = -projMat(1, 3) * height / 2.0f;

    /* update view extent factor */
    d->m_extent /= factor;
}

QMatrix4x4 DiView::getProjMat()const
{
    float w_h = d->m_wDev / d->m_hDev;
    float xViewWidth_2, yViewHeight_2;
    if (w_h > 1.0f)
    {
        xViewWidth_2 = d->m_extent * w_h;
        yViewHeight_2 = d->m_extent;
    }
    else
    {
        xViewWidth_2 = d->m_extent;
        yViewHeight_2 = d->m_extent / w_h;
    }

    QMatrix4x4 proj;
    proj.ortho(d->m_xViewOrig -xViewWidth_2,  d->m_xViewOrig + xViewWidth_2,
               d->m_yViewOrig -yViewHeight_2, d->m_yViewOrig + yViewHeight_2,
               d->m_zNear, d->m_zFar);

    return proj;
}

QMatrix4x4 DiView::getViewMat()const
{
    return d->m_viewMat;
}