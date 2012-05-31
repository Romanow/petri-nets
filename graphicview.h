#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QTimer>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "state.h"
#include "drawer.h"
#include "diagramitem.h"

class View : public QWidget
{
public:
	View(QWidget * parent = 0);
	virtual ~View();

	virtual void drawDiagram(PlanarDrawer * drawer) = 0;
	void clear();

protected:
	QList<DiagramTransitionItem *> transitions;
	QList<DiagramItem *> items;

	QGraphicsScene * scene;
	QGraphicsView * view;
};

class DiagramView : public View
{
	Q_OBJECT

 public:
	 DiagramView(QWidget * parent = 0);
	 ~DiagramView();

	 void drawDiagram(PlanarDrawer * drawer);

signals:
	 void convertToSimpleNet();
	 void convertToColouredNet();

 private:
	 void initInterface();
	 void initConnections();

	 QPushButton * btnConvertToSimpleNet;
	 QPushButton * btnConvertToColouredNet;
};

class NetworkView : public View
{
	Q_OBJECT

 public:
	 NetworkView(QWidget * parent = 0);
	 ~NetworkView();

	 void drawDiagram(PlanarDrawer * drawer);
	 void setNetwork(StateList * states, StateList * netStates);

signals:
	 void reinitMarking();
	 void reinitValues();

private slots:
	 void play();
	 void stop();
	 void timeout();

 private:
	 void initInterface();
	 void initConnections();
	 QMultiMap<NetPlace *, NetTransition *> activeTransitions();

	 void information();

	 bool execution;
	 StateList * states;
	 StateList * netStates;

	 QTimer * timer;
	 QGraphicsScene * scene;
	 QGraphicsView * view;
	 QPushButton * btnPlay;
	 QPushButton * btnStop;
	 QPushButton * btnRefresh;
};

#endif // GRAPHICVIEW_H
