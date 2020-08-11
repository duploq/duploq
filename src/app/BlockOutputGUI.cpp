#include "BlockOutputGUI.h"
#include "ui_BlockOutputGUI.h"

#include <QTreeWidgetItem>
#include <QHeaderView>


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


// BlockOutputGUI
BlockOutputGUI::BlockOutputGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlockOutputGUI)
{
    ui->setupUi(this);

    ui->Output->hide();

    ui->BlockTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    //ui->BlockTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->BlockTree->header()->setStretchLastSection(false);
    ui->BlockTree->header()->setSectionsMovable(false);
}


BlockOutputGUI::~BlockOutputGUI()
{
    delete ui;
}


void BlockOutputGUI::clear()
{
    m_results = nullptr;
    ui->Output->clear();
    ui->BlockTree->clear();
    ui->BlockView->clear();
}


void BlockOutputGUI::setResults(const ResultInfo &results)
{
    clear();

    m_results = &results;

    ui->BlockTree->setUpdatesEnabled(false);

    for (int i = 0; i < results.chunks.count(); ++i)
    {
        const auto& chunkInfo = results.chunks.at(i);

        auto blockItem = new TreeWidgetItem;
        ui->BlockTree->addTopLevelItem(blockItem);

        // chunk index
        blockItem->setData(0, Qt::UserRole, i);

        int lines = chunkInfo.lineCount;
        int files = chunkInfo.locations.count();
        int blocks = 0;

        for (auto it = chunkInfo.locations.begin(); it != chunkInfo.locations.end(); ++it)
        {
            auto fileItem = new TreeWidgetItem(blockItem);
            fileItem->setText(0, it.key());
            fileItem->setForeground(0, Qt::gray);

            blocks += it.value().count();
        }

        QString blockText = tr("[#%4] Length %1, Repeats %3, Files %2").arg(lines).arg(files).arg(blocks).arg(i+1);
        blockItem->setText(0, blockText);
        blockItem->setText(1, QString::number(blocks*lines));
        blockItem->setData(1, Qt::UserRole, blocks*lines);
    }

    ui->BlockTree->header()->setSortIndicator(1, Qt::DescendingOrder);

    ui->BlockTree->expandAll();

    ui->BlockTree->setUpdatesEnabled(true);

#if 0
    ui->Output->show();
    ui->Output->setUpdatesEnabled(false);
    ui->Output->clear();

    for (const auto& chunkInfo: results.chunks)
    {
        for (auto it = chunkInfo.locations.begin(); it != chunkInfo.locations.end(); ++it)
        {
            QString locationString = it.key() + " at ";

            bool first = true;
            for (int start : it.value())
            {
                if (!first)
                    locationString.append(", ");
                first = false;

                locationString.append(QString::number(start+1));
            }

            ui->Output->appendPlainText(locationString);
        }

        QString contentString = "lines: " + QString::number(chunkInfo.lineCount);
        contentString += "\n...\n";  // TO DO

        ui->Output->appendPlainText(contentString);
    }

    ui->Output->setUpdatesEnabled(true);
#else
    ui->Output->hide();
#endif
}


void BlockOutputGUI::storeConfig(QSettings &set)
{

}


void BlockOutputGUI::restoreConfig(QSettings &set)
{

}


void BlockOutputGUI::showBlock(int index)
{
    for (int i = 0; i < ui->BlockTree->topLevelItemCount(); ++i)
    {
        auto item = ui->BlockTree->topLevelItem(i);
        int itemIndex = item->data(0, Qt::UserRole).toInt();
        if (itemIndex == index)
        {
            ui->BlockTree->setCurrentItem(item);
            return;
        }
    }
}


void BlockOutputGUI::on_BlockTree_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem * /*previous*/)
{
    if (!item || !m_results)
        return;

    if (item->parent())
    {
        // show file
        int index = item->parent()->data(0, Qt::UserRole).toInt();

        ui->BlockView->setContentFrom(index, item->text(0), *m_results);
    }
    else
    {
        // show content
        int index = item->data(0, Qt::UserRole).toInt();

        ui->BlockView->setBlockFrom(index, *m_results);
    }
}


