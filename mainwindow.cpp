#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QStackedWidget(parent)
    , index(0)
{

    for (int i = 0; i < 10; i++) {
        window[i] = new Window(this);
        this->addWidget(window[i]);
    }
    setCurrentIndex(0);

    this->resize(800, 600);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_PageUp:
            if ((--index) < 0) {
                index = 9;
            }
            this->setCurrentIndex(index);
            break;
        case Qt::Key_PageDown:
            (++index) %= 10;
            this->setCurrentIndex(index);
            break;
    }
}
