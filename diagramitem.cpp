#include "diagramitem.h"
#include "state.h"

#include <math.h>
#include <QMenu>
#include <QPainter>
#include <QGraphicsScene>

DiagramItem::DiagramItem(State * state) : m_state(state)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptsHoverEvents(true);
}

// Diagram begin item

DiagramBeginItem::DiagramBeginItem(State * state) : DiagramItem(state)
{
	path.addEllipse(- 10, - 10, 20, 20);
}

QRectF DiagramBeginItem::boundingRect() const
{
	return QRectF(- 6, - 6, 12, 12);
}

void DiagramBeginItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawPath(path);
}

void DiagramBeginItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram final item

DiagramFinalItem::DiagramFinalItem(State * state) : DiagramItem(state)
{
	path.addEllipse(- 10, - 10, 20, 20);
}

QRectF DiagramFinalItem::boundingRect() const
{
	return QRectF(- 6, - 6, 12, 12);
}

void DiagramFinalItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::white);
	painter->drawPath(path);
	painter->setBrush(Qt::black);
	painter->drawEllipse(- 7, - 7, 14, 14);
}

void DiagramFinalItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram action item

DiagramActionItem::DiagramActionItem(State * state) : DiagramItem(state)
{
	path.addRoundedRect(- 50, - 15, 100, 30, 15, 15);
}

QRectF DiagramActionItem::boundingRect() const
{
	return QRectF(- 45, - 14, 90, 28);
}

void DiagramActionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::white);
	painter->drawPath(path);
}

void DiagramActionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram condition item

DiagramConditionItem::DiagramConditionItem(State * state) : DiagramItem(state)
{
	QPolygon polygon;
	polygon.setPoints(4, 0, - 15, 15, 0, 0, 15, - 15, 0);
	path.addPolygon(polygon);
	path.closeSubpath();
}

QRectF DiagramConditionItem::boundingRect() const
{
	return QRectF(-10, -10, 20, 20);
}

void DiagramConditionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

	painter->setBrush(Qt::white);
	painter->drawPath(path);
}

void DiagramConditionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram merge item

DiagramMergeItem::DiagramMergeItem(State * state) : DiagramItem(state)
{
	path.addRect(- 40, - 2, 80, 4);
}

QRectF DiagramMergeItem::boundingRect() const
{
	return QRectF(- 40, - 2, 80, 4);
}

void DiagramMergeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawPath(path);
}

void DiagramMergeItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram fork item

DiagramForkItem::DiagramForkItem(State * state) : DiagramItem(state)
{
	path.addRect(- 40, - 2, 80, 4);
}

QRectF DiagramForkItem::boundingRect() const
{
	return QRectF(- 40, - 2, 80, 4);
}

void DiagramForkItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawPath(path);
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
	if (source->state()->type() == place_node)
	{
		NetPlace * place = dynamic_cast<NetPlace *>(source->state());
		color = place->color();
	}
	else if (target->state()->type() == place_node)
	{
		NetPlace * place = dynamic_cast<NetPlace *>(target->state());
		color = place->color();
	}
	else
		color = Qt::black;
}

QRectF DiagramTransitionItem::boundingRect() const {}

void DiagramTransitionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if (!source->collidesWithItem(target))
    {
        QLineF centerLine(source->pos(), target->pos());
        QPolygonF endPolygon = target->polygon();
        QPointF p1 = endPolygon.first() + target->pos();
        QPointF p2;
        QLineF polyLine;
        QPointF intersectPoint;
        for (int i = 1; i < endPolygon.count(); ++i)
        {
            p2 = endPolygon.at(i) + target->pos();
            polyLine = QLineF(p1, p2);
            if (polyLine.intersect(centerLine, &intersectPoint) == QLineF::BoundedIntersection)
                break;

            p1 = p2;
        }

        setLine(QLineF(intersectPoint, source->pos()));
        double angle = acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (M_PI * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * 10, cos(angle + M_PI / 3) * 10);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + 2 * M_PI / 3) * 10, cos(angle + 2 * M_PI / 3) * 10);

        QPolygonF arrowHead;
        arrowHead << line().p1() << arrowP1 << arrowP2;

		painter->setPen(color);
        painter->drawLine(line());
		painter->setBrush(color);
        painter->drawPolygon(arrowHead);
    }
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

DiagramNetPlaceItem::DiagramNetPlaceItem(State * state) : DiagramItem(state)
{
	path.addEllipse(- 12, - 12, 24, 24);
}

QRectF DiagramNetPlaceItem::boundingRect() const
{
	return QRectF(- 9, - 9, 18, 18);
}

void DiagramNetPlaceItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	NetPlace * place = dynamic_cast<NetPlace *>(state());
	painter->setBrush(Qt::white);
	painter->setPen(QPen(place->color(), 2));
	painter->drawPath(path);
	if (place->marking() > 0)
		if (place->marking() < 5)
		{
			painter->setBrush(place->color());
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

	QString variableList;
	foreach (QString variable, place->variables())
		variableList += variable + "\n";
	painter->drawText(QRect(15, 5, 100, 80), variableList);
}

void DiagramNetPlaceItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}

// Diagram net transition item

DiagramNetTransitionItem::DiagramNetTransitionItem(State * state) : DiagramItem(state)
{
	path.addRect(- 25, - 2, 50, 4);
}

QRectF DiagramNetTransitionItem::boundingRect() const
{
	return QRectF(- 25, - 2, 50, 4);
}

void DiagramNetTransitionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setBrush(Qt::black);
	painter->drawPath(path);
}

void DiagramNetTransitionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	scene()->update();
}
