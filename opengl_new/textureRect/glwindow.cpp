#include "glwindow.h"
#include <QDebug>

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
    glFunc->glClearColor(0.0f, 0.8f, 1.0f, 1.0f);

    initShaderProgram();

    Vertex vertexs[] = {
        {QVector3D(-0.5f, 0.5f, 0), QVector2D(0.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, 0), QVector2D(0.0f, 1.0f)},
        {QVector3D(0.5f, 0.5f, 0), QVector2D(1.0f, 0.0f)},

        {QVector3D(0.5f, 0.5f, 0), QVector2D(1.0f, 0.0f)},
        {QVector3D(-0.5f, -0.5f, 0), QVector2D(0.0f, 1.0f)},
        {QVector3D(0.5f, -0.5f, 0), QVector2D(1.0f, 1.0f)}
    };

    mBuffer.create();
    mBuffer.bind();
    mBuffer.allocate(vertexs, sizeof(Vertex) * 6);
    mBuffer.release();
    mVertexCount = 6;

    mTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    QImage img;
    if(img.load(":/brick.png")) {
        int imgW = img.width();
        int imgH = img.height();
        int mipLevelMax = imgW <= imgH ? imgW : imgH;

        mTexture->setData(img);
        mTexture->setMipLevelRange(0, mipLevelMax);
        mTexture->setMinificationFilter(QOpenGLTexture::NearestMipMapLinear);
        mTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
        mTexture->setWrapMode(QOpenGLTexture::Repeat);
    }
}

void GLWindow::resizeGL(int w, int h)
{
    mProjMatrix.setToIdentity();
    mProjMatrix.perspective(70.0f, GLfloat(w) / h, 0.001f, 10000.0f);
}

float m_frame = 0;
void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    mShaderProgram->bind();

    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0, 0, -2);
    matrix.rotate(m_frame, 0, 1, 0);
    mShaderProgram->setUniformValue(mMatrixLoc, matrix);
    mShaderProgram->setUniformValue(mProjLoc, mProjMatrix);

    mTexture->bind();
    mBuffer.bind();
    QOpenGLFunctions *f = this->context()->functions();
    f->glVertexAttribPointer(mPosAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    f->glVertexAttribPointer(mTexAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(QVector3D)));
    f->glEnableVertexAttribArray(mPosAttr);
    f->glEnableVertexAttribArray(mTexAttr);

    f->glDrawArrays(GL_TRIANGLES, 0, mVertexCount);

    f->glDisableVertexAttribArray(mTexAttr);
    f->glDisableVertexAttribArray(mPosAttr);
    mBuffer.release();
    mTexture->release();

    mShaderProgram->release();

    m_frame++;
    if(m_frame >= 360.0f)
        m_frame = 0.0f;
}

void GLWindow::timerEvent(QTimerEvent *event)
{
    this->update();
}

static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute highp vec2 texcoord;\n"
        "varying highp vec2 v_texcoord;\n"
        "uniform highp mat4 matrix;\n"
        "uniform highp mat4 projMatrix;\n"
        "void main() {\n"
        "   v_texcoord = texcoord;\n"
        "   gl_Position = projMatrix * matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying highp vec2 v_texcoord;\n"
        "uniform sampler2D texture;\n"
        "void main() {\n"
        "   highp vec4 texColor = texture2D(texture, v_texcoord);\n"
        "   highp vec3 color = vec3(1.0,1.0,1.0);\n"
        "   gl_FragColor = texColor * vec4(color, 1.0);\n"
        "}\n";

void GLWindow::initShaderProgram()
{
    mShaderProgram = new QOpenGLShaderProgram(this);
    mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mShaderProgram->link();

//    mShaderProgram->bind();
    mPosAttr = mShaderProgram->attributeLocation("posAttr");
    mTexAttr = mShaderProgram->attributeLocation("texcoord");
    mMatrixLoc = mShaderProgram->uniformLocation("matrix");
    mProjLoc = mShaderProgram->uniformLocation("projMatrix");
//    mShaderProgram->release();
}
