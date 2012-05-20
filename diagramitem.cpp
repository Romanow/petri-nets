#include "diagramitem.h"
#include "state.h"

#include <QMenu>
#include <QPainter>
#include <QGraphicsScene>

DiagramItem::DiagramItem(State * state) : m_state(state)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptsHoverEvents(true);
}

void DiagramItem::setMenu(QMenu * menu) {}

// Diagram begin item

DiagramBeginItem::DiagramBeginItem(State * state) : DiagramItem(state) {}

void DiagramBeginItem::setMenu(QMenu * menu)
{
	m_menu = menu;
}

QRectF DiagramBeginItem::boundingRect() const
{
	return QRectF(- 6, - 6, 12, 12);
}

void DiagramBeginItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawEllipse(- 10, - 10, 20, 20);
}

void DiagramBeginItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

void DiagramBeginItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	scene()->clearSelection();
	setSelected(true);
	m_menu->exec(event->screenPos());
}

// Diagram final item

DiagramFinalItem::DiagramFinalItem(State * state) : DiagramItem(state) {}

QRectF DiagramFinalItem::boundingRect() const
{
	return QRectF(- 6, - 6, 12, 12);
}

void DiagramFinalItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::white);
	painter->drawEllipse(- 10, - 10, 20, 20);
	painter->setBrush(Qt::black);
	painter->drawEllipse(- 7, - 7, 14, 14);
}

void DiagramFinalItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram action item

DiagramActionItem::DiagramActionItem(State * state) : DiagramItem(state) {}

void DiagramActionItem::setMenu(QMenu * menu)
{
	m_menu = menu;
}

QRectF DiagramActionItem::boundingRect() const
{
	return QRectF(- 48, - 15, 96, 30);
}

void DiagramActionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::white);
	painter->drawRoundedRect(- 50, - 15, 100, 30, 15, 15);
}

void DiagramActionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

void DiagramActionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	scene()->clearSelection();
	setSelected(true);
	m_menu->exec(event->screenPos());
}

// Diagram condition item

DiagramConditionItem::DiagramConditionItem(State * state) : DiagramItem(state) {}

QRectF DiagramConditionItem::boundingRect() const
{
	return QRectF(- 10, - 10, 20, 20);
}

void DiagramConditionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QPolygon polygon;
	polygon.setPoints(4, 0, - 15, 15, 0, 0, 15, - 15, 0);
	painter->setBrush(Qt::white);
	painter->drawPolygon(polygon);
}

void DiagramConditionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram merge item

DiagramMergeItem::DiagramMergeItem(State * state) : DiagramItem(state) {}

QRectF DiagramMergeItem::boundingRect() const
{
	return QRectF(- 40, - 2, 80, 4);
}

void DiagramMergeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawRect(- 40, - 2, 80, 4);
}

void DiagramMergeItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram fork item

DiagramForkItem::DiagramForkItem(State * state) : DiagramItem(state) {}

QRectF DiagramForkItem::boundingRect() const
{
	return QRectF(- 40, - 2, 80, 4);
}

void DiagramForkItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawRect(- 40, - 2, 80, 4);
}

void DiagramForkItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram transition

DiagramTransitionItem::DiagramTransitionItem(DiagramItem * source, DiagramItem * target) :
	source(source), target(target) {}

QRectF DiagramTransitionItem::boundingRect() const {}

void DiagramTransitionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->drawLine(source->pos(), target->pos());
}

DiagramGridItem::DiagramGridItem(QList<QLine> horizontal, QList<QLine> vertical) :
	horizontal(horizontal), vertical(vertical) {}

QRectF DiagramGridItem::boundingRect() const {}

void DiagramGridItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	foreach (QLine line, horizontal)
		painter->drawLine(120 * line.x1(), 60 * line.y1(), 120 * line.x2(), 60 * line.y2());

	foreach (QLine line, vertical)
		painter->drawLine(120 * line.x1(), 60 * line.y1(), 120 * line.x2(), 60 * line.y2());
}

// Diagram net place item

DiagramNetPlaceItem::DiagramNetPlaceItem(State * state) : DiagramItem(state) {}

void DiagramNetPlaceItem::setMenu(QMenu * menu)
{
	m_menu = menu;
}

QRectF DiagramNetPlaceItem::boundingRect() const
{
	return QRectF(- 9, - 9, 18, 18);
}

void DiagramNetPlaceItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::white);
	painter->drawEllipse(- 12, - 12, 24, 24);
	NetPlace * place = dynamic_cast<NetPlace *>(state());
	if (place->marking() > 0)
		if (place->marking() < 5)
		{
			painter->setBrush(Qt::black);
			switch (place->marking())
			{
			case 1:
				painter->drawEllipse(QPoint(0, 0), 2, 2);
				break;

			case 2:
				painter->drawEllipse(QPoint(- 5, 0), 2, 2);
				painter->drawEllipse(QPoint(5, 0), 2, 2);
				break;

			case 3:
				painter->drawEllipse(QPoint(- 5, - 5), 2, 2);
				painter->drawEllipse(QPoint(5, - 5 ), 2, 2);
				painter->drawEllipse(QPoint(0, 5), 2, 2);
				break;

			case 4:
				painter->drawEllipse(QPoint(- 5, - 5), 2, 2);
				painter->drawEllipse(QPoint(5, - 5), 2, 2);
				painter->drawEllipse(QPoint(- 5, 5), 2, 2);
				painter->drawEllipse(QPoint(5, 5), 2, 2);
				break;
			}
		}
		else
			painter->drawText(QRectF(- 10, - 10, 20, 20), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(place->marking()));
}

void DiagramNetPlaceItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

void DiagramNetPlaceItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	scene()->clearSelection();
	setSelected(true);
	m_menu->exec(event->screenPos());
}

// Diagram net transition item

DiagramNetTransitionItem::DiagramNetTransitionItem(State * state) : DiagramItem(state) {}

QRectF DiagramNetTransitionItem::boundingRect() const
{
	return QRectF(- 25, - 2, 50, 4);
}

void DiagramNetTransitionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawRect(- 25, - 2, 50, 4);
}

void DiagramNetTransitionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}
