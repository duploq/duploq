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

	QSettings dqp(m_projectFilePath, QSettings::IniFormat);
	m_engine->storeConfig(dqp);
	m_inputProcessor->storeConfig(dqp);

	return true;
}


bool ProjectManager::openProject(const QString &projectFilePath)
{
	if (!QFile::exists(projectFilePath))
		return false;

	m_projectFilePath = projectFilePath;

	QSettings dqp(m_projectFilePath, QSettings::IniFormat);
	m_engine->restoreConfig(dqp);
	m_inputProcessor->restoreConfig(dqp);

	return true;
}

