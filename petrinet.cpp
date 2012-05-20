#include "petrinet.h"

#include <QDebug>

void PetriNet::convertState(State * state, StateList * states, TransitionList * transitions)
{
	QString id = state->id();
	QString name = state->name();
	switch (state->type())
	{
	case begin_state:
		break;

	case action_state:
	{
		NetPlace * p = new NetPlace(name, QString("%1_place").arg(id));
		NetTransition * t = new NetTransition(name, QString("%1_transition").arg(id));
		t->setExpression(state->expression());

		Transition * transition = new Transition(id);
		p->addOutgoingTransition(transition);
		t->addIncomingTransition(transition);

		states->append(p);
		states->append(t);
		transitions->append(transition);
	}
	break;

	case condition_state:
	{
		NetPlace * p = new NetPlace(name, QString("%1_place").arg(id));
		states->append(p);

		for (int i = 0; i < state->outgoing().count(); ++i)
		{
			NetTransition * t = new NetTransition(name, QString("%1_transition%2").arg(id).arg(i));

			Transition * transition = new Transition(QString("%1%2").arg(id).arg(i));
			p->addOutgoingTransition(transition);
			t->addIncomingTransition(transition);

			states->append(t);
			transitions->append(transition);
		}
	}
	break;

	case fork_state:
	{
		NetPlace * p = new NetPlace(name, QString("%1_place").arg(id));
		NetTransition * t = new NetTransition(name, QString("%1_transition").arg(id));

		Transition * transition = new Transition(id);
		p->addOutgoingTransition(transition);
		t->addIncomingTransition(transition);

		states->append(p);
		states->append(t);
		transitions->append(transition);
	}
	break;

	case merge_state:
	{
		NetTransition * t = new NetTransition(name, QString("%1_transition").arg(id));
		for (int i = 0; i < state->incoming().count(); ++i)
		{
			NetPlace * p = new NetPlace(name, QString("%1_place%2").arg(id).arg(i));

			Transition * transition = new Transition(QString("%1%2").arg(id).arg(i));
			p->addOutgoingTransition(transition);
			t->addIncomingTransition(transition);

			states->append(p);
			transitions->append(transition);
		}
		states->append(t);
	}
	break;

	case final_state:
	{
		NetPlace * p = new NetPlace(name, QString("%1_place").arg(id));
		states->append(p);
	}
	break;

	}
}

State * PetriNet::findFreeTransition(const QList<State *> &transitions, TransitionList * netTransitions)
{
	State * result;
	bool flag = true;
	for (int i = 0; i < transitions.count() && flag; ++i)
	{
		flag = false;
		result = transitions[i];
		for (int j = 0; j < netTransitions->count(); ++j)
			if (netTransitions->at(j)->source() == transitions[i])
			{
				flag = true; break;
			}
	}

	return result;
}

State * PetriNet::findFreePlace(const QList<State *> &places, TransitionList * netTransitions)
{
	State * result;
	bool flag = true;
	for (int i = 0; i < places.count() && flag; ++i)
	{
		flag = false;
		result = places[i];
		for (int j = 0; j < netTransitions->count(); ++j)
			if (netTransitions->at(j)->target() == places[i])
			{
				flag = true; break;
			}
	}

	return result;
}

void PetriNet::convert(StateList * states, StateList * netStates, TransitionList * netTransiitons)
{
	QList<State *> queue;
	State * state = states->find(begin_state).first();
	queue.append(state);

	while (!queue.isEmpty())
	{
		State * state = queue.takeFirst();
		if (netStates->find(state->id()).isEmpty())
			convertState(state, netStates, netTransiitons);

		for (int i = 0; i < state->outgoing().count(); ++i)
		{
			Transition * transition = state->outgoing()[i];
			State * target = transition->target();
			// Если переход имеет ограничения, то эти ограничения накладываются
			// на дугу между позицией и переходом сети
			if (!transition->guard().isEmpty())
			{
				QList<Transition *> list = netTransiitons->find(state->id());
				if (!list.isEmpty())
				{
					Transition * tr = list[i];
					tr->setGuard(transition->guard());
				}
			}

			// Если эта вершина еще не была обработана
			if (netStates->find(target->id()).isEmpty())
			{
				queue.append(target);
				convertState(target, netStates, netTransiitons);
			}

			if (state->type() != begin_state)
			{
				State * transition, * place;
				QList<State *> transitions = netStates->find(QString("%1_transition").arg(state->id()));
				if (!transitions.isEmpty())
				{
					if (state->type() == condition_state)
						transition = findFreeTransition(transitions, netTransiitons);
					else
						transition = transitions.first();
				}

				QList<State *> places = netStates->find(QString("%1_place").arg(target->id()));
				if (!places.isEmpty())
				{
					if (target->type() == merge_state)
						place = findFreePlace(places, netTransiitons);
					else
						place = places.first();
				}

				Transition * tr = new Transition(QString("%1_%2").arg(state->id()).arg(target->id()));
				transition->addOutgoingTransition(tr);
				place->addIncomingTransition(tr);
				netTransiitons->append(tr);
			}
		}
	}
}

void print(QMap<QString, Type *> variables, int level)
{
	foreach (QString name, variables.keys())
	{
		if (variables[name]->type() == complex)
		{
			qDebug() << QString(level, '\t') << variables[name]->name();

			ComplexType * complex = reinterpret_cast<ComplexType *>(variables[name]);
			print(complex->variables(), level + 1);
		}
		else
			qDebug() << QString(level, '\t') << variables[name]->name();
	}
}

void PetriNet::createType(QMap<QString, Type *> &variables, QStringList &variable)
{
	QString name = variable.takeFirst();
	Type * currentType = variables.value(name);
	if (!currentType) // if type don't create yet
	{
		if (!variable.isEmpty()) // complex type
		{
			ComplexType * complexType = new ComplexType(name);
			createType(complexType->variables(), variable);
			variables.insert(name, complexType);
		}
		else // simple type
		{
			SimpleType * simpleType = new SimpleType(name);
			variables.insert(name, simpleType);
		}
	}
	else
	{
		if (!variable.isEmpty()) // complex type
		{
			ComplexType * complexType = reinterpret_cast<ComplexType *>(currentType);
			createType(complexType->variables(), variable);
		}
	}
}

QMap<QString, Type *> PetriNet::variableList(StateList * states)
{
	QMap<QString, Type *> types;
	QList<State *> actions = states->find(action_state);
	foreach (State * action, actions)
	{
		QString expression = action->expression();
		QStringList list = expression.split(QRegExp("([^\\w.]+)"));
		foreach (QString expr, list)
		{
			QStringList variables = expr.split('.');
			createType(types, variables);
		}
	}

	return types;
}
