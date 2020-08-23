#include "SettingsGUI.h"
#include "ui_SettingsGUI.h"

#include "InputProcessor.h"
#include "Engine.h"

#include <QMenu>


enum PresetFilters
{
	C_CPP,
	Qt_CPP,
	CSharp,
	Java,
	JavaScript
};


SettingsGUI::SettingsGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsGUI)
{
    ui->setupUi(this);

	fillPresetMenu();
}


SettingsGUI::~SettingsGUI()
{
    delete ui;
}


bool SettingsGUI::exec(InputProcessor &input, Engine &engine)
{
    InputOptions inputOpts = input.getOptions();
    ui->RecursiveDirs->setChecked(inputOpts.dirsRecursive);
    ui->FileFilters->setText(inputOpts.fileFilters);
    ui->IgnoreFilters->setText(inputOpts.ignoreFilters);

    EngineOptions options = engine.getOptions();
    ui->MinSymbolsInLine->setValue(options.minLineLength);
    ui->MinLinesInBlock->setValue(options.minBlockSize);

    if (QDialog::exec() == QDialog::Accepted)
    {
        options.minBlockSize = ui->MinLinesInBlock->value();
        options.minLineLength = ui->MinSymbolsInLine->value();
        engine.setOptions(options);

        inputOpts.dirsRecursive = ui->RecursiveDirs->isChecked();
        inputOpts.fileFilters = ui->FileFilters->text();
        inputOpts.ignoreFilters = ui->IgnoreFilters->text();
        input.setOptions(inputOpts);

        return true;
    }

    return false;
}


void SettingsGUI::fillPresetMenu()
{
	QMenu *presetMenu = new QMenu(this);
	connect(presetMenu, &QMenu::triggered, this, &SettingsGUI::onPresetMenu);

	presetMenu->addAction("C/C++")->setData(C_CPP);
	presetMenu->addAction("Qt/C++")->setData(Qt_CPP);
	presetMenu->addAction("C#")->setData(CSharp);
	presetMenu->addAction("Java")->setData(Java);
	presetMenu->addAction("JavaScript")->setData(JavaScript);

	ui->PresetFilters->setMenu(presetMenu);
}


void SettingsGUI::onPresetMenu(QAction *action)
{
	int presetId = action->data().toInt();

	switch (presetId)
	{
	case C_CPP:
		ui->FileFilters->setText("*.c *.cpp *.hpp *.cxx *.hxx");
		ui->IgnoreFilters->clear();
		break;

	case Qt_CPP:
		ui->FileFilters->setText("*.c *.cpp *.hpp *.cxx *.hxx");
		ui->IgnoreFilters->setText("moc_* ui_* qrc_*");
		break;

	case CSharp:
		ui->FileFilters->setText("*.cs");
		ui->IgnoreFilters->clear();
		break;	
	
	case Java:
		ui->FileFilters->setText("*.java");
		ui->IgnoreFilters->clear();
		break;	

	case JavaScript:
		ui->FileFilters->setText("*.js");
		ui->IgnoreFilters->clear();
		break;
	}
}

