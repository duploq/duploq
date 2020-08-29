#pragma once

#include <QObject>
#include <QStringList>
#include <QSettings>


class QAction;
class QMenu;


class RecentFilesManager : public QObject
{
	Q_OBJECT

public:

	const QStringList& getRecentFilesList() const;
	void cleanRecentFilesList();
	bool addRecentDocument(const QString& name);
	bool removeRecentDocument(const QString& name);

	QMenu* getRecentFilesMenu();

	void storeConfig(QSettings &set);
	void restoreConfig(QSettings &set);

Q_SIGNALS:
	void selected(const QString& name);

private Q_SLOTS:
	void fillRecentFilesMenu();
	void onRecentFilesMenuAction(QAction*);

private:
	QStringList m_recentFilesList;
	QMenu *m_recentFilesMenu = nullptr;
	//QSettings *m_settings = nullptr;
};

