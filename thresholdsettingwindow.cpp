#include "thresholdsettingwindow.h"
 ChannelThreshold Threshold;

ThresholdSettingWindow::ThresholdSettingWindow(QWidget *parent) :
    QWidget(parent)
{

    ThresholdWindow=new QFrame(this);
    ThresholdWindow->setMouseTracking(true);
    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Background,Qt::red);
     palette->setColor(QPalette::Foreground,Qt::green);
    ThresholdWindow->setPalette(*palette);
    ThresholdWindow->setGeometry(QRect(0,0,160,120));
    ThresholdWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);"));
     ThresholdWindow->resize(525, 437);
        gridLayoutWidget = new QWidget(ThresholdWindow);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 40, 151, 311));

        checkBox1 = new QCheckBox(ThresholdWindow);
        checkBox1->setObjectName(QString::fromUtf8("checkBox"));
        checkBox1->setGeometry(QRect(30,50, 99, 23));
        checkBox1->setChecked(false);
        checkBox2 = new QCheckBox(ThresholdWindow);
        checkBox2->setObjectName(QString::fromUtf8("checkBox"));
        checkBox2->setGeometry(QRect(30,90, 99, 23));
        checkBox2->setChecked(false);

        checkBox3 = new QCheckBox(ThresholdWindow);
        checkBox3->setObjectName(QString::fromUtf8("checkBox"));
        checkBox3->setGeometry(QRect(30,130, 99, 23));
        checkBox3->setChecked(false);

        checkBox4 = new QCheckBox(ThresholdWindow);
        checkBox4->setObjectName(QString::fromUtf8("checkBox"));
        checkBox4->setGeometry(QRect(30,165, 99, 23));
        checkBox4->setChecked(false);

        checkBox5 = new QCheckBox(ThresholdWindow);
        checkBox5->setObjectName(QString::fromUtf8("checkBox"));
        checkBox5->setGeometry(QRect(30,205, 99, 23));
        checkBox5->setChecked(false);

        checkBox6 = new QCheckBox(ThresholdWindow);
        checkBox6->setObjectName(QString::fromUtf8("checkBox"));
        checkBox6->setGeometry(QRect(30,245, 99, 23));
        checkBox6->setChecked(false);

        checkBox7 = new QCheckBox(ThresholdWindow);
        checkBox7->setObjectName(QString::fromUtf8("checkBox"));
        checkBox7->setGeometry(QRect(30,280, 99, 23));
        checkBox7->setChecked(false);

        checkBox8 = new QCheckBox(ThresholdWindow);
        checkBox8->setObjectName(QString::fromUtf8("checkBox"));
        checkBox8->setGeometry(QRect(30,320, 99, 23));
        checkBox8->setChecked(false);



        label11 = new QLabel(ThresholdWindow);
        label11->setText("THRESHOLD1");
        label11->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label11->setGeometry(QRect(60,50, 99, 23));

        label12 = new QLabel(ThresholdWindow);
        label12->setText("THRESHOLD2");
        label12->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label12->setGeometry(QRect(60,90, 99, 23));

        label13 = new QLabel(ThresholdWindow);
        label13->setText("THRESHOLD3");
        label13->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label13->setGeometry(QRect(60,130, 99, 23));

        label14 = new QLabel(ThresholdWindow);
        label14->setText("THRESHOLD4");
        label14->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label14->setGeometry(QRect(60,165, 99, 23));

        label15 = new QLabel(ThresholdWindow);
        label15->setText("THRESHOLD5");
        label15->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label15->setGeometry(QRect(60,205, 99, 23));

        label16 = new QLabel(ThresholdWindow);
        label16->setText("THRESHOLD6");
        label16->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label16->setGeometry(QRect(60,245, 99, 23));

        label17 = new QLabel(ThresholdWindow);
        label17->setText("THRESHOLD7");
        label17->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label17->setGeometry(QRect(60,280, 99, 23));

        label18 = new QLabel(ThresholdWindow);
        label18->setText("THRESHOLD8");
        label18->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label18->setGeometry(QRect(60,320, 99, 23));





        gridLayoutWidget_2 = new QWidget(ThresholdWindow);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(200, 40, 301, 311));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit_2 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout_2->addWidget(lineEdit_2, 0, 0, 1, 1);

        lineEdit = new QLineEdit(gridLayoutWidget_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_2->addWidget(lineEdit, 0, 1, 1, 1);

        lineEdit_3 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout_2->addWidget(lineEdit_3, 1, 0, 1, 1);

        lineEdit_4 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout_2->addWidget(lineEdit_4, 1, 1, 1, 1);

        lineEdit_5 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        gridLayout_2->addWidget(lineEdit_5, 2, 0, 1, 1);

        lineEdit_6 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        gridLayout_2->addWidget(lineEdit_6, 2, 1, 1, 1);

        lineEdit_7 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        gridLayout_2->addWidget(lineEdit_7, 3, 0, 1, 1);

        lineEdit_8 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));

        gridLayout_2->addWidget(lineEdit_8, 3, 1, 1, 1);

        lineEdit_9 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));

        gridLayout_2->addWidget(lineEdit_9, 4, 0, 1, 1);

        lineEdit_10 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));

        gridLayout_2->addWidget(lineEdit_10, 4, 1, 1, 1);

        lineEdit_11 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));

        gridLayout_2->addWidget(lineEdit_11, 5, 0, 1, 1);

        lineEdit_12 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));

        gridLayout_2->addWidget(lineEdit_12, 5, 1, 1, 1);

        lineEdit_13 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));

        gridLayout_2->addWidget(lineEdit_13, 6, 0, 1, 1);

        lineEdit_14 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));

        gridLayout_2->addWidget(lineEdit_14, 6, 1, 1, 1);

        lineEdit_15 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));

        gridLayout_2->addWidget(lineEdit_15, 7, 0, 1, 1);

        lineEdit_16 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));

        gridLayout_2->addWidget(lineEdit_16, 7, 1, 1, 1);

        label = new QLabel(ThresholdWindow);
        label->setGeometry(QRect(220, 10, 131, 20));
        label->setText("FREQUENCY(Hz)");
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label_2 = new QLabel(ThresholdWindow);
        label_2->setText("AMPLITUDE(dB)");
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(239,228,255);\n"
            "color: rgb(0,0,0);"));
        label_2->setGeometry(QRect(370, 10, 131, 20));

        pushButton = new QPushButton(ThresholdWindow);
        pushButton->setStyleSheet(QString::fromUtf8("background-color:rgb(239,228,255)"));
        pushButton->setText("SET");
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(390, 380, 98, 28));
        pushButton_2 = new QPushButton(ThresholdWindow);
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color:rgb(239,228,255)"));
        pushButton_2->setText("CANCEL");
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(280, 380, 98, 28));

    connect(pushButton,SIGNAL(clicked()),this,SLOT(setThresholdConfig()));
    connect(pushButton_2,SIGNAL(clicked()),this,SLOT(cancelThresholdConfig()));

    connect(checkBox1,SIGNAL(stateChanged(int)),SLOT(checkBox1Enable(int)));
    connect(checkBox2,SIGNAL(stateChanged(int)),SLOT(checkBox2Enable(int)));
    connect(checkBox3,SIGNAL(stateChanged(int)),SLOT(checkBox3Enable(int)));


    connect(checkBox4,SIGNAL(stateChanged(int)),SLOT(checkBox4Enable(int)));
    connect(checkBox5,SIGNAL(stateChanged(int)),SLOT(checkBox5Enable(int)));
    connect(checkBox6,SIGNAL(stateChanged(int)),SLOT(checkBox6Enable(int)));


    connect(checkBox7,SIGNAL(stateChanged(int)),SLOT(checkBox7Enable(int)));
    connect(checkBox8,SIGNAL(stateChanged(int)),SLOT(checkBox8Enable(int)));

}
void ThresholdSettingWindow::setThresholdConfig()
{


    ThresholdWindow->hide();


}

void ThresholdSettingWindow::cancelThresholdConfig()
{

    Threshold.TAmplitude=0;
    ThresholdWindow->hide();
    Threshold.TFrequency=0;
}
void ThresholdSettingWindow::checkBox1Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit->setText(Amp);
        lineEdit_2->setText(Freq);
     //   printf("\n Channel=%d ",CurrentChannelNumber);
        Threshold.CH_NO_Freq[CurrentChannelNumber][0]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][0]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][0]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][0]=Threshold.yThresholdPos;
        Threshold.Init_CH_No[CurrentChannelNumber][0]=true;



    }
    else
    {
  //      printf("\n Channel=%d ",CurrentChannelNumber);

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit->setText(Amp);
        lineEdit_2->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][0]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][0]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][0]=0;
    }
}

void ThresholdSettingWindow::checkBox2Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_4->setText(Amp);
        lineEdit_3->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][1]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][1]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][1]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][1]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][1]=Threshold.yThresholdPos;

    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_4->setText(Amp);
        lineEdit_3->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][1]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][1]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][1]=0;
    }
}
void ThresholdSettingWindow::checkBox3Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_6->setText(Amp);
        lineEdit_5->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][2]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][2]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][2]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][2]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][2]=Threshold.yThresholdPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_6->setText(Amp);
        lineEdit_5->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][2]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][2]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][2]=0;
    }
}
void ThresholdSettingWindow::checkBox4Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_8->setText(Amp);
        lineEdit_7->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][3]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][3]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][3]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][3]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][3]=Threshold.yThresholdPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_8->setText(Amp);
        lineEdit_7->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][3]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][3]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][3]=0;
    }
}

void ThresholdSettingWindow::checkBox5Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_10->setText(Amp);
        lineEdit_9->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][4]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][4]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][4]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][4]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][4]=Threshold.yThresholdPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_10->setText(Amp);
        lineEdit_9->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][4]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][4]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][4]=0;
    }
}
void ThresholdSettingWindow::checkBox6Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_12->setText(Amp);
        lineEdit_11->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][5]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][5]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][5]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][5]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][5]=Threshold.yThresholdPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_12->setText(Amp);
        lineEdit_11->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][5]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][5]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][5]=0;
    }
}
void ThresholdSettingWindow::checkBox7Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_14->setText(Amp);
        lineEdit_13->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][6]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][6]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][6]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][6]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][6]=Threshold.yThresholdPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_14->setText(Amp);
        lineEdit_13->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][6]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][6]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][6]=0;
    }
}

void ThresholdSettingWindow::checkBox8Enable(int e)
{
    if(e!=0)
    {
        sprintf(Freq," %.1f  ",Threshold.TFrequency);
        sprintf(Amp," %.1f  ",Threshold.TAmplitude);
        lineEdit_16->setText(Amp);
        lineEdit_15->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][7]=true;
        Threshold.CH_NO_Freq[CurrentChannelNumber][7]=Threshold.TFrequency;
        Threshold.CH_NO_Amp[CurrentChannelNumber][7]=Threshold.TAmplitude;
        CH_NO_FreqPos[CurrentChannelNumber][7]=Threshold.xThresholdPos;
        CH_NO_AmpPos[CurrentChannelNumber][7]=Threshold.yThresholdPos;
    }
    else
    {

        sprintf(Freq," %.1f  ",0);
        sprintf(Amp," %.1f  ",0);
        lineEdit_16->setText(Amp);
        lineEdit_15->setText(Freq);
        Threshold.Init_CH_No[CurrentChannelNumber][7]=false;
        Threshold.CH_NO_Freq[CurrentChannelNumber][7]=0;
        Threshold.CH_NO_Amp[CurrentChannelNumber][7]=0;
    }
}

