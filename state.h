#ifndef STATE_H
#define STATE_H

#include <QXmlStreamReader>
#include <QGraphicsItem>

#include "diagramitem.h"

typedef enum { unknown_state,
			   begin_state,
			   action_state,
			   condition_state,
			   fork_state,
			   merge_state,
			   final_state,
			   place_node,
			   transition_node
			 } StateType;

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
	virtual ~StateList();

	QList<State *> find(const QString &id);
	QList<State *> find(const StateType type);
};

class TransitionList : public QList<Transition *>
{
public:
	virtual ~TransitionList();

	QList<Transition *> find(const QString &id);
};

class Transition
{
public:
	Transition(const QString &id);
	virtual ~Transition();

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

	StateType type() { return m_type; }
	QString name() { return m_name; }
	QString id() { return m_id; }
	virtual QString expression() {}

	QList<Transition *> &outgoing() { return m_outgoing; }
	QList<Transition *> &incoming() { return m_incoming; }

	virtual DiagramItem * diagramItem() = 0;

	virtual void addIncomingTransition(Transition * transition);
	virtual void addOutgoingTransition(Transition * transition);
	virtual void setExpression(const QString &expression);

protected:
	State(const QString &name, const QString &id, StateType type);

	StateType m_type;
	QString m_name, m_id;

	QList<Transition *> m_incoming;
	QList<Transition *> m_outgoing;
};

class NetPlace : public State
{
public:
	NetPlace(const QString &name, const QString &id);
	DiagramItem * diagramItem();

	void setMarking(int marking) { m_marking = marking; }
	int marking() { return m_marking; }

private:
	int m_marking;
};

class NetTransition : public State
{
public:
	NetTransition(const QString &name, const QString &id);
	DiagramItem * diagramItem();

	void setExpression(const QString &expression);
	QString expression() { return m_expression; }

private:
	QString m_expression;
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
