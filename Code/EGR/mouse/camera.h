#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <cv.h>

using namespace cv;

class camera : public QGLWidget
{
	Q_OBJECT
public:

	camera(QWidget *parent = 0) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
	{
		setMinimumSize(160,120);

	}
	void sendImage(Mat *img){
		qframe = QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB32);
		qframe = QGLWidget::convertToGLFormat(qframe);
		this->updateGL();
	}
protected:
	void initializeGL(){
		glClearColor(0.0f,0.0f,0.0f,1.0f);
	}
	void paintGL() {
		glClear (GL_COLOR_BUFFER_BIT);
		glClearColor (0.0,0.0,0.0,1.0);
		if (!qframe.isNull()) {
			qframe = qframe.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
			// you can use glDrawPixels directly
			// glDrawPixels(qframe.width(),qframe.height(), GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits());
			// or do 2D texture mapping
			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0,qframe.width(),qframe.height(),0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(0,qframe.height());
			glTexCoord2f(0,1); glVertex2f(0,0);
			glTexCoord2f(1,1); glVertex2f(qframe.width(),0);
			glTexCoord2f(1,0); glVertex2f(qframe.width(),qframe.height());
			glEnd();
			glDisable(GL_TEXTURE_2D);
			// .... end

			// some example of alpha blending
			//glEnable(GL_DEPTH_TEST);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glColor4f(0.0f,1.0f,1.0f, 0.9f);
			glFlush();
		}
		// qDebug() << "Drawing...";
	}
	void resizeGL(int w, int h) {
		glViewport (0, 0, (GLsizei)w, (GLsizei)h);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
		glMatrixMode (GL_MODELVIEW);
		// qDebug() << "Resizing...";
	}
private:
	QImage qframe;
	QTimer timer;
};

#endif // 
