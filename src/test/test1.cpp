bool ResultProcessor::process(const QDomDocument &input, ResultInfo &output)
{
    output.chunks.clear();
    output.fileCrosses.clear();

    QDomNodeList setList = input.elementsByTagName("set");
    for (int i = 0; i < setList.count(); ++i)
    {
        // chunk
        ChunkInfo chunk;
        int chunkIndex = output.chunks.size();

        auto set = setList.at(i).toElement();
        chunk.lineCount = set.attribute("LineCount").toInt();

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

        // content
        auto lines = set.firstChildElement("lines");
        QDomNodeList lineList = lines.elementsByTagName("line");
        // to do

        output.chunks << chunk;


        // crosses
        CrossInfo cross = { start1, start2, chunkIndex };
		output.fileCrosses[path1].crosses[path2].append(cross);

    }

    return true;
}
