#include "mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

// TODO Properly handle
const QString MainWindow::DL_EXE[] = {
    "yt-dlp",
    "youtube-dl"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Add widget as central widget
    this->mainWidget = new QWidget();
    this->mainLayout = new QVBoxLayout(this->mainWidget);
    this->setCentralWidget(this->mainWidget);

    this->initSourceSelection();
    this->initButtons();
    this->dlProcess = new QProcess(this);
}

void MainWindow::initSourceSelection(void) {
    this->sourceGroupBox = new QGroupBox(tr("Download Settings1"));
    this->sourceSelectionLayout = new QVBoxLayout(this->sourceGroupBox);
    this->mainLayout->addWidget(this->sourceGroupBox);
    // Add url field
    this->urlEdit = new QLineEdit();
    this->urlEdit->setPlaceholderText(tr("Url"));
    connect(this->urlEdit, &QLineEdit::editingFinished, this, &MainWindow::updateAvailableQualities);
    this->sourceSelectionLayout->addWidget(this->urlEdit);
    // Add quality selector
    this->qualitySelection = new QComboBox();
    this->qualitySelection->setPlaceholderText(tr("Enter Url first"));
    this->sourceSelectionLayout->addWidget(this->qualitySelection);
    // Add audio only checkbox
    this->audioOnlyCheckbox = new QCheckBox(tr("Audio only"));
    this->sourceSelectionLayout->addWidget(this->audioOnlyCheckbox);
}

void MainWindow::initButtons(void) {
    this->hGroupBox = new QGroupBox(tr("Buttons"));
    this->mainLayout->addWidget(this->hGroupBox);
    this->buttonLayout = new QHBoxLayout(this->hGroupBox);
    this->cancelButton = new QPushButton(tr("Close"));
    this->okButton = new QPushButton(tr("OK"));
    connect(this->cancelButton, &QPushButton::clicked, this, &QMainWindow::close);
    this->buttonLayout->addWidget(this->cancelButton);
    this->buttonLayout->addWidget(this->okButton);
}

void MainWindow::updateAvailableQualities(void) {
    this->qualitySelection->clear();
    this->dlProcess->start(MainWindow::DL_EXE[0], {"-J", this->urlEdit->text()});
    this->dlProcess->waitForFinished();
    QByteArray rawJson = this->dlProcess->readAllStandardOutput();
    QJsonDocument json = QJsonDocument::fromJson(rawJson);
    if(json.isNull()) {
        std::cout << "Failed parsing " << MainWindow::DL_EXE[0].toStdString() << " output:" << std::endl;
        std::cout << rawJson.toStdString() << std::endl;
        std::cout << this->dlProcess->readAllStandardError().toStdString() << std::endl;
        return;
    }
    QJsonArray formats = json.object().value("formats").toArray();
    for(const QJsonValue& format : formats) {
        QJsonObject formatObj = format.toObject();
        QString formatId = formatObj.value("format_id").toString();
        QString container = formatObj.value("ext").toString();
        QString vCodec = formatObj.value("vcodec").toString();
        QString aCodec = formatObj.value("acodec").toString();
        QString resolution = formatObj.value("resolution").toString();
        std::cout << formatId.toStdString() << ": " << vCodec.toStdString() << "+" << aCodec.toStdString() << " " << resolution.toStdString() << std::endl;

        if(vCodec.compare("none")) {
            this->qualitySelection->insertItem(0, resolution + " " + container + " (" + vCodec + ", " + aCodec + ")", formatId);
        }
    }
    this->qualitySelection->setPlaceholderText(nullptr);
}

MainWindow::~MainWindow()
{
}
