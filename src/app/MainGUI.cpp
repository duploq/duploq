#include "MainGUI.h"
#include "ui_MainGUI.h"

#include "Engine.h"
#include "InputProcessor.h"
#include "ProjectManager.h"
#include "RecentFilesManager.h"
#include "ConsoleGUI.h"
#include "FileListGUI.h"
#include "SettingsGUI.h"
#include "ProjectSettingsGUI.h"
#include "RawOutputGUI.h"
#include "SideBySideOutputGUI.h"
#include "BlockOutputGUI.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QToolBar>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QDebug>
#include <QMessageBox> 
#include <QApplication>
#include <QDesktopWidget>


MainGUI::MainGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainGUI)
{
    ui->setupUi(this);

    //ui->actionExit->setShortcut(QKeySequence::Quit);
	QCoreApplication::setApplicationVersion("0.1");
	QCoreApplication::setOrganizationName("DuploQ");
	QCoreApplication::setApplicationName("DuploQ");

    setWindowIcon(QIcon(":/Icons/MainIcon"));

    ui->actionFindClones->setEnabled(false);

    // process dialog
    m_progressDlg = new QProgressDialog(this);
    m_progressDlg->setMinimumDuration(0);
    m_progressDlg->setWindowModality(Qt::WindowModal);
    m_progressDlg->reset();

	// toolbar
	QToolBar *mainToolBar = addToolBar(tr("Main Toolbar"));
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->setObjectName("MainToolbar");
	mainToolBar->addAction(ui->actionNewProject);
	mainToolBar->addAction(ui->actionOpenProject);
	mainToolBar->addAction(ui->actionEditProject);
	mainToolBar->addSeparator();
	mainToolBar->addAction(ui->actionCheckFiles);
	mainToolBar->addAction(ui->actionCheckDir);
	mainToolBar->addSeparator();
	mainToolBar->addAction(ui->actionSettings);
	mainToolBar->addAction(ui->actionFindClones);

    // console
    m_consoleUI = new ConsoleGUI(this);
    QDockWidget *consoleDock = new QDockWidget(tr("Engine Output"), this);
    consoleDock->setObjectName("ConsoleDock");
    consoleDock->setWidget(m_consoleUI);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    // file list
    m_fileListUI = new FileListGUI(this);
    QDockWidget *fileListDock = new QDockWidget(tr("File List"), this);
    fileListDock->setObjectName("FileListDock");
    fileListDock->setWidget(m_fileListUI);
    addDockWidget(Qt::LeftDockWidgetArea, fileListDock);

    connect(m_fileListUI, &FileListGUI::filesChosen, this, &MainGUI::onFilesChosen);

    // raw output
    m_rawOutputUI = new RawOutputGUI(this);

    // side output
    m_sideOutputUI = new SideBySideOutputGUI(this);

    // block output
    m_blockOutputUI = new BlockOutputGUI(this);

	connect(m_sideOutputUI, &SideBySideOutputGUI::blockShown, m_blockOutputUI, &BlockOutputGUI::showBlock);

    // central UI
    m_mainTabs = new QTabWidget(this);
    m_mainTabs->addTab(m_blockOutputUI, tr("Block Output"));
    m_mainTabs->addTab(m_sideOutputUI, tr("Side-by-Side Output"));
    m_mainTabs->addTab(m_rawOutputUI, tr("Raw Output"));
    setCentralWidget(m_mainTabs);

    // engine
    m_engine = new Engine(this);
    connect(m_engine, &Engine::processOutput, this, &MainGUI::onProcessOutput);
    connect(m_engine, &Engine::errorOutput, this, &MainGUI::onErrorOutput);
    connect(m_engine, &Engine::completed, this, &MainGUI::onCompleted);
    connect(m_engine, &Engine::failed, this, &MainGUI::onFailed);

    // processors
    m_inputProcessor = new InputProcessor;
    m_resultProcessor = new ResultProcessor;

	// project manager
	m_projectManager = new ProjectManager(*m_inputProcessor, *m_engine);

	// recent files manager
	m_recentFilesManager = new RecentFilesManager;
	ui->actionRecentProjects->setMenu(m_recentFilesManager->getRecentFilesMenu());
	connect(m_recentFilesManager, &RecentFilesManager::selected, this, &MainGUI::onRecentProjectSelected);

	// update GUI
	reset();
}


MainGUI::~MainGUI()
{
    delete m_resultProcessor;
    delete m_inputProcessor;

	delete m_recentFilesManager;
	delete m_projectManager;

    delete ui;
}


void MainGUI::storeConfig()
{
    //QSettings set("duploq.ini", QSettings::IniFormat);
    QSettings set(QSettings::UserScope, "duploc", "duploc");

    set.beginGroup("GUI");
    set.setValue("WindowMaximized", isMaximized());
    set.setValue("WindowGeometry", saveGeometry());
    set.setValue("WindowState", saveState());
    set.endGroup();

    m_engine->storeConfig(set);
    m_inputProcessor->storeConfig(set);

	m_recentFilesManager->storeConfig(set);
}


void MainGUI::restoreConfig()
{
	//QSettings set("duploq.ini", QSettings::IniFormat);
    QSettings set(QSettings::UserScope, "duploc", "duploc");

    //qDebug() << set.fileName();

    m_fileListUI->restoreConfig(set);
    m_sideOutputUI->restoreConfig(set);
    m_blockOutputUI->restoreConfig(set);

    set.beginGroup("GUI");
    restoreGeometry(set.value("WindowGeometry").toByteArray());
    if (set.value("WindowMaximized").toBool())
    {
        showMaximized();
        setGeometry( QApplication::desktop()->availableGeometry(this) );
    }
    restoreState(set.value("WindowState").toByteArray());
    set.endGroup();

    m_engine->restoreConfig(set);
    m_inputProcessor->restoreConfig(set);

	m_recentFilesManager->restoreConfig(set);

	updateActions();
}


void MainGUI::closeEvent(QCloseEvent *event)
{
    storeConfig();

    QMainWindow::closeEvent(event);
}


void MainGUI::on_actionOpenList_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Select file list..."), "", "*.txt");
    if (filePath.isEmpty())
        return;

    setWindowFilePath(filePath);
    updateHeader();

    m_fileListPath = filePath;
    m_fileResultPath = "output.xml";

    m_fileList = m_inputProcessor->readFileList(m_fileListPath);

    ui->actionFindClones->setEnabled(true);
    ui->actionFindClones->trigger();
}


void MainGUI::on_actionCheckFiles_triggered()
{
    QStringList filePathList = QFileDialog::getOpenFileNames(nullptr, tr("Select file(s) to check..."), "", "*.*");
    if (filePathList.isEmpty())
        return;

    setWindowFilePath(tr("%1 file(s)").arg(filePathList.size()));
    updateHeader();

    createTempFileList(filePathList);
    createTempResultFile();

    m_fileList = m_inputProcessor->readFileList(m_fileListPath);

    ui->actionFindClones->setEnabled(true);
    ui->actionFindClones->trigger();
}


void MainGUI::on_actionCheckDir_triggered()
{
    const auto &inputOpts = m_inputProcessor->getOptions();
    QString header(
                tr("[%1] [%2] - Select directory to check...")
                .arg(inputOpts.dirsRecursive ? tr("Recursive") : tr("Non-recursive"))
                .arg(inputOpts.fileFilters.isEmpty() ? tr("All files") : inputOpts.fileFilters)
    );

    QString dirPath = QFileDialog::getExistingDirectory(nullptr, header, "");
    if (dirPath.isEmpty())
        return;

    setWindowFilePath(dirPath);
    updateHeader();

    m_fileList = m_inputProcessor->createFileList(dirPath);

    createTempFileList(m_fileList);
    createTempResultFile();

    ui->actionFindClones->setEnabled(true);
    ui->actionFindClones->trigger();
}


void MainGUI::on_actionNewProject_triggered()
{
	QString filePath = QFileDialog::getSaveFileName(nullptr, tr("New project file..."), "", "duploq project file (*.dqp)");
	if (filePath.isEmpty())
		return;

    if (!filePath.endsWith(".dqp"))
    {
        filePath += ".dqp"; 
    }

	if (!m_projectManager->createProject(filePath))
		return;

	m_recentFilesManager->addRecentDocument(filePath);

	setWindowFilePath(filePath);
	updateHeader();
	updateActions();

	m_consoleUI->clear();
	m_rawOutputUI->clear();
	m_sideOutputUI->clear();
	m_blockOutputUI->clear();

	// show current settings as well
	on_actionEditProject_triggered();
	on_actionSettings_triggered();

	QFileInfo projectFileInfo(filePath);
	QString dirPath = projectFileInfo.absolutePath();
	m_fileList = m_inputProcessor->createFileList(dirPath, m_projectManager->getSubdirectories());
	m_fileListUI->setFileList(m_fileList, dirPath);

	ui->actionFindClones->setEnabled(true);
}


void MainGUI::onRecentProjectSelected(const QString& filePath)
{
	doOpenProject(filePath);
}


void MainGUI::on_actionOpenProject_triggered()
{
	QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Open project file..."), "", "duploq project file (*.dqp)");

	doOpenProject(filePath);
}


void MainGUI::doOpenProject(const QString& filePath)
{
	if (filePath.isEmpty())
		return;

	if (!m_projectManager->openProject(filePath))
		return;

	m_recentFilesManager->addRecentDocument(filePath);

	setWindowFilePath(filePath);
	updateHeader();
	updateActions();

	m_consoleUI->clear();
	m_rawOutputUI->clear();
	m_sideOutputUI->clear();
	m_blockOutputUI->clear();

	QFileInfo projectFileInfo(filePath);
	QString dirPath = projectFileInfo.absolutePath();
	m_fileList = m_inputProcessor->createFileList(dirPath, m_projectManager->getSubdirectories());
	m_fileListUI->setFileList(m_fileList, dirPath);

	ui->actionFindClones->setEnabled(true);
}


void MainGUI::on_actionEditProject_triggered()
{
	ProjectSettingsGUI projectSettingsDialog;
	bool ok = projectSettingsDialog.exec(*m_projectManager);

	if (ok && m_projectManager->hasProject())
		m_projectManager->saveCurrentProject();
}


void MainGUI::on_actionCloseProject_triggered()
{
	m_projectManager->closeCurrentProject();

	reset();
}


void MainGUI::on_actionFindClones_triggered()
{
    m_consoleUI->clear();
    m_rawOutputUI->clear();
    m_sideOutputUI->clear();
    m_blockOutputUI->clear();

	QString dirPath;

	if (m_projectManager->hasProject())
	{
		QFileInfo projectFileInfo(m_projectManager->getCurrentProject());
		dirPath = projectFileInfo.absolutePath();
		m_fileList = m_inputProcessor->createFileList(dirPath, m_projectManager->getSubdirectories());
		createTempFileList(m_fileList);
		createTempResultFile();
	}

    m_fileListUI->setFileListFrom(m_fileListPath, dirPath);

    runProcess();
}


void MainGUI::on_actionSettings_triggered()
{
    SettingsGUI settingsDialog;
    bool ok = settingsDialog.exec(*m_inputProcessor, *m_engine);

	if (ok && m_projectManager->hasProject())
		m_projectManager->saveCurrentProject();
}


void MainGUI::on_actionAbout_triggered()
{
	QMessageBox aboutBox;

	aboutBox.setIconPixmap(QPixmap(":/Icons/MainIcon"));
	aboutBox.setWindowTitle(qApp->applicationDisplayName() + " " + qApp->applicationVersion());

	QString text = tr("<b>UI frontend for Duplo clone detector</b>");
	text += "<br><br>";
	text += tr("This is a free software.<br>It comes without warranty of any kind. Use it on your own risk.");
	text += "<br><br>";
	text += tr("2020 &copy; Ars L. Masiuk");
	text += "<hr>";
	text += tr("<i>This software uses:</i><br>");
	text += QString("&nbsp; Qt %1").arg(QT_VERSION_STR) + "<br>";
	text += QString("&nbsp; Duplo %1").arg(DUPLO_VERSION) + "<br>";
	text += QString("&nbsp; Icons from flaticon.com") + "<br>";


	aboutBox.setText(text);

	aboutBox.exec();
}


void MainGUI::onProcessOutput(const QString &output)
{
    m_consoleUI->append(output);
}


void MainGUI::onErrorOutput(const QString &output)
{
    m_consoleUI->append(output);
}


void MainGUI::onCompleted()
{
    m_consoleUI->append(tr("*** Analyze finished successfully ***"));

    m_rawOutputUI->setContentFrom(m_fileResultPath);

    if (m_resultProcessor->processFrom(m_fileResultPath, m_results))
    {
        m_fileListUI->setFileListResults(m_results);
        m_blockOutputUI->setResults(m_results);
    }

    m_progressDlg->reset();
}


void MainGUI::onFailed()
{
    m_consoleUI->append(tr("*** Analyze failed ***"));

    m_progressDlg->reset();
}


void MainGUI::onFilesChosen(const QString &filePath1, const QString &filePath2)
{
    m_mainTabs->setCurrentIndex(1);

    m_sideOutputUI->setContentFrom(filePath1, filePath2, m_results);
}


// privates

void MainGUI::createTempFileList(const QStringList &filePathList)
{
    m_fileListPath = "input.txt";

    QFile fil(m_fileListPath);
    if (fil.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream ts(&fil);
        for (const auto &filePath: filePathList)
        {
            ts << filePath << "\n";
        }
    }
}


void MainGUI::createTempResultFile()
{
    m_fileResultPath = "output.xml";
}


void MainGUI::runProcess()
{
    if (m_engine->start(m_fileListPath, m_fileResultPath))
    {
        m_progressDlg->setLabelText(tr("Processing %1 input files, please wait...").arg(m_fileList.size()));
        m_progressDlg->setRange(0, 0);
        m_progressDlg->show();
    }
}


void MainGUI::updateHeader()
{
    setWindowTitle(windowFilePath() + "[*] - " + qApp->applicationDisplayName() + " " + qApp->applicationVersion());
}


void MainGUI::updateActions()
{
	bool hasProject = m_projectManager->hasProject();
	ui->actionEditProject->setEnabled(hasProject);
	ui->actionCloseProject->setEnabled(hasProject);
	ui->actionSaveProject->setEnabled(hasProject);
	ui->actionOpenList->setEnabled(!hasProject);
	ui->actionCheckDir->setEnabled(!hasProject);
	ui->actionCheckFiles->setEnabled(!hasProject);

	bool hasRecent = m_recentFilesManager->getRecentFilesList().count();
	ui->actionRecentProjects->setEnabled(hasRecent);
}


void MainGUI::reset()
{
	setWindowFilePath(tr("<Nothing loaded>"));
	updateHeader();
	updateActions();

	m_consoleUI->clear();
	m_rawOutputUI->clear();
	m_sideOutputUI->clear();
	m_blockOutputUI->clear();
	m_fileListUI->clear();

	m_fileList.clear();
	m_fileListPath.clear();
	m_fileResultPath.clear();
}
