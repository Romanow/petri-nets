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

	virtual void drawDiagram(PlanarDrawer * drawer) = 0;

protected:
	QList<DiagramTransitionItem *> transitions;
	QList<DiagramItem *> items;
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

	 QGraphicsScene * scene;
	 QGraphicsView * view;
	 QPushButton * btnConvertToSimpleNet;
	 QPushButton * btnConvertToColouredNet;
};

class NetworkView : public View
{
	Q_OBJECT

 public:
	 NetworkView(QWidget * parent = 0);
	 ~NetworkView();

	 QList<QGraphicsItem *> selected();
	 void drawDiagram(PlanarDrawer * drawer);
	 void setNetwork(StateList * netStates);

private slots:
	 void play();
	 void stop();
	 void timeout();

 private:
	 void initInterface();
	 void initConnections();

	 bool execution;
	 StateList * states;

	 QTimer * timer;
	 QGraphicsScene * scene;
	 QGraphicsView * view;
	 QPushButton * btnPlay;
	 QPushButton * btnStop;
};

#endif // GRAPHICVIEW_H
