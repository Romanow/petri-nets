#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QMainWindow>

#include "drawer.h"
#include "petrinet.h"
#include "textbrowser.h"
#include "graphicview.h"
#include "initialmarkingdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private slots:
	void newFile();
	void openFile();
	void openRecentFile();
	void convertToSimpleNet();
	void convertToColouredNet();
	void initialMarking();
	void inputData();

private:
	void initInterface();
	void initConnections();
	void setDesktopCenter();
	bool updateRecentFileList(QMenu * recentFileMenu);
	void setCurrentFile(const QString &file);
	void openFile(const QString &file);
	void processData(const QString &data);

	PetriNet * net;
	StateList * states, * netStates;
	TransitionList * transitions, * netTransitions;

	InitialMarkingDialog * markingDialog;

	QTabWidget * tabWidget;
	TextBrowser * browser;
	DiagramView * diagramView;
	NetworkView * networkView;
	QMenu * netMenu;
	QMenu * diagramMenu;
	QMenu * fileMenu;
	QMenu * recentFileMenu;
	QAction * actionNewFile;
	QAction * actionOpenFile;
	QAction * actionRecentFile[5];
	QAction * actionExit;
};

#endif // MAINWINDOW_H
