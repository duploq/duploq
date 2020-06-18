#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>


struct EngineOptions
{
    int minLineLength = 1;
    int minBlockSize = 10;
};


class Engine : public QObject
{
    Q_OBJECT

public:
    explicit Engine(QObject *parent = nullptr);

    bool start(const QString &fileListPath, const QString &fileResultPath);

    const EngineOptions& getOptions() const;
    void setOptions(const EngineOptions &options);

    void storeConfig(QSettings &set);
    void restoreConfig(QSettings &set);

Q_SIGNALS:
     void processOutput(const QString &output);
     void errorOutput(const QString &output);
     void completed();
     void failed();

private Q_SLOTS:
    void onProcessOutput();
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess m_engineProcess;
    EngineOptions m_engineOptions;
};

#endif // ENGINE_H
