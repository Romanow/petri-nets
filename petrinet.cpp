#include "petrinet.h"
#include "state.h"
#include "rpn.h"

#include <QDebug>

bool isNumber(const QString &variable)
{
	bool flag;
	variable.toDouble(&flag);
	return flag;
}

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

		DiagramAction * action = dynamic_cast<DiagramAction *>(state);

		p->setVariables(action->variables());

		QString expression = action->expression();
		QStringList list = expression.split('=');
		t->setResult(list.first());
		t->setExpression(RPN::convert(list.last()));

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
					QString guard = transition->guard();

					QList<State *> places = netStates->find(QString("%1_place").arg(state->id()));
					QStringList variables = guard.split(QRegExp("[^\\w.]+"));
					foreach (State * state, places)
					{
						NetPlace * place = dynamic_cast<NetPlace *>(state);
						foreach (QString var, variables)
						{
							QString variable = var.split('.').first();
							if (!place->variables().contains(variable) && !isNumber(variable))
								place->addVariable(variable);
						}
					}

					tr->setGuard(RPN::convert(guard));
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
	foreach (State * state, actions)
	{
		DiagramAction * action = dynamic_cast<DiagramAction *>(state);
		QString expression = action->expression();

		QStringList variables;
		QStringList list = expression.split(QRegExp("[^\\w.]+"));
		foreach (QString expr, list)
		{
			QStringList vars = expr.split('.');

			QString variable = vars.first();
			if (!variable.isEmpty() && !variables.contains(variable) && !isNumber(variable))
				variables.append(variable);

			createType(types, vars);
		}
		action->setVariables(variables);
	}

	return types;
}

void PetriNet::setVariablePath(State * state, const QString &variable, QList<State *> &track)
{
	foreach (Transition * transition, state->outgoing())
	{
		State * target = transition->target();
		NetPlace * place = dynamic_cast<NetPlace *>(target);
		if (place != 0 && place->variables().contains(variable)) // && !track.contains(target))
		{
			bool flag = true;
			for (int i = 0; i < track.count() && flag; ++i)
			{
				NetPlace * pl = dynamic_cast<NetPlace *>(track[i]);
				if (pl != 0 && pl->variables().contains(variable))
				{
					flag = false;
					for (int k = i + 1; k < track.count(); ++k)
						if (track[k]->type() == place_node)
						{
							NetPlace * p = dynamic_cast<NetPlace *>(track[k]);
							p->addVariable(variable);
						}
				}
			}
		}

		if (!track.contains(target))
		{
			track.append(target);
			setVariablePath(target, variable, track);
			track.removeLast();
		}
	}
}

void PetriNet::coloring(StateList * netStates, const QMap<QString, Type *> &types)
{
	State * state = netStates->first();
	foreach (QString variable, types.keys())
	{
		QList<State *> track;
		track.append(state);
		setVariablePath(state, variable, track);
	}

	int k = 10;
	QList<QSet<QString> > variableList;
	QStringList colorList = QColor::colorNames();
	QList<State *> places = netStates->find(place_node);
	foreach (State * state, places)
	{
		NetPlace * place = dynamic_cast<NetPlace *>(state);
		QSet<QString> set = place->variables().toSet();
		if (!set.isEmpty())
			if (!variableList.contains(set))
			{
				QColor color = QColor(colorList[k++]);
				variableList.append(set);
				place->setColor(color);
			}
			else
			{
				int index = variableList.indexOf(set);
				place->setColor(colorList[10 + index]);
			}
	}
}

void PetriNet::setInitialMarking(StateList * states, StateList * netStates)
{
	State * begin = states->find(begin_state).first();
	State * state = begin->outgoing().first()->target();
	QList<State *> places = netStates->find(QString("%1_place").arg(state->id()));
	NetPlace * place = dynamic_cast<NetPlace *>(places.first());
	place->addMarking(new Marking);
}

void PetriNet::initVariables(StateList * states, QMap<QString, Type *> types)
{
	bool flag = true;
	QList<State *> places = states->find(place_node);
	for (int i = 0; i < places.count() && flag; ++i)
	{
		NetPlace * place = dynamic_cast<NetPlace *>(places[i]);
		foreach (QString variable, place->variables())
			if (types.contains(variable))
				place->setVariableValue(variable, types.take(variable));

		flag = !types.isEmpty();
	}
}
