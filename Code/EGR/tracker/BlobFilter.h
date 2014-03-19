#pragma once
#include "filter.h"
#include "global.h"
#include "BlobResult.h"

class BlobFilter :
	public Filter
{
public:
	BlobFilter(void);
	~BlobFilter(void);
	BlobFilter* clone();
	CBlobResult blobs;
private:
	IplImage* filter(std::vector<IplImage*> images);
	/*friend class boost::serialization::access; 
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::base_object<Filter>(*this);
	}*/
};
//BOOST_CLASS_EXPORT(BlobFilter);

