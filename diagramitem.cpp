#include "diagramitem.h"
#include "state.h"

#include <QPainter>
#include <QGraphicsScene>

DiagramItem::DiagramItem(State * state) : state(state)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptsHoverEvents(true);
}

void DiagramItem::updateTransitions() const {}

void DiagramItem::addIncomingTransition(DiagramTransitionItem * transition)
{
	incoming.append(transition);
}

void DiagramItem::addOutgoingTransition(DiagramTransitionItem * transition)
{
	outgoing.append(transition);
}

// Diagram begin item

DiagramBeginItem::DiagramBeginItem(State * state) : DiagramItem(state) {}

QRectF DiagramBeginItem::boundingRect() const
{
	return QRectF(- 6, - 6, 12, 12);
}

void DiagramBeginItem::updateTransitions() const {}

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

// Diagram final item

DiagramFinalItem::DiagramFinalItem(State * state) : DiagramItem(state) {}

QRectF DiagramFinalItem::boundingRect() const
{
	return QRectF(- 6, - 6, 12, 12);
}

void DiagramFinalItem::updateTransitions() const {}

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

QRectF DiagramActionItem::boundingRect() const
{
	return QRectF(- 48, - 15, 96, 30);
}

void DiagramActionItem::updateTransitions() const
{
	foreach (DiagramTransitionItem * transition, outgoing)
		transition->setStartPoint(QPoint(0, - 15));
}

void DiagramActionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::white);
	painter->drawRoundedRect(- 50, - 15, 100, 30, 15, 15);
}

void DiagramActionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	updateTransitions();
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram condition item

DiagramConditionItem::DiagramConditionItem(State * state) : DiagramItem(state) {}

QRectF DiagramConditionItem::boundingRect() const
{
	return QRectF(- 10, - 10, 20, 20);
}

void DiagramConditionItem::updateTransitions() const {}

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

void DiagramMergeItem::updateTransitions() const {}

void DiagramMergeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::SolidPattern);
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

void DiagramForkItem::updateTransitions() const {}

void DiagramForkItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::SolidPattern);
	painter->drawRect(- 40, - 2, 80, 4);
}

void DiagramForkItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram transition

DiagramTransitionItem::DiagramTransitionItem(DiagramItem * source, DiagramItem * target) :
	source(source), target(target)
{
	source->addOutgoingTransition(this);
	source->updateTransitions();
	target->addIncomingTransition(this);
	target->updateTransitions();
}

void DiagramTransitionItem::setStartPoint(const QPoint &point)
{
	startPoint = point;
}

void DiagramTransitionItem::setDestinationPoint(const QPoint &point)
{
	destPoint = point;
}

QRectF DiagramTransitionItem::boundingRect() const {}

void DiagramTransitionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->drawLine(source->pos() - startPoint, target->pos() - destPoint);
}

DiagramGridItem::DiagramGridItem(QList<QLine> horizontal, QList<QLine> vertical) :
	horizontal(horizontal), vertical(vertical) {}

QRectF DiagramGridItem::boundingRect() const {}

void DiagramGridItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	foreach (QLine line, horizontal)
		painter->drawLine(120 * line.x1(), 100 * line.y1(), 120 * line.x2(), 100 * line.y2());

	foreach (QLine line, vertical)
		painter->drawLine(120 * line.x1(), 100 * line.y1(), 120 * line.x2(), 100 * line.y2());
}
