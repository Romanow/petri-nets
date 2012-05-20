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

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
	DiagramItem * source, * target;
};

class DiagramItem : public QGraphicsItem
{
public:
	DiagramItem(State * state);

	State * state() { return m_state; }
	virtual void setMenu(QMenu * menu);

protected:
	State * m_state;
};

class DiagramBeginItem : public DiagramItem
{
public:
	DiagramBeginItem(State * state = 0);

	void setMenu(QMenu * menu);
	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
	QMenu * m_menu;
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

	void setMenu(QMenu * menu);
	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
	QMenu * m_menu;
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

	void setMenu(QMenu * menu);
	void setInitialMarking(int n);
	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
	QMenu * m_menu;
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
