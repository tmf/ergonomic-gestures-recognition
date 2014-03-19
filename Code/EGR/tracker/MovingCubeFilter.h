#pragma once
#include "filter.h"

class MovingCubeFilter :
	public Filter
{
public:
    MovingCubeFilter();
    IplImage* filter(std::vector<IplImage*> images);
    MovingCubeFilter* clone();
private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(MovingCubeFilter);
