#pragma once
#include "filter.h"
#include "global.h"

class DilationFilter :
	public Filter
{
public:
	DilationFilter(void);
	~DilationFilter(void);
	DilationFilter* clone();
	IplImage* filter(std::vector<IplImage* > images);
private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(DilationFilter);

