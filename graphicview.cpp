#include "graphicview.h"

#include <QDebug>

#include <QPair>
#include <QStack>
#include <QVBoxLayout>

GraphicView::GraphicView(QWidget * parent) : QWidget(parent)
{
	scene = new QGraphicsScene(QRect(- 100, - 100, 1000, 1000));
	view = new QGraphicsView(scene, this);
	view->setRenderHint(QPainter::Antialiasing, true);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	view->setDragMode(QGraphicsView::ScrollHandDrag);

    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->addWidget(view);
	vLayout->setMargin(0);
    setLayout(vLayout);
}

GraphicView::~GraphicView()
{
	delete scene;
	delete view;
}

void GraphicView::drawDiagram(PlanarDrawer * drawer, StateList * states)
{
	scene = drawer->draw();
	view->setScene(scene);
	scene->update();
}
