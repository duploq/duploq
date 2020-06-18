#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <QString>
#include <QSettings>


struct InputOptions
{
    bool dirsRecursive = true;
    QString fileFilters;
};


class InputProcessor
{
public:
    InputProcessor();

    const InputOptions& getOptions() const				{ return m_inputOptions; }
    void setOptions(const InputOptions &options)		{ m_inputOptions = options; }

    void storeConfig(QSettings &set);
    void restoreConfig(QSettings &set);

    QStringList createFilelist(const QString &dirPath) const;

private:
    InputOptions m_inputOptions;
};

#endif // INPUTPROCESSOR_H
