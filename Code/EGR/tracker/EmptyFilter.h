/* 
 * File:   EmptyFilter.h
 * Author: tmf
 *
 * Created on 24. Oktober 2009, 14:36
 */

#ifndef _EMPTYFILTER_H
#define	_EMPTYFILTER_H
#include "filter.h"

class EmptyFilter : public Filter{
public:
    EmptyFilter();
    IplImage* filter(std::vector<IplImage*> images);
    EmptyFilter* clone();
private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(EmptyFilter);
#endif	/* _EMPTYFILTER_H */

