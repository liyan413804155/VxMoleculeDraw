#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVector>
#include <QVector3D>

#include "VxDisp.h"

class MyDrawing : public QOpenGLWidget, public QOpenGLFunctions
{
public:
    MyDrawing(QWidget* parent)
        : QOpenGLWidget(parent)
    {

    }

    ~MyDrawing()
    {
        delete m_vbo;
    }

private:
    virtual void resizeGL(int w, int h)override
    {

    }

    virtual void initializeGL()override
    {
        initializeOpenGLFunctions();

        QVector<QVector3D> vertex;
        vertex.push_back(QVector3D(0.0f, 0.0f, 0.0f));
        vertex.push_back(QVector3D(0.0f, 0.5f, 0.0f));
        vertex.push_back(QVector3D(0.5f, 0.0f, 0.0f));

        m_vbo = new DiVBO(vertex);

        diShdrActive(DiShdrFace);
    }

    virtual void paintGL()override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        m_vbo->renderSurf(m_surfAt);
    }

private:
    DiVBO *m_vbo;
    DiSurfAt m_surfAt;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainwin;
    mainwin.setCentralWidget(new MyDrawing(&mainwin));
    mainwin.show();

    return a.exec();
}
