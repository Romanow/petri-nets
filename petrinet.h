#ifndef PETRINET_H
#define PETRINET_H

#include "state.h"

typedef enum { complex,
			   tuple,
			   simple
			 } StructType;

typedef enum { int_type,
			   float_type,
			   string_type
			 } VariableType;

class Type
{
public:
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
	SimpleType(const QString &name) : Type(name, simple) {}

private:
	QMultiMap<VariableType, QVariant> value;
};

class ComplexType : public Type
{
public:
	ComplexType(const QString &name) : Type(name, complex) {}
	QMap<QString, Type *> &variables()
	{
		return m_variables;
	}

private:
	QMap<QString, Type *> m_variables;
};

class PetriNet
{
public:
	void convert(StateList * states, StateList * netStates, TransitionList * netTransiitons);
	QMap<QString, Type *> variableList(StateList * states);

private:
	void createType(QMap<QString, Type *> &variables, QStringList &variable);
	void convertState(State * state, StateList * states, TransitionList * transitions);
	State * findFreePlace(const QList<State *> &places, TransitionList * netTransitions);
	State * findFreeTransition(const QList<State *> &transitions, TransitionList * netTransitions);

};

#endif // PETRINET_H
