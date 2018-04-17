#include "box.h"
#include <QVector>

Box::Box(QObject *parent) : QObject(parent)
{
    build();
}

Box::~Box()
{
    if(mVBO.isCreated())
        mVBO.destroy();
}

void Box::bind()
{
    if(mVBO.isCreated())
        mVBO.bind();
}

void Box::release()
{
    if(mVBO.isCreated()) {
        mVBO.release();
    }
}

GLuint Box::getVertexCount()
{
    return this->mVertexCount;
}

void Box::build()
{
    Vertex boxVertexs[] = {
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)},

        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)},

        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)},
        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)},

        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)},

        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)},

        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)},

        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)},

        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)},

        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},

        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},

        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)},

        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)},
    };
    int boxVertexCount = 36;

    Vertex floorVertexs[] = {
        {QVector3D(-2, -1 , -2), QVector3D(1, 1, 1), QVector3D(0, 1, 0)},
        {QVector3D(-2, -1 , 2), QVector3D(1, 1, 1), QVector3D(0, 1, 0)},
        {QVector3D(2, -1 , -2), QVector3D(1, 1, 1), QVector3D(0, 1, 0)},

        {QVector3D(2, -1 , -2), QVector3D(1, 1, 1), QVector3D(0, 1, 0)},
        {QVector3D(-2, -1 , 2), QVector3D(1, 1, 1), QVector3D(0, 1, 0)},
        {QVector3D(2, -1 , 2), QVector3D(1, 1, 1), QVector3D(0, 1, 0)},

        {QVector3D(-2, 2 , 2), QVector3D(1, 1, 1), QVector3D(1, 0, 0)},
        {QVector3D(-2, -1 , 2), QVector3D(1, 1, 1), QVector3D(1, 0, 0)},
        {QVector3D(-2, 2 , -2), QVector3D(1, 1, 1), QVector3D(1, 0, 0)},

        {QVector3D(-2, 2 , -2), QVector3D(1, 1, 1), QVector3D(1, 0, 0)},
        {QVector3D(-2, -1 , 2), QVector3D(1, 1, 1), QVector3D(1, 0, 0)},
        {QVector3D(-2, -1 , -2), QVector3D(1, 1, 1), QVector3D(1, 0, 0)},
    };
    int floorVertexCount = 12;

    QVector<Vertex> vertexs;
    for(int i = 0; i < boxVertexCount; i++) {
        boxVertexs[i].color = QVector3D(1, 1, 1);
        vertexs.append(boxVertexs[i]);
    }
    for(int i = 0; i < floorVertexCount; i++) {
        vertexs.append(floorVertexs[i]);
    }

    mVertexCount = boxVertexCount + floorVertexCount;

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(vertexs.data(), sizeof(Vertex) * mVertexCount);
    mVBO.release();
}
