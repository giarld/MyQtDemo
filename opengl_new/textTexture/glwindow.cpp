#include "glwindow.h"
#include <QDebug>
#include <QPainter>
#include <QDateTime>

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
    , mTexture(Q_NULLPTR)
{
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    this->setFormat(fmt);
    startTimer(1000 / 60);

    rot = false;
    initUi();

    this->setGeometry(300, 200, 640, 480);
}

GLWindow::~GLWindow()
{
    if(mTexture != Q_NULLPTR) {
        mTexture->destroy();
        delete mTexture;
    }
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

//    mTexture = genTexture(1024, 1024, "Gxin你好世界，欢迎来到OpenGL世界。", 150, Qt::red);
    QString date = QDateTime::currentDateTime().toString("现在时间是：yyyy-MM-dd hh:mm:ss.zzz");
    mTexture = genTexture(512, 512, date, 60, Qt::red);
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
}

void GLWindow::timerEvent(QTimerEvent *event)
{
    if(rot) {
        m_frame++;
        if(m_frame >= 360.0f)
            m_frame = 0.0f;
    }

    if(mTexture != Q_NULLPTR) {
        mTexture->destroy();
        delete mTexture;
    }

    QString date = QDateTime::currentDateTime().toString("现在时间是：yyyy-MM-dd hh:mm:ss.zzz");

    mTexture = genTexture(512, 512, date, 60, Qt::red);
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
        "   highp float alpha = texColor.a;\n"
        "   if(alpha < 0.2)\n"
        "       discard;\n"
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

void GLWindow::initUi()
{
    mButton = new QPushButton("旋转", this);
#ifdef __ANDROID__
    mButton->setGeometry(0, 0, 300, 100);
#endif
    connect(mButton, &QPushButton::clicked, this, &GLWindow::buttonClicked);
}

QOpenGLTexture *GLWindow::genTexture(int width, int height, const QString &text, int textPixelSize, const QColor &textColor)
{
    QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor(0, 0, 0, 0));

    QPainter painter;

    QFont font;
    painter.begin(&img);
    font.setPixelSize(textPixelSize);
    painter.setFont(font);
    QPen pen;
    pen.setColor(textColor);
    painter.setPen(pen);
    QTextOption option(Qt::AlignLeft | Qt::AlignTop);
    option.setWrapMode(QTextOption::WordWrap);
    QRectF rect(0, 0, width, height);
    painter.drawText(rect, text, option);
    painter.end();

    //    int mipLevelMax = width <= height ? log(width)/log(2) : log(height)/log(2);

    texture->setData(img);
    //    texture->setMipLevelRange(0, mipLevelMax);    //off mipmap
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}

void GLWindow::buttonClicked()
{
    if(!rot) {
        rot = true;
        mButton->setText("停止旋转");
    } else {
        rot = false;
        mButton->setText("旋转");
    }
}
