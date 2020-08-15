#include "FileListGUI.h"
#include "ui_FileListGUI.h"

#include <QFile>
#include <QDir>
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
	ui->FileListTree->header()->hideSection(4);
}


FileListGUI::~FileListGUI()
{
    delete ui;
}


void FileListGUI::setFileList(const QStringList &fileList, const QString &rootPath)
{
    ui->FileListTree->setUpdatesEnabled(false);

    ui->FileListTree->clear();

	QDir rootDir(rootPath);

    for (const auto &filePath : fileList)
    {
		QString localPath = rootDir.relativeFilePath(filePath);

        auto item = new TreeWidgetItem;
		//item->setData(0, Qt::UserRole, filePath);
		item->setText(0, localPath.size() ? localPath : filePath);
        item->setText(1, "");
        item->setText(2, "");
        //item->setText(3, "");
        item->setText(4, filePath);
        ui->FileListTree->addTopLevelItem(item);
    }

    ui->FileListTree->setUpdatesEnabled(true);

	ui->TotalFiles->setNum(fileList.count());
	ui->ClonedFiles->setText("-");
	ui->ClonedBlocks->setText("-");
	ui->ClonedLines->setText("-");

    // store the list for results
    m_fileList = fileList;
	m_rootPath = rootPath;
}


void FileListGUI::setFileListFrom(const QString &fileListPath, const QString &rootPath)
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

    setFileList(fileList, rootPath);
}


void FileListGUI::setFileListResults(const ResultInfo &results)
{
    // clean file list
    setFileList(m_fileList, m_rootPath);

    // merge with results
    ui->FileListTree->setUpdatesEnabled(false);

    QFont ital;
    ital.setItalic(true);

	int globalRepeatCount = 0;
	int globalRepeatLines = 0;

	QDir rootDir(m_rootPath);

    for (auto it = results.fileCrosses.constBegin(); it != results.fileCrosses.constEnd(); ++it)
    {
        const QString &name1 = it.key();
        int index = m_fileList.indexOf(name1);
        Q_ASSERT(index >= 0);

	/*	QModelIndexList found = ui->FileListTree->model()->match(
			ui->FileListTree->model()->index(0, 0),
			Qt::UserRole,
			name1,
			1,
			Qt::MatchExactly);

		Q_ASSERT(found.size() == 1);

		auto item1 = ui->FileListTree->item(found.first());
		Q_ASSERT(item1);*/

        auto items = ui->FileListTree->findItems(name1, Qt::MatchExactly, 4);
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

				QString localPath = rootDir.relativeFilePath(name2);

                auto item2 = new QTreeWidgetItem(item1);
                item2->setText(0, localPath);
                item2->setText(4, name2);
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
        Q_EMIT(
			filesChosen(
				item->parent()->text(4), item->text(4)
				//item->parent()->data(0, Qt::UserRole).toString(), 
				//item->data(0, Qt::UserRole).toString()
			));
    }
    else
    {
        Q_EMIT(
			filesChosen(
				item->text(4), item->text(4)
				//item->data(0, Qt::UserRole).toString(), 
				//item->data(0, Qt::UserRole).toString()
			));
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
