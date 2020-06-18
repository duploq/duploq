#include "MainGUI.h"
#include "ui_MainGUI.h"

#include "Engine.h"
#include "InputProcessor.h"
#include "ConsoleGUI.h"
#include "FileListGUI.h"
#include "SettingsGUI.h"
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


MainGUI::MainGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainGUI)
{
    ui->setupUi(this);

    //ui->actionExit->setShortcut(QKeySequence::Quit);
	QCoreApplication::setApplicationVersion("0.1");
	QCoreApplication::setOrganizationName("qvge");
	QCoreApplication::setApplicationName("duploq");

    setWindowFilePath(tr("<Nothing loaded>"));
    updateHeader();

    ui->actionFindClones->setEnabled(false);

	// toolbar
	QToolBar *mainToolBar = addToolBar(tr("Main Toolbar"));
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
}


MainGUI::~MainGUI()
{
    storeConfig();

    delete m_resultProcessor;
    delete m_inputProcessor;

    delete ui;
}


void MainGUI::storeConfig()
{
    //QSettings set("duploq.ini", QSettings::IniFormat);
    QSettings set(QSettings::UserScope);

    set.beginGroup("GUI");
    set.setValue("WindowGeometry", saveGeometry());
    set.setValue("WindowState", saveState());
    set.endGroup();

    m_fileListUI->storeConfig(set);
	m_sideOutputUI->storeConfig(set);

    m_engine->storeConfig(set);
    m_inputProcessor->storeConfig(set);
}


void MainGUI::restoreConfig()
{
	//QSettings set("duploq.ini", QSettings::IniFormat);
	QSettings set(QSettings::UserScope);

    //qDebug() << set.fileName();

    set.beginGroup("GUI");
    restoreGeometry(set.value("WindowGeometry").toByteArray());
    restoreState(set.value("WindowState").toByteArray());
    set.endGroup();

    m_fileListUI->restoreConfig(set);
	m_sideOutputUI->restoreConfig(set);

    m_engine->restoreConfig(set);
    m_inputProcessor->restoreConfig(set);
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

    QStringList filePathList = m_inputProcessor->createFilelist(dirPath);

    createTempFileList(filePathList);
    createTempResultFile();

    ui->actionFindClones->setEnabled(true);
    ui->actionFindClones->trigger();
}


void MainGUI::on_actionFindClones_triggered()
{
    m_consoleUI->clear();
    m_rawOutputUI->clear();
    m_sideOutputUI->clear();
    m_blockOutputUI->clear();

    m_fileListUI->setFileListFrom(m_fileListPath);

    runProcess();
}


void MainGUI::on_actionSettings_triggered()
{
    SettingsGUI settingsDialog;
    settingsDialog.exec(*m_inputProcessor, *m_engine);
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
}


void MainGUI::onFailed()
{
    m_consoleUI->append(tr("*** Analyze failed ***"));
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
    m_engine->start(m_fileListPath, m_fileResultPath);
}


void MainGUI::updateHeader()
{
    setWindowTitle(windowFilePath() + " - " + qApp->applicationDisplayName() + " " + qApp->applicationVersion());
}

