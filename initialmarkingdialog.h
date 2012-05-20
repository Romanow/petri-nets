#ifndef INITIALMARKINGDIALOG_H
#define INITIALMARKINGDIALOG_H

#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QDialog>

class InitialMarkingDialog : public QDialog
{
    Q_OBJECT

public:
	InitialMarkingDialog();

	int getMarking();
	void setMarking(int marking);

private:
	void initInterface();
	void initConnections();
	void setDesktopCenter();

	QSpinBox * spnInitialMarking;
	QPushButton * btnCancel;
	QPushButton * btnApply;
};

#endif // INITIALMARKINGDIALOG_H
