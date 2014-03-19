/********************************************************************************
** Form generated from reading UI file 'trainer.ui'
**
** Created: Fri 21. Jan 12:26:16 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINER_H
#define UI_TRAINER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "videoglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_trainerClass
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_collect;
    QPushButton *btnPauseAVI;
    VideoGLWidget *videoWidget;
    QGroupBox *groupBox;
    QLabel *lbl1;
    QLabel *label;
    QLabel *label_2;
    QLabel *Avi;
    QLabel *label_3;
    QLabel *fps;
    QGroupBox *groupBox_2;
    QListWidget *lstAVI;
    QPushButton *btnRemoveAVI;
    QPushButton *btnAddAVI;
    QPushButton *btnCollectSamplesAVI;
    QGroupBox *groupBox_3;
    QPushButton *btnLiveTest;
    QWidget *tab_supervise;
    QScrollArea *scroll;
    QWidget *scrollAreaWidgetContents;
    QLabel *lblSamplesGrid;
    QGroupBox *grpDisplayInfo;
    QComboBox *cmbSampleFilter;
    QLabel *label_4;
    QCheckBox *chkLabelFilter;
    QCheckBox *chkUseCenter;
    QGroupBox *grpSampleInfos;
    QListWidget *lstLabels;
    QPushButton *btnPreviousSample;
    QPushButton *btnNextSample;
    QPushButton *btnAddLabel;
    QLabel *lblSample;
    QLabel *label_6;
    QPushButton *btnRemoveLabel;
    QPushButton *btnRemoveSample;
    QGroupBox *grpLibraryInfo;
    QPushButton *btnLoadLibrary;
    QPushButton *btnSaveLibrary;
    QLabel *lblLibrary;
    QSlider *sldSampleZoom;
    QGroupBox *groupBox_6;
    QLabel *lblCumulative;
    QWidget *tab;
    QGroupBox *groupBox_4;
    QPushButton *btnTrain;
    QProgressBar *accuracy;
    QScrollArea *scrollLog;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *log;
    QGroupBox *groupBox_5;
    QPushButton *btnSaveDecisionFunction;
    QPushButton *btnLoadDecisionFunction;
    QLabel *label_5;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *trainerClass)
    {
        if (trainerClass->objectName().isEmpty())
            trainerClass->setObjectName(QString::fromUtf8("trainerClass"));
        trainerClass->resize(683, 667);
        centralWidget = new QWidget(trainerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(10, 10, 661, 631));
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setTabPosition(QTabWidget::West);
        tab_collect = new QWidget();
        tab_collect->setObjectName(QString::fromUtf8("tab_collect"));
        btnPauseAVI = new QPushButton(tab_collect);
        btnPauseAVI->setObjectName(QString::fromUtf8("btnPauseAVI"));
        btnPauseAVI->setEnabled(false);
        btnPauseAVI->setGeometry(QRect(590, 250, 31, 23));
        videoWidget = new VideoGLWidget(tab_collect);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        videoWidget->setGeometry(QRect(300, 10, 320, 240));
        groupBox = new QGroupBox(tab_collect);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(300, 290, 320, 100));
        groupBox->setLayoutDirection(Qt::LeftToRight);
        lbl1 = new QLabel(groupBox);
        lbl1->setObjectName(QString::fromUtf8("lbl1"));
        lbl1->setGeometry(QRect(10, 20, 81, 16));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(90, 20, 111, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 81, 16));
        Avi = new QLabel(groupBox);
        Avi->setObjectName(QString::fromUtf8("Avi"));
        Avi->setGeometry(QRect(90, 40, 111, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 60, 61, 16));
        fps = new QLabel(groupBox);
        fps->setObjectName(QString::fromUtf8("fps"));
        fps->setGeometry(QRect(90, 60, 91, 16));
        groupBox_2 = new QGroupBox(tab_collect);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 271, 241));
        lstAVI = new QListWidget(groupBox_2);
        lstAVI->setObjectName(QString::fromUtf8("lstAVI"));
        lstAVI->setGeometry(QRect(10, 20, 251, 151));
        btnRemoveAVI = new QPushButton(groupBox_2);
        btnRemoveAVI->setObjectName(QString::fromUtf8("btnRemoveAVI"));
        btnRemoveAVI->setGeometry(QRect(190, 170, 21, 23));
        btnAddAVI = new QPushButton(groupBox_2);
        btnAddAVI->setObjectName(QString::fromUtf8("btnAddAVI"));
        btnAddAVI->setGeometry(QRect(210, 170, 51, 23));
        btnCollectSamplesAVI = new QPushButton(groupBox_2);
        btnCollectSamplesAVI->setObjectName(QString::fromUtf8("btnCollectSamplesAVI"));
        btnCollectSamplesAVI->setEnabled(false);
        btnCollectSamplesAVI->setGeometry(QRect(10, 200, 251, 31));
        groupBox_3 = new QGroupBox(tab_collect);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 290, 271, 101));
        btnLiveTest = new QPushButton(groupBox_3);
        btnLiveTest->setObjectName(QString::fromUtf8("btnLiveTest"));
        btnLiveTest->setEnabled(false);
        btnLiveTest->setGeometry(QRect(10, 20, 250, 31));
        tabWidget->addTab(tab_collect, QString());
        tab_supervise = new QWidget();
        tab_supervise->setObjectName(QString::fromUtf8("tab_supervise"));
        scroll = new QScrollArea(tab_supervise);
        scroll->setObjectName(QString::fromUtf8("scroll"));
        scroll->setGeometry(QRect(6, 10, 404, 581));
        scroll->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 402, 579));
        lblSamplesGrid = new QLabel(scrollAreaWidgetContents);
        lblSamplesGrid->setObjectName(QString::fromUtf8("lblSamplesGrid"));
        lblSamplesGrid->setGeometry(QRect(0, 0, 371, 371));
        scroll->setWidget(scrollAreaWidgetContents);
        grpDisplayInfo = new QGroupBox(tab_supervise);
        grpDisplayInfo->setObjectName(QString::fromUtf8("grpDisplayInfo"));
        grpDisplayInfo->setGeometry(QRect(420, 10, 201, 81));
        cmbSampleFilter = new QComboBox(grpDisplayInfo);
        cmbSampleFilter->setObjectName(QString::fromUtf8("cmbSampleFilter"));
        cmbSampleFilter->setEnabled(false);
        cmbSampleFilter->setGeometry(QRect(30, 30, 161, 22));
        label_4 = new QLabel(grpDisplayInfo);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 10, 71, 20));
        chkLabelFilter = new QCheckBox(grpDisplayInfo);
        chkLabelFilter->setObjectName(QString::fromUtf8("chkLabelFilter"));
        chkLabelFilter->setGeometry(QRect(10, 32, 16, 17));
        chkUseCenter = new QCheckBox(grpDisplayInfo);
        chkUseCenter->setObjectName(QString::fromUtf8("chkUseCenter"));
        chkUseCenter->setGeometry(QRect(10, 60, 141, 17));
        grpSampleInfos = new QGroupBox(tab_supervise);
        grpSampleInfos->setObjectName(QString::fromUtf8("grpSampleInfos"));
        grpSampleInfos->setGeometry(QRect(420, 300, 201, 241));
        lstLabels = new QListWidget(grpSampleInfos);
        lstLabels->setObjectName(QString::fromUtf8("lstLabels"));
        lstLabels->setGeometry(QRect(10, 80, 181, 101));
        btnPreviousSample = new QPushButton(grpSampleInfos);
        btnPreviousSample->setObjectName(QString::fromUtf8("btnPreviousSample"));
        btnPreviousSample->setGeometry(QRect(10, 210, 75, 23));
        btnNextSample = new QPushButton(grpSampleInfos);
        btnNextSample->setObjectName(QString::fromUtf8("btnNextSample"));
        btnNextSample->setGeometry(QRect(120, 210, 75, 23));
        btnAddLabel = new QPushButton(grpSampleInfos);
        btnAddLabel->setObjectName(QString::fromUtf8("btnAddLabel"));
        btnAddLabel->setGeometry(QRect(170, 180, 21, 16));
        lblSample = new QLabel(grpSampleInfos);
        lblSample->setObjectName(QString::fromUtf8("lblSample"));
        lblSample->setGeometry(QRect(10, 20, 181, 16));
        label_6 = new QLabel(grpSampleInfos);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 64, 181, 16));
        btnRemoveLabel = new QPushButton(grpSampleInfos);
        btnRemoveLabel->setObjectName(QString::fromUtf8("btnRemoveLabel"));
        btnRemoveLabel->setGeometry(QRect(150, 180, 21, 16));
        btnRemoveSample = new QPushButton(grpSampleInfos);
        btnRemoveSample->setObjectName(QString::fromUtf8("btnRemoveSample"));
        btnRemoveSample->setGeometry(QRect(10, 40, 181, 23));
        QPalette palette;
        QBrush brush(QColor(255, 85, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        QBrush brush2(QColor(117, 116, 118, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        btnRemoveSample->setPalette(palette);
        QFont font;
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        font.setKerning(false);
        font.setStyleStrategy(QFont::PreferDefault);
        btnRemoveSample->setFont(font);
        grpLibraryInfo = new QGroupBox(tab_supervise);
        grpLibraryInfo->setObjectName(QString::fromUtf8("grpLibraryInfo"));
        grpLibraryInfo->setGeometry(QRect(420, 550, 201, 71));
        btnLoadLibrary = new QPushButton(grpLibraryInfo);
        btnLoadLibrary->setObjectName(QString::fromUtf8("btnLoadLibrary"));
        btnLoadLibrary->setGeometry(QRect(10, 40, 75, 23));
        btnSaveLibrary = new QPushButton(grpLibraryInfo);
        btnSaveLibrary->setObjectName(QString::fromUtf8("btnSaveLibrary"));
        btnSaveLibrary->setGeometry(QRect(120, 40, 75, 23));
        lblLibrary = new QLabel(grpLibraryInfo);
        lblLibrary->setObjectName(QString::fromUtf8("lblLibrary"));
        lblLibrary->setGeometry(QRect(10, 20, 181, 16));
        sldSampleZoom = new QSlider(tab_supervise);
        sldSampleZoom->setObjectName(QString::fromUtf8("sldSampleZoom"));
        sldSampleZoom->setGeometry(QRect(20, 600, 380, 20));
        sldSampleZoom->setMinimum(32);
        sldSampleZoom->setMaximum(96);
        sldSampleZoom->setSingleStep(16);
        sldSampleZoom->setPageStep(64);
        sldSampleZoom->setValue(64);
        sldSampleZoom->setOrientation(Qt::Horizontal);
        groupBox_6 = new QGroupBox(tab_supervise);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(420, 100, 201, 190));
        lblCumulative = new QLabel(groupBox_6);
        lblCumulative->setObjectName(QString::fromUtf8("lblCumulative"));
        lblCumulative->setGeometry(QRect(20, 20, 160, 160));
        tabWidget->addTab(tab_supervise, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 10, 611, 121));
        btnTrain = new QPushButton(groupBox_4);
        btnTrain->setObjectName(QString::fromUtf8("btnTrain"));
        btnTrain->setEnabled(true);
        btnTrain->setGeometry(QRect(10, 50, 141, 23));
        accuracy = new QProgressBar(groupBox_4);
        accuracy->setObjectName(QString::fromUtf8("accuracy"));
        accuracy->setEnabled(false);
        accuracy->setGeometry(QRect(10, 80, 171, 23));
        accuracy->setValue(24);
        scrollLog = new QScrollArea(groupBox_4);
        scrollLog->setObjectName(QString::fromUtf8("scrollLog"));
        scrollLog->setGeometry(QRect(270, 15, 331, 91));
        scrollLog->setFrameShape(QFrame::Box);
        scrollLog->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 329, 89));
        log = new QLabel(scrollAreaWidgetContents_2);
        log->setObjectName(QString::fromUtf8("log"));
        log->setGeometry(QRect(0, 0, 301, 81));
        log->setFrameShape(QFrame::NoFrame);
        log->setFrameShadow(QFrame::Sunken);
        log->setTextFormat(Qt::LogText);
        log->setScaledContents(false);
        log->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        log->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        scrollLog->setWidget(scrollAreaWidgetContents_2);
        groupBox_5 = new QGroupBox(tab);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 140, 611, 51));
        btnSaveDecisionFunction = new QPushButton(groupBox_5);
        btnSaveDecisionFunction->setObjectName(QString::fromUtf8("btnSaveDecisionFunction"));
        btnSaveDecisionFunction->setGeometry(QRect(530, 16, 75, 23));
        btnLoadDecisionFunction = new QPushButton(groupBox_5);
        btnLoadDecisionFunction->setObjectName(QString::fromUtf8("btnLoadDecisionFunction"));
        btnLoadDecisionFunction->setGeometry(QRect(450, 16, 75, 23));
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 401, 16));
        tabWidget->addTab(tab, QString());
        trainerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(trainerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        trainerClass->setStatusBar(statusBar);

        retranslateUi(trainerClass);
        QObject::connect(chkLabelFilter, SIGNAL(toggled(bool)), cmbSampleFilter, SLOT(setEnabled(bool)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(trainerClass);
    } // setupUi

    void retranslateUi(QMainWindow *trainerClass)
    {
        trainerClass->setWindowTitle(QApplication::translate("trainerClass", "trainer", 0, QApplication::UnicodeUTF8));
        btnPauseAVI->setText(QApplication::translate("trainerClass", "||", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("trainerClass", "Batch collecting information", 0, QApplication::UnicodeUTF8));
        lbl1->setText(QApplication::translate("trainerClass", "Current Label:", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QApplication::translate("trainerClass", "Current Source:", 0, QApplication::UnicodeUTF8));
        Avi->setText(QString());
        label_3->setText(QApplication::translate("trainerClass", "FPS:", 0, QApplication::UnicodeUTF8));
        fps->setText(QString());
        groupBox_2->setTitle(QApplication::translate("trainerClass", "Collect samples from AVI", 0, QApplication::UnicodeUTF8));
        btnRemoveAVI->setText(QApplication::translate("trainerClass", "-", 0, QApplication::UnicodeUTF8));
        btnAddAVI->setText(QApplication::translate("trainerClass", "Add AVI", 0, QApplication::UnicodeUTF8));
        btnCollectSamplesAVI->setText(QApplication::translate("trainerClass", "Collect samples from AVI's", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("trainerClass", "Test decision function with Camera", 0, QApplication::UnicodeUTF8));
        btnLiveTest->setText(QApplication::translate("trainerClass", "Live test", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_collect), QApplication::translate("trainerClass", "Collect Data", 0, QApplication::UnicodeUTF8));
        lblSamplesGrid->setText(QString());
        grpDisplayInfo->setTitle(QApplication::translate("trainerClass", "Display", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("trainerClass", "Filter samples:", 0, QApplication::UnicodeUTF8));
        chkLabelFilter->setText(QString());
        chkUseCenter->setText(QApplication::translate("trainerClass", " Hand center as origin", 0, QApplication::UnicodeUTF8));
        grpSampleInfos->setTitle(QApplication::translate("trainerClass", "Sample Information", 0, QApplication::UnicodeUTF8));
        btnPreviousSample->setText(QApplication::translate("trainerClass", "Previous", 0, QApplication::UnicodeUTF8));
        btnNextSample->setText(QApplication::translate("trainerClass", "Next", 0, QApplication::UnicodeUTF8));
        btnAddLabel->setText(QApplication::translate("trainerClass", "+", 0, QApplication::UnicodeUTF8));
        lblSample->setText(QApplication::translate("trainerClass", "Sample n/N", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("trainerClass", "Selected sample label:", 0, QApplication::UnicodeUTF8));
        btnRemoveLabel->setText(QApplication::translate("trainerClass", "-", 0, QApplication::UnicodeUTF8));
        btnRemoveSample->setText(QApplication::translate("trainerClass", "Remove sample", 0, QApplication::UnicodeUTF8));
        grpLibraryInfo->setTitle(QApplication::translate("trainerClass", "Library", 0, QApplication::UnicodeUTF8));
        btnLoadLibrary->setText(QApplication::translate("trainerClass", "Load...", 0, QApplication::UnicodeUTF8));
        btnSaveLibrary->setText(QApplication::translate("trainerClass", "Save...", 0, QApplication::UnicodeUTF8));
        lblLibrary->setText(QApplication::translate("trainerClass", "[library not saved]", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("trainerClass", "Cumulative sample view", 0, QApplication::UnicodeUTF8));
        lblCumulative->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_supervise), QApplication::translate("trainerClass", "Supervise Samples", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("trainerClass", "Training", 0, QApplication::UnicodeUTF8));
        btnTrain->setText(QApplication::translate("trainerClass", "Train", 0, QApplication::UnicodeUTF8));
        log->setText(QString());
        groupBox_5->setTitle(QApplication::translate("trainerClass", "Decision function", 0, QApplication::UnicodeUTF8));
        btnSaveDecisionFunction->setText(QApplication::translate("trainerClass", "Save...", 0, QApplication::UnicodeUTF8));
        btnLoadDecisionFunction->setText(QApplication::translate("trainerClass", "Load...", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("trainerClass", "[not yet trained]", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("trainerClass", "Train Samples", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class trainerClass: public Ui_trainerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINER_H
