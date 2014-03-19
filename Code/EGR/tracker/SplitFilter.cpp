#include "StdAfx.h"
#include "SplitFilter.h"

SplitFilter::SplitFilter(void)
{
   name="Split";
	sourceCount=1;
  params.push_back(new Parameter(1, 1, 4));
    
}
IplImage* SplitFilter::filter(std::vector<IplImage*> images){
  IplImage *src = images[0];
  StorageObject *so_split = getStorageObject(std::string("split"));
    if(so_split->data==0)
        so_split->data = cvCreateImage(cvGetSize(src), src->depth, 1);
    IplImage *split = (IplImage*)so_split->data;
	
    switch((int)params[0]->getVal()){
      case 1:
        cvSplit(src, split, NULL, NULL, NULL);
        break;
      case 2:
        cvSplit(src, NULL, split, NULL, NULL);
        break;
      case 3:
        cvSplit(src, NULL, NULL, split, NULL);
        break;
      case 4:
        cvSplit(src, NULL, NULL, NULL, split);
        break;
    }
	
    return split;
}
SplitFilter* SplitFilter::clone(){
    SplitFilter *sf = new SplitFilter();
    sf->params.clear();
    for(int i=0;i<params.size();i++)
        sf->params.push_back(new Parameter(this->params[i]));
    return sf;
}
