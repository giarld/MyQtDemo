#include "glwindow.h"
#include <QOpenGLVertexArrayObject>

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
        "   highp float alpha = texColor.a;\n"
        "   if(alpha < 0.2)\n"
        "       discard;\n"
        "   highp vec3 color = vec3(1.0,1.0,1.0);\n"
        "   gl_FragColor = texColor * vec4(color, 1.0);\n"
        "}\n";


GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    // support opengl 3.0+
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject(this);
    vao->create();
    vao->bind();

    initShaderProgram();
}

void GLWindow::resizeGL(int w, int h)
{
    mProjMatrix.setToIdentity();
    mProjMatrix.perspective(70.0f, GLfloat(w) / h, 0.001f, 10000.0f);
}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
}

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
