#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class State;
class DiagramItem;

class DiagramTransition : public QGraphicsItem
{
public:
	DiagramTransition(const QString &id, DiagramItem * source, DiagramItem * target);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

	QString &id() { return m_id; }

private:
	QString m_id;
	DiagramItem * m_source, * m_target;
};

class DiagramItem : public QGraphicsItem
{
public:
	DiagramItem(State * state);
	bool compareState(State * another);

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

class DiagramItemList : public QList<DiagramItem *>
{
public:
	DiagramItem * find(State * state);
};

class DiagramTransitionList : public QList<DiagramTransition *>
{
public:
	DiagramTransition * find(const QString &id);
};

#endif // DIAGRAMITEM_H
