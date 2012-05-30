#include "state.h"

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

NetPlace::NetPlace(const QString &name, const QString &id) :
	State(name, id, place_node), m_color(Qt::black) {}

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

#include "petrinet.h"
#include <QDebug>

QVariant value(const QMap<QString, Type *> &type, QStringList variable)
{
	QVariant result;
	QString name = variable.takeFirst();
	if (!variable.isEmpty())
	{
		ComplexType * complexType = reinterpret_cast<ComplexType *>(type[name]);
		result = value(complexType->variables(), variable);
	}
	else
	{
		SimpleType * simpleType = reinterpret_cast<SimpleType *>(type[name]);
		result = simpleType->values();
	}

	return result;
}

void NetPlace::addMarking(Marking * marking)
{
	QMutableMapIterator<QString, Type *> iter(marking->variables);
	while (iter.hasNext())
	{
		iter.next();
		if (!variables().contains(iter.key()))
			iter.remove();
	}

	foreach (QString variable, variables())
		if (!marking->variables.contains(variable))
			marking->variables.insert(variable, m_variables[variable]); // copy variable

	m_marking.append(marking);

	qDebug() << marking->variables;
}
