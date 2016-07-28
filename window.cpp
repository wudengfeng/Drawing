#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>
#include "window.h"


Window::Window(QWidget *parent, bool fs)
    : QOpenGLWidget(parent)
    , fullscreen(fs)
    , scribbling(false)
    , rightPress(false)
    , isSetColor(false)
    , rightChangedColor(0)
{
    setupUI();
    setGeometry(100, 100, 800, 600);


    connect(this, SIGNAL(grabSignal()), this, SLOT(grabWindow()));
    connect(penColorButton, SIGNAL(clicked(bool)), this, SLOT(setColor()));
}

void Window::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    loadGLTextures();

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH_HINT);
    glEnable(GL_LINE_SMOOTH_HINT);
}

void Window::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawBackGround();
    drawLine();

    update();
}

void Window::drawLine()
{
    glLoadIdentity();
    for (int i = 0; i < allLines.size(); i++) {
        glLineWidth(penWidths.at(i));
        glColor3f(penColor.at(i).red() / 255.0, penColor.at(i).green() / 255.0, penColor.at(i).blue() / 255.0);

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < allLines.at(i).size() - 1; j = j + 2) {
            glVertex3f(allLines.at(i).at(j).x(), allLines.at(i).at(j).y(), 0);
            glVertex3f(allLines.at(i).at(j + 1).x(), allLines.at(i).at(j + 1).y(), 0);
        }
        glEnd();
    }
}

void Window::drawBackGround()
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);     glVertex3f(-1, -1, 0);
        glTexCoord2f(1, 0);     glVertex3f(1, -1, 0);
        glTexCoord2f(1, 1);     glVertex3f(1, 1, 0);
        glTexCoord2f(0, 1);     glVertex3f(-1, 1, 0);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Window::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }
    glViewport(100, 100, (GLint)width, (GLint)height);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F11:
        fullscreen = ! fullscreen;
        if (fullscreen) {
            showFullScreen();
        } else {
            showNormal();
            setGeometry(100, 100, 800, 600);
        }
        update();
        break;
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}

void Window::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QVector<QPointF> allPoint;
        lastPoint = event->pos();

        QPointF destPoint = translateGLPoint(lastPoint);
        allPoint.push_back(destPoint);
        allLines.push_back(allPoint);
        penWidths.push_back(penWidthSpinBox->value());
        if (!rightPress && !isSetColor) {
            if (penColor.size() != 0) {
                penColor.push_back(penColor.at(penColor.size() - 1));
            } else {
                penColor.push_back(QColor(0, 0, 0));
            }
        }
        rightPress = false;
        isSetColor = false;
        scribbling = true;
    } else if (event->button() == Qt::MidButton) {
        emit grabSignal();
    } else if (event->button() == Qt::RightButton) {
        rightPress = true;
        (++rightChangedColor) %= 5;
        switch (rightChangedColor) {
        case 0://black
            penColor.push_back(QColor(0, 0, 0));
            break;
        case 1://red
            penColor.push_back(QColor(255, 0, 0));
            break;
        case 2://green
            penColor.push_back(QColor(0, 255, 0));
            break;
        case 3://blue
            penColor.push_back(QColor(0, 0, 255));
            break;
        case 4://white
            penColor.push_back(QColor(255, 255, 255));
            break;
        }
    }
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton ) && scribbling) {
        currentPoint = event->pos();

        QPointF destPoint = translateGLPoint(currentPoint);
        QVector<QPointF> allPoint = allLines.at(allLines.size() - 1);

        allPoint.push_back(destPoint);
        allLines.pop_back();
        allLines.push_back(allPoint);
    }
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        currentPoint = event->pos();
        QPointF destPoint = translateGLPoint(currentPoint);
        QVector<QPointF> allPoint = allLines.at(allLines.size() - 1);

        allPoint.push_back(destPoint);
        allLines.pop_back();
        allLines.push_back(allPoint);

        lastPoint = currentPoint;
        scribbling = false;
    }
}

void Window::enterEvent(QEvent *)
{
    QPixmap pixmap(":/resources/cursor.png");
    pixmap = pixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio);

    QCursor cursor = QCursor(pixmap
                             , this->cursor().hotSpot().x()
                             , this->cursor().hotSpot().y() + 50);
    setCursor(cursor);
}

void Window::leaveEvent(QEvent *)
{
    QCursor cursor = QCursor(Qt::ArrowCursor);
    setCursor(cursor);
}

void Window::setupUI()
{
    penWidthLabel = new QLabel(tr("pen width : "), this);
    penWidthLabel->setGeometry(QRect(10, 10, 70, 20));

    penWidthSpinBox = new MySpinBox(this);
    penWidthSpinBox->setRange(1, 10);
    penWidthSpinBox->setValue(5);
    penWidthSpinBox->setGeometry(QRect(80, 10, 35, 20));

    QPixmap pixmap;
    pixmap.load(":/resources/color.jpg");
    pixmap = pixmap.scaled(30, 20);

    penColorButton = new QPushButton(this);
    penColorButton->setIcon(pixmap);
    penColorButton->setIconSize(QSize(pixmap.width(), pixmap.height()));
    penColorButton->setGeometry(QRect(120, 10, 30, 20));
}

void Window::loadGLTextures()
{
    QImage *image = new QImage(800, 600, QImage::Format_RGB32);
    image->load(":/resources/Tulips.jpg");
    tex = QGLWidget::convertToGLFormat(*image);
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

QPointF Window::translateGLPoint(QPointF srcPoint)
{
    QPointF destPoint;
    destPoint.setX((srcPoint.x() / 400.0) - 1);
    destPoint.setY(-(srcPoint.y() / 300.0) + 1);

    return destPoint;
}

void Window::setColor()
{
    isSetColor = true;

    QColorDialog colorDialog;
    penColor.push_back(colorDialog.getColor());
}

void Window::grabWindow()
{
    QImage picture = grabFramebuffer();
    picture.save("D:\\abc.png");
}
