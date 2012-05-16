#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QMainWindow>

#include "drawer.h"
#include "textbrowser.h"
#include "graphicview.h"

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

private:
	void initInterface();
	void initConnections();
	void setDesktopCenter();
	bool updateRecentFileList(QMenu * recentFileMenu);
	void setCurrentFile(const QString &file);
	void openFile(const QString &file);
	void processData(const QString &data);

	StateList * states;
	TransitionList * transitions;

	QTabWidget * tabScroller;
	TextBrowser * browser;
	GraphicView * diagramView;
	GraphicView * netView;
	QMenu * fileMenu;
	QMenu * recentFileMenu;
	QAction * actionNewFile;
	QAction * actionOpenFile;
	QAction * actionRecentFile[5];
	QAction * actionExit;
};

#endif // MAINWINDOW_H
