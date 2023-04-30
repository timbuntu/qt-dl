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
    QStatusBar* statusBar;
    QProgressBar* progressBar;
    QString url;
    unsigned long received_percent;

    static const QString DL_EXE[];

public:
    explicit Downloader(QStatusBar* statusBar, QProgressBar* progressBar, QObject *parent = nullptr);
    QString getUrl(void)  {return this->url;}

public slots:
    void setUrl(const QString& url) {this->url = url;}
    void fetchFormats(const QString& url);
    void startDownload(const int format, const bool audioOnly, const QString& destFolder);

private slots:
    void processOutputReady(void);
    void downloadFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void formatsFetched(QMap<int, QString> formats);
    void finished(void);
};

#endif // DOWNLOADER_H
