#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QStatusBar>
#include <QProgressBar>
#include <QProcess>
#include <QMap>

class Downloader : public QObject
{
    Q_OBJECT

    QProcess* dlProcess = nullptr;
    QString url;
    unsigned long received_percent;

    static const QString DL_EXE[];

public:
    explicit Downloader(QObject *parent = nullptr);
    QString getUrl(void)  {return this->url;}

public slots:
    void setUrl(const QString& url) {this->url = url;}
    void fetchFormats(const QString& url);
    void startDownload(const int format, const bool audioOnly, const QString& destFolder);

private slots:
    void processOutputReady(void);
    void downloadFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void statusRunning(bool running);
    void showStatusMessage(const QString& message);
    void formatsFetched(const QMap<int, QString> formats);
    void progressUpdate(const int newPercentage);
    void finished(void);
};

#endif // DOWNLOADER_H
