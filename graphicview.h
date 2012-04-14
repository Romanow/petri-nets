#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "state.h"
#include "diagramitem.h"

class GraphicView : public QWidget
{
    Q_OBJECT

public:
    GraphicView(QWidget * parent = 0);
    ~GraphicView();

	void drawDiagram(StateList &states);

private:
    QGraphicsScene * scene;
    QGraphicsView * view;
	DiagramItemList * itemList;
	DiagramTransitionList * transitionList;
};

#endif // GRAPHICVIEW_H
