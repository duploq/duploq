#ifndef MAINGUI_H
#define MAINGUI_H

#include <QMainWindow>
#include <QString>
#include <QTabWidget>
#include <QProgressDialog>

#include "ResultProcessor.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainGUI; }
QT_END_NAMESPACE


class Engine;
//class ResultProcessor;
class InputProcessor;

class ProjectManager;
class RecentFilesManager;

class ConsoleGUI;
class FileListGUI;
class RawOutputGUI;
class SideBySideOutputGUI;
class BlockOutputGUI;


class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    MainGUI(QWidget *parent = nullptr);
    ~MainGUI();

    void storeConfig();
    void restoreConfig();

protected:
    void closeEvent(QCloseEvent *event) override;

private Q_SLOTS:
    void on_actionOpenList_triggered();
    void on_actionCheckFiles_triggered();
    void on_actionCheckDir_triggered();

	void on_actionNewProject_triggered();
	void on_actionOpenProject_triggered();
	void on_actionEditProject_triggered();
	void on_actionCloseProject_triggered();
	void onRecentProjectSelected(const QString& filePath);

    void on_actionFindClones_triggered();
    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

    void onProcessOutput(const QString &output);
    void onErrorOutput(const QString &output);
    void onCompleted();
    void onFailed();

    void onFilesChosen(const QString& filePath1, const QString& filePath2);

private:
    void createTempFileList(const QStringList &filePathList);
    void createTempResultFile();
    void runProcess();

    void updateHeader();
	void updateActions();
	void reset();

	void doOpenProject(const QString& filePath);

private:
    Ui::MainGUI *ui;

    ConsoleGUI *m_consoleUI;
    FileListGUI *m_fileListUI;
    QTabWidget *m_mainTabs;
    RawOutputGUI *m_rawOutputUI;
    SideBySideOutputGUI *m_sideOutputUI;
    BlockOutputGUI *m_blockOutputUI;

    Engine *m_engine = nullptr;
    InputProcessor *m_inputProcessor = nullptr;
    ResultProcessor *m_resultProcessor = nullptr;
    QString m_fileListPath;
    QString m_fileResultPath;
    ResultInfo m_results;

    QString m_headerInfo;

    QStringList m_fileList;
    QProgressDialog *m_progressDlg = nullptr;

	ProjectManager *m_projectManager = nullptr;
	RecentFilesManager *m_recentFilesManager = nullptr;
};

#endif // MAINGUI_H
