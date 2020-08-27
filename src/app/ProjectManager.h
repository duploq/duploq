#pragma once

#include <QString>
#include <QStringList>
#include <QSettings>

class Engine;
class InputProcessor;


class ProjectManager
{
public:
	ProjectManager(InputProcessor &input, Engine &engine);

	bool hasProject() const							{ return m_projectFilePath.size(); }
	const QString& getCurrentProject() const		{ return m_projectFilePath; }
	const QStringList& getSubdirectories() const	{ return m_subdirs; }
	void setSubdirectories(const QStringList& subdirs);

	bool createProject(const QString& projectFilePath);
	bool openProject(const QString& projectFilePath);
	bool saveCurrentProject();
	void closeCurrentProject();

protected:
	void storeConfig(QSettings &set);
	void restoreConfig(QSettings &set);

private:
	QString m_projectFilePath;
	QStringList m_subdirs;

	Engine *m_engine = nullptr;
	InputProcessor *m_inputProcessor = nullptr;
};
