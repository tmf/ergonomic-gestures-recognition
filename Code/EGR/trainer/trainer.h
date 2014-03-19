#pragma once
#ifndef TRAINER_H
#define TRAINER_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include "ui_trainer.h"
#include "EGRHand.h"

#include <vector>
#include <string>
using namespace std;

class trainer : public QMainWindow
{
	Q_OBJECT

		
public:
	trainer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~trainer();
	void callback(void);
	void displayLibrary(std::vector<EGRPosture> &postures);
	void extractAllLabels(std::vector<EGRPosture> &postures);
	void saveLibrary(std::vector<EGRPosture> &postures, std::string file);
	void loadLibrary(std::vector<EGRPosture> &postures, string file);
	bool eventFilter( QObject* watched, QEvent* event );
protected:
	void keyPressEvent( QKeyEvent *e );
private:
	Ui::trainerClass ui;
	QTimer timer;
	IplImage *buffer;
	EGRHand* hand;
	int currentAVIIndex;
	bool finished;
	void displayPosture(EGRPosture &p, QPixmap &pxm, int x, int y, int w, bool highlight, bool clear);
	void selectSample(int index);
	int selectedSampleIndex;
	int selectedSampleIndexFiltered;
public slots:
	void frame();
	void addAVI();
	void removeAVI();
	void collect();
	void live();
	void pause();
	void frameForward();
	void saveLibrary();
	void loadLibrary();
	void filterSamples();
	void displaySamples();
	void addLabel();
	void removeLabel();
	void selectNextSample();
	void selectPreviousSample();
	void removeSample();
	void estimate();
	void train();
	void changeSampleLabel();
	void loadDecisionFunction();
	void saveDecisionFunction();
};

#endif // TRAINER_H
