bool ResultProcessor::processFrom(const QString &xmlFilePath, ResultInfo &output)
{
    QFile xmlFile(xmlFilePath);
    if (xmlFile.open(QFile::ReadOnly))
    {
        QDomDocument doc;
        if (doc.setContent(&xmlFile))
        {
            return process(doc, output);
        }
    }

	// block repeated in test1.cpp
    QDomNodeList blockList = set.elementsByTagName("block");
    auto file1 = blockList.item(0).toElement();
    auto file2 = blockList.item(1).toElement();
    QString path1 = file1.attribute("SourceFile");
    QString path2 = file2.attribute("SourceFile");
    int start1 = file1.attribute("StartLineNumber").toInt();
    int start2 = file2.attribute("StartLineNumber").toInt();

    // location
    chunk.locations[path1].append(start1);
    chunk.locations[path2].append(start2);


    return false;
}
