#include "diagramitem.h"
#include "state.h"

#include <QDebug>
#include <QPainter>

DiagramItem * DiagramItemList::find(State * state)
{
	for (QList<DiagramItem *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->compareState(state))
			return (* iter);

	return 0;
}

DiagramTransition * DiagramTransitionList::find(State * source, State * target)
{
	for (QList<DiagramTransition *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->compareState(source, target))
			return (* iter);

	return 0;
}

DiagramItem::DiagramItem(State * state) : state(state)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptsHoverEvents(true);
}

bool DiagramItem::compareState(State * another)
{
	return state->id() == another->id();
}

void DiagramItem::addIncoming(DiagramTransition * transition)
{
	incoming.append(transition);
}

void DiagramItem::addOutgoing(DiagramTransition * transition)
{
	outgoing.append(transition);
}

// Diagram begin item

DiagramBeginItem::DiagramBeginItem(State * state) : DiagramItem(state) {}

QRectF DiagramBeginItem::boundingRect() const
{
	double penWidth = 1;
	return QRectF(- 10 - penWidth / 2, - 10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void DiagramBeginItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::SolidPattern);
	painter->drawEllipse(- 10, - 10, 20, 20);
}

// Diagram final item

DiagramFinalItem::DiagramFinalItem(State * state) : DiagramItem(state) {}

QRectF DiagramFinalItem::boundingRect() const
{
	double penWidth = 1;
	return QRectF(- 10 - penWidth / 2, - 10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void DiagramFinalItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->drawEllipse(- 10, - 10, 20, 20);
	painter->setBrush(Qt::SolidPattern);
	painter->drawEllipse(- 7, - 7, 14, 14);
}

// Diagram action item

DiagramActionItem::DiagramActionItem(State * state) : DiagramItem(state) {}

QRectF DiagramActionItem::boundingRect() const
{
	double penWidth = 1;
	return QRectF(- 50 - penWidth / 2, - 15 - penWidth / 2, 100 + penWidth, 30 + penWidth);
}

void DiagramActionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->drawRoundedRect(- 50, - 15, 100, 30, 15, 15);
}

// Diagram condition item

DiagramConditionItem::DiagramConditionItem(State * state) : DiagramItem(state) {}

QRectF DiagramConditionItem::boundingRect() const
{
	double penWidth = 1;
	return QRectF(- 15 - penWidth / 2, - 15 - penWidth / 2, 30 + penWidth, 30 + penWidth);
}

void DiagramConditionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QPolygon polygon;
	polygon.setPoints(4,
					  0, - 15,
					  15, 0,
					  0, 15,
					  - 15, 0);
	painter->drawPolygon(polygon);
}

// Diagram merge item

DiagramMergeItem::DiagramMergeItem(State * state) : DiagramItem(state) {}

QRectF DiagramMergeItem::boundingRect() const {}

void DiagramMergeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {}

// Diagram fork item

DiagramForkItem::DiagramForkItem(State * state) : DiagramItem(state) {}

QRectF DiagramForkItem::boundingRect() const {}

void DiagramForkItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {}

// Diagram transition

DiagramTransition::DiagramTransition(DiagramItem * source, DiagramItem * target) : source(source), target(target) {}

QRectF DiagramTransition::boundingRect() const
{
	int penWidth = 1;
	QPointF p1 = source->pos();
	QPointF p2 = target->pos();

	return QRectF(0, 0, abs(p1.x() + p2.x()) + penWidth, abs(p1.y() + p2.y()) + penWidth);
}

void DiagramTransition::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QPointF p1 = source->pos();
	QPointF p2 = target->pos();
	painter->drawLine(p1, p2);
}

bool DiagramTransition::compareState(State * s, State * t)
{
	return (source->compareState(s) && target->compareState(t));
}
