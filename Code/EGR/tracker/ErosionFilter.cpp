#include "stdafx.h"
#include "ErosionFilter.h"

ErosionFilter::ErosionFilter(void)
{
	name = "Erosion Filter";
	sourceCount=1;
    params.push_back(new Parameter(0.0f, 1.0f, 5.0f));
}

ErosionFilter::~ErosionFilter(void)
{
}

ErosionFilter* ErosionFilter::clone(){
	ErosionFilter *ef = new ErosionFilter();
    ef->params.clear();
    for(int i=0;i<(int)params.size();i++)
        ef->params.push_back(new Parameter(this->params[i]));
    return ef;
}

IplImage* ErosionFilter::filter(std::vector<IplImage*> images){
	IplImage *img = 0;
	if(images.size()>0)
		img = images[0];

	Parameter *pLoop = params[0];
	
	if(pLoop != 0 ){
		cvErode(img, img, 0, pLoop->getVal());
	}
	return img;

}