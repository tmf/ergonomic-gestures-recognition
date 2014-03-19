/* 
 * File:   Step.h
 * Author: tmf
 *
 * Created on 21. Oktober 2009, 16:09
 */

#ifndef _STEP_H
#define	_STEP_H

#include "overlay.h"
#include "cv.h"
#include "Filter.h"
#include <iostream>
#include <string>

//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/vector.hpp>

//because overlay.h includes step.h->let compiler know there is a class named Overlay
class Overlay;

class Step {
public:
    Step();
	Step(int n);
    Step(const Step& orig);
    virtual ~Step();
    bool process(bool showOverlay, int key);
    void setFilter(Filter *f);
    std::vector<Parameter*> getFilterParameters();
	std::vector<std::string> getFilterInputs();
    int number;
	float executionTime;
	//void setSources(std::vector<char *> sources);
	std::vector<std::string> sources;
	Filter *flt;
	Overlay* onscreen;
	
private:
	/*friend class boost::serialization::access;
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version){
		ar & number;
		ar & sources;
		ar & flt;
	}*/

    
    
	
};

#endif	/* _STEP_H */

