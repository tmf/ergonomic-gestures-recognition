#include "trainer.h"
#include "callback.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	trainer *w = new trainer();
	instance = (void *) w;
	w->show();
	return a.exec();
}
