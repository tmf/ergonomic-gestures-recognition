#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "EGRHand.h"
#include "Socket.h"
#include "tracker.h"
#include "srt2label.h"
#include <exception>
#include "Permutation.h"
#include "linreg.h"
#include "EGRBrain.h"


using namespace std;

int SERVER_PORT = 1234;
string SERVER_NAME = "localhost";
int CMD_LOG = 255;
int DATA_list = 10;


class Hand : public EGRHand {
public:
	Hand();
	void init(string trackerConfig, string fingerBlob, string wristBlob);
	void log(string l);
	EGRPoint getWristPosition();
	list<EGRPoint > getFingerPositions();
	bool start();
	void startTracking();
	void startRecognition();
	void stop();
	void stopTracking();
	void stopRecognition();
	void pause(bool pauseTracker);
	void frameForward();
	void updateWristPosition(Point p, int frame);
	void updateFingerPosition(Point p, int frame);
	void adjust();

	void setMode(int mode);
	void setAVISource(string avi);
	void setFinishedCallback(void (*callback)(void));

	IplImage* getTrackerImage();
	IplImage* getTrackerImage(string screen);
	string currentLabel();
	static void  _thread(void *arg);
	void _loop();

	int currentGesture;

	void drawFingers(IplImage *img, float scale);
	float getFPS();

	void livePause(bool hand);
	std::vector<EGRPosture> &getPostures(){return brain.getPostures();}
	void addPosture(EGRPosture p){ brain.addPosture(p); }
	void removePosture(int index){ brain.removePosture(index); }
	void clearPostures(){ brain.clearPostures(); }
	void setClassifier(classifier &c){ df = c; setMode(MODE_CLASSIFY); }
	classifier getClassifier(){ return df; }
	string getClassifiedPosture();
	int getMode(){return mode;}
	void trackerConfiguration(bool show){ if(show){EGRTrackerShow();}else{EGRTrackerHide();} }
private:
	EGRBrain brain;

	classifier df;
	SocketClient *s;
	list<EGRPoint> wrist;
	list<list<EGRPoint>> fingers;
	EGRPoint unadjustedWristPosition;
	list<EGRPoint> unadjustedFingersPosition;

	EGRPoint center;
	bool flipped;

	bool running;
	bool tracking;
	bool recognising;
	bool initialised;
	string trackerConfigurationFile;
	string gartOptionsFile;
	string gartLibraryFile;
	string aviFile;
	string fingerBlobScreen;
	string wristBlobScreen;

	EGRCallback *callback;
	int mode;

	int currentFrame;
	int skippedFrames;
	float fps;

	bool handModelPause;
	bool livePaused;
	bool handTick;

	bool oneFrame;
	//bool compare_angle(EGRPoint p1, EGRPoint p2);
	std::vector<string> labels;

	uintptr_t thread;
	IplImage *alphaBuffer;
	void springWrist(list<EGRPoint> *l);
	void springFingers(list<list<EGRPoint>> *l);
	void spring(EGRPoint *p1, EGRPoint *p2, float D, float friction);
	static std::vector<std::vector<int>> permutations(int a, int b);

	static float dist(EGRPoint p1, EGRPoint p2);
	static float dist_permutation(std::vector<EGRPoint> f1, std::vector<EGRPoint> f2, int *permutation);
	static int *choose_minimal_permutation(std::vector<EGRPoint> f1, std::vector<EGRPoint> f2);
	static list<EGRPoint> apply_minimal_permutation(list<EGRPoint> f1, list<EGRPoint> f2);

	
};

Hand::Hand(){
	s=0;
	handModelPause = false;
	livePaused = false;
	fps=0.0;
	initialised = false;
	mode=MODE_CLASSIFY;
	flipped = false;
	skippedFrames = 0;
	this->alphaBuffer = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 4);
	this->center.x = center.y = center.vx = center.vy = 0;
}


void Hand::init(string trackerConfig, string fingerBlob, string wristBlob){
	try{
		if(!this->initialised){


			this->initialised = true;
			this->trackerConfigurationFile = trackerConfig;
			this->fingerBlobScreen = fingerBlob;
			this->wristBlobScreen = wristBlob;
			EGRTrackerLoadConfiguration(this->trackerConfigurationFile);

			//s = new SocketClient(SERVER_NAME, SERVER_PORT);
		}
	}catch(...){
		s = 0;
		this->initialised = false;
	}
}
void Hand::log(string l){
	if(s!=0){
		s->SendByte(CMD_LOG);
		s->SendLine(l);
	}
}

string Hand::getClassifiedPosture(){
	string result = "";
	if(mode==MODE_CLASSIFY){
		list<EGRPoint> f = getFingerPositions();
		int estimatedFingers = 0;
		for(list<EGRPoint>::iterator i=f.begin(); i!=f.end(); i++){
			if((*i).estimated==true){
				estimatedFingers++;
			}
		}
		if(f.size()==MAX_FINGERS && estimatedFingers<MAX_ESTIMATED_FINGERS){
			EGRPoint c;
			c.x = f.front().centerx;
			c.y = f.front().centery;
			EGRPosture p(f, c, getWristPosition(), "");
			
			matrix<int, 2, 1> mwrist = p.getOrigin();
			int ox, oy;
			ox = mwrist(0);
			oy = mwrist(1);

			matrix<int, 2, 1> mcenter = p.getCenter();
			int cx, cy;
			cx = mcenter(0);
			cy = mcenter(1);

			matrix<double, 10,1> mpoints = p.getPoints();
			for(int i =0; i<MAX_FINGERS; i++){
				int px, py;
				mpoints(2*i) = mpoints(2*i)-ox;
				mpoints(2*i+1) = mpoints(2*i+1)-oy;
			}
			result = df(mpoints);
		}
	}
	return result;
}
EGRPoint Hand::getWristPosition(){
	if(wrist.size()>0){
		return wrist.back();
	}else{
		return EGRPoint();
	}
}
list<EGRPoint > Hand::getFingerPositions(){
	if(fingers.size()>0)
		return fingers.back();
	else
		return list<EGRPoint>();
}
bool Hand::start(){
	if(this->initialised){
		this->running = true;
		this->tracking = true;
		this->recognising = false;
		thread = _beginthread(Hand::_thread,0,  reinterpret_cast<void *>(this));
		return true;
	}else{
		return false;
	}



}

void Hand::setAVISource(string avi){
	this->aviFile = avi;
	EGRTrackerSetSource(avi, "camera_65", true);
	this->labels = srt2label(avi);
}
void Hand::startTracking(){


}

float Hand::getFPS(){
	return this->fps;
}
void Hand::startRecognition(){
	this->recognising = true;
}
void Hand::stop(){
	this->running = false;
	this->tracking = false;
	this->recognising = false;
}
void Hand::stopTracking(){
	this->tracking = false;
}
void Hand::stopRecognition(){
	this->recognising = false;
}

void Hand::updateWristPosition(Point p, int frame){


	this->unadjustedWristPosition.x = p.x;
	this->unadjustedWristPosition.y = p.y;
	this->unadjustedWristPosition.frame = frame;
	this->unadjustedWristPosition.vx = 0;
	this->unadjustedWristPosition.vy = 0;
	this->unadjustedWristPosition.lastRealFrame = frame;
	this->unadjustedWristPosition.estimated = false;


}
void Hand::updateFingerPosition(Point p, int frame){

	EGRPoint f;
	f.x = p.x;
	f.y = p.y;
	f.frame = frame;
	f.vx = 0;
	f.vy = 0;
	f.lastRealFrame = 0;
	unadjustedFingersPosition.push_back(f);
}
float Hand::dist(EGRPoint p1, EGRPoint p2){
	return sqrt((float)((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y)));
}
void Hand::springFingers(list<list<EGRPoint>> *l){
}
void Hand::springWrist(list<EGRPoint> *l){
	list<EGRPoint>::iterator p1 = l->end();
	list<EGRPoint>::iterator p2 = l->end();
	p1--;p1--;
	p2--;
	this->spring(&(*p1), &(*p2), 0.5, 0.5);
	/*float D = 0.5f;
	float friction = 0.5f;

	int dx = p2->x - p1->x;
	int dy = p2->y - p1->y;
	p2->vx = ((float)dx*D + (float)p1->vx)*friction;
	p2->vy = ((float)dy*D + (float)p1->vy)*friction;
	p2->x = p1->x + p2->vx;
	p2->y = p1->y + p2->vy;*/

}
void Hand::spring(EGRPoint *p1, EGRPoint *p2, float D, float friction){
	


	int dx = p2->x - p1->x;
	int dy = p2->y - p1->y;
	p2->vx = ((float)dx*D + (float)p1->vx)*friction;
	p2->vy = ((float)dy*D + (float)p1->vy)*friction;
	p2->x = p1->x + p2->vx;
	p2->y = p1->y + p2->vy;
}
float Hand::dist_permutation(std::vector<EGRPoint> f1, std::vector<EGRPoint> f2, int *permutation){
	float sum=0.0f;
	int max_size = f1.size()>f2.size() ? f1.size() : f2.size();
	int min_size = f1.size()>f2.size() ? f2.size() : f1.size();
	bool invert = f2.size() > f1.size();
	int p=0;
	for(int i=0; i<min_size; i++){
		p = permutation[i];
		if(invert){
			sum+=dist(f1[i], f2[p]);
		}else{
			sum+=dist(f1[p], f2[i]);
		}
	}
	return sum;
}
int *Hand::choose_minimal_permutation(std::vector<EGRPoint> f1, std::vector<EGRPoint> f2){
	int s1 = f1.size();
	int s2 = f2.size();
	int min = s1<s2?s1:s2;
	int *min_permutation = Permutation::first_permutation(s1, s2);
	int *permutation = Permutation::first_permutation(s1, s2);
	float min_dist = 10000;
	float d=0.0;
	while(Permutation::is_valid_permutation(s1, s2, permutation)){
		d=dist_permutation(f1, f2, permutation);
		if(d<min_dist){
			min_dist = d;
			Permutation::copy_permutation(permutation, min_permutation, min);
		}
		Permutation::next_permutation(s1, s2, permutation);
	}
	return min_permutation;
}
list<EGRPoint> Hand::apply_minimal_permutation(std::list<EGRPoint> lf1, std::list<EGRPoint> lf2){
	std::vector<EGRPoint> f1(lf1.begin(), lf1.end());
	std::vector<EGRPoint> f2(lf2.begin(), lf2.end());

	int s1 = f1.size(); 
	int s2 = f2.size();
	int min = s1<s2?s1:s2;
	int *chosen_permutation = choose_minimal_permutation(f1, f2);


	std::vector<EGRPoint> newfingers(s1);
	for(int i=0; i<s1; i++){

		newfingers[i].estimated = true;
	}
	for(int i=0; i<min; i++){
		int index = chosen_permutation[i];
		newfingers[index] = f2[i];
		newfingers[index].estimated = false;
	}

	list<EGRPoint> result(newfingers.begin(), newfingers.end());
	return result;
}
bool compare_angle_asc(EGRPoint p1, EGRPoint p2){
	if(p1.estimated){
		return false;
	}
	return p1.angle < p2.angle;
}
bool compare_angle_desc(EGRPoint p1, EGRPoint p2){
	if(p1.estimated){
		return false;
	}
	return p1.angle > p2.angle;
}
void Hand::adjust(){

	EGRPoint newWrist;
	EGRPoint newCenter;
	if(unadjustedWristPosition.frame>=0){
		newWrist.x = this->unadjustedWristPosition.x;
		newWrist.y = this->unadjustedWristPosition.y;
		newWrist.frame = this->unadjustedWristPosition.frame;
		newWrist.vx = 0;
		newWrist.vy = 0;


		this->wrist.push_back(newWrist);
		if(wrist.size()>1){
			this->springWrist(&wrist);
		}
	}


	int newFingerVectorSize = getFingerPositions().size();
	if(this->unadjustedFingersPosition.size()>getFingerPositions().size())
		newFingerVectorSize = this->unadjustedFingersPosition.size();
	if(newFingerVectorSize>MAX_FINGERS){
		newFingerVectorSize=MAX_FINGERS;
	}
	list<EGRPoint> newFingers(newFingerVectorSize);
	float dist_longest_finger = 0.0;
	int longest_finger =0, i=0;
	if(newFingerVectorSize>0){
		/*
		vector<EGRPoint> vNewFingers(newFingerVectorSize);
		list<EGRPoint> lOldFingers = this->getFingerPositions();
		vector<EGRPoint> vOldFingers(lOldFingers.begin(), lOldFingers.end());
		int k=0;
		for(list<EGRPoint>::iterator i = this->unadjustedFingersPosition.begin(); i!=this->unadjustedFingersPosition.end(); i++){
		float min_dist=10000.0;
		int min_index = -1;
		for(int j=0; j<vOldFingers.size(); j++){
		float d = Hand::dist(vOldFingers[j], (*i));
		if(d<min_dist){
		min_dist = d;
		min_index = j;
		}
		}
		if(min_index>=0){
		vNewFingers[min_index].x = (*i).x;
		vNewFingers[min_index].y = (*i).y;
		vNewFingers[min_index].frame = (*i).frame;
		vNewFingers[min_index].estimated = false;
		vNewFingers[min_index].lastRealFrame = (*i).frame;
		}else{
		vNewFingers[k].x = (*i).x;
		vNewFingers[k].y = (*i).y;
		vNewFingers[k].frame = (*i).frame;
		vNewFingers[k].estimated = false;
		vNewFingers[k].lastRealFrame = (*i).frame;
		k++;
		}
		}
		list<EGRPoint> lNewFingers(vNewFingers.begin(), vNewFingers.end());
		this->fingers.push_back(lNewFingers);*/

		//works somewhat, not preserving order 


		list<EGRPoint>::iterator f2 = newFingers.begin();
		newCenter.x = newCenter.y  = newCenter.vx = newCenter.vy = 0;
		for(list<EGRPoint>::iterator f = this->unadjustedFingersPosition.begin(); f!= this->unadjustedFingersPosition.end() && f2!=newFingers.end(); f++){
			f2->x = f->x;
			f2->y = f->y;
			f2->frame = f->frame;
			f2->estimated = false;
			f2->lastRealFrame = f->frame;

			newCenter.x+=f2->x;
			newCenter.y += f2->y;
			if(dist(*f, this->getWristPosition())>dist_longest_finger){
				dist_longest_finger = dist(*f, this->getWristPosition());
				longest_finger = i;
			}
			i++;
			f2++;

		}
		//if(wrist.size()>0){
			//int dWristX = getWristPosition().x - newWrist.x;
			//int dWristY = getWristPosition().y - newWrist.y;

			//list<EGRPoint> lOldFingers = this->getFingerPositions();
			//vector<EGRPoint> vOldFingers(lOldFingers.begin(), lOldFingers.end());
			for(;f2!=newFingers.end();f2++){
				f2->estimated = true;
				//if(i<vOldFingers.size()){
					//f2->x = vOldFingers[i].x + dWristX;
					//f2->y = vOldFingers[i].y + dWristY;

				//}
			}
		//}

		/*
		if(this->getFingerPositions().size()>0 && getFingerPositions().size()>=unadjustedFingersPosition.size()){
		fingers.push_back(apply_minimal_permutation(this->getFingerPositions(), this->unadjustedFingersPosition));
		}else{
		list<EGRPoint> newFingers(newFingerVectorSize);
		list<EGRPoint>::iterator f2 = newFingers.begin();
		for(list<EGRPoint>::iterator f = this->unadjustedFingersPosition.begin(); f!= this->unadjustedFingersPosition.end() && f2!=newFingers.end(); f++){
		f2->x = f->x;
		f2->y = f->y;
		f2->frame = f->frame;
		f2->estimated = false;
		f2->lastRealFrame = f->frame;
		f2++;
		}
		for(;f2!=newFingers.end();f2++){
		f2->estimated = true;
		}
		fingers.push_back(newFingers);
		}*/

	}

	if(newFingerVectorSize>0){
		int weight = 3;
		newCenter.x += weight * this->getWristPosition().x;
		newCenter.y += weight * this->getWristPosition().y;
		if(this->unadjustedFingersPosition.size() >0){
			int n = this->unadjustedFingersPosition.size();
			n = n > MAX_FINGERS ? MAX_FINGERS : n;
			newCenter.x /= (weight+n);
			newCenter.y /= (weight+n);
		}
		if(this->unadjustedFingersPosition.size()>0){
			spring(&(this->center), &newCenter, 0.75, 0.5);
		}
		center.x = newCenter.x;
		center.y = newCenter.y;
		center.vx = newCenter.vx;
		center.vy = newCenter.vy;
		float angle_longest_finger = 0.0;
		float angle_wrist_longest_finger = 0.0;
		i=0;
		bool found_flipped = true;
		for(list<EGRPoint>::iterator f = newFingers.begin(); f!= newFingers.end(); f++, i++){
			if(f->estimated==false){
				f->centerx = center.x;
				f->centery = center.y;
				int dx = f->x - center.x;
				int dy = f->y - center.y;
				f->angle = -atan2((double)dy, (double)dx);
				if(f->angle<0){
					f->angle = 2*M_PI+f->angle;
				}
				if(i==longest_finger){
					angle_longest_finger = f->angle;


				}
				dx = f->x - getWristPosition().x;
				dy = f->y  - getWristPosition().y;
				f->angle2 = -atan2((double)dy, (double)dx);
			}
		}
		this->flipped = found_flipped;
		for(list<EGRPoint>::iterator f = newFingers.begin(); f!= newFingers.end(); f++){
			f->angle -= angle_longest_finger;
			f->angle += 0.001;
			if(f->angle<0){
				f->angle = 2*M_PI+f->angle;
			}
		}
		newFingers.sort(compare_angle_asc);

		EGRPoint fictionalFinger;
		fictionalFinger.estimated = true;
		int cf=0;
		std::vector<EGRPoint> vnf(newFingers.begin(), newFingers.end());
		for(int k=1; k<(newFingers.size()-1); k++){
			if(!vnf[k].estimated && !vnf[k+1].estimated){
				fictionalFinger.x += vnf[k].x;
				fictionalFinger.y += vnf[k].y;
				fictionalFinger.centerx = vnf[k].centerx;
				fictionalFinger.centery = vnf[k].centery;
				cf++;
			}
		}
		if(cf>0){
			fictionalFinger.x/=cf;
			fictionalFinger.y/=cf;
			fictionalFinger.estimated = true;
		}
		for(int k=1; k<(vnf.size()); k++){
			if(vnf[k].estimated){
				vnf[k] = fictionalFinger;
			}
		}
		list<EGRPoint> completeFingers(vnf.begin(), vnf.end());
		/*if(fingers.size()>2){
			list<list<EGRPoint>>::iterator lLastFingers = fingers.end();
			list<list<EGRPoint>>::iterator lLastFingers2 = fingers.end();
			lLastFingers--;
			lLastFingers2--;lLastFingers2--;
			vector<EGRPoint> lastFingers(lLastFingers->begin(), lLastFingers->end());
			vector<EGRPoint> lastFingers2(lLastFingers2->begin(), lLastFingers2->end());
			int k=0;
			for(list<EGRPoint>::iterator nf = newFingers.begin(); nf!=newFingers.end(); nf++){
				if(nf->estimated && k<lastFingers.size() && k<lastFingers2.size()){
					int dx = lastFingers[k].x - lastFingers2[k].x;
					int dy = lastFingers[k].y - lastFingers2[k].y;
					nf->x = lastFingers[k].x + dx;
					nf->y = lastFingers[k].y + dy;

				}
				k++;
			}
		}*/
		/*
		list<EGRPoint>::iterator thumb;
		float d_angle = 0.0;
		float max_angle = 0.0;
		float thumb_angle;
		for(list<EGRPoint>::iterator f = newFingers.begin(); f!= newFingers.end(); f++){
		list<EGRPoint>::iterator next = f;
		next++;
		if(next==newFingers.end()){
		next = newFingers.begin();
		}
		d_angle = next->angle - f->angle;
		if(d_angle<0){
		d_angle += 2*M_PI;
		}
		if(d_angle>max_angle){
		max_angle = d_angle;
		thumb = next;
		thumb_angle = next->angle;
		}
		}
		if(this->unadjustedFingersPosition.size()>0){
		for(list<EGRPoint>::iterator f = newFingers.begin(); f!= newFingers.end(); f++){

		f->angle -= thumb_angle;
		f->angle += 0.01;
		if(f->angle<0){
		f->angle = 2*M_PI+f->angle;
		}
		}
		}
		if(flipped){
		newFingers.sort(compare_angle_desc);
		}else{
		newFingers.sort(compare_angle_asc);
		}*/
		fingers.push_back(completeFingers);
	}
	if(this->fingers.size() >= HISTORY_SIZE){
		this->fingers.pop_front();
		this->wrist.pop_front();
	}
	unadjustedFingersPosition.clear();


}

void Hand::setMode(int _mode){
	this->mode = _mode;
	
}

string Hand::currentLabel(){
	long frameNumber = EGRTrackerGetFrameNumber();
	if(this->labels.size()>(frameNumber-skippedFrames)){
		return labels.at(frameNumber-skippedFrames);
	}else{
		return string("");

	}
}
void Hand::frameForward(){
	EGRTrackerPause();
	this->oneFrame = true;
	this->tracking = false;


}
void Hand::livePause(bool hand){
	if(livePaused){
		EGRTrackerLiveUnPause();
	}else{
		EGRTrackerLivePause();
	}
	livePaused=!livePaused;
	if(hand){
		handModelPause = livePaused;
		handTick = livePaused;
	}
}
void Hand::pause(bool pauseTracker){
	this->tracking = !this->tracking;
	if(pauseTracker){
		EGRTrackerPause();
	}


}


void Hand::drawFingers(IplImage *img, float scale){
	if(alphaBuffer->width!=img->width){
		cvReleaseImage(&alphaBuffer);
		this->alphaBuffer = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 4);
	}
	CvPoint pWrist;
	int fingerColor=0;
	char *fingerIndex = new char[2];
	fingerIndex[0]=49;
	fingerIndex[1]='\0';
	char *strBuffer = new char[128];
	CvFont font;
	//cvSet(img, CV_RGB(0, 0, 0));
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
	pWrist.x = getWristPosition().x*scale;
	pWrist.y = getWristPosition().y*scale ;
	cvSet(this->alphaBuffer, CV_RGB(0,0,0));
	cvCircle(this->alphaBuffer, pWrist, 2, CV_RGB(75, 230, 67), -1);
	list<EGRPoint> lstFingers = getFingerPositions();
	float avgx=0.0, avgy=0.0;
	int count=0;
	int l = 0;
	float d = 0.0;
	int j=0;
	int index_lastfinger = 0;
	LinearRegression lr;
	int estimatedFingers = 0;
	for(list<EGRPoint>::iterator i=lstFingers.begin(); i!=lstFingers.end(); i++){
		if((*i).estimated==true){
			estimatedFingers++;
		}
	}
	if(estimatedFingers < MAX_ESTIMATED_FINGERS){
	for(list<EGRPoint>::iterator f = lstFingers.begin(); f!=lstFingers.end(); f++){
		j++;
		if(f->estimated==false){
			index_lastfinger = j-1;

			CvPoint pFinger;

			pFinger.x = f->x*scale;
			pFinger.y = f->y*scale;

			//if(j>1&&j<lstFingers.size()){
				cvCircle(alphaBuffer, pFinger, 3, CV_RGB(255, 255, 0),-1);
				lr.addXY(pFinger.x, pFinger.y);
			//}
			count++;
			avgx+=pFinger.x;
			avgy+=pFinger.y;

			//cvLine(alphaBuffer, pWrist, pFinger, CV_RGB(fingerColor, 230, fingerColor), 2);
			if(dist(*f, getWristPosition())>d){
				d = dist(*f, getWristPosition());
				l=j;
			}
			//cvPutText(alphaBuffer, fingerIndex, pFinger, &font, CV_RGB(255, 255, 255));
		}else{
			CvPoint pFinger;

			pFinger.x = f->x*scale;
			pFinger.y = f->y*scale;
			pWrist.x = f->centerx * scale;
			pWrist.y = f->centery * scale;
			cvLine(alphaBuffer, pWrist, pFinger, CV_RGB(0, 0, 255));
			cvCircle(alphaBuffer, pFinger, 4, CV_RGB(0, 255, 255),-1);
		}
		fingerColor += 50;

		//fingerIndex[0]++;
	}
	CvPoint pf;
	list<EGRPoint>::iterator ff = lstFingers.begin();

	for(int i=0; i<l; i++,ff++);
	if(lstFingers.size()>1 && ff!=lstFingers.begin()){
		ff--;
		pf.x = ff->x*scale;
		pf.y = ff->y*scale;
		//cvLine(alphaBuffer, pWrist, pf, CV_RGB(0, 0, 255), 2);
	}
	if(lstFingers.size()>1){
		ff = lstFingers.end();
		for(ff--;ff!=lstFingers.begin(); ff--){
			if(ff->estimated==false){
				break;
			}
		}
		
		pf.x = ff->x*scale;
		pf.y = ff->y*scale;
		//cvLine(alphaBuffer, pWrist, pf, CV_RGB(255, 0, 255), 2);
	}
	avgx+=(count*pWrist.x);
	avgy+=(count*pWrist.y);
	count+=count;
	avgx/=count;
	avgy/=count;
	pWrist.x = avgx;
	pWrist.y = avgy;
	int i=0;
	for(list<EGRPoint>::iterator f = lstFingers.begin(); f!=lstFingers.end(); f++,i++){
		if(f->estimated==false){


			CvPoint pFinger;

			pFinger.x = f->x*scale;
			pFinger.y = f->y*scale;
			pWrist.x = f->centerx * scale;
			pWrist.y = f->centery *scale;
			sprintf(strBuffer, "%c", (char)(i+65));
			cvLine(alphaBuffer, pWrist, pFinger, CV_RGB(255, 0, 0),1);
			if(f->x<f->centerx){
				pFinger.x = f->x*scale-20;
			}else{
				pFinger.x = f->x*scale+5;
			}
			if(i==0){
				sprintf(strBuffer, "Index");
				cvPutText(alphaBuffer, strBuffer, pFinger, &font, CV_RGB(255, 255, 0));
			}
			if(i==index_lastfinger){
				sprintf(strBuffer, "Thumb");
				cvPutText(alphaBuffer, strBuffer, pFinger, &font, CV_RGB(255, 255, 0));
			}
		}
		fingerColor += 50;

		fingerIndex[0]++;
	}
	cvCircle(this->alphaBuffer, pWrist,3, CV_RGB(255, 0, 0), -1);
	if(lstFingers.size() > 0 ){
		list<EGRPoint>::iterator finger = lstFingers.begin();
		
		CvPoint a,b;
		a.x = 0;
		a.y = lr.getA();
		b.x = 320;
		b.y = lr.getB() * 320;
		//cvLine(this->alphaBuffer, a, b, CV_RGB(255, 255, 0), 2);
	}
	}
	cvAddWeighted(img, 0.4, this->alphaBuffer, 0.6, 0, img);
	

}
void Hand::_loop(){
	this->running = true;
	EGRTrackerStart();
	//EGRTrackerShow();
	while(this->running){
		if(this->tracking || this->oneFrame){
			list<Point> *blobs;
			if(this->tracking){
				long frameNumber = EGRTrackerGetFrameNumber();
				bool foundStuff=false;
				if(frameNumber>currentFrame || frameNumber ==1){
					fps = EGRTrackerGetFPS();
					this->currentFrame = frameNumber;
					if(!this->handModelPause || this->handTick){
						if(this->handModelPause){
							this->handTick= false;
						}
						blobs = EGRTrackerGetList(this->fingerBlobScreen);

						if(blobs!=0 && blobs->size()>0){

							foundStuff = true;
							for(list<Point>::iterator i=blobs->begin(); i!=blobs->end(); i++){

								this->updateFingerPosition(*i, frameNumber);

							}
						}
						blobs = EGRTrackerGetList(this->wristBlobScreen);
						if(blobs!=0 && blobs->size()>0){
							foundStuff = true;
							for(list<Point>::iterator i=blobs->begin(); i!=blobs->end(); i++){

								this->updateWristPosition(*i, frameNumber);

							}
						}
						if(foundStuff){
							this->adjust(); 
						}
					}else{
						skippedFrames++;
					}
					//Send to GART (Java over Socket)
					//s->SendByte(DATA_list);
					//s->SendByte(12);
					if(mode==MODE_TRAIN){
						EGRPoint c;
						list<EGRPoint> f = getFingerPositions();
						if(f.size()==MAX_FINGERS){
							c.x = f.front().centerx;
							c.y = f.front().centery;
							EGRPosture p(f, c, getWristPosition(), currentLabel());
							brain.addPosture(p);
						}
					}
					
					if(!this->tracking && this->oneFrame){
						EGRTrackerPause();
						this->oneFrame = false;
					}
				}


			}
		}
		Sleep(30L);
	}
	//EGRTrackerHide();
	EGRTrackerStop();
	if(s!=0)
		s->Close();
	s=0; 

	this->running = false;
	this->tracking = false;
	this->recognising = false;

	_endthread();

}
void Hand::_thread(void *arg){
	Hand* instance = reinterpret_cast<Hand*>(arg); 
	instance->_loop();
}

void Hand::setFinishedCallback(void (*callback)(void)){
	EGRTrackerSetFinishedCallback(callback);
}
IplImage* Hand::getTrackerImage(){
	return  EGRTrackerGetImage("Screen 8");
}
IplImage* Hand::getTrackerImage(string screen){
	return  EGRTrackerGetImage(screen);
}

#pragma comment(linker, "/export:createEGRHand=_createEGRHand@0")

DLL EGRHand* APIENTRY createEGRHand()
{
	return new Hand;
}