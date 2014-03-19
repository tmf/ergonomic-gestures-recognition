#pragma once
#include "filter.h"

class AddFilter :
	public Filter
{
public:
	AddFilter(void);
	~AddFilter(void);
	IplImage* filter(std::vector<IplImage*> images);
	AddFilter* clone();
private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(AddFilter);
