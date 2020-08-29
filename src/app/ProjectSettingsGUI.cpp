#include "ProjectSettingsGUI.h"
#include "ui_ProjectSettingsGUI.h"

#include "ProjectManager.h"

#include <QFileInfo>
#include <QFileDialog>


ProjectSettingsGUI::ProjectSettingsGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectSettingsGUI)
{
    ui->setupUi(this);
}


ProjectSettingsGUI::~ProjectSettingsGUI()
{
    delete ui;
}


bool ProjectSettingsGUI::exec(ProjectManager &manager)
{
	QFileInfo rootInfo(manager.getCurrentProject());
	m_rootPath = rootInfo.absolutePath();
	ui->RootDirectory->setText(m_rootPath);

	QStringList subdirs = manager.getSubdirectories();
	ui->DirectoryList->clear();
	ui->DirectoryList->addItems(subdirs);
	ui->DirectoryList->sortItems();

	updateActions();

    if (QDialog::exec() == QDialog::Accepted)
    {
		subdirs.clear();
		for (int i = 0; i < ui->DirectoryList->count(); ++i)
			subdirs << ui->DirectoryList->item(i)->text();

		manager.setSubdirectories(subdirs);

        return true;
    }

    return false;
}


void ProjectSettingsGUI::on_Add_clicked()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Add subdirectory"),
		m_rootPath,
		QFileDialog::ShowDirsOnly 
		| QFileDialog::ReadOnly
		| QFileDialog::DontResolveSymlinks);

	if (dirPath.isEmpty())
		return;

	QDir rootDir(m_rootPath);
	QString relativePath = rootDir.relativeFilePath(dirPath);

	auto foundItems = ui->DirectoryList->findItems(relativePath, Qt::MatchFixedString);
	if (foundItems.size())
	{
		ui->DirectoryList->setCurrentItem(foundItems.first());
		updateActions();
		return;
	}

	ui->DirectoryList->addItem(relativePath);
	ui->DirectoryList->setCurrentRow(ui->DirectoryList->count() - 1);
	ui->DirectoryList->sortItems();

	updateActions();
}


void ProjectSettingsGUI::on_Remove_clicked()
{
	if (ui->DirectoryList->currentRow() >= 0)
	{
		delete ui->DirectoryList->currentItem();

		updateActions();
	}
}


void ProjectSettingsGUI::updateActions()
{
	bool isSelected = (ui->DirectoryList->currentRow() >= 0);
	ui->Remove->setEnabled(isSelected);
}


void ProjectSettingsGUI::on_DirectoryList_itemSelectionChanged()
{
	updateActions();
}
