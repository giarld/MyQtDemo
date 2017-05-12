#ifndef BOX_H
#define BOX_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QVector3D>

class Box : public QObject
{
    Q_OBJECT
public:
    explicit Box(QObject *parent = 0);
    ~Box();

    void bind();
    void release();

    GLuint getVertexCount();

private:
    void build();

private:
    QOpenGLBuffer mVBO;
    GLuint mVertexCount;
};

struct Vertex
{
  QVector3D position;
  QVector3D color;
  QVector3D normal;
};

#endif // BOX_H
