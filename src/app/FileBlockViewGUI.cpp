#include "FileBlockViewGUI.h"
#include "ui_FileBlockViewGUI.h"

#include "ResultProcessor.h"

#include <QFile>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QScrollBar>


FileBlockViewGUI::FileBlockViewGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBlockViewGUI)
{
    ui->setupUi(this);

}


FileBlockViewGUI::~FileBlockViewGUI()
{
    delete ui;
}


void FileBlockViewGUI::clear()
{
    ui->FilePath->clear();
    ui->View->clear();
    ui->LineNumber->setNum(0);
    ui->ChunkLabel->setText(tr("<none>"));

    m_valid = false;
    m_chunkIndex = 0;
    m_startIndex = 0;
    m_linesCount = 0;
    m_starts.clear();

    ui->LineNumber->hide();
    ui->PrevChunk->hide();
    ui->NextChunk->hide();
    ui->ChunkLabel->hide();
}


void FileBlockViewGUI::setContentFrom(int chunkIndex, const QString &filePath, const ResultInfo &results)
{
    clear();

    m_valid = true;
    m_chunkIndex = chunkIndex;

    ui->FilePath->setText(filePath);

    QFile file(filePath);
    if (file.open(QFile::ReadOnly))
    {
        ui->View->setPlainText(file.readAll());
        file.close();
    }
    else
    {
        ui->View->setPlainText(tr("Cannot read file"));
        m_valid = false;
    }

    if (chunkIndex < 0 || chunkIndex >= results.chunks.count())
    {
        m_valid = false;
        return;
    }

    const auto &chunkInfo = results.chunks.at(chunkIndex);

    if (!chunkInfo.locations.contains(filePath))
    {
        m_valid = false;
        return;
    }

    const auto &starts = chunkInfo.locations[filePath];

    if (starts.isEmpty())
    {
        m_valid = false;
        return;
    }

    m_starts = starts.values();
    m_linesCount = chunkInfo.lineCount;

    showBlock(0);
}


void FileBlockViewGUI::setBlockFrom(int chunkIndex, const ResultInfo &results)
{
    clear();

    m_valid = true;
    m_chunkIndex = chunkIndex;

    if (chunkIndex < 0 || chunkIndex >= results.chunks.count())
    {
        m_valid = false;
        return;
    }

	ui->FilePath->setText(tr("Block %1").arg(chunkIndex + 1));

    const auto &chunkInfo = results.chunks.at(chunkIndex);

    ui->View->appendPlainText(chunkInfo.content.join("\n"));

    // clean active selection(s)
    {
        QTextEdit::ExtraSelection selection1;
        selection1.cursor = ui->View->textCursor();
        selection1.cursor.select(QTextCursor::Document);
        selection1.format.setBackground(Qt::NoBrush);
        selection1.cursor.mergeCharFormat(selection1.format);
        ui->View->setExtraSelections({ selection1 });
    }

	QTextCursor cursor(ui->View->textCursor());
	cursor.movePosition(QTextCursor::Start);
	ui->View->setTextCursor(cursor);
    ui->View->verticalScrollBar()->setValue(0);
}


void FileBlockViewGUI::showBlock(int index)
{
    m_startIndex = index;

    ui->ChunkLabel->setText(tr("%1 of %2").arg(index + 1).arg(m_starts.count()));

    int line = m_starts[index];
    ui->LineNumber->setNum(line);

    ui->LineNumber->show();
    ui->PrevChunk->show();
    ui->NextChunk->show();
    ui->ChunkLabel->show();

    setupBlock(ui->View, line-1, m_linesCount);
}


void FileBlockViewGUI::on_PrevChunk_clicked()
{
    if (m_startIndex > 0)
    {
        showBlock(m_startIndex - 1);
    }
}


void FileBlockViewGUI::on_NextChunk_clicked()
{
    if (m_startIndex < m_starts.count() - 1)
    {
        showBlock(m_startIndex + 1);
    }
}



// TO DO: move to Code Editor

void FileBlockViewGUI::setupBlock(QPlainTextEdit *view, int line, int size)
{
    if (!view)
        return;

    // clean active selection(s)
    {
        QTextEdit::ExtraSelection selection1;
        selection1.cursor = view->textCursor();
        selection1.cursor.select(QTextCursor::Document);
        selection1.format.setBackground(Qt::NoBrush);
        selection1.cursor.mergeCharFormat(selection1.format);
        view->setExtraSelections({ selection1 });
    }

    // move
    if (line >= 0)
    {
        QTextCursor cursor(view->textCursor());
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
        view->setTextCursor(cursor);
        view->verticalScrollBar()->setValue(qMax(0, line - 3));
    }

    // highlight
    if (size > 0)
    {
        QTextEdit::ExtraSelection selection;
        selection.cursor = view->textCursor();
        selection.cursor.clearSelection();
        selection.cursor.movePosition(QTextCursor::Start);
        selection.cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);	// move to start line

        selection.cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 1);		// select 1st line - is always ok
        for (int i = 1; i < size; ++i)
        {
            QString textLine = selection.cursor.block().text().trimmed();
            if (textLine.size() <= 1)
                --i;

            selection.cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 1);
        }

        //selection.cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, size);	// select lines
        selection.format.setBackground(QColor("#e6f3ff"));
        selection.cursor.mergeCharFormat(selection.format);
        view->setExtraSelections({ selection });
    }
}

