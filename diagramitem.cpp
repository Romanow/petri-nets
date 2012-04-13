#include "diagramitem.h"

#include <QPainter>

DiagramItem::DiagramItem(State * state) : state(state)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptsHoverEvents(true);
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

DiagramTransition::DiagramTransition(DiagramItem * sorce) : source(source) {}

QRectF DiagramTransition::boundingRect() const {}

void DiagramTransition::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

}
