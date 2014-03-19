#include "gallery.h"
#include "EGRHand.h"

Gallery::Gallery(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	EGRHand *h = createEGRHand();
	h->init("c:\\egr\\tracker.cfg", "blobs_5", "blobs_6");
	h->setAVISource("c:\\egr\\tracker_0103135033.avi");
	h->start();
	h->log("Test!");
}

Gallery::~Gallery()
{

}
