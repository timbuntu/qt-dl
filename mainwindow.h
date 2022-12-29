#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget* mainWidget = nullptr;
    QVBoxLayout* vLayout = nullptr;
    QHBoxLayout* hLayout = nullptr;
    QLineEdit* urlEdit = nullptr;
    QGroupBox* vGroupBox = nullptr;
    QGroupBox* hGroupBox = nullptr;
    QButtonGroup* buttons = nullptr;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
