#include "ProjectSettingsGUI.h"
#include "ui_ProjectSettingsGUI.h"

#include "ProjectManager.h"

#include <QFileInfo>


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
	QString rootPath = rootInfo.absolutePath();
	ui->RootDirectory->setText(rootPath);

    /*InputOptions inputOpts = input.getOptions();
    ui->RecursiveDirs->setChecked(inputOpts.dirsRecursive);
    ui->FileFilters->setText(inputOpts.fileFilters);
    ui->IgnoreFilters->setText(inputOpts.ignoreFilters);

    EngineOptions options = engine.getOptions();
    ui->MinSymbolsInLine->setValue(options.minLineLength);
    ui->MinLinesInBlock->setValue(options.minBlockSize);
	*/
    if (QDialog::exec() == QDialog::Accepted)
    {
        //options.minBlockSize = ui->MinLinesInBlock->value();
        //options.minLineLength = ui->MinSymbolsInLine->value();
        //engine.setOptions(options);

        //inputOpts.dirsRecursive = ui->RecursiveDirs->isChecked();
        //inputOpts.fileFilters = ui->FileFilters->text();
        //inputOpts.ignoreFilters = ui->IgnoreFilters->text();
        //input.setOptions(inputOpts);

        return true;
    }

    return false;
}

