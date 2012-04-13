#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>

class State;

class DiagramItem : public QGraphicsItem
{
public:
	DiagramItem(State * state = 0);

private:
	State * state;
};

class DiagramBeginItem : public DiagramItem
{
public:
	DiagramBeginItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class DiagramFinalItem : public DiagramItem
{
public:
	DiagramFinalItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class DiagramActionItem : public DiagramItem
{
public:
	DiagramActionItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
	State * state;
};

class DiagramConditionItem : public DiagramItem
{
public:
	DiagramConditionItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class DiagramMergeItem : public DiagramItem
{
public:
	DiagramMergeItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class DiagramForkItem : public DiagramItem
{
public:
	DiagramForkItem(State * state = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class DiagramTransition : public QGraphicsItem
{
public:
	DiagramTransition(DiagramItem * sorce);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
	DiagramItem * source;
	QList<DiagramItem *> incoming;
	QList<DiagramItem *> outgoing;
};

#endif // DIAGRAMITEM_H
