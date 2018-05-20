#include <QApplication>

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVector>
#include <QVector3D>
#include <QWheelEvent>

#include "VxDisp.h"

class MyDrawing : public QOpenGLWidget, public QOpenGLFunctions
{
public:
    MyDrawing(QWidget* parent)
        : QOpenGLWidget(parent)
    {
        m_beginRot = false;

        m_surfAt.front_color = QVector3D(1.0f, 0.0f, 0.0f);
        m_surfAt.back_color = QVector3D(0.0f, 1.0f, 0.0f);
        m_surfAt.diffuse = 0.1;
        m_surfAt.specular = 0.4;
        m_surfAt.shininess = 10;

        m_lightAt.ambient = 0.5;
        m_lightAt.color = QVector3D(1.0f, 1.0f, 1.0f);
        m_lightAt.direction = QVector3D(1.0f, 1.0f, 1.0f);

        m_wireAt.color = QVector3D(0.0f, 0.0f, 0.0f);
    }

    ~MyDrawing()
    {
        delete m_faceVbo;
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
        faceVertex.push_back(QVector3D(0.0f, 0.0f, 0.0f));
        faceVertex.push_back(QVector3D(-1.0f, -1.0f, -1.0f).normalized());
        faceVertex.push_back(QVector3D(0.0f, 1.0f, 0.0f));
        faceVertex.push_back(QVector3D(0.0f, 1.0f, 0.0f));
        faceVertex.push_back(QVector3D(1.0f, 0.0f, 0.0f));
        faceVertex.push_back(QVector3D(1.0f, 0.0f, 0.0f));
        faceVertex.push_back(QVector3D(0.0f, 0.0f, 1.0f));
        faceVertex.push_back(QVector3D(0.0f, 0.0f, 1.0f));

        QVector<short> faceIndex;
        faceIndex.push_back(0);
        faceIndex.push_back(1);
        faceIndex.push_back(2);
        faceIndex.push_back(0);
        faceIndex.push_back(2);
        faceIndex.push_back(3);
        faceIndex.push_back(0);
        faceIndex.push_back(2);
        faceIndex.push_back(3);
        faceIndex.push_back(0);
        faceIndex.push_back(3);
        faceIndex.push_back(1);
        faceIndex.push_back(1);
        faceIndex.push_back(3);
        faceIndex.push_back(2);
        m_faceVbo = new DiVBO(faceVertex, faceIndex);

        QVector<QVector3D> wireVertex;
        wireVertex.push_back(QVector3D(0.0f, 0.0f, 0.0f));
        wireVertex.push_back(QVector3D(0.0f, 1.0f, 0.0f));
        wireVertex.push_back(QVector3D(1.0f, 0.0f, 0.0f));
        wireVertex.push_back(QVector3D(0.0f, 0.0f, 1.0f));

        QVector<short> wireIndex;
        wireIndex.push_back(0);
        wireIndex.push_back(1);
        wireIndex.push_back(0);
        wireIndex.push_back(2);
        wireIndex.push_back(0);
        wireIndex.push_back(3);
        wireIndex.push_back(1);
        wireIndex.push_back(2);
        wireIndex.push_back(1);
        wireIndex.push_back(3);
        wireIndex.push_back(2);
        wireIndex.push_back(3);
        m_wireVbo = new DiVBO(wireVertex, wireIndex);

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);
    }

    virtual void paintGL()override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shdr.active(DiShdrFace);

        m_shdr.setLightAt(m_lightAt);
        m_shdr.setSurfAt(m_surfAt);
        m_shdr.setProjMat(m_view.getProjMat());
        m_shdr.setViewMat(m_view.getViewMat());

        m_faceVbo->renderSurf(m_shdr);

        m_shdr.active(DiShdrWire);

        m_shdr.setWireAt(m_wireAt);
        m_shdr.setProjMat(m_view.getProjMat());
        m_shdr.setViewMat(m_view.getViewMat());

        m_wireVbo->renderWire(m_shdr);
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
        m_rotViewSrc = QPointF(event->localPos().x(), m_view.getHeight() - event->localPos().y());

        m_rotViewMat = m_view.getViewMat();

        m_beginRot = true;

        __super::mousePressEvent(event);
    }

    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        if (m_beginRot)
        {
            QPointF rotViewDes = QPointF(event->localPos().x(), m_view.getHeight() - event->localPos().y());

            m_view.rotate(m_rotViewMat, m_rotViewSrc, rotViewDes);

            update();
        }

        __super::mouseMoveEvent(event);
    }

    virtual void mouseReleaseEvent(QMouseEvent *event)
    {
        m_beginRot = false;
    }

private:
    DiVBO *m_faceVbo;
    DiSurfAt m_surfAt;

    DiVBO *m_wireVbo;
    DiWireAt m_wireAt;

    DiLightAt m_lightAt;

    DiShdr m_shdr;

    DiView m_view;

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
