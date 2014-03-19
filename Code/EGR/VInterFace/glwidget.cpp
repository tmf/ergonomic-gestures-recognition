/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#include <stdlib.h>
#include <QDir>
#include <math.h>
#include <QFileDialog>
#include "bubble.h"
#include "qtlogo.h"
#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	QTime midnight(0, 0, 0);
	qsrand(midnight.secsTo(QTime::currentTime()));

	//logo = 0;
	xRot = 0;
	yRot = 0;
	zRot = 0;

	qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
	qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

	animationTimer.setSingleShot(false);
	handTimer.setSingleShot(false);
	connect(&animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
	connect(&handTimer, SIGNAL(timeout()), this, SLOT(handFrame()));
	animationTimer.start(25);
	

	backgroundImage = QImage(tr("c:\\egr\\bg.png"));
	foregroundImage = QImage(tr("c:\\egr\\fg.png"));
	setAutoFillBackground(false);
	setMinimumSize(200, 200);
	setWindowTitle(tr("Overpainting a Scene"));

	destinationX = 0;
	hand = createEGRHand();
	QString trackerConfigFile("c:\\egr\\tracker.cfg");
	if(!QFile::exists(trackerConfigFile)){
		trackerConfigFile = QFileDialog::getOpenFileName ( this, tr("Please select a tracker configuration file"), tr("c:\\egr"), tr("EGR Tracker Configuration Files (*.cfg)"));
	}
	hand->init(trackerConfigFile.toStdString(), "blobs_5", "blobs_6");
	classifier df;
	QString classifierFile("c:\\egr\\3gest.egrdf");
	if(!QFile::exists(classifierFile)){
		classifierFile = QFileDialog::getOpenFileName ( this, tr("Please select a trained decision function file"), tr("c:\\egr"), tr("EGR Decision Function Files (*.egrdf)"));
	}
	ifstream fin(classifierFile.toStdString().c_str(), ios::binary);
    deserialize(df, fin);
	hand->setClassifier(df);
	hand->start();
	handTimer.start(40);
	gestureFrameCounter = -1;
}

GLWidget::~GLWidget()
{
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != xRot)
		xRot = angle;
}

void GLWidget::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != yRot)
		yRot = angle;
}

void GLWidget::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != zRot)
		zRot = angle;
}

void GLWidget::initializeGL()
{
	glEnable(GL_MULTISAMPLE);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	currentBubbleIndex++;
	if(currentBubbleIndex >= bubbles.size())
		currentBubbleIndex = 0;
	this->destinationX = -bubbles.at(currentBubbleIndex)->originalPosition.x();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
}

void GLWidget::paintEvent(QPaintEvent *event)
{

	makeCurrent();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	qglClearColor(qtPurple.dark());
	/*
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	*/
	setupViewport(width(), height());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);*/

	//logo->draw();

	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	drawBackgroundInstructions(&painter);
	foreach (Bubble *bubble, bubbles) {
		if (bubble->rect().intersects(event->rect()))
			bubble->drawBubble(&painter);
	}
	drawForegroundInstructions(&painter);
	drawInstructions(&painter);
	painter.end();
}

void GLWidget::resizeGL(int width, int height)
{
	setupViewport(width, height);
}

void GLWidget::showEvent(QShowEvent *event)
{
	Q_UNUSED(event);
	createBubbles();
}

QSize GLWidget::sizeHint() const
{
	return QSize(400, 400);
}

void GLWidget::createBubbles()
{
	QDir egrDir(tr("c:\\egr\\images"));
	QStringList nameFilter;
	nameFilter << "*.png";
	QFileInfoList list = egrDir.entryInfoList (nameFilter, QDir::Files);
	foreach(QFileInfo img, list){
		QPointF position;
		if(bubbles.size()>0){
			position.setX(bubbles.back()->rect().x() + bubbles.back()->rect().width() + 10);
			position.setY(height()/2+100);

		}else{
			position.setX(0);
			position.setY(height()/2);
		}
		/*QPointF position(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
			height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));*/
		bubbles.append(new Bubble(position, img.absoluteFilePath()));
	}
	/*for (int i = 0; i < number; ++i) {
		qreal radius = qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0));
		QPointF velocity(width()*0.0125*(-0.5 + qrand()/(RAND_MAX+1.0)),
			height()*0.0125*(-0.5 + qrand()/(RAND_MAX+1.0)));
	}*/
}
void GLWidget::handFrame(){
	IplImage *tracker_frame = (IplImage *) hand->getTrackerImage("Screen 8");
	string posture = hand->getClassifiedPosture();
		if(tracker_frame!=0){
			
			
			handleGesture(posture);
		}
}
void GLWidget::animate()
{
	QMutableListIterator<Bubble*> iter(bubbles);

	while (iter.hasNext()) {
		Bubble *bubble = iter.next();
		bubble->move(currentX);
	}
	float D = 0.3;
	float friction = 0.2;
	int dx = destinationX - currentX ;
	velocityX = ((float)dx*D + (float)velocityX)*friction;
	currentX = currentX + velocityX;
	

	update();
}

string GLWidget::filterPostures(string newposture, int window_size){
	if(postures.size()>=HISTORY_SIZE){
		postures.pop_front();
	}
	postures.push_back(newposture);
	map<string, int> histogram;
	int j=0;
	for(std::list<string>::iterator i=postures.end(); i!=postures.begin() && j<window_size;j++){
		i--;
		histogram[(*i)] = 0;
	}
	j=0;
	for(std::list<string>::iterator i=postures.end(); i!=postures.begin() && j<window_size;j++){
		i--;
		histogram[(*i)] = histogram[(*i)] + 1;
	}
	int max=0;
	string robustPosture = "";
	for(map<string, int>::iterator i=histogram.begin(); i!=histogram.end(); i++){
		if(i->second>max){
			max = i->second;
			robustPosture = i->first;
		}
	}
	
	return robustPosture;
}
void GLWidget::handleGesture(string newposture){
	if(gestureFrameCounter >=0 ){
		gestureFrameCounter++;
	}
	if(gestureFrameCounter>30){
		gestureFrameCounter=-1;
	}
	
	string posture = filterPostures(newposture, 10);
	if(posture.compare("")==0){
		
	}
	//ui.posture->setText(QString::fromStdString(posture));
	if(posture.compare(string("POINT"))==0){
		if(newposture.compare(posture)==0){
			cv::Point p;
			p.y = 9999;
			list<EGRPoint> fingers = hand->getFingerPositions();
			for(list<EGRPoint>::iterator i = fingers.begin(); i!=fingers.end(); i++){
				if((*i).y < p.y){
					p.x = i->x;
					p.y = i->y;
					
				}
			}
			
			//updateMousePointer(p);
		}
	}else if(posture.compare(string("SWIPELEFT"))==0){
		if(newposture.compare(string("SWIPERIGHT"))==0){
			if(gestureFrameCounter<0){
			gestureFrameCounter = 0;
			currentBubbleIndex = (currentBubbleIndex+1) < bubbles.size() ? currentBubbleIndex + 1 : currentBubbleIndex;
			this->destinationX = -bubbles.at(currentBubbleIndex)->originalPosition.x() + 400;
			}
		}
	}else if(posture.compare(string("SWIPERIGHT"))==0){
		if(newposture.compare(string("SWIPELEFT"))==0){
			if(gestureFrameCounter<0){
			gestureFrameCounter = 0;
			currentBubbleIndex = (currentBubbleIndex-1) >= 0 ? currentBubbleIndex - 1 : currentBubbleIndex;
			this->destinationX = -bubbles.at(currentBubbleIndex)->originalPosition.x() + 400;
			}
		}
	}else if(posture.compare(string("ZOOM"))==0){
		if(newposture.compare(posture)==0){
			int d=0;

			cv::Point p1;
			cv::Point p2;
			p1.y = 9999;
			p2.y = 9999;
			list<EGRPoint> fingers = hand->getFingerPositions();
			for(list<EGRPoint>::iterator i = fingers.begin(); i!=fingers.end(); i++){
				if((*i).y < p2.y && (*i).y > p1.y){
					p2.x = i->x;
					p2.y = i->y;
					
					
				}
				if((*i).y < p1.y){
					
					p1.x = i->x;
					p1.y = i->y;
					
				}
			}
			/*
			list<EGRPoint> fingers = hand->getFingerPositions();
			list<EGRPoint>::iterator indexFinger = fingers.begin();
			list<EGRPoint>::iterator thumbFinger = fingers.end();
			thumbFinger--;
			int dx = indexFinger->x - thumbFinger->x;
			int dy = indexFinger->y - thumbFinger->y;*/
			int dx = p2.x - p1.x;
			int dy = p2.y - p1.y;
			d = sqrt((double)(dx*dx + dy*dy));
			//updateZoom(d);
		}
	}
}
void GLWidget::setupViewport(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef QT_OPENGL_ES
	glOrthof(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#else
	glOrtho(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#endif
	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::drawInstructions(QPainter *painter)
{
	
	QString text;//tr("Click and drag with the left mouse button to rotate the Qt logo.");
	text.setNum(this->gestureFrameCounter);
	QFontMetrics metrics = QFontMetrics(font());
	int border = qMax(4, metrics.leading());

	QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
		Qt::AlignCenter | Qt::TextWordWrap, text);
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
		QColor(0, 0, 0, 127));
	
	painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
		QColor(0, 0, 0, 127));
	
	painter->setPen(Qt::white);
	painter->drawText((width() - rect.width())/2, border,
		rect.width(), rect.height(),
		Qt::AlignCenter | Qt::TextWordWrap, text);
}

void GLWidget::drawBackgroundInstructions(QPainter *painter)
{
	painter->drawImage(QRect(0, 0, width(), height()), backgroundImage, backgroundImage.rect());
}
void GLWidget::drawForegroundInstructions(QPainter *painter)
{
	painter->drawImage(QRect(0, 0, width(), height()), foregroundImage, foregroundImage.rect());
}