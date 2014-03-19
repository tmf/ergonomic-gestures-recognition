#include "stdafx.h"
#include "AddFilter.h"
#include "global.h"
#include "highgui.h"

AddFilter::AddFilter(void)
{
	name="Add Filter";
	sourceCount = 2;
}

AddFilter* AddFilter::clone(){
	AddFilter *af = new AddFilter();
    af->params.clear();
    for(int i=0;i<(int)params.size();i++)
        af->params.push_back(new Parameter(this->params[i]));
    return af;
}
AddFilter::~AddFilter(void)
{
}
IplImage* AddFilter::filter(std::vector<IplImage*> images){
	
	IplImage *result;
	if(images.size()==sourceCount){
		StorageObject *so_add = getStorageObject(std::string("add"));
		IplImage *tmp = images[0];
		if(so_add->data==0)
			so_add->data = cvCreateImage(cvGetSize(tmp), IPL_DEPTH_8U, tmp->nChannels);
		result = (IplImage *)so_add->data;
		EnterCriticalSection(&cs_img);
		cvOr(images[0], images[1], result);
		LeaveCriticalSection(&cs_img);
		
		 
	}else{
		result = images[0];
	}
	return result;
}
