#include "downloader.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QApplication>

// TODO Properly handle
const QString Downloader::DL_EXE[] = {
    "yt-dlp",
    "youtube-dl"
};

Downloader::Downloader(QStatusBar* statusBar, QProgressBar* progressBar, QObject *parent)
    : QObject{parent}
{
    this->statusBar = statusBar;
    this->progressBar = progressBar;
}

void Downloader::fetchFormats(const QString& url) {
    if(this->dlProcess == nullptr) {
        this->dlProcess = new QProcess(this);
    }
    this->url = url;
    QMap<int, QString> availableFormats;
    this->dlProcess->start(Downloader::DL_EXE[0], {"-J", url});
    this->statusBar->showMessage(tr("Fetching available formats"));
    this->progressBar->setRange(0, 0);
    this->dlProcess->waitForFinished();
    QByteArray rawJson = this->dlProcess->readAllStandardOutput();
    QJsonDocument json = QJsonDocument::fromJson(rawJson);
    if(json.isNull()) {
        std::cout << "Failed parsing " << Downloader::DL_EXE[0].toStdString() << " output:" << std::endl;
        std::cout << rawJson.toStdString() << std::endl;
        std::cout << this->dlProcess->readAllStandardError().toStdString() << std::endl;
        this->statusBar->showMessage(tr("Error fetching"));
        this->progressBar->setRange(0, 100);
        return;
    }
    QJsonArray formats = json.object().value("formats").toArray();
    for(const QJsonValue& format : formats) {
        QJsonObject formatObj = format.toObject();
        QString resolution = formatObj.value("resolution").toString();
        // TODO Don't skip lower resolutions if there is no higher one
        if(strtol(resolution.toLocal8Bit(), NULL, 10) < 1280)
            continue;
        int formatId = formatObj.value("format_id").toString().toInt();
        QString container = formatObj.value("ext").toString();
        QString vCodec = formatObj.value("vcodec").toString();
        QString aCodec = formatObj.value("acodec").toString();
        std::cout << formatId << ": " << vCodec.toStdString() << "+" << aCodec.toStdString() << " " << resolution.toStdString() << std::endl;

        availableFormats.insert(formatId, resolution + " " + container + " (" + vCodec + ", " + aCodec + ")");
    }
    this->progressBar->setRange(0, 100);
    this->statusBar->showMessage(tr("Ready"));

    emit this->formatsFetched(availableFormats);
}

void Downloader::startDownload(const int format, const bool audioOnly, const QString& destFolder) {
    QStringList args;
    this->progressBar->setRange(0, 0);
    args << "-q" << "--progress" << "--progress-template='%(progress.downloaded_bytes)s / %(progress.total_bytes)s'";
    args << "--no-colors" << "-f" << QString::number(format) + "+bestaudio" << "-o" << destFolder + "/%(title)s.%(ext)s";
    args << this->url;
    if(audioOnly)
        args << "-x";

    std::cout << "Starting download with following command:" << std::endl;
    std::cout << Downloader::DL_EXE[0].toStdString() << " ";
    for(QString& arg : args) {
        std::cout << arg.toStdString() << " ";
    }
    std::cout << std::endl << std::endl;
    connect(this->dlProcess, &QProcess::readyReadStandardOutput, this, &Downloader::processOutputReady);
    connect(this->dlProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(downloadFinished(int,QProcess::ExitStatus)));
    this->dlProcess->start(Downloader::DL_EXE[0], args);
    this->statusBar->showMessage(tr("Downloading"));
    std::cout << "Downloading..." << std::endl;
    this->progressBar->setRange(0, 100);
}

void Downloader::processOutputReady(void) {
    QByteArray rawJson = this->dlProcess->readLine(2048);
    int received = strtoul(rawJson.data()+2, nullptr, 10);
    int total = strtol(rawJson.data() + rawJson.indexOf('/') + 2, nullptr, 10);
    unsigned long current_percent = (received / (double)total) * 100;
    std::cout << "Received " << current_percent << "% = " << received << " / " << total << std::endl;
    if(this->received_percent != current_percent) {
        this->progressBar->setValue(current_percent);
        this->received_percent = current_percent;
    }
    QApplication::processEvents();
}

void Downloader::downloadFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    this->statusBar->showMessage(tr("Download finished"));
    this->progressBar->setValue(this->progressBar->maximum());
    emit this->finished();
}
