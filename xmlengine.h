#ifndef XMLENGINE_H
#define XMLENGINE_H

#include <QXmlStreamReader>
#include <QtXmlPatterns/QAbstractMessageHandler>
#include <QtXmlPatterns/QXmlSchema>

#include "state.h"

class MessageHandler : public QAbstractMessageHandler
{
public:
    QString message() const
    {
        return messageDescription;
    }

    int line() const
    {
        return messageLocation.line();
    }

    int column() const
    {
        return messageLocation.column();
    }

protected:
    virtual void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation)
    {
        messageType = type;
        messageDescription = description;
        messageLocation = sourceLocation;
    }

private:
    QtMsgType messageType;
    QString messageDescription;
    QSourceLocation messageLocation;

};

class XMLValidator
{
public:
    XMLValidator();
	XMLValidator(const QString &fileName);
    ~XMLValidator();

    bool validate(const QByteArray &data);

    bool setSchemaFile(const QString &fileName);

    QString message() const
    {
        return handler->message();
    }

    int line() const
    {
        return handler->line();
    }

    int column() const
    {
        return handler->column();
    }

private:
    QString schemaFile;
    MessageHandler * handler;
	QXmlSchema * schema;

};

class XMLEngine
{
public:
	XMLEngine();
	XMLEngine(const QString &schema);
	~XMLEngine();

	StateList parse(const QString &data);

private:
	bool parseStates(QXmlStreamReader * reader, StateList &states, TransitionList &transitions);
	bool parseState(QXmlStreamReader * reader, State * state, TransitionList &transitions);
	bool parseIncomingTransitions(QXmlStreamReader * reader, State * state, TransitionList &transitions);
	bool parseOutgoingTransitions(QXmlStreamReader * reader, State * state, TransitionList &transitions);
	bool parseExpression(QXmlStreamReader * reader, State * state);

	bool parseTransitions(QXmlStreamReader * reader, StateList &states, TransitionList &transitions);
	bool parseTransition(QXmlStreamReader * reader, Transition * transition, StateList &states);
	bool parseSourceStates(QXmlStreamReader * reader, Transition * transition, StateList &states);
	bool parseTargetStates(QXmlStreamReader * reader, Transition * transition, StateList &states);
	bool parseGuardExpression(QXmlStreamReader * reader, Transition * transition);

	XMLValidator * validator;
};

#endif // XMLENGINE_H
