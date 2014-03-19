#include "stdafx.h"
#include "MovingCubeFilter.h"
#include "global.h"

MovingCubeFilter::MovingCubeFilter(){
    name = "Moving Cube Filter";
	sourceCount=1;
    params.push_back(new Parameter(false));
    
}
IplImage* MovingCubeFilter::filter(std::vector<IplImage*> images){
	IplImage *img = images[0];
    StorageObject *so = getStorageObject(std::string("cube"));
    if(so->data==0)
		so->data = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    IplImage *tmp = (IplImage*)so->data;
    cvZero(tmp);
	CvPoint pos = cvPoint(5, 5);
	StorageObject *so_time = getStorageObject(std::string("cube_t_start"));
	if(params[0]->getToggle()){
		
		if(so_time->data==0){
			float tmp_time = milliseconds();
			so_time->data = &tmp_time;
		}
		float start = *((float *)so_time->data);
		float now = milliseconds();
		
		
		float duration = now -start;
		

	}else{
		
		so_time->data = 0;
	}
    //if(img->nChannels>1)
    //    cvCvtColor(img, tmp, CV_RGB2GRAY);
    //cvThreshold(tmp, tmp, params[0]->getVal(), 255, CV_THRESH_TOZERO);
    return tmp;

}
MovingCubeFilter* MovingCubeFilter::clone(){
    MovingCubeFilter *mcf = new MovingCubeFilter();
    mcf->params.clear();
    for(int i=0;i<(int)params.size();i++)
        mcf->params.push_back(new Parameter(this->params[i]));
    return mcf;
}