#include "InputProcessor.h"

#include <QDir>
#include <QDirIterator>
#include <QTextStream>


InputProcessor::InputProcessor()
{

}


void InputProcessor::storeConfig(QSettings &set)
{
    set.beginGroup("Input");
    set.setValue("Recursive", m_inputOptions.dirsRecursive);
    set.setValue("Filters", m_inputOptions.fileFilters);
    set.setValue("IgnoreFilters", m_inputOptions.ignoreFilters);
    set.endGroup();
}


void InputProcessor::restoreConfig(QSettings &set)
{
    set.beginGroup("Input");
    m_inputOptions.dirsRecursive = set.value("Recursive", m_inputOptions.dirsRecursive).toBool();
    m_inputOptions.fileFilters = set.value("Filters", m_inputOptions.fileFilters).toString();
    m_inputOptions.ignoreFilters = set.value("IgnoreFilters", m_inputOptions.ignoreFilters).toString();
    set.endGroup();
}


QStringList InputProcessor::readFileList(const QString &fileListPath) const
{
    QStringList filePathList;

    QFile fil(fileListPath);
    if (fil.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream ts(&fil);
        while (!ts.atEnd())
        {
            QString line = ts.readLine().trimmed();
            if (!line.isEmpty())
                filePathList << line;
        }
    }

    return filePathList;
}


QStringList InputProcessor::createFileList(const QString &dirPath, const QStringList &subdirs) const
{
    QStringList filePathList;

    QStringList filters = m_inputOptions.fileFilters.split(' ', QString::SkipEmptyParts);

    QStringList ignored = m_inputOptions.ignoreFilters.split(' ', QString::SkipEmptyParts);
	QList<QRegExp> ignoreRegExp;
	for (const auto& filter : ignored)
	{
#ifdef Q_OS_WIN32
		QRegExp re(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
#else
		QRegExp re(filter, Qt::CaseSensitive, QRegExp::WildcardUnix);
#endif
		ignoreRegExp << re;
	}

	QStringList dirs;
	if (subdirs.isEmpty())
		dirs << dirPath;
	else
	{
		for (const QString& subPath : subdirs)
		{
			QFileInfo dirInfo(dirPath + "/" + subPath);
			dirs << dirInfo.absoluteFilePath();
		}
	}

	for (const QString& path : dirs)
	{
		QDirIterator dir(
			path,
			filters,
			QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
			m_inputOptions.dirsRecursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);

		while (dir.hasNext())
		{
			QString filePath = dir.next();
			if (filePath.isEmpty())
				break;

			// remove if ignored
			if (ignoreRegExp.size())
			{
				for (const auto& re : ignoreRegExp)
				{
					QString fileName = dir.fileName();
					if (re.indexIn(fileName) >= 0)
						goto _skip;
				}
			}

			filePathList << filePath;

		_skip:;

		}
	}

    return filePathList;
}

