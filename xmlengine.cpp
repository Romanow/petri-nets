#include "xmlengine.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QtXmlPatterns/QXmlSchemaValidator>

#include <QDebug>

XMLValidator::XMLValidator()
{
	schema = new QXmlSchema;
	handler = new MessageHandler;
	schema->setMessageHandler(handler);
}

XMLValidator::XMLValidator(const QString &fileName)
{
	schema = new QXmlSchema;
	handler = new MessageHandler;
	schema->setMessageHandler(handler);
	setSchemaFile(fileName);
}

XMLValidator::~XMLValidator()
{
	delete schema;
	delete handler;
}

bool XMLValidator::setSchemaFile(const QString &fileName)
{
	bool result = true;
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		schema->load(file.readAll());
		if (!schema->isValid())
			result = false;
		file.close();
	}
	else
		result = false;

	return result;
}

bool XMLValidator::validate(const QByteArray &data)
{
	bool result = true;
	if (schema->isValid())
	{
		QXmlSchemaValidator validator(* schema);
		if (!validator.validate(data))
			result = false;
	}
	else
		result = false;

	if (!result)
		qDebug() << line() << column() << message();

	qDebug() << "Validation result" << result;

	return result;
}

XMLEngine::XMLEngine()
{
	validator = new XMLValidator(":/schema.xsd");
}

XMLEngine::XMLEngine(const QString &schema)
{
	validator = new XMLValidator(schema);
}

XMLEngine::~XMLEngine()
{
	delete validator;
}

template <class A, class B>
bool compare(const A &a, const B &b)
{
	return !a.compare(b, Qt::CaseInsensitive);
}

bool XMLEngine::parse(const QString &data, StateList * states, TransitionList * transitions)
{
	bool result = true;
	if (validator->validate(data.toAscii()))
	{
		QXmlStreamReader reader(data);
		if (reader.readNextStartElement())
		{
			if (compare(reader.name(), "activity_diagram"))
			{
				result &= parseStates(&reader, states, transitions);
				result &= parseTransitions(&reader, states, transitions);
			}
		}
		result &= !reader.hasError();
	}

	qDebug() << "Parse result" << result;

	return result;
}

bool XMLEngine::parseStates(QXmlStreamReader * reader, StateList * states, TransitionList * transitions)
{
	if (reader->readNextStartElement() && compare(reader->name(), "states"))
	{
		while (reader->readNextStartElement() && compare(reader->name(), "state"))
		{
			QString name = reader->attributes().value("name").toString();
			QString id = reader->attributes().value("id").toString();
			QString type = reader->attributes().value("type").toString();
			State * state = Factory::create(name, id, type);

			parseState(reader, state, transitions);

			states->append(state);
		}
	}

	return !reader->hasError();
}

bool XMLEngine::parseState(QXmlStreamReader * reader, State * state, TransitionList * transitions)
{
	while (reader->readNextStartElement())
	{
		if (compare(reader->name(), "incoming"))
		{
			parseIncomingTransitions(reader, state, transitions);
		}
		else if (compare(reader->name(), "outgoing"))
		{
			parseOutgoingTransitions(reader, state, transitions);
		}
		else if (compare(reader->name(), "expression") && state->type() == action_state)
		{
			parseExpression(reader, state);
		}
		else
			reader->skipCurrentElement();
	}
	return !reader->hasError();
}

bool XMLEngine::parseIncomingTransitions(QXmlStreamReader * reader, State * state, TransitionList * transitions)
{
	while (reader->readNextStartElement())
	{
		QString id = reader->attributes().value("idref").toString();
		reader->readNextStartElement();

		Transition * transition;
		QList<Transition *> list = transitions->find(id);
		if (list.isEmpty())
		{
			transition = new Transition(id);
			transitions->append(transition);
		}
		else
			transition = list.first();

		state->addIncomingTransition(transition);
	}

	return !reader->hasError();
}

bool XMLEngine::parseOutgoingTransitions(QXmlStreamReader * reader, State * state, TransitionList * transitions)
{
	while (reader->readNextStartElement())
	{
		QString id = reader->attributes().value("idref").toString();
		reader->readNextStartElement();

		Transition * transition;
		QList<Transition *> list = transitions->find(id);
		if (list.isEmpty())
		{
			transition = new Transition(id);
			transitions->append(transition);
		}
		else
			transition = list.first();
		state->addOutgoingTransition(transition);
	}

	return !reader->hasError();
}

bool XMLEngine::parseExpression(QXmlStreamReader * reader, State * state)
{
	QString expression = reader->readElementText();
	state->setExpression(expression);

	return !reader->hasError();
}

bool XMLEngine::parseTransitions(QXmlStreamReader * reader, StateList * states, TransitionList * transitions)
{
	bool result = true;
	if (reader->readNextStartElement() && compare(reader->name(), "transitions"))
	{
		while (reader->readNextStartElement() && compare(reader->name(), "transition"))
		{
			QString id = reader->attributes().value("id").toString();
			Transition * transition = transitions->find(id).first();
			if (transition != 0)
				result &= parseTransition(reader, transition, states);
			else
				result = false;
		}
	}

	return result & !reader->hasError();
}

bool XMLEngine::parseTransition(QXmlStreamReader * reader, Transition * transition, StateList * states)
{
	bool result = true;
	while (reader->readNextStartElement())
	{
		if (compare(reader->name(), "source"))
		{
			result &= parseSourceState(reader, transition, states);
		}
		else if (compare(reader->name(), "target"))
		{
			result &= parseTargetState(reader, transition, states);
		}
		else if (compare(reader->name(), "guard"))
		{
			result &= parseGuardExpression(reader, transition);
		}
	}
	return result & !reader->hasError();
}

bool XMLEngine::parseSourceState(QXmlStreamReader * reader, Transition * transition, StateList * states)
{
	bool result = true;
	while (reader->readNextStartElement())
	{
		QString stateId = reader->attributes().value("idref").toString();
		reader->readNextStartElement();

		State * state = states->find(stateId).first();
		if (state == 0 || transition->source() != state)
			result = false;
	}

	return result & !reader->hasError();
}

bool XMLEngine::parseTargetState(QXmlStreamReader * reader, Transition * transition, StateList * states)
{
	bool result = true;
	while (reader->readNextStartElement())
	{
		QString stateId = reader->attributes().value("idref").toString();
		reader->readNextStartElement();
		State * state = states->find(stateId).first();
		if (state == 0 || transition->target() != state)
			result = false;
	}

	return result & !reader->hasError();
}

bool XMLEngine::parseGuardExpression(QXmlStreamReader * reader, Transition * transition)
{
	QString guard = reader->readElementText();
	transition->setGuard(guard);

	return !reader->hasError();
}
