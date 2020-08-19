#include "Engine.h"


Engine::Engine(QObject *parent) : QObject(parent)
{
    connect(&m_engineProcess, &QProcess::readyReadStandardOutput, this, &Engine::onProcessOutput);
    connect(&m_engineProcess, &QProcess::errorOccurred, this, &Engine::onProcessError);
    connect(&m_engineProcess,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),    // ugly workaround for compiler crap
            this, &Engine::onProcessFinished);
}


bool Engine::start(const QString &fileListPath, const QString &fileResultPath)
{
    if (fileListPath.isEmpty() || fileResultPath.isEmpty())
        return false;

    QStringList argz;
    argz << "-xml";
    argz << "-ip";
    argz << "-pt10";
    argz << "-ml" << QString::number(m_engineOptions.minBlockSize);
    argz << "-mc" << QString::number(m_engineOptions.minLineLength);
    argz << fileListPath;
    argz << fileResultPath;
#ifdef Q_OS_WINDOWS
    m_engineProcess.start("duplo.exe", argz);
#else
    m_engineProcess.start("duplo", argz);
#endif
    return true;
}


const EngineOptions &Engine::getOptions() const
{
    return m_engineOptions;
}


void Engine::setOptions(const EngineOptions &options)
{
    m_engineOptions = options;
}


void Engine::storeConfig(QSettings &set)
{
    set.beginGroup("Engine");
    set.setValue("MinBlockSize", m_engineOptions.minBlockSize);
    set.setValue("MinLineLength", m_engineOptions.minLineLength);
    set.endGroup();
}


void Engine::restoreConfig(QSettings &set)
{
    set.beginGroup("Engine");
    m_engineOptions.minBlockSize = set.value("MinBlockSize", m_engineOptions.minBlockSize).toInt();
    m_engineOptions.minLineLength = set.value("MinLineLength", m_engineOptions.minLineLength).toInt();
    set.endGroup();
}


void Engine::onProcessOutput()
{
    QString output = QString::fromLatin1( m_engineProcess.readAllStandardOutput() );

    Q_EMIT processOutput(output);
}


void Engine::onProcessError(QProcess::ProcessError error)
{
    QString output = tr("Cannot run the engine, error: %1").arg((int)error);

    Q_EMIT errorOutput(output);

    if (m_engineProcess.state() == QProcess::NotRunning)
    {
        Q_EMIT failed();
    }
}


void Engine::onProcessFinished(int /*exitCode*/, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
        Q_EMIT completed();
    else
        Q_EMIT failed();
}
