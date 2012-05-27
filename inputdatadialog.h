#ifndef INPUTDATADIALOG_H
#define INPUTDATADIALOG_H

#include <QList>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "petrinet.h"

class InputDataDialog : public QDialog
{
    Q_OBJECT

public:
    InputDataDialog(QWidget * parent = 0);

    void setVariableList(const QMap<QString, Type *> &types);
    void getVariableList(QMap<QString, Type *> &variables);

private:
    void initInterface();
    void initConnections();
    void setDesktopCenter();

	void addInterfaceElement(SimpleType * simple, int x, int y);
	void drawInterface(QMap<QString, Type *> variables, QPainter &painter, int hLevel, int &vLevel);
	QVariant serialize(const QString &line);
	QString serialize(const QVariant &value);
    void getValues(QMap<QString, Type *> &variables, int &number);

	QLabel * inputWidget;
	QList<QLineEdit *> lines;
    QPushButton * btnApply;
    QPushButton * btnCancel;

};

#endif // INPUTDATADIALOG_H
