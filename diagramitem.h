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

class DiagramTransitionItem : public QGraphicsItem
{
public:
	DiagramTransitionItem(DiagramItem * source, DiagramItem * target);

	void setStartPoint(const QPoint &point);
	void setDestinationPoint(const QPoint &point);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
	QPoint startPoint, destPoint;
	DiagramItem * source, * target;
};

class DiagramItem : public QGraphicsItem
{
public:
	DiagramItem(State * state);
	void updateTransitions() const;
	void addIncomingTransition(DiagramTransitionItem * transition);
	void addOutgoingTransition(DiagramTransitionItem * transition);

protected:
	QList<DiagramTransitionItem *> incoming;
	QList<DiagramTransitionItem *> outgoing;
	State * state;

};

class DiagramBeginItem : public DiagramItem
{
public:
	DiagramBeginItem(State * state = 0);

	QRectF boundingRect() const;
	virtual void updateTransitions() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramFinalItem : public DiagramItem
{
public:
	DiagramFinalItem(State * state = 0);

	QRectF boundingRect() const;
	virtual void updateTransitions() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramActionItem : public DiagramItem
{
public:
	DiagramActionItem(State * state = 0);

	QRectF boundingRect() const;
	virtual void updateTransitions() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramConditionItem : public DiagramItem
{
public:
	DiagramConditionItem(State * state = 0);

	QRectF boundingRect() const;
	virtual void updateTransitions() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramMergeItem : public DiagramItem
{
public:
	DiagramMergeItem(State * state = 0);

	QRectF boundingRect() const;
	virtual void updateTransitions() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

class DiagramForkItem : public DiagramItem
{
public:
	DiagramForkItem(State * state = 0);

	QRectF boundingRect() const;
	virtual void updateTransitions() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
};

#endif // DIAGRAMITEM_H
