#include "RecentFilesManager.h"

#include <QMenu>
#include <QAction>


const QStringList& RecentFilesManager::getRecentFilesList() const
{
	return m_recentFilesList;
}


void RecentFilesManager::cleanRecentFilesList()
{
	m_recentFilesList.clear();

	fillRecentFilesMenu();
}


bool RecentFilesManager::addRecentDocument(const QString& name)
{
	if (name.isEmpty())
		return false;

	int index = m_recentFilesList.indexOf(name);
	if (index >= 0) 
	{
		m_recentFilesList.move(index, 0);
	}
	else 
	{
		m_recentFilesList.prepend(name);

		// to parameterize
		if (m_recentFilesList.size() > 20)
			m_recentFilesList.removeLast();
	}

	fillRecentFilesMenu();

	return true;
}


bool RecentFilesManager::removeRecentDocument(const QString& name)
{
	return false;
}


QMenu* RecentFilesManager::getRecentFilesMenu()
{
	if (m_recentFilesMenu == nullptr)
	{
		m_recentFilesMenu = new QMenu;

		connect(m_recentFilesMenu, &QMenu::aboutToShow, this, &RecentFilesManager::fillRecentFilesMenu);
		connect(m_recentFilesMenu, &QMenu::triggered, this, &RecentFilesManager::onRecentFilesMenuAction);

		fillRecentFilesMenu();
	}

	return m_recentFilesMenu;
}


void RecentFilesManager::fillRecentFilesMenu()
{
	if (m_recentFilesMenu == nullptr)
		return;

	m_recentFilesMenu->clear();

	for (int i = 0; i < m_recentFilesList.size(); ++i)
	{
		QAction *recentAction = m_recentFilesMenu->addAction(m_recentFilesList.at(i));
		recentAction->setData(i);
	}
}


void RecentFilesManager::onRecentFilesMenuAction(QAction* recentAction)
{
	if (recentAction == nullptr)
		return;

	QString filePath = recentAction->text();
	if (filePath.isEmpty())
		return;

	Q_EMIT selected(filePath);
}


void RecentFilesManager::storeConfig(QSettings &set)
{
	m_recentFilesList.removeDuplicates();

	set.beginGroup("RecentFiles");
	set.setValue("RecentFilesList", m_recentFilesList);
	set.endGroup();
}


void RecentFilesManager::restoreConfig(QSettings &set)
{
	set.beginGroup("RecentFiles");
	m_recentFilesList = set.value("RecentFilesList", m_recentFilesList).toStringList();
	set.endGroup();
	
	m_recentFilesList.removeDuplicates();
}

