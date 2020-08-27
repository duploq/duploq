#pragma once

#include <QDialog>

namespace Ui {
class ProjectSettingsGUI;
}

class ProjectManager;


class ProjectSettingsGUI : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectSettingsGUI(QWidget *parent = nullptr);
    ~ProjectSettingsGUI();

    bool exec(ProjectManager &manager);

private Q_SLOTS:
	void on_Add_clicked();
	void on_Remove_clicked();
	void on_DirectoryList_itemSelectionChanged();

private:
	void updateActions();

private:
	QString m_rootPath;

    Ui::ProjectSettingsGUI *ui;
};
