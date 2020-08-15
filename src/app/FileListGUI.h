#ifndef FILELISTGUI_H
#define FILELISTGUI_H

#include <QWidget>
#include <QStringList>
#include <QSettings>

#include "ResultProcessor.h"

class QTreeWidgetItem;

namespace Ui {
class FileListGUI;
}

class FileListGUI : public QWidget
{
    Q_OBJECT

public:
    explicit FileListGUI(QWidget *parent = nullptr);
    ~FileListGUI();

	void clear() { setFileList(QStringList(), QString()); }

    void setFileList(const QStringList &fileList, const QString &rootPath);
    void setFileListFrom(const QString &fileListPath, const QString &rootPath);

    void setFileListResults(const ResultInfo &results);

    void storeConfig(QSettings &set);
    void restoreConfig(QSettings &set);

Q_SIGNALS:
    void filesChosen(const QString& filePath1, const QString& filePath2);

private Q_SLOTS:
    void on_FileListTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_HideNotCloned_toggled(bool on);
    void on_ExpandItems_toggled(bool on);

private:
    void doFilterTree();

private:
    Ui::FileListGUI *ui;

    QStringList m_fileList;
	QString m_rootPath;
};

#endif // FILELISTGUI_H
