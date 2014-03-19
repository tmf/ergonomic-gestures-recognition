#pragma once
#include "filter.h"
#include "global.h"
#include "cvfast.h"

class FastCornerFilter :
	public Filter
{
public:
	FastCornerFilter(void);
	~FastCornerFilter(void);
	FastCornerFilter* clone();
	IplImage* filter(std::vector<IplImage*> images);
  double dist(int x1, int y1, int x2, int y2);
  double dist(int dx, int dy);
private:
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(FastCornerFilter);
