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

 #include "bubble.h"

 Bubble::Bubble(QPointF &position, QString &file)
     : position(position), image(QImage(file))
 {
	 image.load(file);
	 this->position.setY(position.y() - image.height()/2);
	 this->originalPosition = this->position;
	 /*QPointF velocity(1000/image.width()*(-0.5 + qrand()/(RAND_MAX+1.0)),
			1000/image.height()*(-0.5 + qrand()/(RAND_MAX+1.0)));*/
	 //this->vel = velocity;
     /*innerColor = randomColor();
     outerColor = randomColor();
     updateBrush();*/
 }

 /*void Bubble::updateBrush()
 {
     QRadialGradient gradient(QPointF(radius, radius), radius,
                              QPointF(radius*0.5, radius*0.5));

     gradient.setColorAt(0, QColor(255, 255, 255, 255));
     gradient.setColorAt(0.25, innerColor);
     gradient.setColorAt(1, outerColor);
     brush = QBrush(gradient);
 }
*/
 void Bubble::drawBubble(QPainter *painter)
 {
     painter->save();
	 /*
     painter->translate(position.x() - radius, position.y() - radius);
     painter->setBrush(brush);
     painter->drawEllipse(0, 0, int(2*radius), int(2*radius));
	 */
	 painter->translate(position.x(), position.y());
	 painter->drawImage(0, 0, image);
     painter->restore();
 }

 /*QColor Bubble::randomColor()
 {
     int red = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
     int green = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
     int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
     int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

     return QColor(red, green, blue, alpha);
 }*/

 void Bubble::move(int x)
 {
	 position.setX( originalPosition.x() +x);
	 
     //position += vel;
	 //vel*=0.999;
	 /*
     qreal leftOverflow = position.x() - bbox.left();
	 qreal rightOverflow = position.x() + image.width() - bbox.right();
     qreal topOverflow = position.y()  - bbox.top();
	 qreal bottomOverflow = position.y() + image.height() - bbox.bottom();

     if (leftOverflow < 0.0) {
         //position.setX(position.x() - 2 * leftOverflow);
         vel.setX(-vel.x());
     } else if (rightOverflow > 0.0) {
         //position.setX(position.x() - 2 * rightOverflow);
         vel.setX(-vel.x());
     }

     if (topOverflow < 0.0) {
         //position.setY(position.y() - 2 * topOverflow);
         vel.setY(-vel.y());
     } else if (bottomOverflow > 0.0) {
         //position.setY(position.y() - 2 * bottomOverflow);
         vel.setY(-vel.y());
     }*/
 }

 QRectF Bubble::rect()
 {
     return QRectF(position.x(), position.y() ,
		 image.width(), image.height());
 }