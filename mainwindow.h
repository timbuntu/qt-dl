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
#include <QProgressBar>
#include <QThread>

#include "downloader.h"

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
    QProgressBar* progressBar = nullptr;
    Downloader* downloader = nullptr;
    QThread* downloadThread = nullptr;
    QString outputDir = "";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void urlInputChanged(const QString& newText);
    void updateUrl(void);
    void setAvailableFormats(const QMap<int, QString> formats);
    void audioOnlyToggled(int state);
    void requestDownload(void);

signals:
    void urlChanged(const QString& url);
    void startDownload(const int format, const bool audioOnly, const QString destFolder);

private:
    void initSourceSelection(void);
    void initButtons(void);
    void initProgressFooter(void);
};
#endif // MAINWINDOW_H
