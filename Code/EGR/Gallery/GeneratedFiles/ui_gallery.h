/********************************************************************************
** Form generated from reading UI file 'gallery.ui'
**
** Created: Sun 2. Jan 17:31:53 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GALLERY_H
#define UI_GALLERY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GalleryClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GalleryClass)
    {
        if (GalleryClass->objectName().isEmpty())
            GalleryClass->setObjectName(QString::fromUtf8("GalleryClass"));
        GalleryClass->resize(600, 400);
        menuBar = new QMenuBar(GalleryClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        GalleryClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GalleryClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        GalleryClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GalleryClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        GalleryClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GalleryClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GalleryClass->setStatusBar(statusBar);

        retranslateUi(GalleryClass);

        QMetaObject::connectSlotsByName(GalleryClass);
    } // setupUi

    void retranslateUi(QMainWindow *GalleryClass)
    {
        GalleryClass->setWindowTitle(QApplication::translate("GalleryClass", "Gallery", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GalleryClass: public Ui_GalleryClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GALLERY_H
