#include <QApplication>

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVector>
#include <QVector3D>
#include <QWheelEvent>

#include "VxDisp.h"
#include "VxGeam.h"

struct MyGeam
{
    VxGeam *pGeam;
    DiSurfAt at;
    bool redraw;
};

class MyDrawing : public QOpenGLWidget, public QOpenGLFunctions
{
public:
    MyDrawing(QWidget* parent)
        : QOpenGLWidget(parent)
    {
        m_beginRot = false;

        m_lightAt.ambient = 0.5;
        m_lightAt.color = QVector3D(1.0f, 1.0f, 1.0f);
        m_lightAt.direction = QVector3D(1.0f, 1.0f, 1.0f);

        DiSurfAt at;
        at.front_color = QVector3D(1.0f, 0.0f, 0.0f);
        at.back_color = QVector3D(0.0f, 1.0f, 0.0f);
        at.diffuse = 0.5f;
        at.specular = 0.8f;
        at.shininess = 10;

        m_geam.resize(3);

        QMatrix4x4 plane;

        plane.setToIdentity();
        plane.translate(2.0f, 0.0f, 0.0f);
        m_geam[0].pGeam = new GeCone(plane, 1.0f, 1.0f);
        m_geam[0].at = at;

        m_geam[1].pGeam = new GeSphere(QVector3D(), 1.0f);
        m_geam[1].at = at;
        m_geam[1].at.front_color = QVector3D(1.0f, 1.0f, 0.0f);

        plane.setToIdentity();
        plane.translate(-2.0f, 0.0f, 0.0f);
        m_geam[2].pGeam = new GeCylinder(plane, 1.0f, 1.0f);
        m_geam[2].at = at;
        m_geam[2].at.front_color = QVector3D(1.0f, 0.0f, 1.0f);

        /*setMouseTracking(true);*/
    }

    ~MyDrawing()
    {


    }

private:
    virtual void resizeGL(int w, int h)override
    {
        m_view.reisze(w, h);
    }

    virtual void initializeGL()override
    {
        initializeOpenGLFunctions();

        QVector<QVector3D> faceVertex;
        QVector<short> faceIndex;

        faceVertex.clear();
        faceIndex.clear();
        GeSphere sphere;
        sphere.triFace(faceVertex, faceIndex, 50, 50);
        m_faceVbo[VeGeSphere] = new DiVBO(faceVertex, faceIndex);

        faceVertex.clear();
        faceIndex.clear();
        GeCylinder cylinder;
        cylinder.triFace(faceVertex, faceIndex, 50, 4);
        m_faceVbo[VeGeCylinder] = new DiVBO(faceVertex, faceIndex);

        faceVertex.clear();
        faceIndex.clear();
        GeCone cone;
        cone.triFace(faceVertex, faceIndex, 50, 4);
        m_faceVbo[VeGeCone] = new DiVBO(faceVertex, faceIndex);

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
    }

    virtual void paintGL()override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shdr.active(DiShdrFace);

        m_shdr.setLightAt(m_lightAt);
        m_shdr.setProjMat(m_view.getProjMat());
        m_shdr.setViewMat(m_view.getViewMat());

        for (int i = 0; i < m_geam.size(); i++)
        {
            m_shdr.setSurfAt(m_geam[i].at);
            m_shdr.setModelMat(m_geam[i].pGeam->getXform());
            m_faceVbo[m_geam[i].pGeam->type()]->renderSurf(m_shdr);
        }
    }

    virtual void wheelEvent(QWheelEvent *event)
    {
        float r = event->delta() > 0 ? 1.1f : 1.0f / 1.1f;

        m_view.zoom(event->x(), event->y(), r);

        update();

        __super::wheelEvent(event);
    }

    virtual void mousePressEvent(QMouseEvent *event)
    {
        m_rotViewSrc = event->localPos();

        m_rotViewMat = m_view.getViewMat();

        m_beginRot = true;

        __super::mousePressEvent(event);
    }

    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        if (m_beginRot)
        {

            m_view.rotate(m_rotViewMat, m_rotViewSrc, event->localPos());

            update();
        }
//         else
//         {
//             QVector3D pnt, dir;
//             float param;
// 
//             m_view.dev2Wld(event->localPos(), pnt, dir);
// 
//             bool isect = m_geam->isect(pnt, dir, param);
//             if (isect)
//             {
//                 m_surfAt.front_color = QVector3D(1.0f, 1.0f, 0.0f);
//             }
//             else
//             {
//                 m_surfAt.front_color = QVector3D(1.0f, 0.0f, 0.0f);
//             }
// 
//             if (isect != m_isect)
//             {
//                 m_isect = isect;
//                 update();
//             }
//         }

        __super::mouseMoveEvent(event);
    }

    virtual void mouseReleaseEvent(QMouseEvent *event)
    {
        m_beginRot = false;
    }

private:
    QVector<MyGeam> m_geam;
    DiVBO *m_faceVbo[VeGeTypeLast];
    DiLightAt m_lightAt;
    DiShdr m_shdr;

    /* view */
    DiView m_view;

    /* rotate */
    QMatrix4x4 m_rotViewMat;
    QPointF m_rotViewSrc;
    bool m_beginRot;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainwin;
    mainwin.setCentralWidget(new MyDrawing(&mainwin));
    mainwin.show();

    return a.exec();
}

