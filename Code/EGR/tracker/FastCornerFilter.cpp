#include "stdafx.h"
#include "FastCornerFilter.h"

FastCornerFilter::FastCornerFilter(void)
{
	name = "Fast Corner Filter";
	sourceCount=1;
    params.push_back(new Parameter(0.0f, 20.0f, 255.0f));
    params.push_back(new Parameter(9, 9, 12));
    params.push_back(new Parameter(10, 20, 200));
    params.push_back(new Parameter(5, 10, 200));

	
}

FastCornerFilter::~FastCornerFilter(void)
{
}

FastCornerFilter* FastCornerFilter::clone(){
	FastCornerFilter *fcf = new FastCornerFilter();
    fcf->params.clear();
    for(int i=0;i<(int)params.size();i++)
        fcf->params.push_back(new Parameter(this->params[i]));
    return fcf;
}
IplImage* FastCornerFilter::filter(std::vector<IplImage*> images){
	IplImage *img = 0;
	if(images.size()>0)
		img = images[0];
	
	char *strImg = new char[32];
	sprintf(strImg, "fastcorner_%d", step);
	StorageObject *so_thresh = getStorageObject(std::string(strImg));
    if(so_thresh->data==0)
		so_thresh->data = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage *thresh = (IplImage *)so_thresh->data;
	if(img->nChannels>1)
		cvCvtColor(img, thresh, CV_RGB2GRAY);
	else
		cvCopy(img, thresh);
	Parameter *pThreshold = params[0];
  Parameter *pN = params[1];
  Parameter *pTjump = params[2];
  Parameter *pTBlob = params[3];
  int tJump = pTjump->getVal();
  int tBlob = pTBlob->getVal();
  
std::vector<FCBlob *> blobs;
    FCBlob *b;
 
	if(pThreshold != 0 ){
		CvPoint* corners;
		int numCorners;

		
    cvCornerFast(thresh, pThreshold->getVal(),pN->getVal(), 0, &numCorners, & corners);
    if(numCorners>0){
      b = new FCBlob();
      b->n=0;
      b->x[0] = corners[0].x;
      b->y[0] = corners[0].y;
      b->center_x = corners[0].x;
      b->center_y = corners[0].y;
      blobs.push_back(b);
    }
		for(int i=1; i < numCorners; i++ ) 
		{
      
      int dx = corners[i].x - corners[i-1].x;
      int dy = corners[i].y - corners[i-1].y;
      b = blobs.at(blobs.size()-1);
      if(dist(dx, dy)> tJump){
        bool found = false;
        for(int j=0; j<blobs.size();j++){
          b = blobs.at(j);
          if((abs(b->center_x - corners[i].x)+abs(b->center_y - corners[i].y))<tBlob){
            found = true;
            //add to blob
            if(b->n<100){
              b->x[b->n] = corners[i].x;
              b->y[b->n] = corners[i].y;
              b->center_x = (b->n*b->center_x+corners[i].x)/(b->n+1);
              b->center_y = (b->n*b->center_y+corners[i].y)/(b->n+1);
              b->n = b->n+1;
            }
            break;
          }
        }
        if(!found){
          b = new FCBlob();
          b->n=0;
          b->x[0] = corners[i].x;
          b->y[0] = corners[i].y;
          b->center_x = corners[i].x;
          b->center_y = corners[i].y;
          blobs.push_back(b);
        }
      }else{
        //add to blob
        if(b->n<100){
          b->x[b->n] = corners[i].x;
          b->y[b->n] = corners[i].y;
          b->center_x = (b->n*b->center_x+corners[i].x)/(b->n+1);
          b->center_y = (b->n*b->center_y+corners[i].y)/(b->n+1);
          b->n = b->n+1;
        }
      }
			//cvLine(img, cvPoint(corners[i-1].x, corners[i-1].y), cvPoint(corners[i].x, corners[i].y), CV_RGB(255, 0, 0));
			
      
      
      cvLine( img, 
					cvPoint( corners[i].x-1, corners[i].y ), 
					cvPoint( corners[i].x+1, corners[i].y ), 
					CV_RGB(0,0,255) );
			cvLine( img, 
					cvPoint( corners[i].x, corners[i].y-1 ), 
					cvPoint( corners[i].x, corners[i].y+1 ), 
					CV_RGB(0,0,255) );
			

			/*for(int j=i; j<numCorners;j++){
				int dx = corners[i].x - corners[j].x;
				int dy = corners[i].y - corners[j].y;
				double hoi = dx*dx +dy*dy;
				double dist = sqrt(hoi);
			}*/
		}
    for(int i=0;i<blobs.size();i++){
      cvLine( img, 
					cvPoint( blobs[i]->center_x-1, blobs[i]->center_y ), 
					cvPoint( blobs[i]->center_x+1, blobs[i]->center_y ), 
					CV_RGB(255,0,0) );
			cvLine( img, 
					cvPoint( blobs[i]->center_x, blobs[i]->center_y-1 ), 
					cvPoint( blobs[i]->center_x, blobs[i]->center_y+1 ), 
					CV_RGB(255,0,0) );
    }
	}
	return img;

}

double FastCornerFilter::dist(int x1, int y1, int x2, int y2){
  return sqrt((double)((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}
double FastCornerFilter::dist(int dx, int dy){
  return sqrt((double)(dx*dx + dy*dy));
}