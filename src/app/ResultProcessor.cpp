#include "ResultProcessor.h"

#include <QFile>


ResultProcessor::ResultProcessor()
{

}


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

    return false;
}


bool ResultProcessor::process(const QDomDocument &input, ResultInfo &output)
{
    output.chunks.clear();
    output.fileCrosses.clear();

    QDomNodeList setList = input.elementsByTagName("set");
    for (int i = 0; i < setList.count(); ++i)
    {
        // chunk
//        ChunkInfo chunk;
//        int chunkIndex = output.chunks.size();

        auto set = setList.at(i).toElement();
        int lineCount = set.attribute("LineCount").toInt();

        QDomNodeList blockList = set.elementsByTagName("block");
        auto file1 = blockList.item(0).toElement();
        auto file2 = blockList.item(1).toElement();
        QString path1 = file1.attribute("SourceFile");
        QString path2 = file2.attribute("SourceFile");
        int start1 = file1.attribute("StartLineNumber").toInt();
        int start2 = file2.attribute("StartLineNumber").toInt();

        // look for existing chunks
        int chunkIndex = output.findChunk(path1, start1, lineCount);
        if (chunkIndex < 0)
            chunkIndex = output.findChunk(path2, start2, lineCount);

        if (chunkIndex < 0)
        {
            // create new chunk
            ChunkInfo chunk;
            chunkIndex = output.chunks.size();
            chunk.lineCount = lineCount;

            // location
            chunk.locations[path1].insert(start1);
            chunk.locations[path2].insert(start2);

            // content
            auto lines = set.firstChildElement("lines");
            QDomNodeList lineList = lines.elementsByTagName("line");
            for (int l = 0; l < lineList.count(); ++l)
            {
                auto line = lineList.at(l).toElement();
                chunk.content << line.attribute("Text");
            }

            output.chunks << chunk;
        }
        else
        {
            // chunk found
            ChunkInfo &chunk = output.chunks[chunkIndex];

            // update location
            chunk.locations[path1].insert(start1);
            chunk.locations[path2].insert(start2);
        }

        // crosses
        CrossInfo cross = { start1, start2, chunkIndex };
        output.fileCrosses[path1].crosses[path2].append(cross);

    }

    return true;
}


int ResultInfo::findChunk(const QString &path, int start, int lineCount) const
{
    for (int i = 0; i < chunks.count(); ++i)
    {
        const auto& chunkInfo = chunks.at(i);
        if (chunkInfo.lineCount == lineCount && chunkInfo.locations.contains(path))
        {
            if (chunkInfo.locations[path].contains(start))
                return i;
        }
    }

    return -1;
}
