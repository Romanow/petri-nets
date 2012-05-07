#include "graphicview.h"

#include <QDebug>
#include <QVBoxLayout>

GraphicView::GraphicView(QWidget * parent) : QWidget(parent)
{
	itemList = new DiagramItemList;
	transitionList = new DiagramTransitionList;

    scene = new QGraphicsScene(QRect(- 200, - 200, 400, 400));
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

int dive(State * state, int level)
{
	int max = level;
	int height = level;
	foreach (Transition * transition, state->incoming())
	{
		State * sourceState = transition->source();
		if (sourceState->type() != fork_state)
		{
			height = dive(sourceState, level + 1);
		}
		max = max >= height ? max : height;
	}

	return max;
}

void GraphicView::drawDiagram(StateList &states)
{
	QList<State *> stateQueue;
	QList<int> levelQueue;

	State * state = states.find(begin_state).first();
	stateQueue.append(state);
	levelQueue.append(0);

	QList< QList<State *> > levels;

	while (!stateQueue.isEmpty())
	{
		State * state = stateQueue.takeFirst();
		int level = levelQueue.takeFirst();

		DiagramItem * item = itemList->find(state);
		if (!item)
		{
			item = state->diagramItem();
			itemList->append(item);
		}

		QList<State *> list;
		foreach (Transition * transition, state->outgoing())
		{
			State * targetState = transition->target();

			DiagramItem * targetItem = itemList->find(targetState);
			if (!targetItem)
			{
				targetItem = targetState->diagramItem();
				itemList->append(targetItem);

				stateQueue.append(targetState);
				if (targetState->type() == merge_state)
				{
					int height = dive(targetState, 1);
					qDebug() << height;
					levelQueue.append(level);
				}
				else if (targetState->type() == final_state)
				{
					levelQueue.append(0); // Не требуется, т.к. для конечной вершины нет исходящих переходов
				}
				else
				{
					levelQueue.append(level + 1);
					list.append(targetState);
				}
			}

			if (!transitionList->find(transition->id()))
			{
				DiagramTransition * diagramTransition = new DiagramTransition(transition->id(), item, targetItem);
				transitionList->append(diagramTransition);
				scene->addItem(diagramTransition);
			}
		}
		if (!list.isEmpty())
		{
			if (level >= levels.count())
				levels.append(list);
			else
				levels[level].append(list);
		}

		scene->addItem(item);
	}

	QList<State *> list;
	list.append(states.find(final_state).first());
	levels.append(list);

	int i, j = 0;
	double r = 0;
	foreach (QList<State *> list, levels)
	{
		j++; i = 0;
		r = - 50 * (list.count() - 1);
		foreach (State * state, list)
		{
			if (state)
			{
				DiagramItem * item = itemList->find(state);
				item->setPos(r + i * 130, j * 50);
				i++;
			}
		}
	}
}
