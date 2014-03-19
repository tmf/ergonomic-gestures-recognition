/* 
 * File:   ThresholdFilter.h
 * Author: tmf
 *
 * Created on 24. Oktober 2009, 14:20
 */

#ifndef _THRESHOLDFILTER_H
#define	_THRESHOLDFILTER_H
#include "filter.h"
#include "global.h"

class ThresholdFilter : public Filter {
public:
    ThresholdFilter();
    IplImage* filter(std::vector<IplImage*> images);
    ThresholdFilter* clone();
private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(ThresholdFilter);


#endif	/* _THRESHOLDFILTER_H */

