#pragma once
#include "filter.h"
#include "global.h"
#include <omp.h>

class LabThresholdFilter :
	public Filter
{
public:
	LabThresholdFilter(void);
	IplImage* filter(std::vector<IplImage*> images);
	LabThresholdFilter* clone();
	void mask_threshold(IplImage *src, IplImage *dst);
	void fast_threshold(IplImage *src);

private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(LabThresholdFilter);
