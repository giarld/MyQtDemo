#include "box.h"

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
    Vertex vertexs[] = {
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

    mVertexCount = 36;

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(vertexs, sizeof(Vertex) * mVertexCount);
    mVBO.release();
}
