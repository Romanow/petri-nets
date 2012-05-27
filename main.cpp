#include <QApplication>

#include "mainwindow.h"

#include <QRegExp>
#include <QDebug>

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	return app.exec();
}
