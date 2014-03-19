#pragma once
#include "EGRPoint.h"
#include "dlib/matrix.h"
#include <list>

using namespace std;
using namespace dlib;

class EGRPosture
{
public:
	EGRPosture(list<EGRPoint> _fingers, EGRPoint _center, EGRPoint _wrist, string _label) : fingers(_fingers), center(_center), wrist(_wrist), label(_label){};
	EGRPosture(void){}
	~EGRPosture(void){}
	matrix<double, 10, 1> getPoints(){
		matrix<double, 10, 1> result;
		result = 0, 
				 0,
				 0,
				 0,
				 0,
			     0,
				 0,
				 0,
				 0,
				 0;
		int i=0;
		for(list<EGRPoint>::iterator f = fingers.begin(); f!=fingers.end(); f++){
			int x = f->x;
			int y = f->y;
			result(i) = (double)x;
			result(i+1) = (double)y;
			i+=2;
		}

		return result;
	}
	matrix<int, 2, 1> getCenter(){
		matrix<int, 2, 1> result;
		result = 0, 0;
		result(0) = center.x ;
		result(1) = center.y ;
		return result;
	}
	matrix<int, 2, 1> getOrigin(){
		matrix<int, 2, 1> result;
		result = 0, 0;
	result(0) = wrist.x;
	result(1) = wrist.y;
	return result;
}
string &getLabel(){return label;}
void setLabel(string l){label = l;}
private:
	list<EGRPoint> fingers;
	EGRPoint center;
	EGRPoint wrist;
	string label;
};
