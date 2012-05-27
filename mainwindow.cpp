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

#include <QDebug>

MainWindow::MainWindow() : QMainWindow()
{
	states = new StateList;
	netStates = new StateList;
	transitions = new TransitionList;
	netTransitions = new TransitionList;
	net = new PetriNet;

	markingDialog = new InitialMarkingDialog;
	dataDialog = new InputDataDialog;

	initInterface();
	initConnections();

	resize(1200, 1000);
	setDesktopCenter();
}

MainWindow::~MainWindow()
{
	delete transitions;
	delete netTransitions;
	delete states;
	delete netStates;
	delete net;
}

void MainWindow::initInterface()
{
	netMenu = new QMenu;
	netMenu->addAction(conv("Начальная разметка"), this, SLOT(initialMarking()));

	tabWidget = new QTabWidget(this);
	browser = new TextBrowser(tabWidget);
	tabWidget->addTab(browser, conv("XMI"));
	diagramView = new DiagramView(tabWidget);
	tabWidget->addTab(diagramView, conv("Диаграмма"));
	networkView = new NetworkView(tabWidget);
	tabWidget->addTab(networkView, conv("Сеть Петри"));
	tabWidget->setTabEnabled(2, false);
	setCentralWidget(tabWidget);

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
	connect(diagramView, SIGNAL(convertToSimpleNet()), SLOT(convertToSimpleNet()));
	connect(diagramView, SIGNAL(convertToColouredNet()), SLOT(convertToColouredNet()));
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

void MainWindow::initialMarking()
{
	QList<QGraphicsItem *> selected = networkView->selected();
	if (!selected.isEmpty())
	{
		DiagramItem * item = dynamic_cast<DiagramItem *>(selected.first());
		NetPlace * place = dynamic_cast<NetPlace *>(item->state());
		markingDialog->setMarking(place->marking());
		if (markingDialog->exec() == QDialog::Accepted)
		{
			int marking = markingDialog->getMarking();
			place->setMarking(marking);
		}
	}
}

void MainWindow::convertToSimpleNet()
{
	net->convert(states, netStates, netTransitions);
	PlanarDrawer netPlanarDrawer(netStates, netMenu);
	networkView->drawDiagram(&netPlanarDrawer);
	networkView->setNetwork(netStates);

	tabWidget->setTabEnabled(2, true);
	tabWidget->setCurrentIndex(2);
}

void MainWindow::convertToColouredNet()
{
	types = net->variableList(states);
	net->convert(states, netStates, netTransitions);
	net->coloring(netStates, types);

	dataDialog->setVariableList(types);
	if (dataDialog->exec() == QDialog::Accepted)
	{
		dataDialog->getVariableList(types);
		PlanarDrawer netPlanarDrawer(netStates, netMenu);
		networkView->drawDiagram(&netPlanarDrawer);
		networkView->setNetwork(netStates);

		tabWidget->setTabEnabled(2, true);
		tabWidget->setCurrentIndex(2);
	}
}

void MainWindow::processData(const QString &data)
{
	XMLEngine engine;
	if (engine.parse(data, states, transitions))
	{
		PlanarDrawer diagramPlanarDrawer(states);
		diagramView->drawDiagram(&diagramPlanarDrawer);
	}
}
