#include "initialmarkingdialog.h"
#include "translate.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>

InitialMarkingDialog::InitialMarkingDialog() : QDialog()
{
	initInterface();
	initConnections();

	setFixedSize(350, 110);
	setWindowTitle(conv("Начальная разметка"));
	setDesktopCenter();
}

void InitialMarkingDialog::initInterface()
{
	QHBoxLayout * hLayout = new QHBoxLayout;
	QVBoxLayout * vLayout = new QVBoxLayout;
	QLabel * lblInitialMarking = new QLabel(conv("Количество фишек в позиции"), this);
	hLayout->addWidget(lblInitialMarking);
	spnInitialMarking = new QSpinBox(this);
	spnInitialMarking->setRange(0, 10);
	spnInitialMarking->setFixedWidth(100);
	hLayout->addWidget(spnInitialMarking);
	vLayout->addLayout(hLayout);

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

void InitialMarkingDialog::initConnections()
{
	connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));
	connect(btnApply, SIGNAL(clicked()), SLOT(accept()));
}

void InitialMarkingDialog::setDesktopCenter()
{
	move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void InitialMarkingDialog::setMarking(int marking)
{
	spnInitialMarking->setValue(marking);
}

int InitialMarkingDialog::getMarking()
{
	return spnInitialMarking->value();
}
