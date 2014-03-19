/* 
 * File:   Parameter.h
 * Author: tmf
 *
 * Created on 23. Oktober 2009, 20:23
 */

#ifndef _PARAMETER_H
#define	_PARAMETER_H
#include "cv.h"
//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/export.hpp>

enum pType {pRange, pBandpass, pPoint, pToggle};
class Parameter {
public:
    Parameter();
	Parameter(bool toggle);
    Parameter(float min, float max);
    Parameter(float min, float val, float max);
    Parameter(float min, float low, float high, float max);
    Parameter(int x, int y, int screen_x, int screen_y);
    Parameter(const Parameter* orig);
	Parameter(int pType, float pMin, float pMax, float pVal, float pLow, float pHigh, bool pTogl, int pX, int pY, int pScreenX, int pScreenY);
    void setValues(int pType, float pMin, float pMax, float pVal, float pLow, float pHigh, bool pTogl, int pX, int pY, int pScreenX, int pScreenY);
	virtual ~Parameter();
    void setRange(float min, float max);
	void set(bool toggle);
    void set(float value);
    void set(float low, float high);
    void setPoint(int x, int y, int screen_x, int screen_y);
    int getType();
    CvPoint getPos() const;
    CvPoint getScreenPos() const;
    
	bool getToggle() const;
    float getHigh() const;
    float getLow() const;
    float getVal() const;
    float getMax() const;
    float getMin() const;

private:

	bool toggle;
    float min, max, val, low, high;
    int x, y, screen_x, screen_y;
    int type;

	/*friend class boost::serialization::access;
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version){
		ar & x;
		ar & y;
		ar & screen_x;
		ar & screen_y;
		ar & type;
		ar & toggle;
    ar & val;
		ar & min;
		ar & max;
		ar & low;
		ar & high;
	}*/
	

};
//BOOST_CLASS_EXPORT(Parameter);

#endif	/* _PARAMETER_H */

