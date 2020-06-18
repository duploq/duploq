#ifndef RESULTPROCESSOR_H
#define RESULTPROCESSOR_H

#include <QMap>
#include <QString>
#include <QList>
#include <QSet>
#include <QDomDocument>


struct ChunkInfo
{
    int lineCount = 0;
    QMap<QString, QSet<int>> locations;    // <file:<start>>
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
