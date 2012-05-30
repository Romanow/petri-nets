#include "state.h"
#include "petrinet.h"

#include <QDebug>

template <class A, class B>
bool compare(const A &a, const B &b)
{
	return !a.compare(b, Qt::CaseInsensitive);
}

StateList::~StateList()
{
	qDeleteAll(begin(), end());
	clear();
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
	qDeleteAll(begin(), end());
	clear();
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

void NetPlace::addVariableValue(const QString &name, Type * type)
{
	if (type->type() == complex)
	{
		ComplexType * complexType = reinterpret_cast<ComplexType *>(type);
		foreach (QString variable, complexType->variables().keys())
			addVariableValue(name + "." + variable, complexType->variables()[variable]);
	}
	else
	{
		SimpleType * simpleType = reinterpret_cast<SimpleType *>(type);
		m_values.insert(name, simpleType->values());
		m_types.insert(name, simpleType->values().count() > 1 ? array : variable);
	}
}

bool NetPlace::isArray(const QString &variable)
{
	foreach (QString name, m_values.keys())
		if (variable == name.split('.').first() && m_types[name] == array)
			return true;

	return false;
}

bool NetPlace::addMarking(Marking * marking)
{
	bool result = true;

	QMutableStringListIterator iter(marking->variables());
	while (iter.hasNext())
	{
		QString variable = iter.next();
		// Если перменная фишки не содержится в переменных позиции
		// или в этой позиции для переменной содержится массив значений
		if (!variables().contains(variable) ||
				isArray(variable))
		{
			// Удаляем переменную
			iter.remove();
			// Удаляем все значения пермееной
			QMutableMapIterator<QString, QVariant> i(marking->values());
			while (i.hasNext())
			{
				i.next();
				QString name = i.key().split('.').first();
				if (name == variable)
					i.remove();
			}
		}
	}

	foreach (QString variable, variables())
	{
		if (!marking->variables().contains(variable) && result)
		{
			foreach (QString name, m_values.keys())
				if (variable == name.split('.').first())
				{
					QVariantList valueList = m_values[name];
					if (!valueList.isEmpty())
					{
						QVariant value = valueList.takeFirst();
						if (m_types[name] == array)
							m_values[name] = valueList;

						marking->values().insert(name, value);
					}
					else if (m_types[name] == array)
					{
						marking->values().insert(name, 0);
					}
					else
					{
						result = false; break;
					}
				}

			if (result)
				marking->addVariable(variable);
		}
	}

	m_marking.append(marking);

	return result;
}

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

QString NetPlace::substituteValues(const QString &expression)
{
	Marking * marking = m_marking.first();
	return marking->substituteValues(expression);
}

QString Marking::substituteValues(const QString &expression)
{
	QString result = expression;
	foreach (QString variable, m_values.keys())
		result = result.replace(variable, m_values[variable].toString());

	return result;
}
