#include "graphicview.h"

#include <QVBoxLayout>

GraphicView::GraphicView(QWidget * parent) : QWidget(parent)
{
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
    delete scene;
    delete view;
}

void GraphicView::drawDiagram(StateList &states)
{
	QList<int> levelList;
	QList<State *> queue;
	queue.append(states.find(begin_state));
	levelList.append(1);

	int level = 0;
	int counter = 0;
	int middle = 100;
	while (!queue.isEmpty())
	{
		State * state = queue.takeFirst();

		levelList.append(state->outgoing().count());
		foreach (Transition * transition, state->outgoing())
			queue.append(transition->target());

		QGraphicsItem * item;
		switch (state->type())
		{
		case begin_state:
			item = new DiagramBeginItem;
			break;

		case action_state:
			item = new DiagramActionItem(state);
			break;

		case condition_state:
			item = new DiagramConditionItem;
			break;

		case final_state:
			item = new DiagramFinalItem;
			break;
		}

		item->setPos(middle - (counter / 2) * 40, level * 70);
		scene->addItem(item);

		if (levelList.first() == ++counter)
		{
			levelList.takeFirst();
			counter = 0;
			level++;
		}
	}
}
