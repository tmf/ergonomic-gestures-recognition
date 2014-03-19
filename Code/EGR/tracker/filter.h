/* 
 * File:   Filter.h
 * Author: tmf
 *
 * Created on 21. Oktober 2009, 16:24
 */

#ifndef _FILTER_H
#define	_FILTER_H

#include "cv.h"
#include <vector>
#include <iostream>
#include <string>

#include "parameter.h"
/*#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>*/

class Filter {
public:
    Filter();
    Filter(const Filter& orig);
    virtual ~Filter();
	virtual IplImage* filter(std::vector<IplImage*> images) = 0;
    std::vector<Parameter*> params;
	std::vector<std::string> inputs;
	int step;
	std::string name;
	int sourceCount;
    virtual Filter* clone() =0;
private:
	/*friend class boost::serialization::access;
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version){
	{
		ar & step;
		ar & name;
		ar & sourceCount;
		ar & inputs;
		ar & params;
}
	}*/
    
};

#endif	/* _FILTER_H */

