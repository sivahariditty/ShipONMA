#include "trackwindowsetting.h"


ChannelTrack CH_Track;
ChannelTrackValue Track_Data[NO_OF_TRACK];
TrackWindowSetting::TrackWindowSetting(QWidget *parent) :
    QWidget(parent)
{


    for(xCount=0;xCount<64;xCount++)
    {
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
    }

    trackWindow=new QFrame();
    trackWindow->setGeometry(QRect(0,0,900,1100));
    trackWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);"));
    label2 = new QLabel(trackWindow);
    label2->setText("Freq");
    label2->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
        "color: rgb(0,0,0);"));
    label2->setGeometry(QRect(130,10,200, 23));
    label3 = new QLabel(trackWindow);
    label3->setText("Amp");
    label3->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
        "color: rgb(0,0,0);"));
    label3->setGeometry(QRect(230,10,200, 23));

    label4 = new QLabel(trackWindow);
    label4->setText("Freq");
    label4->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
        "color: rgb(0,0,0);"));
    label4->setGeometry(QRect(460,10,200, 23));
    label5 = new QLabel(trackWindow);
    label5->setText("Amp");
    label5->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
        "color: rgb(0,0,0);"));
    label5->setGeometry(QRect(560,10,200, 23));

    OK_BUTTON = new QPushButton(trackWindow);
    OK_BUTTON->setStyleSheet(QString::fromUtf8("background-color:rgb(239,228,255)"));
    OK_BUTTON->setText("CLOSE");
    OK_BUTTON->setObjectName(QString::fromUtf8("pushButton_2"));
    OK_BUTTON->setGeometry(QRect(600,10, 98, 28));



    for(count_=0;count_<32;count_++)
    {
        yCount=30*count_;
        trackcheckBox[count_] = new QCheckBox(trackWindow);
        trackcheckBox[count_]->setObjectName(QString::fromUtf8("checkBox"));
        trackcheckBox[count_]->setGeometry(QRect(12,40+yCount,15, 23));
        trackcheckBox[count_]->setChecked(false);
        trackcheckBox[count_]->show();
        sprintf(TrackNo,"Track %d",(count_+1));
        label[count_] = new QLabel(trackWindow);
        label[count_] ->setText(TrackNo);
        label[count_] ->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label[count_]->setGeometry(QRect(40,40+yCount, 99, 23));
        lineEdit_a1[count_] = new QLineEdit(trackWindow);
        lineEdit_a1[count_]->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_a1[count_]->setGeometry(QRect(110,40+yCount, 99, 23));
        lineEdit_a1[count_]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));
        lineEdit_b1[count_] = new QLineEdit(trackWindow);
        lineEdit_b1[count_]->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_b1[count_]->setGeometry(QRect(210,40+yCount, 99, 23));
        lineEdit_b1[count_]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));


    }

    for(xCount=0;xCount<32;xCount++)
    {
        yCount=30*xCount;
        count_=xCount+32;
        trackcheckBox[count_] = new QCheckBox(trackWindow);
        trackcheckBox[count_]->setObjectName(QString::fromUtf8("checkBox"));
        trackcheckBox[count_]->setGeometry(QRect(332,40+yCount,15, 23));
        trackcheckBox[count_]->setChecked(false);
        trackcheckBox[count_]->show();
        sprintf(TrackNo,"Track %d",(count_+1));
        label[count_] = new QLabel(trackWindow);
        label[count_] ->setText(TrackNo);
        label[count_] ->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label[count_]->setGeometry(QRect(360,40+yCount, 99, 23));
        lineEdit_a1[count_] = new QLineEdit(trackWindow);
        lineEdit_a1[count_]->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_a1[count_]->setGeometry(QRect(430,40+yCount, 99, 23));
        lineEdit_a1[count_]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));

        lineEdit_b1[count_] = new QLineEdit(trackWindow);
        lineEdit_b1[count_]->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit_b1[count_]->setGeometry(QRect(530,40+yCount, 99, 23));
        lineEdit_b1[count_]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));


    }

    connect(trackcheckBox[0],SIGNAL(stateChanged(int)),SLOT(checkBox1Enable(int)));
    connect(trackcheckBox[1],SIGNAL(stateChanged(int)),SLOT(checkBox2Enable(int)));
    connect(trackcheckBox[2],SIGNAL(stateChanged(int)),SLOT(checkBox3Enable(int)));
    connect(trackcheckBox[3],SIGNAL(stateChanged(int)),SLOT(checkBox4Enable(int)));
    connect(trackcheckBox[4],SIGNAL(stateChanged(int)),SLOT(checkBox5Enable(int)));
    connect(trackcheckBox[5],SIGNAL(stateChanged(int)),SLOT(checkBox6Enable(int)));
    connect(trackcheckBox[6],SIGNAL(stateChanged(int)),SLOT(checkBox7Enable(int)));
    connect(trackcheckBox[7],SIGNAL(stateChanged(int)),SLOT(checkBox8Enable(int)));
    connect(trackcheckBox[8],SIGNAL(stateChanged(int)),SLOT(checkBox9Enable(int)));
    connect(trackcheckBox[9],SIGNAL(stateChanged(int)),SLOT(checkBox10Enable(int)));
    connect(trackcheckBox[10],SIGNAL(stateChanged(int)),SLOT(checkBox11Enable(int)));
    connect(trackcheckBox[11],SIGNAL(stateChanged(int)),SLOT(checkBox12Enable(int)));
    connect(trackcheckBox[12],SIGNAL(stateChanged(int)),SLOT(checkBox13Enable(int)));
    connect(trackcheckBox[13],SIGNAL(stateChanged(int)),SLOT(checkBox14Enable(int)));
    connect(trackcheckBox[14],SIGNAL(stateChanged(int)),SLOT(checkBox15Enable(int)));
    connect(trackcheckBox[15],SIGNAL(stateChanged(int)),SLOT(checkBox16Enable(int)));
    connect(trackcheckBox[16],SIGNAL(stateChanged(int)),SLOT(checkBox17Enable(int)));
    connect(trackcheckBox[17],SIGNAL(stateChanged(int)),SLOT(checkBox18Enable(int)));

    connect(trackcheckBox[18],SIGNAL(stateChanged(int)),SLOT(checkBox19Enable(int)));
    connect(trackcheckBox[19],SIGNAL(stateChanged(int)),SLOT(checkBox20Enable(int)));
    connect(trackcheckBox[20],SIGNAL(stateChanged(int)),SLOT(checkBox21Enable(int)));
    connect(trackcheckBox[21],SIGNAL(stateChanged(int)),SLOT(checkBox22Enable(int)));
    connect(trackcheckBox[22],SIGNAL(stateChanged(int)),SLOT(checkBox23Enable(int)));
    connect(trackcheckBox[23],SIGNAL(stateChanged(int)),SLOT(checkBox24Enable(int)));
    connect(trackcheckBox[24],SIGNAL(stateChanged(int)),SLOT(checkBox25Enable(int)));
    connect(trackcheckBox[25],SIGNAL(stateChanged(int)),SLOT(checkBox26Enable(int)));
    connect(trackcheckBox[26],SIGNAL(stateChanged(int)),SLOT(checkBox27Enable(int)));
    connect(trackcheckBox[27],SIGNAL(stateChanged(int)),SLOT(checkBox28Enable(int)));
    connect(trackcheckBox[28],SIGNAL(stateChanged(int)),SLOT(checkBox29Enable(int)));
    connect(trackcheckBox[29],SIGNAL(stateChanged(int)),SLOT(checkBox30Enable(int)));
    connect(trackcheckBox[20],SIGNAL(stateChanged(int)),SLOT(checkBox31Enable(int)));
    connect(trackcheckBox[31],SIGNAL(stateChanged(int)),SLOT(checkBox32Enable(int)));
    connect(trackcheckBox[32],SIGNAL(stateChanged(int)),SLOT(checkBox33Enable(int)));
    connect(trackcheckBox[33],SIGNAL(stateChanged(int)),SLOT(checkBox34Enable(int)));
    connect(trackcheckBox[34],SIGNAL(stateChanged(int)),SLOT(checkBox35Enable(int)));
    connect(trackcheckBox[35],SIGNAL(stateChanged(int)),SLOT(checkBox36Enable(int)));
    connect(trackcheckBox[36],SIGNAL(stateChanged(int)),SLOT(checkBox37Enable(int)));
    connect(trackcheckBox[37],SIGNAL(stateChanged(int)),SLOT(checkBox38Enable(int)));
    connect(trackcheckBox[38],SIGNAL(stateChanged(int)),SLOT(checkBox39Enable(int)));
    connect(trackcheckBox[39],SIGNAL(stateChanged(int)),SLOT(checkBox40Enable(int)));

    connect(trackcheckBox[40],SIGNAL(stateChanged(int)),SLOT(checkBox41Enable(int)));
    connect(trackcheckBox[41],SIGNAL(stateChanged(int)),SLOT(checkBox42Enable(int)));
    connect(trackcheckBox[42],SIGNAL(stateChanged(int)),SLOT(checkBox43Enable(int)));
    connect(trackcheckBox[43],SIGNAL(stateChanged(int)),SLOT(checkBox44Enable(int)));
    connect(trackcheckBox[44],SIGNAL(stateChanged(int)),SLOT(checkBox45Enable(int)));
    connect(trackcheckBox[45],SIGNAL(stateChanged(int)),SLOT(checkBox46Enable(int)));
    connect(trackcheckBox[46],SIGNAL(stateChanged(int)),SLOT(checkBox47Enable(int)));
    connect(trackcheckBox[47],SIGNAL(stateChanged(int)),SLOT(checkBox48Enable(int)));
    connect(trackcheckBox[48],SIGNAL(stateChanged(int)),SLOT(checkBox49Enable(int)));
    connect(trackcheckBox[49],SIGNAL(stateChanged(int)),SLOT(checkBox50Enable(int)));


    connect(trackcheckBox[50],SIGNAL(stateChanged(int)),SLOT(checkBox51Enable(int)));
    connect(trackcheckBox[51],SIGNAL(stateChanged(int)),SLOT(checkBox52Enable(int)));
    connect(trackcheckBox[52],SIGNAL(stateChanged(int)),SLOT(checkBox53Enable(int)));
    connect(trackcheckBox[53],SIGNAL(stateChanged(int)),SLOT(checkBox54Enable(int)));
    connect(trackcheckBox[54],SIGNAL(stateChanged(int)),SLOT(checkBox55Enable(int)));
    connect(trackcheckBox[55],SIGNAL(stateChanged(int)),SLOT(checkBox56Enable(int)));
    connect(trackcheckBox[56],SIGNAL(stateChanged(int)),SLOT(checkBox57Enable(int)));
    connect(trackcheckBox[57],SIGNAL(stateChanged(int)),SLOT(checkBox58Enable(int)));
    connect(trackcheckBox[58],SIGNAL(stateChanged(int)),SLOT(checkBox59Enable(int)));
    connect(trackcheckBox[59],SIGNAL(stateChanged(int)),SLOT(checkBox60Enable(int)));


    connect(trackcheckBox[60],SIGNAL(stateChanged(int)),SLOT(checkBox61Enable(int)));
    connect(trackcheckBox[61],SIGNAL(stateChanged(int)),SLOT(checkBox62Enable(int)));
    connect(trackcheckBox[62],SIGNAL(stateChanged(int)),SLOT(checkBox63Enable(int)));
    connect(trackcheckBox[63],SIGNAL(stateChanged(int)),SLOT(checkBox64Enable(int)));


    connect(OK_BUTTON,SIGNAL(clicked()),this,SLOT(WindowHide()));





}

void TrackWindowSetting::checkBox1Enable(int e)
{
   xCount=0;
   if(e!=0)
   {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO = CurrentChannel;
        Track_Data[xCount].TrackStatus=true;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;


    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackMiss=true;
        Track_Data[xCount].TrackStatus=false;


    }
}


void TrackWindowSetting::checkBox2Enable(int e)
{
    xCount=1;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].TrackStatus=true;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }


void TrackWindowSetting::checkBox3Enable(int e)
{
    xCount=2;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox4Enable(int e)
{
    xCount=3;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }

 }
void TrackWindowSetting::checkBox5Enable(int e)
{
    xCount=4;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox6Enable(int e)
{
    xCount=5;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox7Enable(int e)
{
    xCount=6;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox8Enable(int e)
{
    xCount=7;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox9Enable(int e)
{
    xCount=8;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox10Enable(int e)
{
    xCount=9;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox11Enable(int e)
{
    xCount=10;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox12Enable(int e)
{
    xCount=11;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox13Enable(int e)
{
    xCount=12;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox14Enable(int e)
{
    xCount=13;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox15Enable(int e)
{
    xCount=14;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox16Enable(int e)
{
    xCount=15;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox17Enable(int e)
{
    xCount=16;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox18Enable(int e)
{
    xCount=17;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox19Enable(int e)
{
    xCount=18;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox20Enable(int e)
{
    xCount=19;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }

void TrackWindowSetting::checkBox21Enable(int e)
{
    xCount=20;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox22Enable(int e)
{
    xCount=21;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox23Enable(int e)
{
    xCount=22;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox24Enable(int e)
{
    xCount=23;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox25Enable(int e)
{
    xCount=24;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox26Enable(int e)
{
    xCount=25;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox27Enable(int e)
{
    xCount=26;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox28Enable(int e)
{
    xCount=27;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox29Enable(int e)
{
    xCount=28;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }


void TrackWindowSetting::checkBox30Enable(int e)
{
    xCount=29;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox31Enable(int e)
{
    xCount=30;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }

void TrackWindowSetting::checkBox32Enable(int e)
{
    xCount=31;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox33Enable(int e)
{
    xCount=32;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox34Enable(int e)
{
    xCount=33;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox35Enable(int e)
{
    xCount=34;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox36Enable(int e)
{
    xCount=35;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox37Enable(int e)
{
    xCount=36;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox38Enable(int e)
{
    xCount=37;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox39Enable(int e)
{
    xCount=38;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox40Enable(int e)
{
    xCount=39;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox41Enable(int e)
{
    xCount=40;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox42Enable(int e)
{
    xCount=41;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox43Enable(int e)
{
    xCount=42;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox44Enable(int e)
{
    xCount=43;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox45Enable(int e)
{
    xCount=44;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox46Enable(int e)
{
    xCount=45;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox47Enable(int e)
{
    xCount=46;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox48Enable(int e)
{
    xCount=47;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox49Enable(int e)
{
    xCount=48;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox50Enable(int e)
{
    xCount=49;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }


void TrackWindowSetting::checkBox51Enable(int e)
{
    xCount=50;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }

void TrackWindowSetting::checkBox52Enable(int e)
{
    xCount=51;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox53Enable(int e)
{
    xCount=52;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox54Enable(int e)
{
    xCount=53;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }

void TrackWindowSetting::checkBox55Enable(int e)
{
    xCount=54;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox56Enable(int e)
{
    xCount=55;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox57Enable(int e)
{
    xCount=56;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox58Enable(int e)
{
    xCount=57;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }

void TrackWindowSetting::checkBox59Enable(int e)
{
    xCount=58;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox60Enable(int e)
{
    xCount=59;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }
void TrackWindowSetting::checkBox61Enable(int e)
{
    xCount=60;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox62Enable(int e)
{
    xCount=61;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }
void TrackWindowSetting::checkBox63Enable(int e)
{
    xCount=62;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;
        Track_Data[xCount].TrackMiss=true;


    }
 }

void TrackWindowSetting::checkBox64Enable(int e)
{
    xCount=63;
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",CH_Track.Frequency);
        sprintf(Amp," %.1f  ",CH_Track.Amplitude);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=CH_Track.Frequency;
        Track_Data[xCount].Amplitude=CH_Track.Amplitude;
        Track_Data[xCount].INT_CH_NO=CurrentChannel;
        Track_Data[xCount].yPos= CH_Track.yTrackPos;
        Track_Data[xCount].xPos=CH_Track.xTrackPos;
        Track_Data[xCount].TrackStatus=true;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_b1[xCount]->setText(Amp);
        lineEdit_a1[xCount]->setText(Freq);
        Track_Data[xCount].Frequency=0;
        Track_Data[xCount].Amplitude=0;
        Track_Data[xCount].TrackStatus=false;

        Track_Data[xCount].TrackMiss=true;

    }
 }

void TrackWindowSetting::WindowHide()
{
trackWindow->hide();
}
