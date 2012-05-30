#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QMainWindow>

#include "drawer.h"
#include "petrinet.h"
#include "textbrowser.h"
#include "graphicview.h"
#include "inputdatadialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private slots:
	void clear();
	void newFile();
	void openFile();
	void openRecentFile();
	void convertToSimpleNet();
	void convertToColouredNet();

private:
	void initInterface();
	void initConnections();
	void setDesktopCenter();
	bool updateRecentFileList(QMenu * recentFileMenu);
	void setCurrentFile(const QString &file);
	void openFile(const QString &file);
	void processData(const QString &data);
	void clearNetStates();
	void clearStates();

	PetriNet * net;
	QMap<QString, Type *> variables;
	StateList * states, * netStates;
	TransitionList * transitions, * netTransitions;

	InputDataDialog * dataDialog;

	QTabWidget * tabWidget;
	TextBrowser * browser;
	DiagramView * diagramView;
	NetworkView * networkView;
	QMenu * fileMenu;
	QMenu * recentFileMenu;
	QAction * actionNewFile;
	QAction * actionOpenFile;
	QAction * actionRecentFile[5];
	QAction * actionExit;
};

#endif // MAINWINDOW_H
