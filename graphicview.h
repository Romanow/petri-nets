#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "state.h"
#include "drawer.h"
#include "diagramitem.h"

class GraphicView : public QWidget
{
    Q_OBJECT

public:
	GraphicView(QWidget * parent = 0);
    ~GraphicView();

	void drawDiagram(PlanarDrawer * drawer, StateList * states);

private:
	Drawer * drawer;
    QGraphicsScene * scene;
    QGraphicsView * view;
    QList<DiagramItem *> itemList;
	QList<DiagramTransitionItem *> transitionList;
};

#endif // GRAPHICVIEW_H
