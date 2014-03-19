#pragma once
#include "dlib/svm.h"

#include "EGRPosture.h"

using namespace std;
using namespace dlib;

typedef matrix<double, 10, 1> sample_type;
class EGRBrain
{
public:
	EGRBrain(void){}
	~EGRBrain(void){}
	void addPosture(EGRPosture p){ 
		postures.push_back(p);
	}
	void removePosture(int index){ 
		if(postures.size()>index){
			int i=0;
			std::vector<EGRPosture>::iterator p = postures.begin();
			for(;p!=postures.end(), i<=index; p++,i++);
			
			postures.erase(p);
		}
	}
	void clearPostures(){
		postures.clear();
	}
	std::vector<EGRPosture> &getPostures(){
		return postures;
	}
private:
	std::vector<EGRPosture> postures;
	std::vector<sample_type> samples;
	std::vector<string> labels;
	
	
};
