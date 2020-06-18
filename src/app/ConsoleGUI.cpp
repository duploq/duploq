#include "ConsoleGUI.h"
#include "ui_ConsoleGUI.h"

ConsoleGUI::ConsoleGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleGUI)
{
    ui->setupUi(this);
}

ConsoleGUI::~ConsoleGUI()
{
    delete ui;
}

void ConsoleGUI::clear()
{
    ui->TextBrowser->clear();
}

void ConsoleGUI::append(const QString &text)
{
    ui->TextBrowser->append(text);
}
