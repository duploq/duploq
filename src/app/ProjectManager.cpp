#include "ProjectManager.h"
#include "Engine.h"
#include "InputProcessor.h"

#include <QSettings>
#include <QFile>


ProjectManager::ProjectManager(InputProcessor &input, Engine &engine)
{
	m_engine = &engine;
	m_inputProcessor = &input;
}


bool ProjectManager::createProject(const QString &projectFilePath)
{
	m_projectFilePath = projectFilePath;

	return saveCurrentProject();
}


bool ProjectManager::openProject(const QString &projectFilePath)
{
	if (!QFile::exists(projectFilePath))
		return false;

	saveCurrentProject();

	m_projectFilePath = projectFilePath;

	QSettings dqp(m_projectFilePath, QSettings::IniFormat);
	m_engine->restoreConfig(dqp);
	m_inputProcessor->restoreConfig(dqp);

	return true;
}


bool ProjectManager::saveCurrentProject()
{
	if (m_projectFilePath.isEmpty())
		return false;

	QSettings dqp(m_projectFilePath, QSettings::IniFormat);
	m_engine->storeConfig(dqp);
	m_inputProcessor->storeConfig(dqp);

	return true;
}


void ProjectManager::closeCurrentProject()
{
	if (m_projectFilePath.isEmpty())
		return;

	saveCurrentProject();

	m_projectFilePath.clear();
}
