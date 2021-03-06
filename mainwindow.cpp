#include "mainwindow.h"
#include "xmlengine.h"
#include "translate.h"

#include <QSettings>
#include <QMenuBar>
#include <QFileInfo>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow() : QMainWindow()
{
	states = new StateList;
	netStates = new StateList;
	transitions = new TransitionList;
	netTransitions = new TransitionList;
	net = new PetriNet;

	dataDialog = new InputDataDialog;

	initInterface();
	initConnections();

	resize(1200, 1000);
	setDesktopCenter();
}

MainWindow::~MainWindow()
{
	delete net;
	delete transitions;
	delete netTransitions;
	delete states;
	delete netStates;
	delete dataDialog;

	qDeleteAll(variables);
	variables.clear();
}

void MainWindow::initInterface()
{
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
	connect(networkView, SIGNAL(reinitMarking()), SLOT(reinitMarking()));
	connect(networkView, SIGNAL(reinitValues()), SLOT(reinitValues()));
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
	 clear();
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
		clear();

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

void MainWindow::clear()
{
	browser->clear();

	clearStates();
	clearNetStates();

	tabWidget->setCurrentIndex(0);
	tabWidget->setTabEnabled(2, false);
}

void MainWindow::clearStates()
{
	diagramView->clear();

	qDeleteAll(states->begin(), states->end());
	states->clear();
	qDeleteAll(netStates->begin(), netStates->end());
	netStates->clear();
}

void MainWindow::clearNetStates()
{
	networkView->clear();

	qDeleteAll(netStates->begin(), netStates->end());
	netStates->clear();
	qDeleteAll(netTransitions->begin(), netTransitions->end());
	netTransitions->clear();

	qDeleteAll(variables);
	variables.clear();
}

void MainWindow::convertToSimpleNet()
{
	clearNetStates();

	net->convert(states, netStates, netTransitions);
	net->setInitialMarking(states, netStates);

	PlanarDrawer netPlanarDrawer(netStates);
	networkView->drawDiagram(&netPlanarDrawer);
	networkView->setNetwork(states, netStates);

	tabWidget->setTabEnabled(2, true);
	tabWidget->setCurrentIndex(2);
}

void MainWindow::reinitMarking()
{
	net->initVariables(netStates, variables);
	net->setInitialMarking(states, netStates);
	networkView->update();
}

void MainWindow::reinitValues()
{
	dataDialog->setVariableList(variables);
	if (dataDialog->exec() == QDialog::Accepted)
	{
		dataDialog->getVariableList(variables);
		net->initVariables(netStates, variables);
		net->setInitialMarking(states, netStates);
		networkView->update();
	}
}

void MainWindow::convertToColouredNet()
{
	clearNetStates();

	// Выделение внутренних переменных состояния и построение списка типов
	variables = net->variableList(states, transitions);
	// Преобразование в простую сеть Петри
	net->convert(states, netStates, netTransitions, true);
	if (!variables.isEmpty())
	{
		// Основываясь на списке внутренних переменных выстраивается множество раскрасок сети
		net->coloring(netStates, variables);
		// Введение начальных значений переменных
		dataDialog->setVariableList(variables);
		if (dataDialog->exec() == QDialog::Accepted)
		{
			dataDialog->getVariableList(variables);
			// Задание начальных значений переменных с привязкой к состоянию сети
			net->initVariables(netStates, variables);
		}
	}
	// Установка фишки в начальную позицию
	net->setInitialMarking(states, netStates);

	// Отображение сети
	PlanarDrawer netPlanarDrawer(netStates);
	networkView->drawDiagram(&netPlanarDrawer);
	networkView->setNetwork(states, netStates);

	tabWidget->setTabEnabled(2, true);
	tabWidget->setCurrentIndex(2);
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
