#include "glwindow.h"

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    this->setFormat(fmt);
    startTimer(1000 / 60);
}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    QOpenGLFunctions *glFunc = this->context()->functions();
    glFunc->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    initShaderProgram();

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mShaderProgram->bind();

    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0, 0, -2);
    matrix.rotate(m_frame_y, 0, 1, 0);
    matrix.rotate(m_frame_x, 1, 0, 0);
    matrix.rotate(m_frame_z, 0, 0, 1);
    mShaderProgram->setUniformValue(mMatrixLoc, matrix);
    mShaderProgram->setUniformValue(mProjLoc, mProjMatrix);

    mBuffer.bind();
    QOpenGLFunctions *f = this->context()->functions();
    f->glVertexAttribPointer(mPosAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    f->glVertexAttribPointer(mColAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D)));
    f->glEnableVertexAttribArray(mPosAttr);
    f->glEnableVertexAttribArray(mColAttr);

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
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "uniform highp mat4 projMatrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = projMatrix * matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";

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
