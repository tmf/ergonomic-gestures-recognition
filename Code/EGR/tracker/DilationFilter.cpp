#include "stdafx.h"
#include "DilationFilter.h"

DilationFilter::DilationFilter(void)
{
	name = "Erosion Filter";
	sourceCount=1;
    params.push_back(new Parameter(0.0f, 1.0f, 5.0f));
}

DilationFilter::~DilationFilter(void)
{
}

DilationFilter* DilationFilter::clone(void){
	DilationFilter *df = new DilationFilter();
    df->params.clear();
    for(int i=0;i<(int)params.size();i++)
        df->params.push_back(new Parameter(this->params[i]));
    return df;
}

IplImage* DilationFilter::filter(std::vector<IplImage*> images){
	IplImage *img = 0;
	if(images.size()>0)
		img = images[0];

	Parameter *pLoop = params[0];
	
	if(pLoop != 0 ){
		cvDilate(img, img, 0, pLoop->getVal());
	}
	return img;

}