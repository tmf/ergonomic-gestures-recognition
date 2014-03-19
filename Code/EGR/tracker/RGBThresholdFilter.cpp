#include "stdafx.h"
#include "RGBThresholdFilter.h"

RGBThresholdFilter::RGBThresholdFilter(void)
{
	name = "RGB Threshold Filter";
	sourceCount=1;
    params.push_back(new Parameter(0.0f, 140.0f, 255.0f, 255.0f));
	params.push_back(new Parameter(0.0f, 0.0f, 120.0f, 255.0f));
	params.push_back(new Parameter(0.0f, 0.0f, 255.0f, 255.0f));
	params.push_back(new Parameter(1.0f, 1.0f, 3.0f));
	/*
	green l 128-143 a 104-123 b 114-137
	blue l 68-126 a 121-132 b 102-112
	*/
}
IplImage* RGBThresholdFilter::filter(std::vector<IplImage*> images){
    IplImage *img = images[0];

	CvSize normalSize = cvGetSize(img);
	/*int n=1;
	for(int i=1; i<params[3]->getVal(); i++)
		n = 2*n;

	CvSize smallSize = cvSize((int)(normalSize.width / n), (int)(normalSize.height/n));*/

	char *strRGBmask = new char[32];
	sprintf(strRGBmask, "rgbmask_%d", step);
	char *strRGB = new char[32];
	sprintf(strRGB, "rgb_%d", step);
	StorageObject *so_mask = getStorageObject(std::string("rgbmask"));
	StorageObject *so_rgb = getStorageObject(std::string(strRGB));
    if(so_rgb->data==0)
		so_rgb->data = cvCreateImage(normalSize, IPL_DEPTH_8U, img->nChannels);
    IplImage *tmp_rgb = (IplImage*)so_rgb->data;
	if(so_mask->data==0)
		so_mask->data = cvCreateImage(normalSize, IPL_DEPTH_8U, 3);
	//small size but recheck if size is supposed to be different from previous in memory
    IplImage *tmp_mask = (IplImage*)so_mask->data;
	/*CvSize tmpSize = cvGetSize(tmp_mask);
	if(tmpSize.width != smallSize.width){
		cvReleaseImage(&tmp_mask);
		so_mask->data = cvCreateImage(smallSize, IPL_DEPTH_8U, 3);
		tmp_mask = (IplImage*)so_mask->data;
	}

    IplImage *tmp_mask_big = cvCreateImage(normalSize, IPL_DEPTH_8U, 3);*/
    //if(img->nChannels>1)
        //cvCvtColor(img, tmp, CV_RGB2GRAY);

	if(img->nChannels==4)
		cvCvtColor(img, tmp_mask, CV_BGRA2BGR);
	else
		cvCopy(img, tmp_mask);
	/*
	if(img->nChannels==4)
		cvCvtColor(img, tmp_mask_big, CV_BGRA2BGR);
	else
		cvCopy(img, tmp_mask_big);
*/
	//downsize
	//cvResize(tmp_mask_big, tmp_mask);

	//cvCvtColor(tmp_mask, tmp_mask, CV_BGR2Lab);
	fast_threshold(tmp_mask); //masks are stored in c1,c2,c3 (downsized)
	mask_threshold(img, tmp_rgb);
		//cvCvtColor(lab, frame, CV_Lab2BGR);
		//cvShowImage("left", frame);

    //cvThreshold(tmp, tmp, params[0]->getVal(), 255, CV_THRESH_TOZERO);
	cvReleaseImage(&tmp_mask);
	so_mask->data = 0;

	//cvReleaseImage(&tmp_mask_big);
    return tmp_rgb;

}
RGBThresholdFilter* RGBThresholdFilter::clone(){
    RGBThresholdFilter *rtf = new RGBThresholdFilter();
    rtf->params.clear();
    for(int i=0;i<(int)params.size();i++)
        rtf->params.push_back(new Parameter(this->params[i]));
    return rtf;
}
void RGBThresholdFilter::fast_threshold(IplImage *src){
	StorageObject *so_rgb_c1 = getStorageObject(std::string("rgb_c1"));
    if(so_rgb_c1->data==0)
        so_rgb_c1->data = cvCreateImage(cvGetSize(src), src->depth, 1);
    IplImage *c1 = (IplImage*)so_rgb_c1->data;
	StorageObject *so_rgb_c2 = getStorageObject(std::string("rgb_c2"));
    if(so_rgb_c2->data==0)
        so_rgb_c2->data = cvCreateImage(cvGetSize(src), src->depth, 1);
    IplImage *c2 = (IplImage*)so_rgb_c2->data;
	StorageObject *so_rgb_c3 = getStorageObject(std::string("rgb_c3"));
    if(so_rgb_c3->data==0)
        so_rgb_c3->data = cvCreateImage(cvGetSize(src), src->depth, 1);
    IplImage *c3 = (IplImage*)so_rgb_c3->data;
	cvSplit(src, c1, c2, c3, NULL);
	Parameter *p1 =NULL;
	Parameter *p2 =NULL;
	Parameter *p3 =NULL;
	p1 = this->params[0];
	p2 = this->params[1];
	p3 = this->params[2];
	int tid;
	#pragma omp parallel private(tid) num_threads(3)
	{
		tid = omp_get_thread_num();
		switch(tid){
			case 0:
				if(p1!=NULL){
					int high, low;
					high = (int)(p1->getHigh() > p1->getLow() ? p1->getHigh() : p1->getLow());
					low = (int)(p1->getHigh() < p1->getLow() ? p1->getHigh() : p1->getLow());

					cvThreshold(c1, c1, high, 255, CV_THRESH_TOZERO_INV);
					cvThreshold(c1, c1, low, 255, CV_THRESH_TOZERO);
				}
				break;
			case 1:
				if(p2!=NULL){
					int high, low;
					high = (int)(p2->getHigh() > p2->getLow() ? p2->getHigh() : p2->getLow());
					low = (int)(p2->getHigh() < p2->getLow() ? p2->getHigh() : p2->getLow());

					cvThreshold(c2, c2, high, 255, CV_THRESH_TOZERO_INV);
					cvThreshold(c2, c2, low, 255, CV_THRESH_TOZERO);
				}
				break;
			case 2:
				if(p3!=NULL){
					int high, low;
					high = (int)(p3->getHigh() > p3->getLow() ? p3->getHigh() : p3->getLow());
					low = (int)(p3->getHigh() < p3->getLow() ? p3->getHigh() : p3->getLow());

					cvThreshold(c3, c3, high, 255, CV_THRESH_TOZERO_INV);
					cvThreshold(c3, c3, low, 128, CV_THRESH_TOZERO);

				}
				break;
		}
	}
	


}
void RGBThresholdFilter::mask_threshold(IplImage *src, IplImage *dst){
	IplImage *tmp1, *tmp2;
	StorageObject *so_rgb_c1 = getStorageObject(std::string("rgb_c1"));
    IplImage *c1 = (IplImage*)so_rgb_c1->data;
	StorageObject *so_rgb_c2 = getStorageObject(std::string("rgb_c2"));
    IplImage *c2 = (IplImage*)so_rgb_c2->data;
	StorageObject *so_rgb_c3 = getStorageObject(std::string("rgb_c3"));
    IplImage *c3 = (IplImage*)so_rgb_c3->data;
	

	
	tmp1 = cvCreateImage(cvSize(c1->width, c1->height), src->depth, 1);
	//tmp2 = cvCreateImage(cvSize(dst->width, dst->height), src->depth, 1);
	cvZero(tmp1);
	cvZero(dst);
	
				
	cvMin(c1, c2, c2);
	cvMin(c2, c3, tmp1);
	
	//cvResize(tmp1, tmp2);
	cvCopy(src, dst, tmp1);
	
	cvReleaseImage(&tmp1);
	//cvReleaseImage(&tmp2);
	cvReleaseImage(&c1);
	so_rgb_c1->data = 0;
	cvReleaseImage(&c2);
	so_rgb_c2->data = 0;
	cvReleaseImage(&c3);
	so_rgb_c3->data = 0;
	

}