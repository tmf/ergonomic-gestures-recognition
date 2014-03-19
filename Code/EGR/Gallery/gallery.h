#ifndef GALLERY_H
#define GALLERY_H


#include <QtGui/QMainWindow>
#include "ui_gallery.h"

class Gallery : public QMainWindow
{
	Q_OBJECT

public:
	Gallery(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Gallery();

private:
	Ui::GalleryClass ui;
};

#endif // GALLERY_H
