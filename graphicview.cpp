#include "graphicview.h"

#include <QDebug>
#include <QVBoxLayout>

GraphicView::GraphicView(QWidget * parent) : QWidget(parent)
{
	itemList = new DiagramItemList;
	transitionList = new DiagramTransitionList;

    scene = new QGraphicsScene;
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
	while (!itemList->isEmpty())
	{
		DiagramItem * item = itemList->takeFirst();
		delete item;
	}
	delete itemList;
	delete transitionList;

	delete scene;
	delete view;
}

void GraphicView::drawDiagram(StateList &states)
{
	QList<State *> queue;
	State * state = states.find(begin_state).first();
	queue.append(state);

	int level = 0;
	while (!queue.isEmpty())
	{
		State * state = queue.takeFirst();
		DiagramItem * item = itemList->find(state);
		if (!item)
			item = state->diagramItem();

		foreach (Transition * transition, state->incoming())
		{
			State * sourceState = transition->source();

			DiagramItem * sourceItem = itemList->find(sourceState);
			if (!sourceItem)
			{
				sourceItem = sourceState->diagramItem();
				itemList->append(sourceItem);
				queue.append(sourceState);
			}

			if (!transitionList->find(sourceState, state))
			{
				DiagramTransition * diagramTransition = new DiagramTransition(sourceItem, item);
				transitionList->append(diagramTransition);
				scene->addItem(diagramTransition);
			}
		}

		foreach (Transition * transition, state->outgoing())
		{
			State * targetState = transition->target();

			DiagramItem * targetItem = itemList->find(targetState);
			if (!targetItem)
			{
				targetItem = targetState->diagramItem();
				itemList->append(targetItem);
				queue.append(targetState);
			}

			if (!transitionList->find(state, targetState))
			{
				DiagramTransition * diagramTransition = new DiagramTransition(item, targetItem);
				transitionList->append(diagramTransition);
				scene->addItem(diagramTransition);
			}
		}
		item->setPos(100, level * 50);
		scene->addItem(item);
		itemList->append(item);

		level++;
	}
}
