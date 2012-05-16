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
	states = new StateList;
	transitions = new TransitionList;

	initInterface();
	initConnections();

	resize(1200, 1000);
	setDesktopCenter();
}

MainWindow::~MainWindow()
{
	delete transitions;
	delete states;
}

void MainWindow::initInterface()
{
	tabScroller = new QTabWidget(this);
	browser = new TextBrowser(tabScroller);
	tabScroller->addTab(browser, conv("XMI"));
	diagramView = new GraphicView(tabScroller);
	tabScroller->addTab(diagramView, conv("Диаграмма"));
	netView = new GraphicView(tabScroller);
	tabScroller->addTab(netView, conv("Сеть Петри"));
	setCentralWidget(tabScroller);

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

void MainWindow::openFile(const QString &fileName)
{
	setCurrentFile(fileName);

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString data = file.readAll();
		browser->setPlainText(data);

		processData(data);
	}
	file.close();
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

void MainWindow::processData(const QString &data)
{
	XMLEngine engine;
	if (engine.parse(data, states, transitions))
	{
		PlanarDrawer * planarDrawer = new PlanarDrawer(states);
		diagramView->drawDiagram(planarDrawer, states);
		delete planarDrawer;
	}
}
