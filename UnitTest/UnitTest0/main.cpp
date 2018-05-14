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
    }

    ~MyDrawing()
    {
        delete m_vbo;
    }

private:
    virtual void resizeGL(int w, int h)override
    {
        m_view.reisze(w, h);
    }

    virtual void initializeGL()override
    {
        initializeOpenGLFunctions();

        QVector<QVector3D> vertex;
        vertex.push_back(QVector3D(0.0f, 0.0f, 0.0f));
        vertex.push_back(QVector3D(0.0f, 0.5f, 0.0f));
        vertex.push_back(QVector3D(0.5f, 0.0f, 0.0f));

        m_vbo = new DiVBO(vertex);
    }

    virtual void paintGL()override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        m_shdr.active(DiShdrFace);

        m_shdr.setProjMat(m_view.getProjMat());
        m_shdr.setViewMat(m_view.getViewMat());

        m_vbo->renderSurf(m_shdr);
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
    DiVBO *m_vbo;
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
