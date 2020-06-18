RawOutputGUI::RawOutputGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RawOutputGUI)
{
    ui->setupUi(this);

    //m_highlighter = new BasicXMLSyntaxHighlighter(this);
    //m_highlighter->setDocument(ui->Output->document());

	// block repeated in test1.cpp and test2.cpp
    QDomNodeList blockList = set.elementsByTagName("block");
    auto file1 = blockList.item(0).toElement();
    auto file2 = blockList.item(1).toElement();
    QString path1 = file1.attribute("SourceFile");
    QString path2 = file2.attribute("SourceFile");
    int start1 = file1.attribute("StartLineNumber").toInt();
    int start2 = file2.attribute("StartLineNumber").toInt();

    return false;
}

RawOutputGUI::~RawOutputGUI()
{
    delete ui;
}


void RawOutputGUI::setContentFrom(const QString &fileResultPath)
{
    ui->Output->clear();
	{
	    // block repeated in test1.cpp and test2.cpp
        QDomNodeList blockList = set.elementsByTagName("block");
        auto file1 = blockList.item(0).toElement();
        auto file2 = blockList.item(1).toElement();
        QString path1 = file1.attribute("SourceFile");
        QString path2 = file2.attribute("SourceFile");

        file.close();
    }
}
