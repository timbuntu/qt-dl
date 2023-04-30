#include "mainwindow.h"
#include <QApplication>
#include <QEventLoop>
#include <QStatusBar>
#include <iostream>
#include <QFileDialog>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Add widget as central widget
    this->mainWidget = new QWidget();
    this->mainLayout = new QVBoxLayout(this->mainWidget);
    this->setCentralWidget(this->mainWidget);

    this->initSourceSelection();
    this->initButtons();
    this->initProgressFooter();

    this->downloadThread = new QThread(this);
    this->downloader = new Downloader(this->statusBar(), this->progressBar, this);
    this->downloader->moveToThread(this->downloadThread);
    connect(this, &MainWindow::urlChanged, this->downloader, &Downloader::fetchFormats, Qt::QueuedConnection);
    connect(this->downloader, &Downloader::formatsFetched, this, &MainWindow::setAvailableFormats, Qt::QueuedConnection);
    connect(this, &MainWindow::startDownload, this->downloader, &Downloader::startDownload, Qt::QueuedConnection);
    this->downloadThread->start();

    this->setMinimumSize(360, 240);
}

void MainWindow::initSourceSelection(void) {
    this->sourceGroupBox = new QGroupBox(tr("Download Settings"));
    this->sourceSelectionLayout = new QVBoxLayout(this->sourceGroupBox);
    this->mainLayout->addWidget(this->sourceGroupBox);
    // Add url field
    this->urlEdit = new QLineEdit();
    this->urlEdit->setPlaceholderText(tr("Url"));
    connect(this->urlEdit, &QLineEdit::textEdited, this, &MainWindow::urlInputChanged);
    connect(this->urlEdit, &QLineEdit::editingFinished, this, &MainWindow::updateUrl);
    this->sourceSelectionLayout->addWidget(this->urlEdit);
    // Add quality selector
    this->qualitySelection = new QComboBox();
    this->qualitySelection->setPlaceholderText(tr("Enter Url first"));
    this->sourceSelectionLayout->addWidget(this->qualitySelection);
    // Add audio only checkbox
    this->audioOnlyCheckbox = new QCheckBox(tr("Audio only"));
    connect(this->audioOnlyCheckbox, &QCheckBox::stateChanged, this, &MainWindow::audioOnlyToggled);
    this->sourceSelectionLayout->addWidget(this->audioOnlyCheckbox);
}

void MainWindow::initButtons(void) {
    this->hGroupBox = new QGroupBox();
    this->mainLayout->addWidget(this->hGroupBox);
    this->buttonLayout = new QHBoxLayout(this->hGroupBox);
    this->cancelButton = new QPushButton(tr("Close"));
    this->okButton = new QPushButton(tr("Download"));
    connect(this->cancelButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(this->okButton, &QPushButton::clicked, this, &MainWindow::requestDownload);
    this->buttonLayout->addWidget(this->cancelButton);
    this->buttonLayout->addWidget(this->okButton);
}

void MainWindow::initProgressFooter(void) {
    this->statusBar()->setSizeGripEnabled(false);
    this->progressBar = new QProgressBar();
    this->statusBar()->addPermanentWidget(this->progressBar);
    this->statusBar()->showMessage(tr("Waiting for URL"));
}

void MainWindow::urlInputChanged(const QString& newText) {
    std::cout << "Url input changed to: " << newText.toStdString() << std::endl;
    if(newText.compare(QGuiApplication::clipboard()->text(QClipboard::Mode::Clipboard)) == 0 || newText.compare(QGuiApplication::clipboard()->text(QClipboard::Mode::Selection)) == 0) {
        this->updateUrl();
    }
}
void MainWindow::updateUrl(void) {
    const QString& url = this->urlEdit->text();
    if(url.isEmpty()) {
        this->downloader->setUrl("");
        this->qualitySelection->clear();
    } else if(this->downloader->getUrl().compare(url) != 0) {
        std::cout << "Emitting urlChanged" << std::endl;
        emit this->urlChanged(url);
    }
}

void MainWindow::setAvailableFormats(const QMap<int, QString> formats) {
    this->qualitySelection->clear();
    std::cout << "Setting " << formats.size() << " available formats..." << std::endl;
    for(auto format = formats.begin(); format != formats.end(); format++) {
        this->qualitySelection->insertItem(0, format.value(), format.key());
    }
    this->qualitySelection->setPlaceholderText(nullptr);
}

void MainWindow::audioOnlyToggled(int state) {
    if(state == Qt::CheckState::Checked) {
        this->qualitySelection->setDisabled(true);
    } else {
        this->qualitySelection->setDisabled(false);
    }
}

void MainWindow::requestDownload(void) {
    std::cout << "Download requested" << std::endl;
    this->outputDir = QFileDialog::getExistingDirectory(this, tr("Save location"), this->outputDir);
    emit this->startDownload(this->qualitySelection->currentData().toInt(), this->audioOnlyCheckbox->isChecked(), this->outputDir);
}

MainWindow::~MainWindow()
{
}
