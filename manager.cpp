#include "manager.h"

Manager::Manager() : QObject(0) {}

Manager::~Manager() {}

int Manager::manage()
{
	window = new MainWindow;
	window->show();
}
