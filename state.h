#ifndef STATE_H
#define STATE_H

#include <QXmlStreamReader>
#include <QGraphicsItem>

#include "diagramitem.h"

enum Type { unknown_state, begin_state, action_state, condition_state, fork_state, merge_state, final_state };

class State;
class DiagramTransition;

class Factory
{
public:
	static State * create(const QString &name, const QString &id, const QString &type);
};

class StateList : public QList<State *>
{
public:
	virtual ~StateList();

	void append(State * state);

	State * find(const QString &id);
	QList<State *> find(const Type type);

};

class TransitionList : public QList<DiagramTransition *>
{
public:
	virtual ~TransitionList();

	void append(DiagramTransition * transition);
	DiagramTransition * find(const QString &id);

};

class DiagramTransition
{
public:
	DiagramTransition(const QString &id);
	virtual ~DiagramTransition();

	QString id() { return m_id; }
	QString guard() { return m_guard; }
	State * source() { return m_source; }
	State * target() { return m_target; }

	void setSourceState(State * state);
	void setTargetState(State * state);
	void setGuard(const QString &expression);

private:
	QString m_id;
	QString m_guard;
	State * m_source;
	State * m_target;
};

class State
{
public:
	virtual ~State();

	Type type() { return m_type; }
	QString name() { return m_name; }
	QString id() { return m_id; }

	QList<DiagramTransition *> &outgoing() { return m_outgoing; }
	QList<DiagramTransition *> &incoming() { return m_incoming; }

	virtual DiagramItem * diagramItem() = 0;

	virtual void addIncomingTransition(DiagramTransition * transition);
	virtual void addOutgoingTransition(DiagramTransition * transition);
	virtual void setExpression(const QString &expression);

protected:
	State(const QString &name, const QString &id, Type type);

	Type m_type;
	QString m_name, m_id;

	QList<DiagramTransition *> m_incoming;
	QList<DiagramTransition *> m_outgoing;
};

class DiagramBegin : public State
{
public:
	DiagramBegin(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class DiagramAction : public State
{
public:
	DiagramAction(const QString &name, const QString &id);
	DiagramItem * diagramItem();

	void setExpression(const QString &expression);
	QString expression() { return m_expression; }

private:
	QString m_expression;
};

class DiagramCondition: public State
{
public:
	DiagramCondition(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class Fork : public State
{
public:
	Fork(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class DiagramMerge : public State
{
public:
	DiagramMerge(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class DiagramFinal: public State
{
public:
	DiagramFinal(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

#endif // STATE_H
