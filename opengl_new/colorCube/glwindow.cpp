#include "glwindow.h"

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    startTimer(1000 / 60);
}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    QOpenGLFunctions *glFunc = QOpenGLContext::currentContext()->functions();
    glFunc->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    initShaderProgram();

    mVAO.create();

    Vertex vertexs[] = {
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f)},

        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f)},

        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f)},

        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 0.0f)},

        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f)},

        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f)},
        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f)},

        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 1.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(1.0f, 1.0f, 0.0f)},

        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(1.0f, 1.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f)},

        {QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 1.0f)},

        {QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f)},
        {QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(1.0f, 0.0f, 1.0f)},

        {QVector3D(0.5f, 0.5f, 0.5f), QVector3D(0.0f, 1.0f, 1.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.0f, 1.0f, 1.0f)},
        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f)},

        {QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f)},
        {QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.0f, 1.0f, 1.0f)},
        {QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f)},
    };

    mVertexCount = 36;

    mBuffer.create();
    mBuffer.bind();
    mBuffer.allocate(vertexs, sizeof(Vertex) * mVertexCount);
    mBuffer.release();
}

void GLWindow::resizeGL(int w, int h)
{
    mProjMatrix.setToIdentity();
    mProjMatrix.perspective(70.0f, GLfloat(w) / h, 0.001f, 10000.0f);
}

float m_frame_y = 0;
float m_frame_x = 0;
float m_frame_z = 0;

void GLWindow::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);

    mShaderProgram->bind();

    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0, 0, -2);
    matrix.rotate(m_frame_y, 0, 1, 0);
    matrix.rotate(m_frame_x, 1, 0, 0);
    matrix.rotate(m_frame_z, 0, 0, 1);
    mShaderProgram->setUniformValue(mMatrixLoc, matrix);
    mShaderProgram->setUniformValue(mProjLoc, mProjMatrix);

    //巨坑！！当使用compatibility profile，那么默认就有个全局vao 0；如果使用core profile，必须手动bind一个vao。
    QOpenGLVertexArrayObject::Binder binder(&mVAO);

    mBuffer.bind();
    f->glEnableVertexAttribArray(mPosAttr);
    f->glEnableVertexAttribArray(mColAttr);

    f->glVertexAttribPointer(mPosAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    f->glVertexAttribPointer(mColAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D)));

    f->glDrawArrays(GL_TRIANGLES, 0, mVertexCount);

    f->glDisableVertexAttribArray(mColAttr);
    f->glDisableVertexAttribArray(mPosAttr);
    mBuffer.release();

    mShaderProgram->release();

    m_frame_y+=1;
    if(m_frame_y >= 360.0f)
        m_frame_y = 0.0f;

    m_frame_x+=0.5;
    if(m_frame_x >= 360.0f)
        m_frame_x = 0.0f;

    m_frame_z+=0.5;
    if(m_frame_z >= 360.0f)
        m_frame_z = 0.0f;
}

void GLWindow::timerEvent(QTimerEvent *event)
{
    this->update();
}

static const char *vertexShaderSource =
        "#version 330 core\n"
        "in vec4 posAttr;\n"
        "in vec3 colAttr;\n"
        "out vec3 col;\n"
        "uniform mat4 matrix;\n"
        "uniform mat4 projMatrix;\n"
        "void main()\n"
        "{\n"
        "    col = colAttr;\n"
        "    gl_Position = projMatrix * matrix * posAttr;\n"
        "}";


static const char *fragmentShaderSource =
        "#version 330 core\n"
        "in vec3 col;\n"
        "out vec4 fragColor;\n"
        "void main()\n"
        "{\n"
        "   fragColor = vec4(col, 1.0);\n"
        "}";


void GLWindow::initShaderProgram()
{
    mShaderProgram = new QOpenGLShaderProgram(this);
    mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mShaderProgram->link();

    mShaderProgram->bind();
    mPosAttr = mShaderProgram->attributeLocation("posAttr");
    mColAttr = mShaderProgram->attributeLocation("colAttr");
    mMatrixLoc = mShaderProgram->uniformLocation("matrix");
    mProjLoc = mShaderProgram->uniformLocation("projMatrix");
    mShaderProgram->release();
}
