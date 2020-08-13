#pragma once

#include <QString>

class Engine;
class InputProcessor;


class ProjectManager
{
public:
	ProjectManager(InputProcessor &input, Engine &engine);

	bool hasProject() const							{ return m_projectFilePath.size(); }
	const QString& getCurrentProject() const		{ return m_projectFilePath; }
	bool createProject(const QString& projectFilePath);
	bool openProject(const QString& projectFilePath);

private:
	QString m_projectFilePath;

	Engine *m_engine = nullptr;
	InputProcessor *m_inputProcessor = nullptr;
};
