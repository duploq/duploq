#include "ProjectManager.h"
#include "Engine.h"
#include "InputProcessor.h"

#include <QSettings>


ProjectManager::ProjectManager(InputProcessor &input, Engine &engine)
{
	m_engine = &engine;
	m_inputProcessor = &input;
}


bool ProjectManager::setCurrentProject(const QString &projectFilePath)
{
	m_projectFilePath = projectFilePath;

	QSettings dqp(m_projectFilePath, QSettings::IniFormat);
	m_engine->storeConfig(dqp);
	m_inputProcessor->storeConfig(dqp);

	return true;
}
