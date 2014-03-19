/* 
 * File:   Step.cpp
 * Author: tmf
 * 
 * Created on 21. Oktober 2009, 16:09
 */

#include "stdafx.h"
#include "Step.h"
#include <sstream>
#include "overlay.h"
#include "global.h"

Step::Step() {
    onscreen = new Overlay(this);
    flt =0;
	
}
Step::Step(int n) {
    onscreen = new Overlay(this);
    flt =0;
	number = n+1;
	//char *name = new char[32];
	std::stringstream str;
	str<<"Screen "<<number;
	std::string name(str.str());
	//sprintf(name, "Screen %d", number);
	
	strSourceList.push_back(name);
}

Step::Step(const Step& orig) {
}

Step::~Step() {
}

bool Step::process(bool showOverlay, int key){

	

	IplImage *img=0;
	std::vector<IplImage*> images;
	//char *so_result_name = new char[32];
	//sprintf(so_result_name, "Screen %d", number);
	std::stringstream str;
	str<<"Screen "<<number;
	std::string so_result_name(str.str());
	StorageObject *so_result = getStorageObject(so_result_name);
	if(sources.size()>0){
		for(int i=0;i<sources.size();i++){
			StorageObject *so_img = getStorageObject(sources[i]);
			img = (IplImage *)so_img->data;
			if(img==0){
				if(so_result->data==0)
					img = cvCreateImage(cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U, 4);
				else
					img = (IplImage*)so_result->data;
			}
			images.push_back(img);
		}
	}else{
		if(so_result->data==0)
					img = cvCreateImage(cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U, 4);
				else
					img = (IplImage*)so_result->data;
		images.push_back(img);
	}
	
    //TODO filter stuff
    IplImage *result = 0;
	
	//images.push_back(img);

	LONGLONG g_Frequency, g_CurentCount, g_LastCount;
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);
	

	if(flt!=0 && sources.size() >= flt->sourceCount )
        result = flt->filter(images);
    else
        result = img;

	QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);

	executionTime = 1000 * (((double)(g_LastCount-g_CurentCount))/((double)g_Frequency));
	

	so_result->data = result;

    if(showOverlay){
        onscreen->show(result, key);
    }
	return flt!=0;
}

void Step::setFilter(Filter* f){
    if(flt !=0){
        delete flt;
    }
    flt = f;
	if(flt!=0)
		flt->step = number;
}
std::vector<Parameter*> Step::getFilterParameters(){
    if(flt!=0){
        return this->flt->params;
    }else{
        std::vector<Parameter*> v;
        return v;
    }
}
std::vector<std::string> Step::getFilterInputs(){
	if(flt!=0){
		return this->flt->inputs;
	}else{
		std::vector<std::string> v;
		return v;
	}

}

