#include "mainwindow.h"
#include "xmlengine.h"
#include "translate.h"

#include <QSettings>
#include <QSplitter>
#include <QMenuBar>
#include <QFileInfo>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow() : QMainWindow(0)
{
	initInterface();
	initConnections();

	resize(600, 400);
	setDesktopCenter();
}

MainWindow::~MainWindow() {}

void MainWindow::initInterface()
{
	QSplitter * splitter = new QSplitter(this);
	browser = new TextBrowser(splitter);
	graphicView = new GraphicView(splitter);
	splitter->addWidget(browser);
	splitter->addWidget(graphicView);
	setCentralWidget(splitter);

	// Actions
	// Create new file
	actionNewFile = new QAction(QIcon(":NewFile"), conv("&Новый файл"), this);
	actionNewFile->setShortcut(QKeySequence::New);
	actionNewFile->setToolTip(conv("Создать файл"));
	actionNewFile->setStatusTip(conv("Создать файл"));
	// Open existence file
	actionOpenFile = new QAction(QIcon(":OpenFile"), conv("&Открыть как..."), this);
	actionOpenFile->setShortcut(QKeySequence::Open);
	actionOpenFile->setToolTip(conv("Открыть как..."));
	actionOpenFile->setStatusTip(conv("Открыть как..."));
	// Exit
	actionExit = new QAction(QIcon(), conv("&Выход"), this);
	actionExit->setShortcuts(QKeySequence::Quit);
	actionExit->setStatusTip(conv("Выйти из приложения"));
	actionExit->setToolTip(conv("Выйти из приложения"));

	// Create menu
	// File menu
	fileMenu = menuBar()->addMenu(conv("&Файл"));
	fileMenu->addAction(actionNewFile);
	recentFileMenu = fileMenu->addMenu(QIcon(":OpenFile"), conv("Открыть файл"));
	bool enabled = updateRecentFileList(recentFileMenu);
	recentFileMenu->setEnabled(enabled);
	fileMenu->addAction(actionOpenFile);
	fileMenu->addSeparator();
	fileMenu->addAction(actionExit);
}

void MainWindow::initConnections()
{
	connect(actionNewFile, SIGNAL(triggered()), SLOT(newFile()));
	connect(actionOpenFile, SIGNAL(triggered()), SLOT(openFile()));
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void MainWindow::setDesktopCenter()
{
	move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

bool MainWindow::updateRecentFileList(QMenu * recentFileMenu)
{
	recentFileMenu->clear();

	QSettings settings;
	QStringList files = settings.value("Recent file list").toStringList();
	int count = qMin(files.count(), 5);
	for (int i = 0; i < count; ++i)
	{
		QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
		actionRecentFile[i] = new QAction(text, this);
		actionRecentFile[i]->setData(files[i]);
		connect(actionRecentFile[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
		recentFileMenu->addAction(actionRecentFile[i]);
	}

	return count > 0;
}

void MainWindow::newFile()
{
	browser->clear();
}

void MainWindow::openFile()
{
	QFileDialog dialog(this, conv("Выберета файлы"), "", conv("Описание uml (*.xmi *.xml);;Все файлы (*.*)"));
	dialog.setFileMode(QFileDialog::ExistingFiles);
	if (dialog.exec() == QDialog::Accepted)
	{
		openFile(dialog.selectedFiles().first());
	}
}

void MainWindow::openFile(const QString &file)
{
	setCurrentFile(file);
	browser->openFile(file);

	QFile f(file);
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		XMLEngine engine;
		StateList states = engine.parse(f.readAll());
		graphicView->drawDiagram(states);
	}
	f.close();
}

void MainWindow::openRecentFile()
{
	QAction * action = qobject_cast<QAction *>(sender());
	if (action)
		openFile(action->data().toString());
}

void MainWindow::setCurrentFile(const QString &file)
{
	QSettings settings;
	QStringList files = settings.value("Recent file list").toStringList();
	files.removeAll(file);
	files.prepend(file);
	while (files.count() > 5)
		files.removeLast();

	settings.setValue("Recent file list", files);
	bool enabled = updateRecentFileList(recentFileMenu);
	recentFileMenu->setEnabled(enabled);
}
