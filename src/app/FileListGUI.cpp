#include "FileListGUI.h"
#include "ui_FileListGUI.h"

#include <QFile>
#include <QTextStream>


// custom sort
class TreeWidgetItem : public QTreeWidgetItem
{
public:
	TreeWidgetItem(QTreeWidget* parent = nullptr) :QTreeWidgetItem(parent) {}
	TreeWidgetItem(QTreeWidgetItem* parent) :QTreeWidgetItem(parent) {}

private:
	bool operator<(const QTreeWidgetItem &other) const
	{
		int column = treeWidget()->sortColumn();
		if (column == 0)
		{
			return text(column).toLower() < other.text(column).toLower();
		}
		else
		{
			int t1 = data(column, Qt::UserRole).toInt();
			int t2 = other.data(column, Qt::UserRole).toInt();
			return t1 < t2;
		}
	}
};


// FileListGUI

FileListGUI::FileListGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileListGUI)
{
    ui->setupUi(this);

    ui->FileListTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->FileListTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->FileListTree->header()->setSectionsMovable(false);
}


FileListGUI::~FileListGUI()
{
    delete ui;
}


void FileListGUI::setFileList(const QStringList &fileList)
{
    ui->FileListTree->setUpdatesEnabled(false);

    ui->FileListTree->clear();

    for (const auto &filePath : fileList)
    {
        auto item = new TreeWidgetItem;
        item->setText(0, filePath);
        item->setText(1, "");
        item->setText(2, "");
        ui->FileListTree->addTopLevelItem(item);
    }

    ui->FileListTree->setUpdatesEnabled(true);

	ui->TotalFiles->setNum(fileList.count());
	ui->ClonedFiles->setText("-");
	ui->ClonedBlocks->setText("-");
	ui->ClonedLines->setText("-");

    // store the list for results
    m_fileList = fileList;
}


void FileListGUI::setFileListFrom(const QString &fileListPath)
{
    QStringList fileList;

    QFile file(fileListPath);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream ts(&file);
        while (!ts.atEnd())
        {
            auto line = ts.readLine().trimmed();
            if (!line.isEmpty())
                fileList << line;
        }

        file.close();
    }

    setFileList(fileList);
}


void FileListGUI::setFileListResults(const ResultInfo &results)
{
    // clean file list
    setFileList(m_fileList);

    // merge with results
    ui->FileListTree->setUpdatesEnabled(false);

    QFont ital;
    ital.setItalic(true);

	int globalRepeatCount = 0;
	int globalRepeatLines = 0;

    for (auto it = results.fileCrosses.constBegin(); it != results.fileCrosses.constEnd(); ++it)
    {
        const QString &name1 = it.key();
        int index = m_fileList.indexOf(name1);
        Q_ASSERT(index >= 0);

        auto items = ui->FileListTree->findItems(name1, Qt::MatchExactly);
        Q_ASSERT(items.size() == 1);

        auto item1 = items.first();
        Q_ASSERT(item1);

        int extRepeatCount = 0;
        int extRepeatLines = 0;

        int totalRepeatCount = 0;
        int totalRepeatLines = 0;

        for (auto it2 = it.value().crosses.constBegin(); it2 != it.value().crosses.constEnd(); ++it2)
        {
            int repeatCount = it2.value().count();

            int repeatLines = 0;
            for (const auto &cross : it2.value())
            {
                repeatLines += results.chunks[cross.chunkIndex].lineCount;
            }

            totalRepeatCount += repeatCount;
            totalRepeatLines += repeatLines;

            const QString &name2 = it2.key();
            if (name2 == name1)
            {
                item1->setText(1, QString("%2 / %1").arg(repeatCount).arg(repeatLines));
				item1->setData(1, Qt::UserRole, repeatLines);

                item1->setForeground(0, QColor("orange"));
                item1->setForeground(1, Qt::gray);
            }
            else
            {
                extRepeatCount += repeatCount;
                extRepeatLines += repeatLines;

                auto item2 = new QTreeWidgetItem(item1);
                item2->setText(0, name2);
                item2->setText(2, QString("%2 / %1").arg(repeatCount).arg(repeatLines));
				item1->setData(2, Qt::UserRole, repeatLines);

                //item2->setForeground(0, QColor("red"));
                item2->setFont(0, ital);
                //item2->setFont(1, ital);
                item2->setFont(2, ital);
                item2->setForeground(0, Qt::gray);
                item2->setForeground(2, Qt::gray);
            }
        }

        if (extRepeatCount > 0)
        {
            item1->setText(2, QString("%2 / %1").arg(extRepeatCount).arg(extRepeatLines));
			item1->setData(2, Qt::UserRole, extRepeatLines);

            item1->setForeground(0, Qt::red);
            item1->setForeground(2, Qt::gray);
        }

        if (totalRepeatCount > 0)
        {
            item1->setText(3, QString("%2 / %1").arg(totalRepeatCount).arg(totalRepeatLines));
			item1->setData(3, Qt::UserRole, totalRepeatLines);

			globalRepeatCount += totalRepeatCount;
			globalRepeatLines += totalRepeatLines;
        }
    }

    ui->FileListTree->header()->setSortIndicator(3, Qt::DescendingOrder);

    ui->FileListTree->setUpdatesEnabled(true);


	ui->ClonedFiles->setNum(results.fileCrosses.count());
	ui->ClonedBlocks->setNum(globalRepeatCount);
	ui->ClonedLines->setNum(globalRepeatLines);


    doFilterTree();
}


void FileListGUI::storeConfig(QSettings &set)
{
    set.beginGroup("FileListGUI");
    set.setValue("HideNotCloned", ui->HideNotCloned->isChecked());
    set.setValue("ExpandItems", ui->ExpandItems->isChecked());
    set.endGroup();
}


void FileListGUI::restoreConfig(QSettings &set)
{
    set.beginGroup("FileListGUI");
    ui->HideNotCloned->setChecked( set.value("HideNotCloned", true).toBool() );
    ui->ExpandItems->setChecked( set.value("ExpandItems", false).toBool() );
    set.endGroup();
}


void FileListGUI::on_FileListTree_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem * /*previous*/)
{
    if (!item)
        return;

    if (item->parent())
    {
        Q_EMIT(filesChosen(item->parent()->text(0), item->text(0)));
    }
    else
    {
        Q_EMIT(filesChosen(item->text(0), item->text(0)));
    }
}


void FileListGUI::on_HideNotCloned_toggled(bool /*on*/)
{
    doFilterTree();
}


void FileListGUI::on_ExpandItems_toggled(bool /*on*/)
{
    doFilterTree();
}


void FileListGUI::doFilterTree()
{
    bool clonesOnly = ui->HideNotCloned->isChecked();
    bool expandItems = ui->ExpandItems->isChecked();

    ui->FileListTree->setUpdatesEnabled(false);

    for (int i = 0; i < ui->FileListTree->topLevelItemCount(); ++i)
    {
        auto *item = ui->FileListTree->topLevelItem(i);

        if (clonesOnly)
            item->setHidden(item->text(3).isEmpty());
        else
            item->setHidden(false);

        item->setExpanded(expandItems);
    }

    ui->FileListTree->setUpdatesEnabled(true);
}
