/* 
 * File:   ThresholdFilter.cpp
 * Author: tmf
 * 
 * Created on 24. Oktober 2009, 14:20
 */
#include "stdafx.h"
#include "ThresholdFilter.h"

ThresholdFilter::ThresholdFilter(){
    name = "Threshold Filter";
	sourceCount=1;
    params.push_back(new Parameter(0, 255, 255));
    
}
IplImage* ThresholdFilter::filter(std::vector<IplImage*> images){
	IplImage *img = images[0];
    StorageObject *so = getStorageObject(std::string("grayimg"));
    if(so->data==0)
        so->data = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    IplImage *tmp = (IplImage*)so->data;
    
    if(img->nChannels>1){
        cvCvtColor(img, tmp, CV_RGB2GRAY);
        cvThreshold(tmp, tmp, params[0]->getVal(), 255, CV_THRESH_TOZERO);
    }else{
      cvThreshold(img, tmp, params[0]->getVal(), 255, CV_THRESH_TOZERO);
    }
    
    return tmp;

}
ThresholdFilter* ThresholdFilter::clone(){
    ThresholdFilter *tf = new ThresholdFilter();
    tf->params.clear();
    for(int i=0;i<params.size();i++)
        tf->params.push_back(new Parameter(this->params[i]));
    return tf;
}