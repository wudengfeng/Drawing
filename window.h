#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QImage>
#include <QColor>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSpinBox;
class QColorDialog;
QT_END_NAMESPACE

class Window : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = 0, bool fs = false);

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
    void drawBackGround();
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
    QSpinBox *penWidthSpinBox;
    QPushButton *penColorButton;
};

#endif // WINDOW_H
