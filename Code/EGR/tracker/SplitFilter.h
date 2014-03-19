#pragma once
#include "filter.h"
#include "global.h"

class SplitFilter : public Filter {
public:
    SplitFilter();
    IplImage* filter(std::vector<IplImage*> images);
    SplitFilter* clone();
/*private:
	friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(SplitFilter);
