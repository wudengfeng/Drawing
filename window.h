#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QImage>
#include <QColor>
#include <QSpinBox>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QColorDialog;
QT_END_NAMESPACE

class MySpinBox : public QSpinBox
{
public:
    MySpinBox(QWidget *parent = 0) : QSpinBox(parent){}
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE
    {
        if (event->key() == Qt::Key_Down) {
            setValue(value() - 1);
        } else if(event->key() == Qt::Key_Up) {
            setValue(value() + 1);
        } else {
            event->ignore();
        }
    }
};

class Window : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Window(QString filename, QWidget *parent = 0, bool fs = false);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int, int) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    void enterEvent(QEvent *) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

private:
    void setupUI();

    void loadGLTextures();
    void drawLine();
    void drawClock();
    void drawBackGround();
    void clearScene();
    void undoStep();
    void redoStep();
    QPointF translateGLPoint(QPointF);

private slots:
    void setColor();
    void grabWindow();

signals:
    void grabSignal();

private:
    QVector<QVector<QPointF>> allLines;
    QVector<int> penWidths;
    QVector<QColor> penColor;

    QVector<QVector<QPointF>> allLinesTemp;
    QVector<int> penWidthsTemp;
    QVector<QColor> penColorTemp;

    QString backgroundImage;
    QImage tex;

    QPointF lastPoint;
    QPointF currentPoint;

    QStringList imageList;
    QVector<QSize> imageSize;
    GLuint texture[2];

    bool fullscreen;
    bool scribbling;
    bool rightPress;
    bool isSetColor;
    int rightChangedColor;

    QLabel *penWidthLabel;
    MySpinBox *penWidthSpinBox;
    QPushButton *penColorButton;
};

#endif // WINDOW_H
