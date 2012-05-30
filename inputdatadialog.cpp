#include "inputdatadialog.h"
#include "translate.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>

InputDataDialog::InputDataDialog(QWidget * parent) : QDialog(parent)
{
	initInterface();
	initConnections();

	setWindowTitle(conv("Начальные данные"));
}

InputDataDialog::~InputDataDialog()
{
	qDeleteAll(lines);
	lines.clear();
}

void InputDataDialog::initInterface()
{
	QHBoxLayout * hLayout = new QHBoxLayout;
	QVBoxLayout * vLayout = new QVBoxLayout;
	inputWidget = new QLabel(this);
	inputWidget->setFixedWidth(340);
	vLayout->addWidget(inputWidget);

	hLayout = new QHBoxLayout;
	btnApply = new QPushButton(conv("Применить"), this);
	btnApply->setFixedSize(100, 40);
	btnCancel = new QPushButton(conv("Отмена"), this);
	btnCancel->setFixedSize(100, 40);
	hLayout->addStretch();
	hLayout->addWidget(btnApply);
	hLayout->addWidget(btnCancel);
	hLayout->addStretch();
	vLayout->addLayout(hLayout);

	setLayout(vLayout);
}

void InputDataDialog::initConnections()
{
	connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));
	connect(btnApply, SIGNAL(clicked()), SLOT(accept()));
}

void InputDataDialog::setDesktopCenter()
{
	move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

QVariant InputDataDialog::serialize(const QString &line)
{
	bool flag;
	QVariant result = line.toDouble(&flag);
	if (!flag)
		result = line;

	return result;
}

QString InputDataDialog::serialize(const QVariant &value)
{
	QString result;
	if (value.type() == QVariant::Int || value.type() == QVariant::Double)
		result.setNum(value.toDouble());
	else if (value.type() == QVariant::String)
		result = value.toString();

	return result;
}

void InputDataDialog::addInterfaceElement(SimpleType * simple, int x, int y)
{
	QLineEdit * line = new QLineEdit(inputWidget);
	line->setFixedSize(120, 25);
	line->move(210, 18 + 30 * y);

	QString valueString;
	QVariantList values = simple->values();
	if (values.count() > 0)
	{
		foreach (QVariant value, values)
			valueString += serialize(value) + "; ";

		valueString = valueString.left(valueString.length() - 2);
		if (values.count() > 1)
			valueString = "[" + valueString + "]";

		line->setText(valueString);
	}
	lines.append(line);
}

void InputDataDialog::drawInterface(QMap<QString, Type *> variables, QPainter &painter, int hLevel, int &vLevel)
{
	foreach (QString name, variables.keys())
	{
		vLevel += 1;
		if (variables[name]->type() == complex)
		{
			ComplexType * complex = reinterpret_cast<ComplexType *>(variables[name]);
			painter.drawText(25 + 50 * hLevel, 25 + 30 * vLevel, variables[name]->name());
			drawInterface(complex->variables(), painter, hLevel + 1, vLevel);
		}
		else
		{
			painter.drawText(25 + 50 * hLevel, 25 + 30 * vLevel, variables[name]->name());
			if (hLevel != 0)
			{
				painter.drawLine(- 10 + 50 * hLevel, 20 + 30 * vLevel, 20 + 50 * hLevel, 20 + 30 * vLevel);
				painter.drawLine(- 10 + 50 * hLevel, 30 * vLevel, - 10 + 50 * hLevel, 20 + 30 * vLevel);
			}

			SimpleType * simple = reinterpret_cast<SimpleType *>(variables[name]);
			addInterfaceElement(simple, hLevel, vLevel);
		}
	}
}

void InputDataDialog::setVariableList(const QMap<QString, Type *> &types)
{
	qDeleteAll(lines);
	lines.clear();

	QPixmap pixmap(180, 500);
	QPainter painter(&pixmap);

	int hLevel = 0, vLevel = - 1;
	painter.fillRect(0, 0, pixmap.width(), pixmap.height(), palette().window());

	drawInterface(types, painter, hLevel, vLevel);

	inputWidget->setPixmap(pixmap.copy(0, 0, 180, 25 + 30 * vLevel));
	setFixedSize(380, 140 + 30 * vLevel);
	setDesktopCenter();
}

void InputDataDialog::getValues(QMap<QString, Type *> &variables, int &number)
{
	foreach (QString name, variables.keys())
	{
		if (variables[name]->type() == complex)
		{
			ComplexType * complex = reinterpret_cast<ComplexType *>(variables[name]);
			getValues(complex->variables(), number);
		}
		else
		{
			SimpleType * simple = reinterpret_cast<SimpleType *>(variables[name]);
			QString line = lines[number]->text();

			QStringList array = line.split(QRegExp("[\\[;\\]]"), QString::SkipEmptyParts);
			foreach (QString value, array)
				simple->values().append(serialize(value));

			number += 1;
		}
	}
}

void InputDataDialog::getVariableList(QMap<QString, Type *> &types)
{
	int number = 0;
	getValues(types, number);
}
