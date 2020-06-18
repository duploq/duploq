#include "SettingsGUI.h"
#include "ui_SettingsGUI.h"

#include "InputProcessor.h"
#include "Engine.h"

SettingsGUI::SettingsGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsGUI)
{
    ui->setupUi(this);
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
        input.setOptions(inputOpts);

        return true;
    }

    return false;
}
