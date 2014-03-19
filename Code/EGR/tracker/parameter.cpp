/* 
 * File:   Parameter.cpp
 * Author: tmf
 * 
 * Created on 23. Oktober 2009, 20:23
 */

#include "stdafx.h"
#include "Parameter.h"

Parameter::Parameter(float min, float max){
    
    this->setRange(min, max);
    this->set(min);
	this->type = pRange;
}
Parameter::Parameter(bool toggle){
	this->set(toggle);
	this->type = pToggle;
}
Parameter::Parameter(float min, float val, float max){
  
    this->setRange(min, max);
    this->set(val);
	this->type = pRange;
}
Parameter::Parameter(float min, float low, float high, float max){
    this->setRange(min, max);
    this->set(low, high);
	this->type = pBandpass;
}
Parameter::Parameter(int x, int y, int screen_x, int screen_y){
    this->setPoint(x, y, screen_x, screen_y);
}
void Parameter::setRange(float min, float max){
    this->min = min;
    this->max = max;
	
}
void Parameter::set(bool toggle){
    this->toggle = toggle;
    this->type = pToggle;
}
void Parameter::set(float value){
    this->val = value;
    this->type = pRange;
}
void Parameter::set(float low, float high){
    this->low = low;
    this->high = high;
    this->type = pBandpass;
}
void Parameter::setPoint(int x, int y, int screen_x, int screen_y){
    this->x = x;
    this->y = y;
    this->screen_x = screen_x;
    this->screen_y = screen_y;
    this->type = pPoint;
}
int Parameter::getType(){
    return this->type;
}
Parameter::Parameter() {
}
void Parameter::setValues(int pType, float pMin, float pMax, float pVal, float pLow, float pHigh, bool pTogl, int pX, int pY, int pScreenX, int pScreenY){
	switch(pType){
		case pRange:
			this->set(pMin, pMax);
			this->set(pVal);
			break;
		case pBandpass:
			this->setRange(pMin, pMax);
			this->set(pLow, pHigh);
			break;
		case pPoint:
			this->setPoint(pX, pY, pScreenX, pScreenY);
			break;
		case pToggle:
			this->set(pTogl);
			break;
	}
}
Parameter::Parameter(int pType, float pMin, float pMax, float pVal, float pLow, float pHigh, bool pTogl, int pX, int pY, int pScreenX, int pScreenY){
	this->setValues(pType, pMin, pMax, pVal, pLow, pHigh, pTogl, pX, pY, pScreenX, pScreenY);
}
Parameter::Parameter(const Parameter* orig) {
    min = orig->min;
    max = orig->max;
    low = orig->low;
    high = orig->high;
    val = orig->val;
	toggle = orig->toggle;
    type = orig->type;
    x = orig->x;
    y = orig->y;
    screen_x = orig->screen_x;
    screen_y = orig->screen_y;

}

Parameter::~Parameter() {
}

bool Parameter::getToggle() const{
	return toggle;
}
float Parameter::getHigh() const{
    return high;
}
float Parameter::getLow() const{
    return low;
}
float Parameter::getMax() const{
    return max;
}
float Parameter::getMin() const{
    return min;
}
float Parameter::getVal() const{
    return val;
}
CvPoint Parameter::getPos() const{
    return cvPoint(x, y);
}
CvPoint Parameter::getScreenPos() const{
    return cvPoint(screen_x, screen_y);
}

