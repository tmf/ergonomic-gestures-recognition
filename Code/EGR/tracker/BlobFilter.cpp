#include "stdafx.h"
#include "BlobFilter.h"
#include "BlobResult.h"
#include "global.h"

BlobFilter::BlobFilter(void)
{
	name = "Blob Filter";
	sourceCount=1;
	params.push_back(new Parameter(0.0f, 100.0f, 255.0f));
	params.push_back(new Parameter(0.0f, 1.0f, 255.0f));
}

BlobFilter::~BlobFilter(void)
{
}
BlobFilter* BlobFilter::clone(){
	BlobFilter *bf = new BlobFilter();
	bf->blobs.ClearBlobs();
	bf->params.clear();
	for(int i=0;i<(int)params.size();i++)
		bf->params.push_back(new Parameter(this->params[i]));
	return bf;
}
IplImage* BlobFilter::filter(std::vector<IplImage*> images){
	IplImage *img = 0;
	if(images.size()>0)
		img = images[0];

	char *strThreshold = new char[32];
	sprintf(strThreshold, "blobthreshold_%d", step);
	StorageObject *so_thresh = getStorageObject(std::string(strThreshold));
	if(so_thresh->data==0)
		so_thresh->data = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage *thresh = (IplImage *)so_thresh->data;
	if(img->nChannels>1)
		cvCvtColor(img, thresh, CV_RGB2GRAY);
	Parameter *pThreshold = params[1];
	Parameter *pBlobsize = params[0];
	if(pThreshold != 0 && pBlobsize != 0){
		cvThreshold(thresh, thresh, pThreshold->getVal(), 255, CV_THRESH_BINARY);
		//cvErode(thresh, thresh);
		//cvDilate(thresh, thresh);

		// find blobs in image
		blobs = CBlobResult( thresh, NULL, 0 );
		//blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS,  200000.0f);
		blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS,  pBlobsize->getVal());

		CBlob *currentBlob;
		int n = blobs.GetNumBlobs();

		char *strBlobs = new char[32];
		sprintf(strBlobs, "blobs_%d", step);
		StorageObject *so_blobs = getStorageObject(std::string(strBlobs));
		if(so_blobs->data==0)
			so_blobs->data = new std::list<Point>();
		std::list<Point> *blobs_list = (std::list<Point>*)so_blobs->data;
		EnterCriticalSection(&cs_v);
		blobs_list->clear();
		for (int i = 0; i < blobs.GetNumBlobs(); i++ )
		{
			currentBlob = blobs.GetBlob(i);
			
			currentBlob->FillBlob( img, CV_RGB(255,0,255));
			Point p;
			CvRect bb = currentBlob->GetBoundingBox();
			p.x = bb.x+bb.width/2;
			p.y = bb.y+bb.height/2;
			
			blobs_list->push_back(p);
			
			
		}
		LeaveCriticalSection(&cs_v);
		
	}
	return img;
}