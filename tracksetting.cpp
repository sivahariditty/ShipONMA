#include "tracksetting.h"

TrackSetting::TrackSetting(QWidget *parent):QWidget(parent)
{

createTrackWindow();

}
void TrackSetting::createTrackWindow()
{


    trackWindow=new QFrame();
    trackWindow->setGeometry(QRect(150,400,1024,420));
    trackWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);"));
   // trackWindow->resize(525, 437);


    //gridLayoutWidget = new QWidget(trackWindow);
   // gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
   // gridLayoutWidget->setGeometry(QRect(20, 40, 151, 311));

    trackcheckBox1 = new QCheckBox(trackWindow);
    trackcheckBox1->setObjectName(QString::fromUtf8("checkBox"));
    trackcheckBox1->setGeometry(QRect(30,50, 99, 23));
    trackcheckBox1->setChecked(false);
    trackcheckBox2 = new QCheckBox(trackWindow);
    trackcheckBox2->setObjectName(QString::fromUtf8("checkBox"));
    trackcheckBox2->setGeometry(QRect(30,70, 99, 23));
    trackcheckBox2->setChecked(false);



}
