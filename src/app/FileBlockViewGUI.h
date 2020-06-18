#ifndef FILEBLOCKVIEWGUI_H
#define FILEBLOCKVIEWGUI_H

#include <QWidget>
#include <QString>
#include <QList>

class QPlainTextEdit;

struct ResultInfo;

namespace Ui {
class FileBlockViewGUI;
}


class FileBlockViewGUI : public QWidget
{
    Q_OBJECT

public:
    explicit FileBlockViewGUI(QWidget *parent = nullptr);
    ~FileBlockViewGUI();

    void clear();
    void setContentFrom(int chunkIndex, const QString &filePath, const ResultInfo &results);
    void setBlockFrom(int chunkIndex, const ResultInfo &results);

private:
    void showBlock(int index);
    void setupBlock(QPlainTextEdit *view, int line, int size);

private Q_SLOTS:
    void on_PrevChunk_clicked();
    void on_NextChunk_clicked();

private:
    Ui::FileBlockViewGUI *ui;

    bool m_valid = false;

    int m_chunkIndex = 0;
    int m_startIndex = 0;
    int m_linesCount = 0;
    QList<int> m_starts;
};

#endif // FILEBLOCKVIEWGUI_H
