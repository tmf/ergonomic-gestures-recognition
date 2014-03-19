/********************************************************************************
** Form generated from reading UI file 'recorder.ui'
**
** Created: Fri 7. Jan 09:44:57 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORDER_H
#define UI_RECORDER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "videoglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_recorderClass
{
public:
    QWidget *panel;
    VideoGLWidget *videoWidget;
    QGroupBox *groupBox;
    QPushButton *saveTrackerConfig;
    QPushButton *showTracker;
    QGroupBox *groupBox_2;
    QLineEdit *aviFilename;
    QPushButton *recordAVI;
    QLabel *label;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *recorderClass)
    {
        if (recorderClass->objectName().isEmpty())
            recorderClass->setObjectName(QString::fromUtf8("recorderClass"));
        recorderClass->resize(342, 356);
        recorderClass->setMinimumSize(QSize(342, 356));
        recorderClass->setMaximumSize(QSize(342, 356));
        panel = new QWidget(recorderClass);
        panel->setObjectName(QString::fromUtf8("panel"));
        videoWidget = new VideoGLWidget(panel);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        videoWidget->setGeometry(QRect(10, 10, 320, 240));
        groupBox = new QGroupBox(panel);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 260, 121, 81));
        saveTrackerConfig = new QPushButton(groupBox);
        saveTrackerConfig->setObjectName(QString::fromUtf8("saveTrackerConfig"));
        saveTrackerConfig->setGeometry(QRect(10, 50, 101, 23));
        showTracker = new QPushButton(groupBox);
        showTracker->setObjectName(QString::fromUtf8("showTracker"));
        showTracker->setGeometry(QRect(10, 20, 101, 23));
        groupBox_2 = new QGroupBox(panel);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(140, 260, 191, 80));
        aviFilename = new QLineEdit(groupBox_2);
        aviFilename->setObjectName(QString::fromUtf8("aviFilename"));
        aviFilename->setGeometry(QRect(80, 20, 101, 20));
        recordAVI = new QPushButton(groupBox_2);
        recordAVI->setObjectName(QString::fromUtf8("recordAVI"));
        recordAVI->setGeometry(QRect(10, 50, 171, 23));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 24, 61, 16));
        recorderClass->setCentralWidget(panel);
        statusBar = new QStatusBar(recorderClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        recorderClass->setStatusBar(statusBar);

        retranslateUi(recorderClass);

        QMetaObject::connectSlotsByName(recorderClass);
    } // setupUi

    void retranslateUi(QMainWindow *recorderClass)
    {
        recorderClass->setWindowTitle(QApplication::translate("recorderClass", "recorder", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("recorderClass", "Tracker", 0, QApplication::UnicodeUTF8));
        saveTrackerConfig->setText(QApplication::translate("recorderClass", "Save configuration", 0, QApplication::UnicodeUTF8));
        showTracker->setText(QApplication::translate("recorderClass", "Configure...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("recorderClass", "Recorder", 0, QApplication::UnicodeUTF8));
        aviFilename->setText(QApplication::translate("recorderClass", "tracker_%s.avi", 0, QApplication::UnicodeUTF8));
        recordAVI->setText(QApplication::translate("recorderClass", "Record AVI", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("recorderClass", "AVI Filename", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class recorderClass: public Ui_recorderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDER_H
