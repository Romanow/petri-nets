#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class State;
class DiagramItem;

class DiagramGridItem : public QGraphicsItem
{
public:
	DiagramGridItem(QList<QLine> horizontal, QList<QLine> vertical);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
	QList<QLine> horizontal, vertical;
};

class DiagramTransitionItem : public QGraphicsLineItem
{
public:
	DiagramTransitionItem(DiagramItem * source, DiagramItem * target);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
	QColor color;
	DiagramItem * source, * target;
};

class DiagramItem : public QGraphicsItem
{
public:
	DiagramItem(State * state);

	QPolygonF polygon() { return path.toFillPolygon(); }
	State * state() { return m_state; }

protected:
	State * m_state;
	QPainterPath path;
};

class DiagramBeginItem : public DiagramItem
{
public:
	DiagramBeginItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramFinalItem : public DiagramItem
{
public:
	DiagramFinalItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramActionItem : public DiagramItem
{
public:
	DiagramActionItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

private:
	QString expression;
};

class DiagramConditionItem : public DiagramItem
{
public:
	DiagramConditionItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramMergeItem : public DiagramItem
{
public:
	DiagramMergeItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramForkItem : public DiagramItem
{
public:
	DiagramForkItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramNetPlaceItem : public DiagramItem
{
public:
	DiagramNetPlaceItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramNetTransitionItem : public DiagramItem
{
public:
	DiagramNetTransitionItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

#endif // DIAGRAMITEM_H
