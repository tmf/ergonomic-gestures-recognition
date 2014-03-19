


#include "trainer.h"
#include <libconfig.h++>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPixmap>
#include <math.h>
#include "egr_dlib.h"
#include "dlib/svm.h"

extern void __cdecl EGRTrackerFinished(void);
using namespace libconfig;
using namespace std;
using namespace dlib;
trainer::trainer(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	hand = createEGRHand();
	timer.setSingleShot(false);
	buffer = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 4);

	connect(ui.btnAddAVI, SIGNAL(clicked()), this, SLOT(addAVI()));
	connect(ui.btnRemoveAVI, SIGNAL(clicked()), this, SLOT(removeAVI()));
	connect(ui.btnCollectSamplesAVI, SIGNAL(clicked()), this, SLOT(collect()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(frame()));
	connect(ui.btnLiveTest, SIGNAL(clicked()), this, SLOT(live()));

	connect(ui.btnLoadLibrary, SIGNAL(clicked()), this, SLOT(loadLibrary()));
	connect(ui.btnSaveLibrary, SIGNAL(clicked()), this, SLOT(saveLibrary()));
	connect(ui.btnPauseAVI, SIGNAL(clicked()), this, SLOT(pause()));
	connect(ui.btnTrain, SIGNAL(clicked()), this, SLOT(train()));
	connect(ui.cmbSampleFilter, SIGNAL(activated(int)), this, SLOT(filterSamples()));
	connect(ui.sldSampleZoom, SIGNAL(sliderReleased()), this, SLOT(displaySamples()));
	connect(ui.chkLabelFilter, SIGNAL(stateChanged(int)), this, SLOT(displaySamples()));
	connect(ui.chkUseCenter, SIGNAL(stateChanged(int)), this, SLOT(displaySamples()));
	connect(ui.btnRemoveLabel, SIGNAL(clicked()), this, SLOT(removeLabel()));
	connect(ui.btnAddLabel, SIGNAL(clicked()), this, SLOT(addLabel()));
	connect(ui.btnNextSample, SIGNAL(clicked()), this, SLOT(selectNextSample()));
	connect(ui.btnPreviousSample, SIGNAL(clicked()), this, SLOT(selectPreviousSample()));
	connect(ui.btnRemoveSample, SIGNAL(clicked()), this, SLOT(removeSample()));
	connect(ui.lstLabels, SIGNAL(	itemClicked ( QListWidgetItem *  )), this, SLOT(changeSampleLabel()));
	//connect(ui.btnEstimate, SIGNAL(clicked()), this, SLOT(estimate()));
	//connect(ui.btnTrain, SIGNAL(clicked()), this, SLOT(train()));
	connect(ui.btnLoadDecisionFunction, SIGNAL(clicked()), this, SLOT(loadDecisionFunction()));
	connect(ui.btnSaveDecisionFunction, SIGNAL(clicked()), this, SLOT(saveDecisionFunction()));

	currentAVIIndex = 0;
	
	hand->setMode(MODE_TRAIN);
	ui.lblSamplesGrid->installEventFilter( this );


}
void trainer::loadDecisionFunction(){
	QString file = QFileDialog::getOpenFileName(this, tr("Select trained classifier..."), tr("c:\\egr"), tr("EGR Decision function (*.egrdf)"));
	classifier df;
	ifstream fin(file.toStdString().c_str(), ios::binary);
    deserialize(df, fin);
	hand->setClassifier(df);
}
void trainer::saveDecisionFunction(){
	QString file = QFileDialog::getSaveFileName(this, tr("Select destination..."), tr("c:\\egr"), tr("EGR Decision function (*.egrdf)"));
	classifier df = hand->getClassifier();
	ofstream fout(file.toStdString().c_str(), ios::binary);
	serialize(df, fout);
	fout.close();

}
void trainer::changeSampleLabel(){
	QList<QListWidgetItem *> selitems = ui.lstLabels->selectedItems();
	if(selitems.count()>0){
		QString label = selitems.at(0)->text();
		std::vector<EGRPosture> &postures = hand->getPostures();
		if(selectedSampleIndex>=0 && selectedSampleIndex<postures.size()){
			postures[selectedSampleIndex].setLabel(label.toStdString());
		}
	}
}
void trainer::keyPressEvent( QKeyEvent *e ){
	qDebug() << e->type();
}
void trainer::displaySamples(){
	displayLibrary(hand->getPostures());
}
void trainer::selectNextSample(){
	std::vector<EGRPosture> &postures = hand->getPostures();
	int index=0;
	if(selectedSampleIndex>=0){
		if(ui.chkLabelFilter->isChecked()){
			string label = ui.cmbSampleFilter->currentText().toStdString();
			for(int i=selectedSampleIndex+1; i<postures.size(); i++){
				if(postures[i].getLabel().compare(label)==0){
					break;
				}
				
			}
			index = selectedSampleIndexFiltered+1;
		}else{
			index =  (selectedSampleIndex+1)<postures.size() ? selectedSampleIndex+1 : postures.size();
		}
	}
	selectSample(index);
}
void trainer::selectPreviousSample(){
	std::vector<EGRPosture> &postures = hand->getPostures();
	int index=0;
	if(selectedSampleIndex>=0){
		if(ui.chkLabelFilter->isChecked()){
			string label = ui.cmbSampleFilter->currentText().toStdString();
			for(int i=selectedSampleIndex-1; i>=0; i--){
				if(postures[i].getLabel().compare(label)==0){
					break;
				}
				
			}
			index=selectedSampleIndexFiltered-1;
			
		}else{
			index  = (selectedSampleIndex-1)<postures.size() ? selectedSampleIndex-1 : postures.size();
		}
	}
	selectSample(index);
}
void trainer::displayPosture(EGRPosture &posture, QPixmap &pxm, int x, int y, int w, bool highlight, bool clear){
	int b = 5;
	float scale = (float)w/320.0;
	matrix<int, 2, 1> mwrist = posture.getOrigin();
	int ox, oy;
	ox = mwrist(0);
	oy = mwrist(1);

	matrix<int, 2, 1> mcenter = posture.getCenter();
	int cx, cy;
	cx = mcenter(0);
	cy = mcenter(1);

	QPainter p(&pxm);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine, Qt::RoundCap));
	//p.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
	if(clear){
		p.fillRect(x, y, w, w, QColor(Qt::white));
		p.drawLine(x+(w/2)+b, y+b, x+(w/2)+b, y+w-b);
	
		if(ui.chkUseCenter->isChecked()){
			p.drawLine(x+b, y+w-(oy-cy)*scale-b-1, x+w, y+w-(oy-cy)*scale-b-1);
		}else{
			p.drawLine(x+b, y+w-b-1, x+w-b, y+w-b-1);
		}
	}
	
	matrix<double, 10, 1> mpoints = posture.getPoints();
	for(int i =0; i<MAX_FINGERS; i++){
		int px, py;
		px=mpoints(i*2);
		py=mpoints(i*2+1);
		p.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine, Qt::RoundCap));
		if(clear)
		p.drawLine(x+b+w/2+(cx-ox)*scale, y-b+w-(oy-cy)*scale, x+b+w/2+(px-ox)*scale, y-b+w-(oy-py)*scale);
		p.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
		if(clear)
			p.drawEllipse(x+b+w/2+(px-ox)*scale - 1, y-b+w-(oy-py)*scale -1 , 3, 3);
		else
			p.drawEllipse(x+b+w/2+(px-ox)*scale , y-b+w-(oy-py)*scale  , 1, 1);
	}
	if(highlight){
		p.setPen(QPen(QColor(170, 200, 255), b, Qt::SolidLine, Qt::RoundCap));
		p.drawRoundedRect(x+ceil((double)b/2), y+ceil((double)b/2), w-b-1, w-b-1, b, b);
	}
}
void trainer::estimate(){

}
void trainer::train(){
	std::vector<EGRPosture> &postures = hand->getPostures();
	std::vector<sample_fingers_type> samples;
	std::vector<string> labels;
	for(int i=0; i<postures.size(); i++){
		matrix<int, 2, 1> mwrist = postures[i].getOrigin();
		int ox, oy;
		ox = mwrist(0);
		oy = mwrist(1);

		matrix<int, 2, 1> mcenter = postures[i].getCenter();
		int cx, cy;
		cx = mcenter(0);
		cy = mcenter(1);

		matrix<double, 10,1> mpoints = postures[i].getPoints();
		for(int i =0; i<MAX_FINGERS; i++){
			int px, py;
			mpoints(2*i) = mpoints(2*i)-ox;
			mpoints(2*i+1) = mpoints(2*i+1)-oy;
		}
		samples.push_back(mpoints);
		labels.push_back(postures[i].getLabel());
	}
	/*
	ui.log->setText( QString::fromStdString(dlib_train(samples, labels)));
	ui.log->adjustSize();
	ui.scrollAreaWidgetContents_2->setMinimumSize(ui.log->size());*/
	classifier c = dlib_train(samples, labels);
	int incorrect = 0;
	for(int i=0;i<samples.size(); i++){
		if(labels[i].compare(string(c(samples[i])))!=0){
			incorrect++;
		}
	}
	stringstream sstr;
	sstr<<test_multiclass_decision_function(c, samples, labels);
	ui.log->setText(QString::fromStdString(sstr.str()));
	ui.accuracy->setValue((int)((float)(100*(samples.size()-incorrect)/samples.size())));
	hand->setClassifier(c);
	ui.btnLiveTest->setEnabled(true);
}
void trainer::live(){
	ui.btnPauseAVI->setEnabled(true);

	finished=false;
	hand->init("c:\\egr\\tracker.cfg", "blobs_5", "blobs_6");
	
	bool x = hand->start();
	
	timer.start(35);


	
}
void trainer::pause(){
	hand->livePause(true);
}
void trainer::frameForward(){
	hand->frameForward();
}
void trainer::frame(){
	if(finished){
		ui.btnCollectSamplesAVI->setEnabled(true);
		ui.btnAddAVI->setEnabled(true);
		ui.btnLiveTest->setEnabled(true);
		ui.btnPauseAVI->setEnabled(false);
		ui.btnRemoveAVI->setEnabled(true);
		timer.stop();
		finished=false;
		this->extractAllLabels(hand->getPostures());
	}else{
		IplImage *tracker_frame = (IplImage *) hand->getTrackerImage("Screen 8");
		if(hand->getMode()==MODE_TRAIN){
			ui.label->setText(QString(hand->currentLabel().c_str()));
		}else{
			ui.label->setText(QString::fromStdString(hand->getClassifiedPosture()));
		}
		if(tracker_frame!=0 && (currentAVIIndex<ui.lstAVI->count() || hand->getMode()==MODE_CLASSIFY)){
			ui.fps->setText(QString::number(hand->getFPS()));
			cvResize(tracker_frame, buffer);
			hand->drawFingers(buffer,(float) buffer->width/tracker_frame->width);
			Mat xy = Mat(buffer);
			ui.videoWidget->sendImage(&xy);
		}
	}
}
void trainer::addAVI(){
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select AVI"), tr("c:\\egr"), tr("Avi Files (*.avi)"));

	ui.lstAVI->addItems(files);
	if(ui.lstAVI->count()>0){
		ui.btnCollectSamplesAVI->setEnabled(true);
	}else{
		ui.btnCollectSamplesAVI->setEnabled(false);
	}

}
void trainer::removeAVI(){
	qDeleteAll(ui.lstAVI->selectedItems());
	if(ui.lstAVI->count()>0){
		ui.btnCollectSamplesAVI->setEnabled(true);
	}else{
		ui.btnCollectSamplesAVI->setEnabled(false);
	}

}
void trainer::collect(){
	if(currentAVIIndex==0){
		ui.btnCollectSamplesAVI->setEnabled(false);
		ui.btnAddAVI->setEnabled(false);
		ui.btnLiveTest->setEnabled(false);
		ui.btnPauseAVI->setEnabled(true);
		ui.btnRemoveAVI->setEnabled(false);
		hand->init("c:\\egr\\tracker.cfg", "blobs_5", "blobs_6");
		finished=false;
		timer.start(35);

	}

	if(currentAVIIndex<ui.lstAVI->count()){



		//connect(&timer, SIGNAL(timeout()), this, SLOT(frame()));

		hand->setFinishedCallback(EGRTrackerFinished);




		QString file = ui.lstAVI->item(currentAVIIndex)->text();
		hand->setAVISource(file.toStdString());
		ui.Avi->setText(file);


	}	
	if(currentAVIIndex == 0){
		hand->start();
	}else{
		hand->pause(true);
	}
}
void trainer::callback(void){
	//timer.stop();
	hand->pause(false);

	currentAVIIndex++;
	if(currentAVIIndex>=ui.lstAVI->count()){

		currentAVIIndex = 0;
		hand->stop();
		finished=true;
	}else{
		this->collect();
	}
}
void trainer::extractAllLabels(std::vector<EGRPosture> &postures){
	for(int i=0; i<postures.size(); i++){
		bool found = false;
		for(int j=0; j<ui.cmbSampleFilter->count(); j++){
			if(ui.cmbSampleFilter->itemText(j).toStdString().compare(postures[i].getLabel())==0){
				found = true;
			}
		}
		if(!found){
			ui.cmbSampleFilter->addItem(QString(postures[i].getLabel().c_str()));
			ui.lstLabels->addItem(QString(postures[i].getLabel().c_str()));
		}
	}
}
void trainer::addLabel(){
	bool ok;
	QString l = QInputDialog::getText(this, tr("EGR sample supervision"),tr("New label:"), QLineEdit::Normal, tr(""), &ok);
	if(ok && !l.isEmpty()){
		ui.cmbSampleFilter->addItem(l);
		ui.lstLabels->addItem(l);
	}
}
void trainer::removeLabel(){
	QList<QListWidgetItem *> selitems = ui.lstLabels->selectedItems();
	if(selitems.count()>0){
		QString label = selitems.at(0)->text();
		if(!label.isEmpty()){
			std::vector<EGRPosture> &postures = hand->getPostures();
			int count=0;
			for(int i=0; i<postures.size(); i++){
				if(postures[i].getLabel().compare(label.toStdString())==0){
					count++;
				}
			}
			bool confirmation=true;
			if(count>0){
				//get confirmation
				QMessageBox::warning(this, tr("EGR sample supervision"), tr("Are you sure? "), QMessageBox::Yes | QMessageBox::No);
			}
			if(confirmation){
				//delete label from lstlabels and cmbSelectfilter
			}
		}else{
			qDebug() << "attempted to remove empty label";
		}
	}
	
}
void trainer::selectSample(int index){
	int w = ui.sldSampleZoom->value();
	int scrbw = 20;
	int nx = (int)floor((double)((ui.scroll->width()-scrbw)/w));
	
	
	int oldSelectedSampleIndex = selectedSampleIndex;
	int oldSelectedSampleIndexFiltered = selectedSampleIndexFiltered;
	std::vector<EGRPosture> &postures = hand->getPostures();
	int nPostures=-1;
	bool useFilter = ui.chkLabelFilter->isChecked();
	if(useFilter){
		string filter = ui.cmbSampleFilter->currentText().toStdString();
		for(int i=0; i<postures.size(); i++){
			if(postures[i].getLabel().compare(filter)==0){
				nPostures++;
			}
			if(nPostures==index){
				selectedSampleIndex = i;
				selectedSampleIndexFiltered = index;
				
			}
		}
	}else{
		selectedSampleIndex = index;
		selectedSampleIndexFiltered = index;
	}
	const QPixmap *lpxm = ui.lblSamplesGrid->pixmap();
	QPixmap pxm(*lpxm);
	if(oldSelectedSampleIndexFiltered>=0){
		displayPosture(postures[oldSelectedSampleIndex], pxm, (oldSelectedSampleIndexFiltered%nx)*w, (int)floor((double)oldSelectedSampleIndexFiltered/nx) * w, w, false, true);
	}
	if(selectedSampleIndex < postures.size()){
		displayPosture(postures[selectedSampleIndex], pxm, (index%nx)*w, (int)floor((double)index/nx) * w, w, true, true);
	}
	ui.lblSamplesGrid->setPixmap(pxm);
	if(useFilter){
		ui.lblSample->setText(QString("Sample %1 / %2").arg(selectedSampleIndexFiltered).arg( nPostures));
	}else{
		QString s = QString("Sample %1 / %2").arg(selectedSampleIndex).arg( (int)postures.size());
		qDebug() << s;
		ui.lblSample->setText(s);
	}
	
	for(int i=0;i<ui.lstLabels->count(); i++){
		ui.lstLabels->setItemSelected(ui.lstLabels->item(i), false);
		if(ui.lstLabels->item(i)->text().toStdString().compare(postures[selectedSampleIndex].getLabel())==0){
			ui.lstLabels->setItemSelected(ui.lstLabels->item(i), true);
		}
	}
}
bool trainer::eventFilter( QObject* watched, QEvent* event ) {
	
	if ( event->type() != QEvent::MouseButtonPress  && event->type() != QEvent::KeyPress)
        return false;
	if ( watched != ui.lblSamplesGrid)
        return false;
    const QMouseEvent* const me = static_cast<const QMouseEvent*>( event );
    //might want to check the buttons here
	
    const QPoint p = me->pos(); //...or ->globalPos();
	
	int w = ui.sldSampleZoom->value();
	int scrbw = 20;
	int nx = (int)floor((double)((ui.scroll->width()-scrbw)/w));;
	int filteredIndex = (int)(floor((double)p.x()/w)) + nx*(int)(floor((double)p.y()/w));
	selectSample(filteredIndex);

    return false;
}
void trainer::removeSample(){
	std::vector<EGRPosture> &postures = hand->getPostures();
	if(selectedSampleIndex >=0 && selectedSampleIndex < postures.size()){
		std::vector<EGRPosture>::iterator it = postures.begin() + selectedSampleIndex;
		postures.erase(it);
	}
	displayLibrary(hand->getPostures());
}
void trainer::displayLibrary(std::vector<EGRPosture> &postures){
	string filter = ui.cmbSampleFilter->currentText().toStdString();

	selectedSampleIndexFiltered = -1;
	selectedSampleIndex = -1;

	int width=0, height=0, nPostures=0, nx=0, ny=0, w=0, scrbw=20;
	bool useFilter = ui.chkLabelFilter->isChecked();
	if(useFilter){
		for(int i=0; i<postures.size(); i++){
			if(postures[i].getLabel().compare(filter)==0){
				nPostures++;
			}
		}
	}else{
		nPostures = postures.size();
	}
	width = ui.scroll->width()-scrbw;
	w = ui.sldSampleZoom->value();
	nx = (int)floor((double)(width/w));
	ny = (int)ceil((double)(nPostures/nx));
	ui.lblSamplesGrid->setFixedHeight((ny+1)*w);
	ui.lblSamplesGrid->setFixedWidth(nx*w);

	QPixmap p(nx*w, (ny+1)*w);
	QPixmap pc(160, 160);
	if(true){
	QPainter pw(&pc);
	pw.fillRect(0, 0, 160, 160, QColor(Qt::white));
	}
	int x=0, y=0;
	for(int i=0; i<postures.size(); i++){
		if(useFilter){
			if(postures[i].getLabel().compare(filter)==0){
				displayPosture(postures.at(i), p, x, y, w, false, true);
				displayPosture(postures.at(i), pc, 0, 0, 160, false, false);
				x+=w;
				if(x>=nx*w){
					y+=w;
					x=0;
				}
			}
		}else{
			displayPosture(postures.at(i), p, x, y, w, false, true);
			x+=w;
			if(x>=nx*w){
				y+=w;
				x=0;
			}
		}
		
	}
	ui.lblSamplesGrid->setPixmap(p);
	ui.lblSamplesGrid->resize(p.size());
	ui.scrollAreaWidgetContents->setMinimumSize(p.size());

	if(useFilter){
		ui.lblCumulative->setPixmap(pc);
	}
	
	/*
	for(int i=0; i<postures.size(); i++){
		if(postures[i].getLabel().compare(label)==0){
			QListWidgetItem *sample = new QListWidgetItem(ui.lstSamples);
			sample->setText(tr(label.c_str()));
			QPixmap pxm(128, 128);
			QPainter p(&pxm);
			p.fillRect(0, 0, 128, 128, QColor(Qt::white));
			p.drawLine(64, 0, 64, 128);
			p.drawLine(0, 127, 128, 127);
			matrix<int, 2, 1> mwrist = postures.at(i).getOrigin();
			int ox, oy;
			ox = mwrist(0);
			oy = mwrist(1);
			matrix<int, 2, 1> mcenter = postures.at(i).getCenter();
			int cx, cy;
			cx = mcenter(0);
			cy = mcenter(1);

			matrix<int, 2, 5> mpoints = postures.at(i).getPoints();
			for(int i =0; i<MAX_FINGERS; i++){
				int x, y;
				x=mpoints(0,i);
				y=mpoints(1,i);
				p.drawLine(64+(cx-ox)/2, 128-(oy-cy)/2, 64+(x-ox)/2, 128-(oy-y)/2);
			}
			sample->setIcon(QIcon(pxm));
		}
	}
	ui.lstSamples->setIconSize(QSize(128, 128));*/
}
void trainer::saveLibrary(std::vector<EGRPosture> &postures, std::string file){
	Config cfg;
	try{
		Setting &root = cfg.getRoot();
		Setting &lib = root.add("postures", Setting::TypeList);

		
		for(int j=0; j<postures.size(); j++){
			Setting &posture = lib.add(Setting::TypeGroup);
			posture.add("label", Setting::TypeString) = postures.at(j).getLabel();
			matrix<double, 10, 1> mpoints = postures.at(j).getPoints();
			
			Setting &fingers = posture.add("fingers", Setting::TypeList);
			for(int i =0; i<MAX_FINGERS; i++){
				Setting &f = fingers.add(Setting::TypeArray);
				float x = mpoints(i*2);
				float y = mpoints(i*2+1);
				f.add(Setting::TypeInt) = (int)x;
				f.add(Setting::TypeInt) = (int)y;
			}
			
			matrix<int, 2, 1> mcenter = postures.at(j).getCenter();
			Setting &center = posture.add("center", Setting::TypeArray);
			center.add(Setting::TypeInt) = mcenter(0);
			center.add(Setting::TypeInt) = mcenter(1);

			matrix<int, 2, 1> mwrist = postures.at(j).getOrigin();
			Setting &wrist = posture.add("wrist", Setting::TypeArray);
			wrist.add(Setting::TypeInt) = mwrist(0);
			wrist.add(Setting::TypeInt) = mwrist(1);
		}
		cfg.writeFile(file.c_str());
	}
	catch(const SettingException &sex){return ;}
	catch(const FileIOException &fioex){return ;}
	catch(const ParseException &pex){return ; }
}

void trainer::loadLibrary(std::vector<EGRPosture> &postures,std::string file){
	Config cfg;
	try
	{
		cfg.readFile(file.c_str());
	
		Setting &lib = cfg.getRoot()["postures"];
		for(int i=0; i<lib.getLength(); i++){
			Setting &posture = lib[i];
			list<EGRPoint> fingers;
			EGRPoint center;
			EGRPoint wrist;
			string label;
			posture.lookupValue("label", label);
			center.x = posture["center"][0];
			center.y = posture["center"][1];
			wrist.x = posture["wrist"][0];
			wrist.y = posture["wrist"][1];
			for(int j=0; j<posture["fingers"].getLength(); j++){
				EGRPoint f;
				f.x = (int)posture["fingers"][j][0];
				f.y = (int)posture["fingers"][j][1];
				fingers.push_back(f);
			}
			EGRPosture p(fingers, center, wrist, label);
			postures.push_back(p);
		}

	}
	catch(const SettingException &sex){return ;}
	catch(const FileIOException &fioex){return ;}
	catch(const ParseException &pex){return ; }
	

}
void trainer::filterSamples(){
	displayLibrary(hand->getPostures());
}
void trainer::saveLibrary(){
	QString file = QFileDialog::getSaveFileName(this, tr("Select destination..."), tr("c:\\egr"), tr("EGR Library (*.egrlib)"));
	saveLibrary(hand->getPostures(), file.toStdString());
}
void trainer::loadLibrary(){
	QString file = QFileDialog::getOpenFileName(this, tr("Select existing library..."), tr("c:\\egr"), tr("EGR Library (*.egrlib)"));
	std::vector<EGRPosture> postures;
	loadLibrary(postures, file.toStdString());
	ui.lblLibrary->setText(file);
	hand->clearPostures();
	for(int i=0; i<postures.size(); i++){
		hand->addPosture(postures[i]);
	}
	extractAllLabels(hand->getPostures());
	displaySamples();
}
trainer::~trainer()
{

}
