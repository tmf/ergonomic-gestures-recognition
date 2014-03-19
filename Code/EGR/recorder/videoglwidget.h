#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <cv.h>

using namespace cv;

class VideoGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    
	VideoGLWidget(QWidget *parent = 0);
    void sendImage(Mat *img);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    QImage qframe;
	QTimer timer;
};

#endif // GLWIDGET_H
