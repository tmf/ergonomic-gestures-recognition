#include "stdafx.h"
#include "SmoothFilter.h"

SmoothFilter::SmoothFilter(void)
{
	name = "Smooth Filter";
	sourceCount=1;
    params.push_back(new Parameter(0.0f, 1.0f, 5.0f));
}

SmoothFilter::~SmoothFilter(void)
{
}

SmoothFilter* SmoothFilter::clone(){
	SmoothFilter *sf = new SmoothFilter();
    sf->params.clear();
    for(int i=0;i<(int)params.size();i++)
        sf->params.push_back(new Parameter(this->params[i]));
    return sf;
}

IplImage* SmoothFilter::filter(std::vector<IplImage*> images){
	IplImage *img = 0;
	if(images.size()>0)
		img = images[0];
	
	//char *strImg = new char[32];
	//sprintf(strImg, "smooth_%d", step);
	//StorageObject *so_thresh = getStorageObject(strImg);
    //if(so_thresh->data==0)
	//	so_thresh->data = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	//IplImage *thresh = (IplImage *)so_thresh->data;
	//if(img->nChannels>1)
	//	cvCvtColor(img, thresh, CV_RGB2GRAY);
	//else
	//	cvCopy(img, thresh);
	Parameter *pLoop = params[0];
	
	if(pLoop != 0 ){
		for(int i=0; i<pLoop->getVal(); i++)
			cvSmooth(img, img, CV_GAUSSIAN);
	}
	return img;

}