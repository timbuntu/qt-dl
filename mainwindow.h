#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QProcess>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget* mainWidget = nullptr;
    QVBoxLayout* mainLayout = nullptr;
    QVBoxLayout* sourceSelectionLayout = nullptr;
    QHBoxLayout* buttonLayout = nullptr;
    QLineEdit* urlEdit = nullptr;
    QComboBox* qualitySelection = nullptr;
    QCheckBox* audioOnlyCheckbox = nullptr;
    QGroupBox* sourceGroupBox = nullptr;
    QGroupBox* hGroupBox = nullptr;
    QButtonGroup* buttons = nullptr;
    QPushButton* okButton = nullptr;
    QPushButton* cancelButton = nullptr;
    QProcess* dlProcess = nullptr;

    static const QString DL_EXE[];

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateAvailableQualities(void);

private:
    void initSourceSelection(void);
    void initButtons(void);
};
#endif // MAINWINDOW_H
