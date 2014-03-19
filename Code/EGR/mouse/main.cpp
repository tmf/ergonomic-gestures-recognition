#include "mouse.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mouse w;
	w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	QDesktopWidget *desktop = QApplication::desktop();
	w.setGeometry(desktop->screenGeometry().width() , 0, w.width(), desktop->screenGeometry().height());
	w.show();
	return a.exec();
}
