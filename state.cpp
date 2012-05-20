#include "state.h"

#include <QDebug>

template <class A, class B>
bool compare(const A &a, const B &b)
{
	return !a.compare(b, Qt::CaseInsensitive);
}

StateList::~StateList()
{
	while (!isEmpty())
	{
		State * state = takeFirst();
		delete state;
	}

	qDebug() << "Delete states list";
}

QList<State *> StateList::find(const QString &id)
{
	QList<State *> list;
	for (QList<State *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->id().startsWith(id))
			list.append(* iter);

	return list;
}

QList<State *> StateList::find(const StateType type)
{
	QList<State *> list;
	for (QList<State *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->type() == type)
			list.append(* iter);

	return list;
}

TransitionList::~TransitionList()
{
	while (!isEmpty())
	{
		Transition * transition = takeFirst();
		delete transition;
	}

	qDebug() << "Delete transitions list";
}

QList<Transition *> TransitionList::find(const QString &id)
{
	QList<Transition *> list;
	for (QList<Transition *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->id().startsWith(id))
			list.append(* iter);

	return list;
}

Transition::Transition(const QString &id) : m_id(id) {}

Transition::~Transition() {}

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
		return new DiagramBegin(name, id);
	else if (compare(type, "action"))
		return new DiagramAction(name, id);
	else if (compare(type, "condition"))
		return new DiagramCondition(name, id);
	else if (compare(type, "fork"))
		return new Fork(name, id);
	else if (compare(type, "merge"))
		return new DiagramMerge(name, id);
	else if (compare(type, "final"))
		return new DiagramFinal(name, id);
}

State::State(const QString &name, const QString &id, StateType type) :
	m_name(name), m_id(id), m_type(type) {}

State::~State() {}

void State::addIncomingTransition(Transition * transition)
{
	m_incoming.append(transition);
	transition->setTargetState(this);
}

void State::addOutgoingTransition(Transition * transition)
{
	m_outgoing.append(transition);
	transition->setSourceState(this);
}

void State::setExpression(const QString &expression) {}

NetPlace::NetPlace(const QString &name, const QString &id) :
	State(name, id, place_node), m_marking(0) {}

DiagramItem * NetPlace::diagramItem()
{
	return new DiagramNetPlaceItem(this);
}

NetTransition::NetTransition(const QString &name, const QString &id) :
	State(name, id, transition_node) {}

DiagramItem * NetTransition::diagramItem()
{
	return new DiagramNetTransitionItem(this);
}

void NetTransition::setExpression(const QString &expression)
{
	m_expression = expression;
}

DiagramBegin::DiagramBegin(const QString &name, const QString &id) :
	State(name, id, begin_state) {}

DiagramItem * DiagramBegin::diagramItem()
{
	return new DiagramBeginItem(this);
}

DiagramAction::DiagramAction(const QString &name, const QString &id) :
	State(name, id, action_state) {}

DiagramItem * DiagramAction::diagramItem()
{
	return new DiagramActionItem(this);
}

void DiagramAction::setExpression(const QString &expression)
{
	m_expression = expression;
}

DiagramCondition::DiagramCondition(const QString &name, const QString &id) :
	State(name, id, condition_state) {}

DiagramItem * DiagramCondition::diagramItem()
{
	return new DiagramConditionItem(this);
}

Fork::Fork(const QString &name, const QString &id) :
	State(name, id, fork_state) {}

DiagramItem * Fork::diagramItem()
{
	return new DiagramForkItem(this);
}

DiagramMerge::DiagramMerge(const QString &name, const QString &id) :
	State(name, id, merge_state) {}

DiagramItem * DiagramMerge::diagramItem()
{
	return new DiagramMergeItem(this);
}

DiagramFinal::DiagramFinal(const QString &name, const QString &id) :
	State(name, id, final_state) {}

DiagramItem * DiagramFinal::diagramItem()
{
	return new DiagramFinalItem(this);
}
