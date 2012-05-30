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

typedef enum { array,
			   variable
			 } VariableType;

class Type;
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

	State * source() { return m_source; }
	void setSourceState(State * state) { m_source = state; }

	State * target() { return m_target; }
	void setTargetState(State * state) { m_target = state; }

	QString guard() { return m_guard; }
	void setGuard(const QString &expression) { m_guard = expression; }

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

	virtual DiagramItem * diagramItem() = 0;

	QList<Transition *> &incoming() { return m_incoming; }
	void addIncomingTransition(Transition * transition);

	QList<Transition *> &outgoing() { return m_outgoing; }
	void addOutgoingTransition(Transition * transition);

protected:
	State(const QString &name, const QString &id, StateType type);

	StateType m_type;
	QString m_name, m_id;

	QList<Transition *> m_incoming;
	QList<Transition *> m_outgoing;
};

class Marking
{
public:
	// Variables
	void addVariable(const QString &variable) { m_variables.append(variable); }
	QStringList &variables() { return m_variables; }

	// Values
	void addVariableValue(const QString &name, const QVariant &value)
	{
		if (!m_values.contains(name))
			m_values.insert(name, value);
		else
			m_values[name] = value;
	}
	QMap<QString, QVariant> &values() { return m_values; }

	// Substitute the values
	QString substituteValues(const QString &expression);

private:
	QStringList m_variables;
	QMap<QString, QVariant> m_values;
};

class NetPlace : public State
{
public:
	NetPlace(const QString &name, const QString &id);
	DiagramItem * diagramItem();

	// Color
	QColor color() { return m_color; }
	void setColor(const QColor &color) { m_color = color; }

	// Marking
	int marking() { return m_marking.count(); }
	Marking * takeMarking() { return m_marking.takeFirst(); }
	bool addMarking(Marking * marking);

	// Variables
	QStringList variables() { return m_variables; }
	bool addVariable(const QString &variable)
	{
		bool result = false;
		if (!variable.isEmpty() && !m_variables.contains(variable))
		{
			m_variables.append(variable);
			result = true;
		}

		return result;
	}
	bool removeVariable(const QString &variable) { return m_variables.removeOne(variable); }
	void addVariableValue(const QString &variable, Type * type);

	// Substitute the values
	QString substituteValues(const QString &expression);

private:
	bool isArray(const QString &variable);

	QMap<QString, QVariantList> m_values;
	QMap<QString, VariableType> m_types;
	QList<Marking *> m_marking;
	QStringList m_variables;
	QColor m_color;
};

class NetTransition : public State
{
public:
	NetTransition(const QString &name, const QString &id);
	DiagramItem * diagramItem();

	QString expression() { return m_expression; }
	void setExpression(const QString &expression) { m_expression = expression; }

	QString result() { return m_result; }
	void setResult(const QString &result) { m_result = result; }

private:
	QString m_result, m_expression;
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

	QString expression() { return m_expression; }
	void setExpression(const QString &expression) { m_expression = expression; }
	QStringList variables() { return m_variables; }
	void setVariables(const QStringList &variables) { m_variables = variables; }

private:
	QString m_expression;
	QStringList m_variables;
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
