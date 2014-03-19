/* 
 * File:   overlay.cpp
 * Author: tmf
 * 
 * Created on 21. Oktober 2009, 11:09
 */
#include "stdafx.h"
#include "overlay.h"
#include "global.h"
#include <stdio.h>

Overlay::Overlay(Step* step) {
    parent = step;
	selectedSourceItem = 0;
    selectedFilterItem = 0;
    selectedFilterParameter = 0;
    mode = mMenu;
    toggle = false;
	showOverlay=true;
}

void Overlay::show(IplImage* source, int key) {

    IplImage *img;
    if(source->nChannels>1)
        img = cvCloneImage(source);
    else{
        img = cvCreateImage(cvGetSize(source), IPL_DEPTH_8U, 3);
        cvCvtColor(source, img, CV_GRAY2RGB);
    }
	if(showOverlay){
		showScreenTitle(img);
		showFPS(img);
	}	
    int oldMode = mode;
	if(key=='s'){
		if(mode!=mScreen){
			mode=mScreen;
			selectedScreen = parent->number-1;
			prevMode=oldMode;
		}else{
			mode = prevMode;
		}
	}
	if(key=='o')
		showOverlay=!showOverlay;
	if (key == 13){
		switch(mode){
			case mMenu:
				mode=mSource;
				break;
			case mSource:
				parent->sources.push_back(strSourceList[selectedSourceItem]);
				if(parent->sources.size() >= filterList[selectedFilterItem]->sourceCount){
					mode=mBar;
				}
				break;
			case mBar:
				mode=mMenu;
				break;
			case mScreen:
				if(selectedScreen>=MAX_STEPS){
					Step *s = new Step(MAX_STEPS);
					screenSteps.push_back(s);
					MAX_STEPS++;
				}
				currentScreen = selectedScreen+1;
				mode = prevMode;
				break;
		}
		//	mode=mSource;
		
		//mode = mode == mMenu ? mSource : mode==mSource ? mBar : mMenu;
	}
	if (key == ' ')
        toggle = !toggle;
	if (mode == mScreen){
		std::vector<std::string> tmpScreenList;
		for(int s=0;s<MAX_STEPS;s++){
			std::stringstream str;
			str<<"Screen "<<s+1;
			std::string screenName(str.str());
			//char *screenName = new char[32];
			//sprintf(screenName, "Screen %d", s+1);
			tmpScreenList.push_back(screenName);
		}
		tmpScreenList.push_back("New Screen");

		if (key == KEY_DOWN)
            selectedScreen++;
        if (key == KEY_UP)
            selectedScreen--;

		showMenu(img, "Choose Screen:", tmpScreenList, selectedScreen);

	}else if (mode == mMenu) {
        //if(parent->getFilterParameters().size()>0)
            //save parameters?
        parent->setFilter(0);
		parent->sources.clear();
        if (key == KEY_DOWN)
            selectedFilterItem++;
        if (key == KEY_UP)
            selectedFilterItem--;

        selectedFilterItem = selectedFilterItem >= strFilterList.size() && selectedFilterItem >= 0 ? strFilterList.size() - 1 : selectedFilterItem;
        selectedFilterItem = selectedFilterItem < 0 ? 0 : selectedFilterItem;


        showMenu(img, "Choose Filter:", strFilterList, selectedFilterItem);
	} else if (mode==mSource){
		if (mode != oldMode) {
            parent->setFilter(filterList[selectedFilterItem]->clone());
            selectedSourceItem = 0;
        }
		if (key == KEY_DOWN)
            selectedSourceItem++;
        if (key == KEY_UP)
            selectedSourceItem--;

        selectedSourceItem = selectedSourceItem >= strSourceList.size() && selectedSourceItem >= 0 ? strSourceList.size() - 1 : selectedSourceItem;
        selectedSourceItem = selectedSourceItem < 0 ? 0 : selectedSourceItem;

		//char *menuTitle = new char[32];
		std::stringstream str;
		str<<"Choose Source "<<parent->sources.size()+1<<"/"<<filterList[selectedFilterItem]->sourceCount;
		std::string menuTitle(str.str());
		//sprintf(menuTitle, "Choose Source %d/%d:", parent->sources.size()+1, filterList[selectedFilterItem]->sourceCount);
		showMenu(img, menuTitle.c_str(), strSourceList, selectedSourceItem);
	} else if(mode==mBar) {
        if (mode != oldMode) {
			//parent->setSource(strSourceList[selectedSourceItem]);
            selectedFilterParameter=0;
        }
        if (key == KEY_UP)
            selectedFilterParameter++;
        if (key == KEY_DOWN)
            selectedFilterParameter--;

        selectedFilterParameter = selectedFilterParameter >= parent->getFilterParameters().size() && selectedFilterParameter >= 0 ? parent->getFilterParameters().size() - 1 : selectedFilterParameter;
        selectedFilterParameter = selectedFilterParameter < 0 ? 0 : selectedFilterParameter;

		if(parent->getFilterParameters().size()>0){
        if(key==KEY_LEFT || key==KEY_SHIFT_LEFT || key==KEY_RIGHT || key==KEY_SHIFT_RIGHT || key==' ' || key==KEY_UP || key==KEY_SHIFT_UP || key==KEY_DOWN || key==KEY_SHIFT_DOWN )
            updateParameter(parent->getFilterParameters()[selectedFilterParameter], key, toggle);
		}
		if(showOverlay)
			showSettings(img, parent->getFilterParameters(), selectedFilterParameter, toggle);
    }


	if(key=='f')
		saveScreenshot(img);
	if(key=='r'){
		recordingAVI = !recordingAVI;
		int fps = (int)1000.0f/(totalExecutionTime+10);
		if(fps!=0)
			prepareAVI(fps);
	}
	if(visible)
    cvShowImage("Calibration", img);
	if(recordingAVI){
		
		writeAVI(img);
		
	}
    cvReleaseImage(&img);

}

void Overlay::showScreenTitle(IplImage* img) {
    char *screenTitle = new char[64];
    sprintf(screenTitle, "Screen %d", parent->number);
    cvPutText(img, screenTitle, cvPoint(0, 50), &titleFont, green);
	if(parent->flt!=0)
		cvPutText(img, parent->flt->name.c_str(), cvPoint(150, 50), &menuFont, green);
    delete screenTitle;
}

void Overlay::showFPS(IplImage *img){
	char *fps_step = new char[64];
	sprintf(fps_step, "Step: %3.2f ms", parent->executionTime);
	/*float total = 0.0f;
	for(int i=0; i<screenSteps.size();i++)
		total+=screenSteps[i]->executionTime;*/
	char *fps_total = new char[64]; 
	sprintf(fps_total, "Total: %3.2f ms", totalExecutionTime);

	char *fps = new char[64]; 
	sprintf(fps, "FPS: %3.2f ", 1000.0f/totalExecutionTime);
	
	cvPutText(img, fps_step, cvPoint(img->width - 150, 20), &menuFont, green);
	cvPutText(img, fps_total, cvPoint(img->width - 150, 40), &menuFont, green);
	cvPutText(img, fps, cvPoint(img->width - 150, 60), &menuFont, green);
	delete fps_step;
    delete fps_total;
}

void Overlay::showMenu(IplImage* img, const char* title, std::vector<std::string> items, int active) {
    int itemHeight = 22;
    cvRectangle(img, cvPoint(70, 70), cvPoint(WIDTH-70, 70 + (items.size() + 1) * itemHeight), green);
    cvRectangle(img, cvPoint(70, 70), cvPoint(WIDTH-70, 70 + itemHeight), green, -1);
    cvPutText(img, title, cvPoint(70, 70 + itemHeight - 5), &menuFont, black);
    for (int i = 0; i < items.size(); i++) {
        if (i == active) {
            cvRectangle(img, cvPoint(70 + 2, 70 + 2 + (i + 1) * itemHeight), cvPoint(WIDTH-70 - 2, 70 - 3 + (i + 2) * itemHeight), green, 5, 4);
			cvPutText(img, items[i].c_str(), cvPoint(70 + 5, 70 - 2 + (i + 2) * itemHeight - 5), &menuFont, green);
        } else
			cvPutText(img, items[i].c_str(), cvPoint(70 + 5, 70 - 2 + (i + 2) * itemHeight - 5), &menuFont, green);
    }
}

void Overlay::showSettings(IplImage* img, std::vector<Parameter*> params, int active, bool x) {
    for (int i = 0; i < params.size(); i++) {
        switch (params[i]->getType()) {
            case pRange:
                showBar(img, params[i], img->height - 5 - (i + 1)*20, active == i);
                break;
            case pBandpass:
                showBandpass(img, params[i], img->height - 5 - (i + 1)*20, active == i, toggle);
                break;
            case pPoint:
                showPoint(img, params[i], active == i);
                break;
			case pToggle:
				showToggle(img, params[i], img->height -5 -(i+1)*20, active==i);
				break;
        }
    }
}

void Overlay::showBar(IplImage* img, Parameter* param, int y, bool active) {
    char *min = new char[6];
    char *max = new char[6];
    char *val = new char[6];
    sprintf(min, "%d", static_cast<int> (param->getMin()));
    sprintf(max, "%d", static_cast<int> (param->getMax()));
    sprintf(val, "%d", static_cast<int> (param->getVal()));
    cvRectangle(img, cvPoint(40, y + 2), cvPoint(img->width - 40, y + 20 - 2), green);
    cvPutText(img, min, cvPoint(2, y + 20 - 5), &menuFont, green);
    cvPutText(img, max, cvPoint(img->width - 40 + 2, y + 20 - 5), &menuFont, green);
    cvRectangle(img, cvPoint(1+40, 1+y + 2), cvPoint(-1+40 + (img->width - 2 * 40) * (param->getVal() - param->getMin()) / (param->getMax() - param->getMin()), -1+y + 20 - 2), green, active? -1:2);
    cvPutText(img, val, cvPoint(2+40 + (img->width - 2 * 40) * (param->getVal() - param->getMin()) / (param->getMax() - param->getMin()), y + 20 - 5), active ? &menuFontBold: &menuFont, green);
    delete min, max, val;
}

void Overlay::showBandpass(IplImage* img, Parameter* param, int y, bool active, bool x) {
    char *min = new char[6];
    char *max = new char[6];
    char *low = new char[6];
    char *high = new char[6];
    sprintf(min, "%d", static_cast<int> (param->getMin()));
    sprintf(max, "%d", static_cast<int> (param->getMax()));
    sprintf(low, "%d", static_cast<int> (param->getLow()));
    sprintf(high, "%d", static_cast<int> (param->getHigh()));
    cvRectangle(img, cvPoint(40, y + 2), cvPoint(img->width - 40, y + 20 - 2), green);
    cvPutText(img, min, cvPoint(2, y + 20 - 5), &menuFont, green);
    cvPutText(img, max, cvPoint(img->width - 40 + 2, y + 20 - 5), &menuFont, green);
    cvRectangle(img, cvPoint(1+40 + (img->width - 2 * 40) * (param->getLow() - param->getMin()) / (param->getMax() - param->getMin()), 1+y + 2), cvPoint(-1+40 + (img->width - 2 * 40) * (param->getHigh() - param->getMin()) / (param->getMax() - param->getMin()), -1+y + 20 - 2), green, active? -1:2);
    cvPutText(img, low, cvPoint(10 + (img->width - 2 * 40) * (param->getLow() - param->getMin()) / (param->getMax() - param->getMin()), y + 20 - 5), active && x?&menuFontBold:&menuFont, green);
    cvPutText(img, high, cvPoint(2+40 + (img->width - 2 * 40) * (param->getHigh() - param->getMin()) / (param->getMax() - param->getMin()), y + 20 - 5), active && !x?&menuFontBold: &menuFont, green);
    delete min, max, low, high;
}

void Overlay::showPoint(IplImage* img, Parameter* param, bool active) {

}
void Overlay::showToggle(IplImage *img, Parameter *param, int y, bool active){
	char *str = new char[6];
	if(param->getToggle()){
		sprintf(str, "On");
	}else{
		sprintf(str, "Off");
	}
	cvRectangle(img, cvPoint(100, y + 2), cvPoint(img->width - 100, y + 20 - 2), green, active? -1:2);
    cvPutText(img, str, cvPoint(img->width/2, y + 20 - 5), &menuFont, active?black:green);
	delete str;
}
int Overlay::getFilterParameterSize() {
    std::vector<Parameter*> params = parent->getFilterParameters();
    int count = params.size();
    for (int i = 0; i < params.size(); i++) {
        if (params[i]->getType() == pBandpass)
            count++;
    }
    return count;

}

void Overlay::updateParameter(Parameter* param, int key, bool x) {
    float val=0;
    switch (param->getType()) {
        case pBandpass:
            
            if(key==KEY_LEFT)//28
                val = -1;
            if(key==KEY_RIGHT)//29
                val=1;
			if(key==KEY_SHIFT_LEFT)//28
                val = -10;
            if(key==KEY_SHIFT_RIGHT)//29
                val=10;
            if(x){

                val = param->getLow() + val;
                if(val>param->getMax())
                    val= param->getMax();
                if(val<param->getMin())
                    val = param->getMin();
                param->set(val, param->getHigh());
            }else{
                val = param->getHigh() + val;
                param->set(param->getLow(), val);
            }
            break;
        case pRange:
            val = 0;
            if(key==KEY_LEFT)//28
                val = -1;
            if(key==KEY_RIGHT)//29
                val=1;
			if(key==KEY_SHIFT_LEFT)//28
                val = -10;
            if(key==KEY_SHIFT_RIGHT)//29
                val=10;
            if(val + param->getVal() > param->getMax())
                    val= param->getMax();
                if(val + param->getVal() <param->getMin())
                    val = param->getMin();
            param->set(val+param->getVal());
            break;
        case pPoint:
            break;
		case pToggle:
			if(key==' ' || key==KEY_LEFT || key==KEY_RIGHT)
				param->set(!param->getToggle());
			break;
    }
}

Overlay::Overlay() {
}

Overlay::Overlay(const Overlay& orig) {
}

Overlay::~Overlay() {
}