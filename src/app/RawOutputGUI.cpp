#include "RawOutputGUI.h"
#include "ui_RawOutputGUI.h"

//#include "BasicXMLSyntaxHighlighter.h"

#include <QFile>


RawOutputGUI::RawOutputGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RawOutputGUI)
{
    ui->setupUi(this);

    //m_highlighter = new BasicXMLSyntaxHighlighter(this);
    //m_highlighter->setDocument(ui->Output->document());
}


RawOutputGUI::~RawOutputGUI()
{
    delete ui;
}


void RawOutputGUI::clear()
{
    ui->Output->clear();
}


void RawOutputGUI::setContentFrom(const QString &fileResultPath)
{
    ui->Output->clear();

    QFile file(fileResultPath);
    if (file.open(QFile::ReadOnly))
    {
        QString text( file.readAll() );
        ui->Output->setPlainText(text);

        file.close();
    }
}
