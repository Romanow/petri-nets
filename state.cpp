#include "state.h"

#include <QDebug>

bool compare(const QString &a, const QString &b)
{
	return !a.compare(b, Qt::CaseInsensitive);
}

void StateList::addState(State * state)
{
	if (!find(state->id()))
		states.append(state);
}

State * StateList::find(const QString &id)
{
	foreach (State * state, states)
		if (state->id() == id)
			return state;

	return 0;
}


QList<State *> StateList::find(const Type type)
{
	QList<State *> list;
	foreach (State * state, states)
		if (state->type() == type)
			list.append(state);

	return list;
}

void TransitionList::addTransition(Transition * transition)
{
	if (!find(transition->id()))
		transitions.append(transition);
}

Transition * TransitionList::find(const QString &id)
{
	foreach (Transition * transition, transitions)
		if (transition->id() == id)
			return transition;

	return 0;
}

Transition::Transition(const QString &id) : m_id(id) {}

void Transition::setSourceState(State * state)
{
	m_source = state;
}

void Transition::setTargetState(State * state)
{
	m_target = state;
}

void Transition::setGuard(const QString &expression)
{
	m_guard = expression;
}

State * Factory::create(const QString &name, const QString &id, const QString &type)
{
	if (compare(type, "init"))
		return new Begin(name, id);
	else if (compare(type, "action"))
		return new Action(name, id);
	else if (compare(type, "condition"))
		return new Condition(name, id);
	else if (compare(type, "fork"))
		return new Fork(name, id);
	else if (compare(type, "merge"))
		return new Merge(name, id);
	else if (compare(type, "final"))
		return new Final(name, id);
}

State::State(const QString &name, const QString &id, Type type) :
	m_name(name), m_id(id), m_type(type) {}

void State::addIncomingTransition(Transition * transition)
{
	m_incoming.append(transition);
}

void State::addOutgoingTransition(Transition * transition)
{
	m_outgoing.append(transition);
}

void State::setExpression(const QString &expression) {}

Begin::Begin(const QString &name, const QString &id) :
	State(name, id, begin_state) {}

DiagramItem * Begin::diagramItem()
{
	return new DiagramBeginItem(this);
}

Action::Action(const QString &name, const QString &id) :
	State(name, id, action_state) {}

DiagramItem * Action::diagramItem()
{
	return new DiagramActionItem(this);
}

void Action::setExpression(const QString &expression)
{
	m_expression = expression;
}

Condition::Condition(const QString &name, const QString &id) :
	State(name, id, condition_state) {}

DiagramItem * Condition::diagramItem()
{
	return new DiagramConditionItem(this);
}

Fork::Fork(const QString &name, const QString &id) :
	State(name, id, fork_state) {}

DiagramItem * Fork::diagramItem()
{
	return new DiagramForkItem(this);
}

Merge::Merge(const QString &name, const QString &id) :
	State(name, id, merge_state) {}

DiagramItem * Merge::diagramItem()
{
	return new DiagramMergeItem(this);
}

Final::Final(const QString &name, const QString &id) :
	State(name, id, final_state) {}

DiagramItem * Final::diagramItem()
{
	return new DiagramFinalItem(this);
}