#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <QStringList>
#include <QSettings>


struct InputOptions
{
    bool dirsRecursive = true;
    QString fileFilters;
    QString ignoreFilters;
};


class InputProcessor
{
public:
    InputProcessor();

    const InputOptions& getOptions() const				{ return m_inputOptions; }
    void setOptions(const InputOptions &options)		{ m_inputOptions = options; }

    void storeConfig(QSettings &set);
    void restoreConfig(QSettings &set);

	QStringList createFileList(const QString &dirPath, const QStringList &subdirs = {}) const;
    QStringList readFileList(const QString &fileListPath) const;

private:
    InputOptions m_inputOptions;
};

#endif // INPUTPROCESSOR_H
