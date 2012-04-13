#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

#include "mainwindow.h"

class Manager : public QObject
{
    Q_OBJECT

public:
	Manager();
	~Manager();

	int manage();

private:
	MainWindow * window;
};

#endif // MANAGER_H
