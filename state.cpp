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

void StateList::append(State * state)
{
	if (!find(state->id()))
		push_back(state);
}

State * StateList::find(const QString &id)
{
	for (QList<State *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->id() == id)
			return (* iter);

	return 0;
}

QList<State *> StateList::find(const Type type)
{
	QList<State *> list;
	for (QList<State *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->type() == type)
			list.append((* iter));

	return list;
}

TransitionList::~TransitionList()
{
	while (!isEmpty())
	{
		DiagramTransition * transition = takeFirst();
		delete transition;
	}

	qDebug() << "Delete transitions list";
}

void TransitionList::append(DiagramTransition * transition)
{
	if (!find(transition->id()))
		push_back(transition);
}

DiagramTransition * TransitionList::find(const QString &id)
{
	for (QList<DiagramTransition *>::iterator iter = begin(); iter != end(); iter++)
		if ((* iter)->id() == id)
			return (* iter);

	return 0;
}

DiagramTransition::DiagramTransition(const QString &id) : m_id(id) {}

DiagramTransition::~DiagramTransition() {}

void DiagramTransition::setSourceState(State * state)
{
	m_source = state;
}

void DiagramTransition::setTargetState(State * state)
{
	m_target = state;
}

void DiagramTransition::setGuard(const QString &expression)
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

State::State(const QString &name, const QString &id, Type type) :
	m_name(name), m_id(id), m_type(type) {}

State::~State() {}

void State::addIncomingTransition(DiagramTransition * transition)
{
	m_incoming.append(transition);
}

void State::addOutgoingTransition(DiagramTransition * transition)
{
	m_outgoing.append(transition);
}

void State::setExpression(const QString &expression) {}

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
