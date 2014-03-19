/* 
 * File:   EmptyFilter.cpp
 * Author: tmf
 * 
 * Created on 24. Oktober 2009, 14:36
 */
#include "stdafx.h"
#include "EmptyFilter.h"

EmptyFilter::EmptyFilter(){
    name="Empty Filter";
	sourceCount=1;
}
IplImage* EmptyFilter::filter(std::vector<IplImage*> images){
    return images[0];
}
EmptyFilter* EmptyFilter::clone(){
    EmptyFilter *ef = new EmptyFilter();
    return ef;
}