#include <QApplication>

#include <QMainWindow>
#include <QMap>
#include <QOpenGLWidget>
#include <QVector>
#include <QVector3D>
#include <QWheelEvent>

#include "VxDisp.h"
#include "VxGeam.h"
#include "VxDb.h"

struct MyGeam
{
    VxGeam *pGeam;
    DiSurfAt at;
    bool pick;
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

        m_hiColor = QVector3D(1.0f, 1.0f, 0.0f);

        setMouseTracking(true);

        initDb();
    }

    ~MyDrawing()
    {


    }

private:
    void initDb()
    {
        m_bin = new DbBin("MyDrawing");


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
        m_faceVbo[VeGeSphere] = new DiVBOFace(faceVertex, faceIndex);

        faceVertex.clear();
        faceIndex.clear();
        GeCylinder cylinder;
        cylinder.triFace(faceVertex, faceIndex, 50, 4);
        m_faceVbo[VeGeCylinder] = new DiVBOFace(faceVertex, faceIndex);

        faceVertex.clear();
        faceIndex.clear();
        GeCone cone;
        cone.triFace(faceVertex, faceIndex, 50, 4);
        m_faceVbo[VeGeCone] = new DiVBOFace(faceVertex, faceIndex);

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
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
            if (m_geam[i].pick)
            {
                DiSurfAt at = m_geam[i].at;
                at.front_color = m_hiColor;
                m_shdr.setSurfAt(at);
            }
            else
            {
                m_shdr.setSurfAt(m_geam[i].at);
            }
            m_shdr.setModelMat(m_geam[i].pGeam->getXform());
            m_faceVbo[m_geam[i].pGeam->type()]->render(m_shdr);
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
        else
        {
            QVector3D pnt, dir;

            m_view.dev2Wld(event->localPos(), pnt, dir);

            QMap<float, MyGeam*> isect;
            QVector<bool> savePick(m_geam.size());

            for (int i = 0; i < m_geam.size(); i++)
            {
                float param;
                if (m_geam[i].pGeam->isect(pnt, dir, param))
                {
                    isect[param] = &m_geam[i];
                }
                savePick[i] = m_geam[i].pick;
            }

            for (int i = 0; i < m_geam.size(); i++)
            {
                m_geam[i].pick = false;
            }

            if (isect.size() > 0)
            {
                auto it = isect.begin();
                (*it)->pick = true;
            }

            for (int i = 0; i < m_geam.size(); i++)
            {
                if (m_geam[i].pick != savePick[i])
                {
                    update();
                    break;
                }
            }
        }

        __super::mouseMoveEvent(event);
    }

    virtual void mouseReleaseEvent(QMouseEvent *event)
    {
        m_beginRot = false;
    }

private:
    QVector3D m_hiColor;

    QVector<MyGeam> m_geam;
    DiVBOFace *m_faceVbo[VeGeTypeLast];
    DiLightAt m_lightAt;
    DiShdr m_shdr;

    /* view */
    DiView m_view;

    /* rotate */
    QMatrix4x4 m_rotViewMat;
    QPointF m_rotViewSrc;
    bool m_beginRot;

    DbBin *m_bin;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainwin;
    mainwin.setCentralWidget(new MyDrawing(&mainwin));
    mainwin.show();

    return a.exec();
}

