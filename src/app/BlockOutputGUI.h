#ifndef BLOCKOUTPUTGUI_H
#define BLOCKOUTPUTGUI_H

#include "ResultProcessor.h"

#include <QSettings>
#include <QWidget>

class QTreeWidgetItem;

namespace Ui {
class BlockOutputGUI;
}

struct ResultInfo;

class BlockOutputGUI : public QWidget
{
    Q_OBJECT

public:
    explicit BlockOutputGUI(QWidget *parent = nullptr);
    ~BlockOutputGUI();

    void clear();
    void setResults(const ResultInfo &results);

    void storeConfig(QSettings &set);
    void restoreConfig(QSettings &set);

public Q_SLOTS:
    void showBlock(int index);

private Q_SLOTS:
    void on_BlockTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::BlockOutputGUI *ui;

    const ResultInfo *m_results = nullptr;
};

#endif // BLOCKOUTPUTGUI_H
