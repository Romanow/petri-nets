#ifndef STATE_H
#define STATE_H

#include <QXmlStreamReader>
#include <QGraphicsItem>

#include "diagramitem.h"

enum Type { unknown_state, begin_state, action_state, condition_state, fork_state, merge_state, final_state };

class State;
class Transition;

class Factory
{
public:
	static State * create(const QString &name, const QString &id, const QString &type);
};

class StateList : public QList<State *>
{
public:
	void append(State * state);

	State * find(const QString &id);
	QList<State *> find(const Type type);

};

class TransitionList : public QList<Transition *>
{
public:
	void append(Transition * transition);
	Transition * find(const QString &id);

};

class Transition
{
public:
	Transition(const QString &id);

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
	Type type() { return m_type; }
	QString name() { return m_name; }
	QString id() { return m_id; }

	QList<Transition *> &outgoing() { return m_outgoing; }
	QList<Transition *> &incoming() { return m_incoming; }

	virtual DiagramItem * diagramItem() = 0;

	virtual void addIncomingTransition(Transition * transition);
	virtual void addOutgoingTransition(Transition * transition);
	virtual void setExpression(const QString &expression);

protected:
	State(const QString &name, const QString &id, Type type);

	Type m_type;
	QString m_name, m_id;

	QList<Transition *> m_incoming;
	QList<Transition *> m_outgoing;
};

class Begin : public State
{
public:
	Begin(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class Action : public State
{
public:
	Action(const QString &name, const QString &id);
	DiagramItem * diagramItem();

	void setExpression(const QString &expression);
	QString expression() { return m_expression; }

private:
	QString m_expression;
};

class Condition: public State
{
public:
	Condition(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class Fork : public State
{
public:
	Fork(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class Merge : public State
{
public:
	Merge(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

class Final: public State
{
public:
	Final(const QString &name, const QString &id);
	DiagramItem * diagramItem();
};

#endif // STATE_H
