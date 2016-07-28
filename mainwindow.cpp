#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QStackedWidget(parent)
    , index(0)
{
    QStringList list;
    list << ":/resources/Chrysanthemum.jpg" << ":/resources/Desert.jpg"
         << ":/resources/Hydrangeas.jpg" << ":/resources/Jellyfish.jpg"
         << ":/resources/Koala.jpg" << ":/resources/Lighthouse.jpg"
         << ":/resources/Penguins.jpg" << ":/resources/Tulips.jpg";

    for (int i = 0; i < 8; i++) {
        window[i] = new Window(list.at(i), this, false);
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
                index = 7;
            }
            this->setCurrentIndex(index);
            break;
        case Qt::Key_PageDown:
            (++index) %= 8;
            this->setCurrentIndex(index);
            break;
    }
}
