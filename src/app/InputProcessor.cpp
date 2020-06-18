#include "InputProcessor.h"

#include <QDir>
#include <QDirIterator>


InputProcessor::InputProcessor()
{

}


void InputProcessor::storeConfig(QSettings &set)
{
    set.beginGroup("Input");
    set.setValue("Recursive", m_inputOptions.dirsRecursive);
    set.setValue("Filters", m_inputOptions.fileFilters);
    set.endGroup();
}


void InputProcessor::restoreConfig(QSettings &set)
{
    set.beginGroup("Input");
    m_inputOptions.dirsRecursive = set.value("Recursive", m_inputOptions.dirsRecursive).toBool();
    m_inputOptions.fileFilters = set.value("Filters", m_inputOptions.fileFilters).toString();
    set.endGroup();
}


QStringList InputProcessor::createFilelist(const QString &dirPath) const
{
    QStringList filePathList;

    QStringList filters = m_inputOptions.fileFilters.split(' ', QString::SkipEmptyParts);

    QDirIterator dir(dirPath, filters, QDir::Files, QDirIterator::Subdirectories);
    while (dir.hasNext())
    {
        filePathList << dir.filePath();
        dir.next();
    };

    return filePathList;
}

