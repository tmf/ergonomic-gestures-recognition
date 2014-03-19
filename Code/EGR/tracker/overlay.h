/* 
 * File:   overlay.h
 * Author: tmf
 *
 * Created on 21. Oktober 2009, 11:09
 */

#ifndef _OVERLAY_H
#define	_OVERLAY_H

#include "Step.h"
#include "highgui.h"
#include <vector>
#include <string>
#include "parameter.h"
#include "filter.h"

enum overlayMode {mMenu, mSource, mBar, mScreen};

class Step;
class Overlay {
public:
    Overlay();
    Overlay(Step *step);
    Overlay(const Overlay& orig);
    virtual ~Overlay();
    void show(IplImage *source, int key);
	int mode;
private:
    Step *parent;
    void showScreenTitle(IplImage *img);
	void showFPS(IplImage* img);
	void showMenu(IplImage *img, const char* title, std::vector<std::string> items, int active);
    void showSettings(IplImage *img, std::vector<Parameter*> params, int active, bool x);
    void showBar(IplImage *img, Parameter* param, int y, bool active);
    void showBandpass(IplImage *img, Parameter* param, int y, bool activeLow, bool activeHigh);
    void showPoint(IplImage* img, Parameter* param, bool active);
	void showToggle(IplImage *img, Parameter *param, int y, bool active);

    
	int prevMode;
    bool toggle;
	bool showOverlay;
    int selectedFilterItem;
	int selectedSourceItem;
    int selectedFilterParameter;
	int selectedScreen;
    int getFilterParameterSize();
    void updateParameter(Parameter *param, int key, bool x);
};

#endif	/* _OVERLAY_H */

