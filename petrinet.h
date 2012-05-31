#ifndef PETRINET_H
#define PETRINET_H

#include "state.h"

#include <QDebug>

typedef enum { complex,
			   simple
			 } StructType;

class Type
{
public:
	virtual ~Type() {}

	StructType type() { return m_type; }
	QString name() { return m_name; }

protected:
	Type(const QString &name, StructType type) :
		m_name(name), m_type(type) {}

private:
	QString m_name;
	StructType m_type;
};

class SimpleType : public Type
{
public:
	~SimpleType() {}

	SimpleType(const QString &name) : Type(name, simple) {}
	QVariantList &values() { return m_values; }

private:
	QVariantList m_values;
};

class ComplexType : public Type
{
public:
	~ComplexType()
	{
		qDeleteAll(m_variables);
		m_variables.clear();
	}

	ComplexType(const QString &name) : Type(name, complex) {}
	QMap<QString, Type *> &variables() { return m_variables; }

private:
	QMap<QString, Type *> m_variables;
};

class PetriNet
{
public:
	void convert(StateList * states, StateList * netStates, TransitionList * netTransiitons, bool colored = false);
	QMap<QString, Type *> variableList(StateList * states, TransitionList * transitions);
	void coloring(StateList * netStates, const QMap<QString, Type *> &types);
	void setInitialMarking(StateList * states, StateList * netStates);
	void initVariables(StateList * states, QMap<QString, Type *> types);

private:
	void createType(QMap<QString, Type *> &variables, QStringList &variable);
	void convertState(State * state, StateList * states, TransitionList * transitions, bool colored);
	State * findFreePlace(const QList<State *> &places, TransitionList * netTransitions);
	State * findFreeTransition(const QList<State *> &transitions, TransitionList * netTransitions);
	void setVariablePath(State * state, const QString &variable, QList<State *> &track);

};

#endif // PETRINET_H
