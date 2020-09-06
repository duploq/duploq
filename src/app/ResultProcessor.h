#ifndef RESULTPROCESSOR_H
#define RESULTPROCESSOR_H

#include <QMap>
#include <QString>
#include <QList>
#include <QSet>
#include <QPair>
#include <QDomDocument>


struct ChunkInfo
{
    static int findLast(const QSet<QPair<int,int>>& set, int first)
    {
        for (const auto& pos: set)
            if (pos.first == first)
                return pos.second;

        return -1;
    }

    int lineCount = 0;
    QMap<QString, QSet<QPair<int,int>>> locations;    // <file:<start,end>>
    QStringList content;
};


struct CrossInfo
{
    int start1;
    int start2;
    //ChunkInfo *chunkPtr;
    int chunkIndex;
};


struct FileInfo
{
    QMap<QString, QList<CrossInfo>> crosses;      // <file:<cross>>
};


struct ResultInfo
{
    int findChunk(const QString &path, int start, int lineCount) const;

    QList<ChunkInfo> chunks;
    QMap<QString, FileInfo> fileCrosses;
};


class ResultProcessor
{
public:
    ResultProcessor();

    bool processFrom(const QString &xmlFilePath, ResultInfo &output);
    bool process(const QDomDocument &input, ResultInfo &output);
};

#endif // RESULTPROCESSOR_H
