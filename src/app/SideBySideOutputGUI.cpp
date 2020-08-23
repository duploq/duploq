#include "SideBySideOutputGUI.h"
#include "ui_SideBySideOutput.h"

#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QScrollBar>


SideBySideOutputGUI::SideBySideOutputGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBySideOutput)
{
    ui->setupUi(this);

    clear();

    // connect slots
    connect(ui->View1->horizontalScrollBar(), &QScrollBar::valueChanged, ui->View2->horizontalScrollBar(), &QScrollBar::setValue);
    connect(ui->View2->horizontalScrollBar(), &QScrollBar::valueChanged, ui->View1->horizontalScrollBar(), &QScrollBar::setValue);

    // intercept font sizes
    connect(ui->View1, &CodeEditor::sizeChanged, this, [=]()
    {
        if (ui->View1->font().pointSize() != ui->View2->font().pointSize())
        {
            ui->View2->setFont(ui->View1->font());
        }
    });

    connect(ui->View2, &CodeEditor::sizeChanged, this, [=]()
    {
        if (ui->View1->font().pointSize() != ui->View2->font().pointSize())
        {
            ui->View1->setFont(ui->View2->font());
        }
    });
}


SideBySideOutputGUI::~SideBySideOutputGUI()
{
    delete ui;
}


void SideBySideOutputGUI::storeConfig(QSettings &set)
{
    set.beginGroup("SideBySideOutputGUI");
    set.setValue("Font", ui->View1->font());
    set.endGroup();
}


void SideBySideOutputGUI::restoreConfig(QSettings &set)
{
    set.beginGroup("SideBySideOutputGUI");

    if (set.contains("Font"))
    {
        QFont f = set.value("Font").value<QFont>();
        ui->View1->setFont(f);
        ui->View2->setFont(f);
    }

    set.endGroup();
}


void SideBySideOutputGUI::clear()
{
    ui->FilePath1->clear();
    ui->FilePath2->clear();
    ui->LineNumber1->clear();
    ui->LineNumber2->clear();
    ui->ChunkLabel->clear();
    ui->View1->clear();
    ui->View2->clear();

    setupBlock(ui->View1, 0, 0);
    setupBlock(ui->View2, 0, 0);

    m_valid = true;
    m_chunkIndex = 0;
    m_results = nullptr;
}


void SideBySideOutputGUI::setContentFrom(const QString &filePath1, const QString &filePath2, const ResultInfo &results)
{
    clear();


    ui->FilePath1->setText(filePath1);
    ui->FilePath2->setText(filePath2);

    QFile file1(filePath1);
    if (file1.open(QFile::ReadOnly))
    {
        ui->View1->setPlainText(file1.readAll());
        file1.close();
    }
    else
    {
        ui->View1->setPlainText(tr("Cannot read file"));
        m_valid = false;
    }

    QFile file2(filePath2);
    if (file2.open(QFile::ReadOnly))
    {
        ui->View2->setPlainText(file2.readAll());
        file2.close();
    }
    else
    {
        ui->View2->setPlainText(tr("Cannot read file"));
        m_valid = false;
    }


    //
    if (results.fileCrosses.contains(filePath1))
    {
        if (results.fileCrosses[filePath1].crosses.contains(filePath2))
        {
            m_results = &results;
            m_crosses = m_results->fileCrosses[filePath1].crosses[filePath2];

            ui->NextChunk->setEnabled(true);
            ui->PrevChunk->setEnabled(true);
            ui->LineNumber1->show();
            ui->LineNumber2->show();

			ui->View2->show();

            showCross(0);

            return;
        }
    }

    // no crosses
    ui->ChunkLabel->setText(tr("<<none>>"));
    ui->NextChunk->setEnabled(false);
    ui->PrevChunk->setEnabled(false);
    ui->LineNumber1->hide();
    ui->LineNumber2->hide();

	ui->View2->hide();
}


void SideBySideOutputGUI::showCross(int index)
{
    m_chunkIndex = index;

    ui->ChunkLabel->setText(tr("%1 of %2").arg(index+1).arg(m_crosses.count()));

    int line1 = m_crosses[index].start1;
    int line2 = m_crosses[index].start2;
    ui->LineNumber1->setNum(line1);
    ui->LineNumber2->setNum(line2);

    int lineCount = m_results->chunks[ m_crosses[index].chunkIndex ].lineCount;

    setupBlock(ui->View1, line1-1, lineCount);
    setupBlock(ui->View2, line2-1, lineCount);

    Q_EMIT blockShown(m_crosses[index].chunkIndex);
}


void SideBySideOutputGUI::on_PrevChunk_clicked()
{
    if (m_chunkIndex > 0)
    {
        showCross(m_chunkIndex - 1);
    }
}


void SideBySideOutputGUI::on_NextChunk_clicked()
{
    if (m_chunkIndex < m_crosses.count() - 1)
    {
        showCross(m_chunkIndex + 1);
    }
}


// TO DO: move to Code Editor

void SideBySideOutputGUI::setupBlock(QPlainTextEdit *view, int line, int size)
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
        QTextCursor cursor1(view->textCursor());
        cursor1.movePosition(QTextCursor::Start);
        cursor1.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
        view->setTextCursor(cursor1);
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
