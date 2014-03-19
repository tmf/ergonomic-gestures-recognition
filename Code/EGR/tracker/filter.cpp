/* 
 * File:   Filter.cpp
 * Author: tmf
 * 
 * Created on 21. Oktober 2009, 16:24
 */
#include "stdafx.h"
#include "Filter.h"

Filter::Filter() {
}

Filter::Filter(const Filter& orig) {
    params.clear();
    for (int i = 0; i < (int)orig.params.size(); i++) {
        params.push_back(new Parameter(*orig.params[i]));
    }
}

Filter::~Filter() {
}
