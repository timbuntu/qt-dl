#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->mainWidget = new QWidget(this);
    this->vLayout = new QVBoxLayout(this->mainWidget);
    this->vGroupBox = new QGroupBox(tr("Download Settings"), this->mainWidget);
    this->hGroupBox = new QGroupBox(tr("Download Settings"), this->mainWidget);
    this->hLayout = new QHBoxLayout(this->hGroupBox);
    this->urlEdit = new QLineEdit(tr("URL"), this->vGroupBox);
}

MainWindow::~MainWindow()
{
}

