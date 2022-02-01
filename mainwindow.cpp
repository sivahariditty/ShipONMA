#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime> 
extern ChannelThreshold Threshold;
extern ChannelTrack CH_Track;
extern RecordingControls RecControl;
extern int16_t CenterOctaveBand[30];
extern int16_t SpectrumZoomCursor,LofarZoomCursor;
extern float SpcetrumBeatFreq,LofarBeatFreq;
char RecorderPathSet[80];
char BufferPath[1024];
char ReplayPathSet[1024];
char RecordPathSet[1024];
QString ContRecFileName;
int ContRecSelCh;
extern int16_t RecordSetFlagCont;
extern int16_t RecordStartFlagCont;
extern int16_t RecordStopFlagCont;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
 //ui->setupUi(this);

    QFont font;
    font.setPointSize(30);
    BaseFrame=new QFrame(this);
    BaseFrame->setMouseTracking(true);
    BaseFrame->setFocusPolicy(Qt::StrongFocus);
    this->setWindowState(Qt::WindowFullScreen);

    ONMAName = new QLabel();
    ONMAName->setObjectName(QString::fromUtf8("label"));
    ONMAName->setGeometry(QRect(400,200,768,150));
    ONMAName->setText("ANALON-SS");
    ONMAName->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(14,38,255);"));
    ONMAName->setFont(font);
    ONMAName->setParent(this);
    ONMAName->show();
    PageID=0;
    setChange[0]=false;
    setChange[1]=false;
    setChange[2]=false;
    setChange[3]=false;

    setLftHarmonicFreq=false;
    setSpecHarmonicFreq=false;
    //this->setGeometry(QRect(0,0,1024,768));
    DrawBasePage(BaseFrame);
    DisplayBase = new DisplayWindow(BaseFrame);
    DisplayBase->DisplayBaseFrame->hide();
    sprintf(ReplayPathSet,"%s","/home");
    sprintf(RecordPathSet,"%s","/home");

    POPThresholdWindow();
    POPTrackWindow();
    RecordWindowSet();
    ReplayWindowSet();
    SensorGroupFrame();
    OctaveSensorGroupFrame();
    drawSelRecChDailog();
    EnergySaveWindow();
    SensorUnitFrame();
    ScreenShotPicture();

    for(iCount=0;iCount<4;iCount++)
    {
    DisplayBase->OctaveSelectGroup[iCount]=iCount;
    }
    sprintf(Buff2[0],"H%d",1);
    sprintf(Buff2[1],"H%d",2);
    sprintf(Buff2[2],"H%d",3);
    sprintf(Buff2[3],"H%d",4);

    CursorSet=0;
    specZoomCursor=0;
    LftZoomCursor=0;

   //Delayed Spectrum
   //Del_Spec_onMouseMove

    connect(DisplayBase->graphPlot->RawDataDelSpectGraphLegend,SIGNAL(mouseMove(QMouseEvent*)),SLOT(Del_Spec_onMouseMove(QMouseEvent*)));
    connect(DisplayBase->graphPlot->RawDataDelSpectGraphLegend,SIGNAL(mousePress(QMouseEvent*)),SLOT(Del_Spec_LegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->RawDataDelSpectGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(Del_Spec_LegendDoubleClickEvent(QMouseEvent*)));

    connect(DisplayBase->graphPlot->LofarGraphLegend,SIGNAL(mousePress(QMouseEvent*)),SLOT(LofarMainLegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(LofarMainLegendDoubleClickEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarGraphLegend,SIGNAL(mouseMove(QMouseEvent*)),SLOT(LofaronMouseMove(QMouseEvent*)));

    connect(DisplayBase->graphPlot->SpectrumGraphLegend,SIGNAL(mouseMove(QMouseEvent*)),SLOT(SpectrumonMouseMove(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumZoomGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(SpectrumZoomLegendDoubleClickEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(SpectrumMainLegendDoubleClickEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarZoomGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(LofarZoomMainLegendDoubleClickEvent(QMouseEvent*)));

    connect(DisplayBase->graphPlot->LofarZoomGraphLegend,SIGNAL(mouseMove(QMouseEvent*)),SLOT(LofarZoomonMouseMove(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarWaterFallLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(LofarWaterfallLegendDoubleClickEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumWaterFallLegend,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(SpectrumWaterfallLegendDoubleClickEvent(QMouseEvent*)));

    connect(DisplayBase->graphPlot->LofarWaterFallLegend,SIGNAL(mousePress(QMouseEvent*)),SLOT(LofarWaterFallLegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarZoomGraphLegend,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(LofarZoomLegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumGraphLegend,SIGNAL(mousePress(QMouseEvent*)),SLOT(SpectrumMainLegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumWaterFallLegend,SIGNAL(mousePress(QMouseEvent*)),SLOT(SpectrumWaterFallLegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumZoomGraphLegend,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(SpectrumZoomLegendPressEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectrumZoomGraphLegend,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(SpectrumZoomLegendMoveEvent(QMouseEvent*)));
    connect(DisplayBase->graphPlot->FirstAction,SIGNAL(triggered()),this,SLOT(HarmonicCursorAction1()));
    connect(DisplayBase->graphPlot->SecondAction,SIGNAL(triggered()),this,SLOT(HarmonicCursorAction2()));
    connect(DisplayBase->graphPlot->ThirdAction,SIGNAL(triggered()),this,SLOT(HarmonicCursorAction3()));
    connect(DisplayBase->Mgc,SIGNAL(activated(int)),this,SLOT(MGC_Slot(int)));
    connect(DisplayBase->Agc,SIGNAL(activated(int)),this,SLOT(AGC_Slot(int)));
    connect(DisplayBase->Preset,SIGNAL(activated(int)),this,SLOT(PRESET_Slot(int)));
    connect(DisplayBase->Track,SIGNAL(activated(int)),this,SLOT(TrackInfo(int)));

    connect(DisplayBase->IssInfo->ScreenShot,SIGNAL(clicked()),this,SLOT(ScreenShotFun()));
    connect(ReplayFiles,SIGNAL(currentIndexChanged(QString)),this,SLOT(FileSelect(QString)));
    connect(EnergyCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(EnergyFileSelect(QString)));
    connect(EnergyFileDelete7,SIGNAL(currentIndexChanged(QString)),this,SLOT(EnergyDeleteFileSelect(QString)));

    connect(ReplaySTART,SIGNAL(clicked()),this, SLOT(ReplayON()));
    connect(PrintScreen, SIGNAL(clicked()), this, SLOT(ScreenShotSave()));
    connect(PrintCancel, SIGNAL(clicked()), this, SLOT(ScreenShotSaveClose()));
    connect(DisplayBase->RecordButton,SIGNAL(clicked()),this,SLOT(RecordStart()));
    connect(DisplayBase->ReplayButton,SIGNAL(clicked()),this,SLOT(ReplayStart()));
    connect(ReorderButton, SIGNAL(clicked()), this, SLOT(RecorderSetParameter()));
    connect(DisplayBase->PageSelection,SIGNAL(activated(int)),this,SLOT(PageSelect(int)));
    connect(SensorGroupCombo[0],SIGNAL(activated(int)),this,SLOT(GroupCHSel1(int)));
    connect(SensorGroupCombo[1],SIGNAL(activated(int)),this,SLOT(GroupCHSel2(int)));
    connect(SensorGroupCombo[2],SIGNAL(activated(int)),this,SLOT(GroupCHSel3(int)));
    connect(SensorGroupCombo[3],SIGNAL(activated(int)),this,SLOT(GroupCHSel4(int)));

    connect(OctaveSensorGroupCombo[0],SIGNAL(activated(int)),this,SLOT(OctaveGroupCHSel1(int)));
    connect(OctaveSensorGroupCombo[1],SIGNAL(activated(int)),this,SLOT(OctaveGroupCHSel2(int)));
    connect(OctaveSensorGroupCombo[2],SIGNAL(activated(int)),this,SLOT(OctaveGroupCHSel3(int)));
    connect(OctaveSensorGroupCombo[3],SIGNAL(activated(int)),this,SLOT(OctaveGroupCHSel4(int)));

   //connect(DisplayBase->IssInfo->HMI,SIGNAL(clicked()),this, SLOT(HMIChange()));

    connect(SensorGroupButton,SIGNAL(clicked()),this, SLOT(SesnorGroupSet()));
    connect(UnitOKButton,SIGNAL(clicked()),this, SLOT(UnitConvertionClose()));
    connect(OctaveSensorGroupButton,SIGNAL(clicked()),this, SLOT(OctaveSesnorGroupSet()));

    connect(HydUnitCombo,SIGNAL(activated(int)),this,SLOT(metricConvertion1(int)));
    connect(AccUnitCombo,SIGNAL(activated(int)),this,SLOT(metricConvertion2(int)));

    connect(DisplayBase->ThresholdSet->pushButton,SIGNAL(clicked()),this,SLOT(setThresholdConfigButton()));
    connect(DisplayBase->ThresholdSet->pushButton_2,SIGNAL(clicked()),this,SLOT(cancelThresholdConfigButton()));
    connect(DisplayBase->TrackWindowSet->OK_BUTTON,SIGNAL(clicked()),this,SLOT(CloseTrackConfigButton()));

    connect(DisplayBase->graphPlot->LofarINT[0],SIGNAL(mousePress(QMouseEvent*)),SLOT(LofarMainLegendPressEvent1(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarINT[1],SIGNAL(mousePress(QMouseEvent*)),SLOT(LofarMainLegendPressEvent2(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarINT[2],SIGNAL(mousePress(QMouseEvent*)),SLOT(LofarMainLegendPressEvent3(QMouseEvent*)));
    connect(DisplayBase->graphPlot->LofarINT[3],SIGNAL(mousePress(QMouseEvent*)),SLOT(LofarMainLegendPressEvent4(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectINT[0],SIGNAL(mousePress(QMouseEvent*)),SLOT(SpectrumIntLegendPressEvent1(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectINT[1],SIGNAL(mousePress(QMouseEvent*)),SLOT(SpectrumIntLegendPressEvent2(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectINT[2],SIGNAL(mousePress(QMouseEvent*)),SLOT(SpectrumIntLegendPressEvent3(QMouseEvent*)));
    connect(DisplayBase->graphPlot->SpectINT[3],SIGNAL(mousePress(QMouseEvent*)),SLOT(SpectrumIntLegendPressEvent4(QMouseEvent*)));
    connect(DisplayBase->graphPlot->OctaveBandLegendPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(OctaveLegrentPressEvent(QMouseEvent*)));

//connect(DisplayBase->graphPlot->SpectINT[0],SIGNAL(mouseMove(QMouseEvent*)),SLOT(SpectrumIntLegendPressEvent(QMouseEvent*)));
//connect(DisplayBase->graphPlot->ThirdAction,SIGNAL(triggered()),this,SLOT(LOFAR_WATERFALL_SET_A3()));

    QTimer::singleShot(2000,this,SLOT(CallStatusPage()));
    QTimer::singleShot(6000,this,SLOT(CallStatusPageClose()));
    UpdateTimer=new QTimer(this);
    connect(UpdateTimer,SIGNAL(timeout()),SLOT(CallUpdateFun()));
    connect(DisplayBase->POPThresoldWindow,SIGNAL(clicked()),this,SLOT(showThresoldWindow()));
    connect(DisplayBase->POPTrackWindow,SIGNAL(clicked()),this,SLOT(showTrackWindow()));
    connect(DisplayBase->EnergyCursorSave,SIGNAL(clicked()),this,SLOT(EnergySaveWindow2()));
    connect(EnergyButton,SIGNAL(clicked()),this,SLOT(EnergySaveWindow3()));

    connect(DisplayBase->OctaveButton,SIGNAL(clicked()),this,SLOT(OctaveGroupWind()));
    connect(DisplayBase->RecChnlButton,SIGNAL(clicked()),this,SLOT(shwChSelDialog()));
    connect(DisplayBase->LFreeze,SIGNAL(clicked()),this,SLOT(freezPlot()));
    connect(DisplayBase->SensorGroup,SIGNAL(clicked()),this,SLOT(SensorGroupWind()));
    connect(DisplayBase->UnitButton,SIGNAL(clicked()),this,SLOT(SenosrUnitConv()));
//connect(DisplayBase->IssInfo->HMI,SIGNAL(clicked()),this,SLOT(HMIChange()));
    connect(DisplayBase->messageBox1,SIGNAL(clicked()),this,SLOT(SetMGroupEnable()));
    connect(StRecBtn,SIGNAL(clicked()),this,SLOT(recChanlData()));
    PageSelect(0);
}

void MainWindow::DrawBasePage(QFrame *BaseFrame)
{
    BaseFrame->setMouseTracking(true);
    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Background,Qt::black);
    BaseFrame->setPalette(*palette);
    BaseFrame->setGeometry(QRect(0,0,1024,768));
}

void MainWindow::freezPlot(){
   if(DisplayBase->SelectedPageID == 0){
      if(DisplayBase->graphPlot->LofDyn == 1) {
         DisplayBase->graphPlot->LofDyn = 0;
	 DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
      else{
         DisplayBase->graphPlot->LofDyn = 1;
	 DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
   }
   else if(DisplayBase->SelectedPageID == 1){
     if(DisplayBase->graphPlot->SpecDyn == 1) {
         DisplayBase->graphPlot->SpecDyn = 0;
         DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
      else{
         DisplayBase->graphPlot->SpecDyn = 1;
         DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }

   }
   else if(DisplayBase->SelectedPageID == 6){
      if(DisplayBase->graphPlot->GDyn == 1) {
         DisplayBase->graphPlot->GDyn = 0;
	 DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
      else{
         DisplayBase->graphPlot->GDyn = 1;
	 DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
   }
   else if(DisplayBase->SelectedPageID == 7){
      if(DisplayBase->graphPlot->CompDyn == 1){
         DisplayBase->graphPlot->CompDyn = 0;
	 DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
      else{
         DisplayBase->graphPlot->CompDyn = 1;
	 DisplayBase->LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
      }
   }
}

void MainWindow::CallStatusPage()
{
 ONMAName->setParent(0);
 ONMAName->hide() ;
 StartPageExecute=new StatusPage(BaseFrame);
 BaseFrame->show();
 DisplayBase->DisplayBaseFrame->hide();
 StartPageExecute->LinkStatusFrame->show();
}

void MainWindow::CallStatusPageClose()
{
UpdateTimer->start(2000);
StartPageExecute->LinkStatusFrame->hide();
DisplayBase->CheckEthernetLinkStatus();
DisplayBase->DisplayBaseFrame->show();
DisplayBase->DisplayBaseFrame->raise();
//QTimer::singleShot(3000,this,SLOT(ReadFileData()));
}

void MainWindow::ReadFileData()
{
 //DisplayBase->ReadEnergyFile(0);
}

void MainWindow::LofarMainLegendPressEvent(QMouseEvent *event)
{
    PageID=0;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    DisplayBase->xPos=xMousePos;
    DisplayBase->TrackXpos=(int32_t)xMousePos;
    DisplayBase->CursorStatus=false;
    CursorPos();
}
//Del_Spec_LegendPressEvent

void MainWindow::Del_Spec_LegendPressEvent(QMouseEvent *event)
{
    PageID=0;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    DisplayBase->xPos=xMousePos;
    DisplayBase->TrackXpos=(int32_t)xMousePos;
    DisplayBase->CursorStatus=false;
    CursorPos();
}


void MainWindow::LofarMainLegendPressEvent1(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[0];
    PageID=0;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();

 //printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->LofarINT[0]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->LofarINT[0]->yAxis->pixelToCoord(yMousePos);
 //DisplayBase->graphPlot->HarmonicCursorFrame->show();
   }

   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->LofarINT[0]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->LofarINT[0]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();

   }
}

void MainWindow::LofarMainLegendPressEvent2(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[1];
    PageID=0;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
      // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->LofarINT[1]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->LofarINT[1]->yAxis->pixelToCoord(yMousePos);
     //DisplayBase->graphPlot->HarmonicCursorFrame->show();
   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->LofarINT[1]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->LofarINT[1]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();
   }
}

void MainWindow::LofarMainLegendPressEvent3(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[2];

    PageID=0;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
      // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->LofarINT[2]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->LofarINT[2]->yAxis->pixelToCoord(yMousePos);
      //DisplayBase->graphPlot->HarmonicCursorFrame->show();

   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->LofarINT[2]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->LofarINT[2]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();
   }
}
void MainWindow::LofarMainLegendPressEvent4(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[3];

    PageID=0;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
      // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->LofarINT[3]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->LofarINT[3]->yAxis->pixelToCoord(yMousePos);
     //DisplayBase->graphPlot->HarmonicCursorFrame->show();
   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->LofarINT[3]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->LofarINT[3]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();
   }
}



void MainWindow::SpectrumMainLegendPressEvent(QMouseEvent *event)
{
    PageID=1;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    DisplayBase->xPos=xMousePos;
    DisplayBase->CursorStatus=false;
    CursorPos();
}


void MainWindow::LofarWaterFallLegendPressEvent(QMouseEvent *event)
{
    PageID=0;
    if(event->buttons() == Qt::RightButton)
    {
        xMousePos=(float)event->pos().x();
        yMousePos=(float)event->pos().y();

        if((yMousePos>18) && (yMousePos<232))
        {
        if((xMousePos>37)&&(xMousePos<985))
        {
       // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
        xFreq=DisplayBase->graphPlot->LofarWaterFallLegend->xAxis->pixelToCoord(xMousePos);
        yAmp=DisplayBase->graphPlot->LofarWaterFallLegend->yAxis->pixelToCoord(yMousePos);

           DisplayBase->graphPlot->HarmonicCursormenu->raise();
           DisplayBase->graphPlot->HarmonicCursormenu->activateWindow();
           DisplayBase->graphPlot->HarmonicCursormenu->setGeometry(xMousePos,460,180,70);
           DisplayBase->graphPlot->HarmonicCursormenu->show();
        }
        }
    // DisplayBase->graphPlot->HarmonicCursorFrame->show();

    }
    else
    {
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->LofarGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->LofarGraphLegend->yAxis->pixelToCoord(yMousePos);
    DisplayBase->checkGain();
    sprintf(DisplayBase->freqVal," %.1f Hz ",xFreq);
    sprintf(DisplayBase->ampVal," %.1f dB ",yAmp);
    if((setLftHarmonicFreq==true))
    {
    DisplayBase->LofarHarmonicCursorPos=xMousePos;
   // for(iCount=0;iCount<5;iCount++)
   // {
     //DisplayBase->graphPlot->HarmonicCursor[iCount]->hide();
   // }
   // DisplayBase->DrawHarmonicCursorLines(0);
    }
    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);
    DisplayBase->GainVal->setText(DisplayBase->gainVal);
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
    DisplayBase->GainVal->show();

    }
}
void MainWindow::SpectrumIntLegendPressEvent1(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[0];
    PageID=1;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
      // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
     //DisplayBase->graphPlot->HarmonicCursorFrame->show();

   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();

   }
}
void MainWindow::SpectrumIntLegendPressEvent2(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[1];

    PageID=1;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
      // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);

 //DisplayBase->graphPlot->HarmonicCursorFrame->show();

   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();

   }
}

void MainWindow::SpectrumIntLegendPressEvent3(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[2];

    PageID=1;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
   //printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
   //DisplayBase->graphPlot->HarmonicCursorFrame->show();

   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();

   }
}

void MainWindow::OctaveLegrentPressEvent(QMouseEvent *event)
{
    int16_t u;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->OctaveBandLegendPlot->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->OctaveBandLegendPlot->yAxis->pixelToCoord(yMousePos);
    DisplayBase->checkGain();
    u=(int16_t)xFreq;
    sprintf(DisplayBase->freqVal," %d Hz ",CenterOctaveBand[u]);

   // sprintf(DisplayBase->ampVal," %.1f dB ",(200-(604-(768-(20*(log2(yAmp))))))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
     sprintf(DisplayBase->ampVal," %.1f dB ",(87+(114-(20*log10(500/yAmp))))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   // DisplayBase->GainVal->setText(DisplayBase->gainVal);
    DisplayBase->AmpVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->GainVal->hide();
    DisplayBase->DisplayGain->hide();
    DisplayBase->DisplayAmp->show();
    DisplayBase->DisplayGain->hide();
    DisplayBase->DisplayFreq->show();
}


void MainWindow::SpectrumIntLegendPressEvent4(QMouseEvent *event)
{
    DisplayBase->CurrentChannelSel=DisplayBase->MGroup[3];

    PageID=1;
   if(event->buttons() == Qt::RightButton)
   {
       xMousePos=(float)event->pos().x();
       yMousePos=(float)event->pos().y();
 //printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
       xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
 //DisplayBase->graphPlot->HarmonicCursorFrame->show();
   }
   else
   {
   xMousePos=(float)event->pos().x();
   yMousePos=(float)event->pos().y();
   xFreq=DisplayBase->graphPlot->SpectINT[0]->xAxis->pixelToCoord(xMousePos);
   yAmp=DisplayBase->graphPlot->SpectINT[0]->yAxis->pixelToCoord(yMousePos);
   DisplayBase->checkGain();
   DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
   DisplayBase->FreqVal->setText(DisplayBase->freqVal);
   DisplayBase->AmpVal->setText(DisplayBase->ampVal);
   DisplayBase->GainVal->setText(DisplayBase->gainVal);
   DisplayBase->AmpVal->show();
   DisplayBase->DisplayAmp->show();
   DisplayBase->GainVal->show();

   }
}
void MainWindow::SpectrumWaterFallLegendPressEvent(QMouseEvent *event)
{
     PageID=1;
    if(event->buttons() == Qt::RightButton)
    {xMousePos=(float)event->pos().x();
        yMousePos=(float)event->pos().y();

        if((yMousePos>18) && (yMousePos<232))
        {
        if((xMousePos>37)&&(xMousePos<985))
        {
       // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
        xFreq=DisplayBase->graphPlot->SpectrumWaterFallLegend->xAxis->pixelToCoord(xMousePos);
        yAmp=DisplayBase->graphPlot->SpectrumWaterFallLegend->yAxis->pixelToCoord(yMousePos);

           DisplayBase->graphPlot->HarmonicCursormenu->raise();
           DisplayBase->graphPlot->HarmonicCursormenu->activateWindow();
           DisplayBase->graphPlot->HarmonicCursormenu->setGeometry(xMousePos+110,460,180,70);
           DisplayBase->graphPlot->HarmonicCursormenu->show();
        }
        }
    // DisplayBase->graphPlot->HarmonicCursorFrame->show();

    }
    else
    {
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->SpectrumGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->SpectrumGraphLegend->yAxis->pixelToCoord(yMousePos);
    DisplayBase->checkGain();
    sprintf(DisplayBase->freqVal," %.1f Hz ",xFreq);
    //sprintf(DisplayBase->ampVal," %.1f dB ",yAmp);
    if(setSpecHarmonicFreq==true)
    {
    DisplayBase->SpectrumHarmonicCursorPos=xMousePos;
 // DisplayBase->DrawHarmonicCursorLines(1);
    }
    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);
    DisplayBase->GainVal->setText(DisplayBase->gainVal);
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
    DisplayBase->GainVal->show();
    }
}


void MainWindow::LofarZoomLegendPressEvent(QMouseEvent *event)
{
    float y;

    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->LofarZoomGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->LofarZoomGraphLegend->yAxis->pixelToCoord(yMousePos);
    if(DisplayBase->CurrentChannelSel < 29)
    {
        if(LofarZoomCursor==200)
        {
        // y=(yAmp/DisplayBase->CHGainFloatValue);
        // DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
            y=(yAmp);
            DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
            DisplayBase->DispFreq=(xFreq*0.4765)+(LofarBeatFreq);
            DisplayBase->DispAmp=(DisplayBase->InterValue);
            sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
            sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
        else if(LofarZoomCursor==100)
        {
      //y=(yAmp/DisplayBase->CHGainFloatValue);
      //DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
            y=(yAmp);
            DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
            DisplayBase->DispFreq=(xFreq*0.23825)+(LofarBeatFreq);
            DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
      //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
        else
        {

    //y=(yAmp/DisplayBase->CHGainFloatValue);
    //DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
            y=(yAmp);
            DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
            DisplayBase->DispFreq=(xFreq*0.119125)+(LofarBeatFreq);
            DisplayBase->DispAmp=(DisplayBase->InterValue);
            sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
          //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
            sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
    }
    else
    {
    if(LofarZoomCursor==200)
    {

    DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
    DisplayBase->DispFreq=(xFreq*0.4765)+(LofarBeatFreq);;
    DisplayBase->DispAmp=( DisplayBase->InterValue);

    sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
 // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    else if(LofarZoomCursor==100)
    {
        DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
        DisplayBase->DispFreq=(xFreq*0.23825)+(LofarBeatFreq);;
        DisplayBase->DispAmp=( DisplayBase->InterValue);

        sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
       // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

    }
     else
    {
        DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
        DisplayBase->DispFreq=(xFreq*0.119125)+(LofarBeatFreq);;
        DisplayBase->DispAmp=( DisplayBase->InterValue);

        sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
     // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf( DisplayBase->ampVal," %.3f g ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

    }
    }
    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);
    DisplayBase->AmpVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->GainVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->DisplayGain->show();
    DisplayBase->DisplayFreq->show();

    if((xMousePos>40 && xMousePos<735) && (yMousePos>17 && yMousePos<224) )
    {
        DisplayBase->ZoomDisplayVFrame->move(xMousePos,15);
        DisplayBase->ZoomDisplayHFrame->move(30,yMousePos);
    }
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
}
void MainWindow::SpectrumZoomLegendPressEvent(QMouseEvent *event)
{
float y;
    PageID=1;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->SpectrumZoomGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->SpectrumZoomGraphLegend->yAxis->pixelToCoord(yMousePos);

    if(DisplayBase->CurrentChannelSel < 29)
    {
    if(SpectrumZoomCursor==244)
    {
       //y=(yAmp/DisplayBase->CHGainFloatValue);
        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
     // DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
        DisplayBase->DispFreq=(xFreq*0.4765)+(SpcetrumBeatFreq);
        DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
      //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    else
    {
     //y=(yAmp/DisplayBase->CHGainFloatValue);
     //DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
        DisplayBase->DispFreq=(xFreq*0.23825)+(SpcetrumBeatFreq);
        DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    }
    else
    {

        if(SpectrumZoomCursor==244)
        {
        DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
        DisplayBase->DispFreq=(xFreq*0.4765)+(SpcetrumBeatFreq);;
        DisplayBase->DispAmp=( DisplayBase->InterValue);

        sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
     // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
         else
        {
            DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
            DisplayBase->DispFreq=(xFreq*0.23825)+(SpcetrumBeatFreq);;
            DisplayBase->DispAmp=( DisplayBase->InterValue);
            sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
         //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
            sprintf( DisplayBase->ampVal," %.3f g ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
    }
    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);
    DisplayBase->AmpVal->show();
     DisplayBase->DisplayAmp->show();
     DisplayBase->DisplayGain->show();
     DisplayBase->DisplayFreq->show();

    if((xMousePos>37 && xMousePos<984) && (yMousePos>17 && yMousePos<224) )
    {
        DisplayBase->ZoomDisplayVFrame->move(xMousePos,15);
        DisplayBase->ZoomDisplayHFrame->move(30,yMousePos);
    }

    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();

}
void MainWindow::SpectrumZoomLegendMoveEvent(QMouseEvent *event)
{
    float y;
    if(specZoomCursor==0)
    {
    PageID=1;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->SpectrumZoomGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->SpectrumZoomGraphLegend->yAxis->pixelToCoord(yMousePos);

    if(DisplayBase->CurrentChannelSel < 29)
    {

    if(SpectrumZoomCursor==244)
    {
     // y=(yAmp/DisplayBase->CHGainFloatValue);
     // DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));

        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
        DisplayBase->DispFreq=(xFreq*0.4765)+(SpcetrumBeatFreq);
        DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    else
    {
       // y=(yAmp/DisplayBase->CHGainFloatValue);
       // DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
        DisplayBase->DispFreq=(xFreq*0.23825)+(SpcetrumBeatFreq);
        DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    }
    else
    {
        if(SpectrumZoomCursor==244)
        {
        DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
        DisplayBase->DispFreq=(xFreq*0.4765)+(SpcetrumBeatFreq);;
        DisplayBase->DispAmp=( DisplayBase->InterValue);

        sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
       // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
         else
        {
            DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
            DisplayBase->DispFreq=(xFreq*0.23825)+(SpcetrumBeatFreq);;
             DisplayBase->DispAmp=( DisplayBase->InterValue);

            sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
         // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
            sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp-DisplayBase->yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
    }

    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);

    DisplayBase->AmpVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->GainVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->DisplayGain->show();
    DisplayBase->DisplayFreq->show();
    if((xMousePos>40 && xMousePos<984) && (yMousePos>17 && yMousePos<224) )
    {
        DisplayBase->ZoomDisplayVFrame->move(xMousePos,15);
        DisplayBase->ZoomDisplayHFrame->move(30,yMousePos);
    }
    }
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
}

void MainWindow::CallUpdateFun()
{
DisplayBase->ResetFlags();
DisplayBase->CheckEthernetLinkStatus();
DisplayBase->ThresholdCheck();
//if(DisplayBase->SensorGroupEnable==true)
//{
//DisplayBase->EnableChannelGroup(DisplayBase->MGroupEnable);
//}
DisplayBase->ThresholdAlarmDisplay();
DisplayBase->HydEnergyCheck();
//DisplayBase->UpdateSonarInformation();
SetDisplayTimer();
for(iCount=0;iCount<29;iCount++)
{
DisplayBase->ChangeEnergyValue(iCount,3);
}
//StorageCapcity();
DisplayBase->MemCheck();
//QTimer::singleShot(000,this,SLOT(UpdateDisplayPage()));
}

void MainWindow::UpdateDisplayPage()
{

DisplayBase->UpdateTimeInfo();
DisplayBase->UpdateSpectrumDisplay();
DisplayBase->RecordTimeEnableFun();
}

void MainWindow::SetDisplayTimer()
{

     if(DisplayBase->SetTimer==2000)
     {
       //  DisplayBase->TimeValue=(DisplayBase->TimeValue+1)%1;
         if(DisplayBase->TimeValue==0)
         {
          UpdateDisplayPage();
         }

     }
     else if(DisplayBase->SetTimer==4000)
     {
         DisplayBase->TimeValue=(DisplayBase->TimeValue+1)%2;
         if(DisplayBase->TimeValue==0)
         {
          UpdateDisplayPage();
         }
     }
     else
     {
         DisplayBase->TimeValue=(DisplayBase->TimeValue+1)%4;
         if(DisplayBase->TimeValue==0)
         {
          UpdateDisplayPage();
         }
     }


}



void MainWindow::EnergySaveWindow2()
{
EnergyLineEdit->clear();
EnergyFileSave->setCheckable(true);
EnergyFileDelete1->setCheckable(true);
EnergyFileRestore->setCheckable(true);
EnergyWindowFileist();
EnergyDialog->show();
}
void MainWindow::EnergySaveWindow3()
{

    int16_t Status=0;


     if(EnergyFileSave->isChecked())
       {
        Status=1;
       }
     else if(EnergyFileRestore->isChecked())
       {
        Status=2;
       }
     else if(EnergyFileDelete1->isChecked())
       {
        Status=3;
       }
     else
        {
         Status=0;
        }


     switch(Status)
     {

     case 0:
                break;

     case 1:
                EnergyPathSettings();
                DisplayBase->EnergyRecordEnableFun();
                break;
     case 2:
                EnergyReplaySettings();
                DisplayBase->EnergyRecordReplayFun();
                break;

     case 3:
                EnergyFileDeleteSettings();
                break;

     default: break;
     };


EnergyDialog->close();
}
void MainWindow::EnergyPathSettings()
{
   char Buffer[400];
   int Size;
   QString FileName;
   FileName=EnergyLineEdit->text();
   QString FileDir = "/"+FileName;
   strcpy(RecControl.EnergyFilePath,RecControl.EnergyPath);
   strcpy(Buffer,FileDir.toStdString().c_str());
   strcat(RecControl.EnergyFilePath,Buffer);
   strcat(RecControl.EnergyFilePath,".bin");
   Size=sizeof(RecControl.EnergyFilePath);
   if((Size > 400) || (FileDir < 0))
   {
       printf("\n File Name Error 0000");
   }
  // printf("\n Channel File Name=%s  %d \n",Graph_plt1.RecorderFilePath,sizeof(Graph_plt1.RecorderFilePath));
}
void MainWindow::EnergyReplaySettings()
{
    char Buffer[400];
   // qDebug()<< ReplayFileSelect;
    QDir Path(RecControl.EnergyPath);
    QString FileDir = "/EnergyStorage/"+EnergyReplytFIleSelect;
    strcpy(RecControl.EnergyReplayFilePath,RecControl.CurrentDir);
    strcpy(Buffer,FileDir.toStdString().c_str());
    strcat(RecControl.EnergyReplayFilePath,Buffer);
//   printf("\n Path=%s",RecControl.ReplayFilePath);
    // DisplayBase->ReplayEnableFun();
}
void MainWindow::EnergyFileDeleteSettings()
{
   char Buffer[400];
   // qDebug()<< ReplayFileSelect;
    QDir Path(RecControl.EnergyPath);
    QString FileDir = "/EnergyStorage/"+EnergyDeletetFIleSelect;
    strcpy(RecControl.EnergyReplayFilePath,RecControl.CurrentDir);
    strcpy(Buffer,FileDir.toStdString().c_str());
    strcat(RecControl.EnergyReplayFilePath,Buffer);
  Path.remove(RecControl.EnergyReplayFilePath);
 // qDebug() << ReplayFileSelect;
}
void MainWindow::showThresoldWindow()
{

    DisplayBase->ThresholdSet->ThresholdWindow->show();
    DisplayBase->ThresholdSet->ThresholdWindow->raise();
    DisplayBase->ThresholdSet->ThresholdWindow->activateWindow();
        thresholdPopUpWindow->show();
        thresholdPopUpWindow->activateWindow();
        thresholdPopUpWindow->raise();
}

void MainWindow::showTrackWindow()
{
  //DisplayBase->TrackWindowSet->trackWindow->setParent(this);
    DisplayBase->TrackWindowSet->trackWindow->show();
    DisplayBase->TrackWindowSet->trackWindow->raise();
    DisplayBase->TrackWindowSet->trackWindow->activateWindow();
  //BaseFrame->activateWindow();
    trackPopUpWindow->show();
    trackPopUpWindow->activateWindow();
    trackPopUpWindow->raise();
}

void MainWindow::HMIChange()
{
 //system("killall -KILL x11vnc");
}




void MainWindow::SensorUnitFrame()
{
    char CHASS[20];
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);

    UnitCon=new QDialog(this);
    UnitCon->setWindowTitle(" METRIC UNIT CONVERTION ");
    UnitCon->setGeometry(QRect(150,100,400,260));
    UnitFrame=new QFrame(UnitCon) ;
    UnitFrame->setMouseTracking(true);
    UnitFrame->setFocusPolicy(Qt::StrongFocus);
  //  DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
    UnitFrame->setGeometry(QRect(0,0,400,260));

    UnitFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
    UnitFrame->setFrameShape(QFrame::StyledPanel);
    UnitFrame->setFrameShadow(QFrame::Raised);


    HydLabel7= new QLabel(UnitFrame);
    HydLabel7->setObjectName(QString::fromUtf8("label"));
    HydLabel7->setGeometry(QRect(10, 31, 200, 16));
    HydLabel7->setFont(font);
    HydLabel7->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
    HydLabel7->setText("Acoustic Sesnor Scale Set :");
    HydLabel7->show();



    AccLabel7= new QLabel(UnitFrame);
    AccLabel7->setObjectName(QString::fromUtf8("label"));
    AccLabel7->setGeometry(QRect(10,100,200,16));
    AccLabel7->setFont(font);
    AccLabel7->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
    AccLabel7->setText("Vibration Sesnor Scale Set :");
    AccLabel7->show();


        HydUnitCombo=new QComboBox(UnitFrame);
        HydUnitCombo->setGeometry(QRect(190, 28, 155, 20));
        HydUnitCombo->setFont(font);
        HydUnitCombo->clear();
        HydUnitCombo->setWindowModality(Qt::WindowModal);
        HydUnitCombo->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                     "color: rgb(0,0,0);"));
        HydUnitCombo->clear();

        HydUnitCombo->addItem("dB");
        HydUnitCombo->addItem("mV(pp)");
        AccUnitCombo=new QComboBox(UnitFrame);
        AccUnitCombo->setGeometry(QRect(190,97, 155, 20));
        AccUnitCombo->setFont(font);
        AccUnitCombo->clear();
        AccUnitCombo->setWindowModality(Qt::WindowModal);
        AccUnitCombo->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                     "color: rgb(0,0,0);"));
        AccUnitCombo->clear();


        AccUnitCombo->addItem("Acceleration(g)");
        AccUnitCombo->addItem("Velocity(m/sec)");
        AccUnitCombo->addItem("Displacement(mm)");

    UnitOKButton = new QPushButton(UnitFrame);
    UnitOKButton->setObjectName(QString::fromUtf8("pushButton"));
    UnitOKButton->setGeometry(QRect(150, 210, 84, 25));
    UnitOKButton->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 160);"));
    UnitOKButton->setText("OK");
}

void MainWindow::SensorGroupFrame()
{
    char CHASS[20];
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);

    SenGroup=new QDialog(this);
    SenGroup->setWindowTitle(" SENSOR GROUP SELECTION ");
    SenGroup->setGeometry(QRect(150,100,400,260));
    SenGroupFrame=new QFrame(SenGroup) ;
    SenGroupFrame->setMouseTracking(true);
    SenGroupFrame->setFocusPolicy(Qt::StrongFocus);
  //  DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
    SenGroupFrame->setGeometry(QRect(0,0,400,260));

    SenGroupFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
    SenGroupFrame->setFrameShape(QFrame::StyledPanel);
    SenGroupFrame->setFrameShadow(QFrame::Raised);
    for(iCount=0;iCount<4;iCount++)
    {
       SensorGroupLabel[iCount]= new QLabel(SenGroupFrame);
       SensorGroupLabel[iCount]->setObjectName(QString::fromUtf8("label"));
       SensorGroupLabel[iCount]->setGeometry(QRect(15, 17,135, 20));
        SensorGroupLabel[iCount]->setFont(font);
        SensorGroupLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
            "color: rgb(0,0,0);"));
       SensorGroupLabel[iCount]->setText("SELECT REPLAY FILE");
       SensorGroupLabel[iCount]->show();
    SensorGroupCombo[iCount]=new QComboBox(SenGroupFrame);
    SensorGroupCombo[iCount]->setGeometry(QRect(160, 17, 145, 20));
    SensorGroupCombo[iCount]->setFont(font);
    SensorGroupCombo[iCount]->clear();
    SensorGroupCombo[iCount]->setWindowModality(Qt::WindowModal);
    SensorGroupCombo[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                 "color: rgb(0,0,0);"));
    SensorGroupCombo[iCount]->clear();
    }

    SensorGroupLabel[0]->setText("CHANNEL 1");
    SensorGroupLabel[0]->setGeometry(QRect(15,40,145,20));
    SensorGroupLabel[1]->setText("CHANNEL 2");
    SensorGroupLabel[1]->setGeometry(QRect(15,80,145,20));
    SensorGroupLabel[2]->setText("CHANNEL 3");
    SensorGroupLabel[2]->setGeometry(QRect(15,120,145,20));
    SensorGroupLabel[3]->setText("CHANNEL 4");
    SensorGroupLabel[3]->setGeometry(QRect(15,160,145,20));

    SensorGroupCombo[0]->setGeometry(QRect(120,40,145,20));
    SensorGroupCombo[1]->setGeometry(QRect(120,80,145,20));
    SensorGroupCombo[2]->setGeometry(QRect(120,120,145,20));
    SensorGroupCombo[3]->setGeometry(QRect(120,160,145,20));

    for(iCount=0;iCount<61;iCount++)
    {
       if(iCount<29)
       {
       sprintf(CHASS,"H%d",iCount+1);
       }
       else
       {
       sprintf(CHASS,"A%d",((iCount-29)+1));
       }
       SensorGroupCombo[0]->addItem(CHASS);
       SensorGroupCombo[1]->addItem(CHASS);
       SensorGroupCombo[2]->addItem(CHASS);
       SensorGroupCombo[3]->addItem(CHASS);
    }

    SensorGroupButton = new QPushButton(SenGroupFrame);
    SensorGroupButton->setObjectName(QString::fromUtf8("pushButton"));
    SensorGroupButton->setGeometry(QRect(150, 210, 84, 25));
    SensorGroupButton->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 160);"));
    SensorGroupButton->setText("OK");
}

void MainWindow::drawSelRecChDailog(){
   QFont font;
   char CHASS[20];
   font.setFamily(QString::fromUtf8("Sans Serif"));
   font.setBold(false);
   font.setPointSize(10);
   font.setPixelSize(12);
   SelectRecChanel = new QDialog(this);
   SelectRecChanel->setWindowTitle("Select Channel to Record");
   SelectRecChanel->setGeometry(QRect(150,100,600,260));
   SelectRecChanelFrame=new QFrame(SelectRecChanel) ;
   SelectRecChanelFrame->setMouseTracking(true);
   SelectRecChanelFrame->setFocusPolicy(Qt::StrongFocus);
   SelectRecChanelFrame->setGeometry(QRect(0,0,600,260));
   SelectRecChanelFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
   SelectRecChanelFrame->setFrameShape(QFrame::StyledPanel);
   SelectRecChanelFrame->setFrameShadow(QFrame::Raised);
   SelectChanRec = new QLabel(SelectRecChanelFrame);
   SelectChanRec->setObjectName(QString::fromUtf8("label"));
   SelectChanRec->setGeometry(QRect(15, 17,135, 20));
   SelectChanRec->setFont(font);
   SelectChanRec->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
            "color: rgb(0,0,0);"));
   SelectChanRec->setText("Select Channel to Record");
   SelectChanRec->show();
   RecChSel = new QComboBox(SelectRecChanelFrame);
   RecChSel->setGeometry(QRect(180, 17, 145, 20));
   RecChSel->setFont(font);
   RecChSel->clear();
   RecChSel->setWindowModality(Qt::WindowModal);
   RecChSel->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                 "color: rgb(0,0,0);"));
   RecChSel->clear();

   for(iCount=0;iCount<29;iCount++){
      sprintf(CHASS,"H%d",iCount+1);
      RecChSel->addItem(CHASS,iCount);
   }
   StRecBtn = new QPushButton(SelectRecChanelFrame);
   StRecBtn->setObjectName(QString::fromUtf8("pushButton"));
   StRecBtn->setGeometry(QRect(180, 100, 100, 35));
   StRecBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 160);"));
   StRecBtn->setText("Start Record");
}

void MainWindow::OctaveSensorGroupFrame()
{
    char CHASS[20];
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);

    OctaveSenGroup=new QDialog(this);
    OctaveSenGroup->setWindowTitle(" 1/3 OCTAVE SENSOR GROUP SELECTION ");
    OctaveSenGroup->setGeometry(QRect(150,100,600,260));
    OctaveSenGroupFrame=new QFrame(OctaveSenGroup) ;
    OctaveSenGroupFrame->setMouseTracking(true);
    OctaveSenGroupFrame->setFocusPolicy(Qt::StrongFocus);
  //DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
    OctaveSenGroupFrame->setGeometry(QRect(0,0,600,260));

    OctaveSenGroupFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
    OctaveSenGroupFrame->setFrameShape(QFrame::StyledPanel);
    OctaveSenGroupFrame->setFrameShadow(QFrame::Raised);
    for(iCount=0;iCount<4;iCount++)
    {
       OctaveSensorGroupLabel[iCount]= new QLabel(OctaveSenGroupFrame);
       OctaveSensorGroupLabel[iCount]->setObjectName(QString::fromUtf8("label"));
       OctaveSensorGroupLabel[iCount]->setGeometry(QRect(15, 17,135, 20));
        OctaveSensorGroupLabel[iCount]->setFont(font);
        OctaveSensorGroupLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
            "color: rgb(0,0,0);"));
       OctaveSensorGroupLabel[iCount]->setText("Select Channel Graph");
       OctaveSensorGroupLabel[iCount]->show();
    OctaveSensorGroupCombo[iCount]=new QComboBox(OctaveSenGroupFrame);
    OctaveSensorGroupCombo[iCount]->setGeometry(QRect(130, 17, 145, 20));
    OctaveSensorGroupCombo[iCount]->setFont(font);
    OctaveSensorGroupCombo[iCount]->clear();
    OctaveSensorGroupCombo[iCount]->setWindowModality(Qt::WindowModal);
    OctaveSensorGroupCombo[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                 "color: rgb(0,0,0);"));
    OctaveSensorGroupCombo[iCount]->clear();

          octavecheckRecBox[iCount] = new QCheckBox(OctaveSenGroupFrame);
          octavecheckRecBox[iCount]->setObjectName(QString::fromUtf8("checkBox"));
          octavecheckRecBox[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
              "color: rgb(0,0,0);"));
          octavecheckRecBox[iCount]->setGeometry(QRect(300,60, 87, 20));
          octaveBarcheckRecBox[iCount] = new QCheckBox(OctaveSenGroupFrame);
          octaveBarcheckRecBox[iCount]->setObjectName(QString::fromUtf8("checkBox"));
          octaveBarcheckRecBox[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
              "color: rgb(0,0,0);"));
          octaveBarcheckRecBox[iCount]->setGeometry(QRect(400,60, 87, 20));

    }

    OctaveSensorGroupLabel[0]->setText("Select Graph 1");
    OctaveSensorGroupLabel[0]->setGeometry(QRect(15,40,145,20));
    OctaveSensorGroupLabel[1]->setText("Select Graph 2");
    OctaveSensorGroupLabel[1]->setGeometry(QRect(15,80,145,20));
    OctaveSensorGroupLabel[2]->setText("Select Graph 3");
    OctaveSensorGroupLabel[2]->setGeometry(QRect(15,120,145,20));
    OctaveSensorGroupLabel[3]->setText("Select Graph 4");
    OctaveSensorGroupLabel[3]->setGeometry(QRect(15,160,145,20));

    OctaveSensorGroupCombo[0]->setGeometry(QRect(120,40,145,20));
    OctaveSensorGroupCombo[1]->setGeometry(QRect(120,80,145,20));
    OctaveSensorGroupCombo[2]->setGeometry(QRect(120,120,145,20));
    OctaveSensorGroupCombo[3]->setGeometry(QRect(120,160,145,20));

    octavecheckRecBox[0]->setGeometry(QRect(350,40,145,20));
    octavecheckRecBox[1]->setGeometry(QRect(350,80,145,20));
    octavecheckRecBox[2]->setGeometry(QRect(350,120,145,20));
    octavecheckRecBox[3]->setGeometry(QRect(350,160,145,20));

    octaveBarcheckRecBox[0]->setGeometry(QRect(490,40,145,20));
    octaveBarcheckRecBox[1]->setGeometry(QRect(490,80,145,20));
    octaveBarcheckRecBox[2]->setGeometry(QRect(490,120,145,20));
    octaveBarcheckRecBox[3]->setGeometry(QRect(490,160,145,20));


    lineGraph= new QLabel(OctaveSenGroupFrame);
    lineGraph->setObjectName(QString::fromUtf8("label"));
    lineGraph->setGeometry(QRect(300,18,145,20));
    lineGraph->setFont(font);
    lineGraph->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
    lineGraph->setText("Line Graph Enable");
    lineGraph->show();

    BarGraph= new QLabel(OctaveSenGroupFrame);
    BarGraph->setObjectName(QString::fromUtf8("label"));
    BarGraph->setGeometry(QRect(450,18,145,20));
    BarGraph->setFont(font);
    BarGraph->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
    BarGraph->setText("Bar Graph Enable");
    BarGraph->show();
    for(iCount=0;iCount<61;iCount++)
    {
       if(iCount<29)
       {
       sprintf(CHASS,"H%d",iCount+1);
       }
       else
       {
       sprintf(CHASS,"A%d",((iCount-29)+1));
       }
       OctaveSensorGroupCombo[0]->addItem(CHASS);
       OctaveSensorGroupCombo[1]->addItem(CHASS);
       OctaveSensorGroupCombo[2]->addItem(CHASS);
       OctaveSensorGroupCombo[3]->addItem(CHASS);
    }
    OctaveSensorGroupButton = new QPushButton(OctaveSenGroupFrame);
    OctaveSensorGroupButton->setObjectName(QString::fromUtf8("pushButton"));
    OctaveSensorGroupButton->setGeometry(QRect(150, 210, 84, 25));
    OctaveSensorGroupButton->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 160);"));
    OctaveSensorGroupButton->setText("OK");

}


void MainWindow::SenosrUnitConv()
{
    char Buffer[200];
    int BinFileSize;

     QFileDialog dialog(this);
     dialog.setFileMode(QFileDialog::DirectoryOnly);
     QString strFileName = dialog.getOpenFileName(this,tr("Select File"), "", tr("All Files (*.*)"));
     strcpy(Buffer,strFileName.toStdString().c_str());
     printf("\n Path %s",Buffer);

  //BinFileSize = BinFileNames.size();
  /*  QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open"),QDir::currentPath(),"All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    if( !fileName.isNull() )
       {
         qDebug() << "selected file path : " << fileName.toUtf8();
       }
    QDebug() << fileName.toStdString().c_str();
*/

}

void MainWindow::SensorGroupWind()
{

  SenGroup->show();
}
void MainWindow::OctaveGroupWind()
{
  OctaveSenGroup->show();
}

void MainWindow::recChanlData(){
       ContRecSelCh = RecChSel->itemData(RecChSel->currentIndex()).toInt();
       QDateTime now = QDateTime::currentDateTime();
       QString now_s = now.toString("dd.MM.yyyy_hh:mm:ss.zzz");
       //ContRecSelCh = 10;
       QFileDialog dialog(this);
       dialog.setFileMode(QFileDialog::DirectoryOnly);
       dialog.setStyleSheet("background-color:white");
       QString strFileName = dialog.getSaveFileName(this,tr("Save File"),QString(RecChSel->itemText(RecChSel->currentIndex()) +"_"+now_s+".txt"), tr("All Files (*.txt)"));
       ContRecFileName = strFileName;
       SelectRecChanel->close();
       RecordStopFlagCont = 0;
       DisplayBase->RecChnlButton->setText("STOP REC");
       DisplayBase->RecChnlButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255,141,133);\n"
                                                  "color: rgb(0, 0, 0);"));

       RecordStartFlagCont = 1;
       RecordSetFlagCont = 1;
}

void MainWindow::shwChSelDialog(){
   if(RecordStartFlagCont == 0){
      SelectRecChanel->show();
   }
   else if(RecordStopFlagCont == 0){
      RecordStopFlagCont = 1;
      RecordStartFlagCont = 0;
      DisplayBase->RecChnlButton->setText("CHNL REC");
      DisplayBase->RecChnlButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));

   }
}

void MainWindow::metricConvertion1(int ID)
{
    switch(ID)
    {
    case 0:   DisplayBase->HydrophoneScale=0;
              break;

    case 1:   DisplayBase->HydrophoneScale=1;
              break;

    default: break;
    };
}

void MainWindow::metricConvertion2(int ID)
{
    switch(ID)
    {
    case 0:   DisplayBase->AccelerometerScale=0;
              break;

    case 1:   DisplayBase->AccelerometerScale=1;
              break;

    case 2:   DisplayBase->AccelerometerScale=2;
              break;

    default: break;
    };
}


void MainWindow::OctaveGroupCHSel1(int ID)
{
    DisplayBase->OctaveSelectGroup[0]=ID;
   if(ID<29)
   {
       sprintf(Buff2[0],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff2[0],"A%d",((ID-29)+1));

   }
     OctvaesetChange[0]=true;
}

void MainWindow::OctaveGroupCHSel2(int ID)
{
   DisplayBase->OctaveSelectGroup[1]=ID;
   if(ID<29)
   {
       sprintf(Buff2[1],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff2[1],"A%d",((ID-29)+1));

   }

   OctvaesetChange[1]=true;



}

void MainWindow::OctaveGroupCHSel3(int ID)
{

   DisplayBase->OctaveSelectGroup[2]=ID;
   if(ID<29)
   {
       sprintf(Buff2[2],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff2[2],"A%d",((ID-29)+1));

   }

   OctvaesetChange[2]=true;



}
void MainWindow::OctaveGroupCHSel4(int ID)
{
   DisplayBase->OctaveSelectGroup[3]=ID;
   if(ID<29)
   {
       sprintf(Buff2[3],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff2[3],"A%d",((ID-29)+1));

   }

   OctvaesetChange[3]=true;



}

void MainWindow::GroupCHSel1(int ID)
{

    DisplayBase->MSelectGroup[0]=ID;
   if(ID<29)
   {
       sprintf(Buff[0],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff[0],"A%d",((ID-29)+1));

   }

     setChange[0]=true;




}
void MainWindow::GroupCHSel2(int ID)
{
   DisplayBase->MSelectGroup[1]=ID;
   if(ID<29)
   {
       sprintf(Buff[1],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff[1],"A%d",((ID-29)+1));

   }

   setChange[1]=true;



}

void MainWindow::GroupCHSel3(int ID)
{

   DisplayBase->MSelectGroup[2]=ID;
   if(ID<29)
   {
       sprintf(Buff[2],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff[2],"A%d",((ID-29)+1));

   }

   setChange[2]=true;



}
void MainWindow::GroupCHSel4(int ID)
{
   DisplayBase->MSelectGroup[3]=ID;
   if(ID<29)
   {
       sprintf(Buff[3],"H%d",(ID+1));
   }
   else
   {
       sprintf(Buff[3],"A%d",((ID-29)+1));

   }

   setChange[3]=true;



}
void MainWindow::PageSelect(int ID)
{

    ID = DisplayBase->PageSelection->itemData(ID).toInt();
    switch(ID)
    {

    case 0: DisplayBase->PageSelectFunction(0);
            break;

    case 1: DisplayBase->PageSelectFunction(1);
            break;

    case 2: DisplayBase->PageSelectFunction(2);
            break;

    case 3: DisplayBase->PageSelectFunction(3);
            break;

    case 4: DisplayBase->PageSelectFunction(4);
            break;
    case 5: DisplayBase->PageSelectFunction(5);
            break;

    case 6: DisplayBase->PageSelectFunction(6);
            break;
    case 7: DisplayBase->PageSelectFunction(7);
	    break;
    case 8:
             RecordDialog->hide();
             ReplayDialog->hide();
             this->close();
             qApp->quit();
             break;

    default: break;
    };
}


void MainWindow::keyPressEvent( QKeyEvent *Keyevent )
    {

//printf("\n Key is selcted");

    switch(Keyevent->key())
    {

    case Qt::Key_Q:   if((Keyevent->modifiers().testFlag(Qt::ControlModifier)))
                         {

                         }

                         break;
    case Qt::Key_Right:

                          if((Keyevent->modifiers().testFlag(Qt::ControlModifier)))
                          {
                              ZoomCursorTickerSize_=1;
                              xPos=xPos+ZoomCursorTickerSize_;
                              if(PageID==0)
                              {
                                  xVal= DisplayBase->graphPlot->LofarGraphLegend->xAxis->pixelToCoord(xPos);
                              }
                              else
                              {
                              xVal= DisplayBase->graphPlot->SpectrumGraphLegend->xAxis->pixelToCoord(xPos);
                              }
                              DisplayBase->SetZoomCursorPositionChange(xPos,xVal);

                          }
                        else
                          {
                              ZoomCursorTickerSize_=4;
                              xPos=xPos+ZoomCursorTickerSize_;
                              if(PageID==0)
                              {



                                  xVal= DisplayBase->graphPlot->LofarGraphLegend->xAxis->pixelToCoord(xPos);
                              }
                              else
                              {
                              xVal= DisplayBase->graphPlot->SpectrumGraphLegend->xAxis->pixelToCoord(xPos);
                              }
                              DisplayBase->SetZoomCursorPositionChange(xPos,xVal);


                          }
                          break;

    case Qt::Key_Left:  if((Keyevent->modifiers().testFlag(Qt::ControlModifier)))
                         {
                         ZoomCursorTickerSize_=1;
                         xPos=xPos-ZoomCursorTickerSize_;
                         if(PageID==0)
                         {
                             xVal= DisplayBase->graphPlot->LofarGraphLegend->xAxis->pixelToCoord(xPos);
                         }
                         else
                         {
                         xVal= DisplayBase->graphPlot->SpectrumGraphLegend->xAxis->pixelToCoord(xPos);
                         }
                         DisplayBase->SetZoomCursorPositionChange(xPos,xVal);

                        }
                         else
                        {

                             ZoomCursorTickerSize_=4;
                             xPos=xPos-ZoomCursorTickerSize_;
                             if(PageID==0)
                             {
                                 xVal= DisplayBase->graphPlot->LofarGraphLegend->xAxis->pixelToCoord(xPos);
                             }
                             else
                             {
                             xVal= DisplayBase->graphPlot->SpectrumGraphLegend->xAxis->pixelToCoord(xPos);
                             }
                             DisplayBase->SetZoomCursorPositionChange(xPos,xVal);
                        }
                          break;
    case Qt::Key_Space:

                           break;


    default:               break;

    };

     }


void MainWindow::mousePressEvent( QMouseEvent *event )
    {

    if(DisplayBase->SelectedPageID==4)
    {
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    if((yMousePos > 121) && (yMousePos < 450))
    {
    if((xMousePos>0) && xMousePos<790)
    {
        DisplayINTObservation(1);
    }
    }
    }
    if(DisplayBase->SelectedPageID==5)
    {
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    if((yMousePos > 121) && (yMousePos < 450))
    {
    if((xMousePos>0) && xMousePos<790)
    {
        DisplayINTObservation(2);
    }
    }
    }

        for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
        {
   //if(DisplayBase->HYDSENSOR[iCount]->isUnderMouse())
             {
   //DisplayBase->SensorGroupSelect(iCount,false);
             }

         }
        for(iCount=0;iCount<ACCELEROMETER_SENSOR;iCount++)
        {
           // if(DisplayBase->ACCSENSOR[iCount]->isUnderMouse())
             {
            //    DisplayBase->SensorGroupSelect(iCount,true);
             }
        }


        for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
        {
            for(jCount=0;jCount<6;jCount++)
            {
               if(DisplayBase->HistoBar[iCount][jCount]->isUnderMouse())
               {
                   DisplayBase->ChangeHistoThresholdValue(iCount,jCount);
               }
            }

         }
  }

void MainWindow::SetMGroupEnable()
{
DisplayBase->SensorGroupEnable=true;
DisplayBase->MGroupEnable=true;
DisplayBase->ThresholdMessage->close();

}

void MainWindow::LofaronMouseMove(QMouseEvent *event )
{
    if(CursorSet==0)
    {
    PageID=0;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    DisplayBase->xPos=xMousePos;
    xPos=xMousePos;
    DisplayBase->CursorStatus=true;
    CursorPos();
    }
}

void MainWindow::LofarZoomonMouseMove(QMouseEvent *event )
{
    float y;
    if(LftZoomCursor==0)
    {

    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    xFreq=DisplayBase->graphPlot->LofarZoomGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->LofarZoomGraphLegend->yAxis->pixelToCoord(yMousePos);
    if(DisplayBase->CurrentChannelSel < 29)
    {

    if(LofarZoomCursor==200)
    {
       // y=(yAmp/DisplayBase->CHGainFloatValue);
       // DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
        DisplayBase->DispFreq=(xFreq*0.4765)+(LofarBeatFreq);
        DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

    }
    else if(LofarZoomCursor==100)
    {

  //  y=(yAmp/DisplayBase->CHGainFloatValue);
  //  DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
    DisplayBase->DispFreq=(xFreq*0.23825)+(LofarBeatFreq);
    DisplayBase->DispAmp=(DisplayBase->InterValue);
    sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
    //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

    }
    else
    {

      // y=(yAmp/DisplayBase->CHGainFloatValue);
      //  DisplayBase->InterValue=(110-(60+(20*log10((y)/250))));
        y=(yAmp);
        DisplayBase->InterValue=(((20*log10(y))*5)-DisplayBase->CHGaindBValue);
        DisplayBase->DispFreq=(xFreq*0.119125)+(LofarBeatFreq);
        DisplayBase->DispAmp=(DisplayBase->InterValue);
        sprintf(DisplayBase->freqVal," %.1f Hz ",DisplayBase->DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(DisplayBase->ampVal," %.1f dB ",DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

    }
    }
    else
    {
        if(LofarZoomCursor==200)
        {
        DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
        DisplayBase->DispFreq=(xFreq*0.4765)+(LofarBeatFreq);;
        DisplayBase->DispAmp=( DisplayBase->InterValue);


        sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
       // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        }
        else if(LofarZoomCursor==100)
        {
            DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
            DisplayBase->DispFreq=(xFreq*0.23825)+(LofarBeatFreq);;
            DisplayBase->DispAmp=( DisplayBase->InterValue);


            sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
           // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
            sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

        }
         else
        {
            DisplayBase->InterValue=(yAmp/(47.2* DisplayBase->CHGainFloatValue));
            DisplayBase->DispFreq=(xFreq*0.119125)+(LofarBeatFreq);;
             DisplayBase->DispAmp=( DisplayBase->InterValue);


            sprintf( DisplayBase->freqVal," %.1f Hz ", DisplayBase->DispFreq);
           // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
            sprintf( DisplayBase->ampVal," %.3f g ", DisplayBase->DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;

        }
    }
    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->setText(DisplayBase->ampVal);
    DisplayBase->AmpVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->GainVal->show();
    DisplayBase->DisplayAmp->show();
    DisplayBase->DisplayGain->show();
    DisplayBase->DisplayFreq->show();
    if((xMousePos>40 && xMousePos<735) && (yMousePos>17 && yMousePos<224) )
    {
        DisplayBase->ZoomDisplayVFrame->move(xMousePos,15);
        DisplayBase->ZoomDisplayHFrame->move(30,yMousePos);
    }
    }
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
}

void MainWindow::SpectrumonMouseMove(QMouseEvent *event )
{
    if(CursorSet==0)
    {
    PageID=1;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    DisplayBase->xPos=xMousePos;
    xPos=xMousePos;
    DisplayBase->CursorStatus=true;
    CursorPos();
    }
}

void MainWindow::Del_Spec_onMouseMove(QMouseEvent *event)
{
    if(CursorSet==0)
    {
    //PageID = 1;
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    //DisplayBase->xPos=xMousePos;
    //xPos=xMousePos;
    //DisplayBase->CursorStatus=true;
    CursorPos();
    }
}

void MainWindow::DemononMouseMove(QMouseEvent *event )
{
}

void MainWindow::DisplayINTObservation(int16_t FLAG)
{
    if(FLAG==1)
    {
    sprintf(DisplayBase->freqVal," %.1f Hz ",(xMousePos*0.878477306));
    }
    else
    {
    sprintf(DisplayBase->freqVal," %.1f Hz ",(xMousePos*2.928257687));
    }
    DisplayBase->FreqVal->setText(DisplayBase->freqVal);
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
}



void MainWindow::mouseDoubleClickEvent(QMouseEvent *Event)
{



}

void MainWindow::CursorPos()

  {
    if(PageID==0)
    {
    xFreq=DisplayBase->graphPlot->LofarGraphLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->LofarGraphLegend->yAxis->pixelToCoord(yMousePos);
    DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
    Threshold.TFrequency=xFreq;
    Threshold.TAmplitude=yAmp;
    Threshold.xThresholdPos=(int32_t)xMousePos;
    Threshold.yThresholdPos=(int32_t)yMousePos;

    CH_Track.Frequency=xFreq;
    CH_Track.Amplitude=yAmp;
    CH_Track.xTrackPos=(int32_t)xMousePos;
    CH_Track.yTrackPos=(int32_t)yMousePos;
    }
    else
    {
        xFreq=DisplayBase->graphPlot->SpectrumGraphLegend->xAxis->pixelToCoord(xMousePos);
        yAmp=DisplayBase->graphPlot->SpectrumGraphLegend->yAxis->pixelToCoord(yMousePos);
        DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
    }

   if (PageID==1) {

       xFreq=DisplayBase->graphPlot->RawDataDelSpectGraphLegend->xAxis->pixelToCoord(xMousePos);
       yAmp=DisplayBase->graphPlot->RawDataDelSpectGraphLegend->yAxis->pixelToCoord(yMousePos);
       //DisplayBase->UpdateObservationData(xFreq,yAmp,xMousePos,yMousePos);
    }


  }

void MainWindow::SpectrumMainLegendDoubleClickEvent(QMouseEvent *event)
{
    CursorSet=(CursorSet+1)%2;
}

void MainWindow::SpectrumZoomLegendDoubleClickEvent(QMouseEvent *event)
{
    specZoomCursor=(specZoomCursor+1)%2;
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
}
void MainWindow::LofarMainLegendDoubleClickEvent(QMouseEvent *event)
{
    CursorSet=(CursorSet+1)%2;
}

void MainWindow::Del_Spec_LegendDoubleClickEvent(QMouseEvent *event)
{
    CursorSet=(CursorSet+1)%2;
}

void MainWindow::LofarZoomMainLegendDoubleClickEvent(QMouseEvent *event)
{
    LftZoomCursor=(LftZoomCursor+1)%2;
    DisplayBase->AmpVal->hide();
    DisplayBase->DisplayAmp->hide();
}
void MainWindow::LofarWaterfallLegendDoubleClickEvent(QMouseEvent *event)
{
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    if((yMousePos>18) && (yMousePos<232))
    {
    if((xMousePos>37)&&(xMousePos<985))
    {
   // printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
    xFreq=DisplayBase->graphPlot->LofarWaterFallLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->LofarWaterFallLegend->yAxis->pixelToCoord(yMousePos);
  //DisplayBase->LofarHarmonicCursorPos=xMousePos;
    }
    }

}

void MainWindow::SpectrumWaterfallLegendDoubleClickEvent(QMouseEvent *event)
{
    xMousePos=(float)event->pos().x();
    yMousePos=(float)event->pos().y();
    if((yMousePos>18) && (yMousePos<232))
    {
    if((xMousePos>37)&&(xMousePos<985))
    {
 //printf("\n %f %f  %f %f",xMousePos,xFreq,yMousePos,yAmp);
    xFreq=DisplayBase->graphPlot->SpectrumWaterFallLegend->xAxis->pixelToCoord(xMousePos);
    yAmp=DisplayBase->graphPlot->SpectrumWaterFallLegend->yAxis->pixelToCoord(yMousePos);
//DisplayBase->SpectrumHarmonicCursorPos=xMousePos;

    }
    }
}



void MainWindow::HarmonicCursorAction1()
{
    if(PageID==0)
    {
    DisplayBase->DrawHarmonicCursorActive(true,0);
    setLftHarmonicFreq=true;
    }
    else
    {
     DisplayBase->DrawHarmonicCursorActive(true,1);
     setSpecHarmonicFreq=true;
    }
   // printf("\n Iam in Action 1");
}

void MainWindow::HarmonicCursorAction2()
{
    if(PageID==0)
    {
    DisplayBase->DrawHarmonicCursorActive(false,0);
    setLftHarmonicFreq=false;
    }
    else
    {
     DisplayBase->DrawHarmonicCursorActive(false,1);
     setSpecHarmonicFreq=false;
    }
//printf("\n Iam in Action 2");
}

void MainWindow::HarmonicCursorAction3()
{
    if(PageID==0)
    {
   DisplayBase->DrawHarmonicCursorActive(true,0);
   setLftHarmonicFreq=false;
    }
    else
    {
     DisplayBase->DrawHarmonicCursorActive(true,1);
     setSpecHarmonicFreq=false;
    }

//printf("\n Iam in Action 2");
}


void MainWindow::MGC_Slot(int Option)
{
    DisplayBase->Mgc_SetCmbo(Option);
 //printf("\n Enterded Value=%d",Option);
}

void MainWindow::AGC_Slot(int Option)
{  
DisplayBase->Agc_SetCmbo(Option);
//printf("\n Enterded Value=%d",Option);
}

void MainWindow::PRESET_Slot(int Option)
{  
DisplayBase->Preset_SetCmbo(Option);
//printf("\n Enterded Value=%d",Option);
}

void MainWindow::TrackInfo(int ID)
{
DisplayBase->TrackDataInfo(ID);
}

MainWindow::~MainWindow()
{
 //   delete ui;

}
 void MainWindow::CloseTrackConfigButton()
 {

    trackPopUpWindow->hide();
    DisplayBase->TrackWindowSet->WindowHide();
 }

 void MainWindow::setThresholdConfigButton()
 {

     thresholdPopUpWindow->hide();
     DisplayBase->ThresholdSet->setThresholdConfig();

 }

 void MainWindow::cancelThresholdConfigButton()
 {
       thresholdPopUpWindow->hide();
       DisplayBase->ThresholdSet->cancelThresholdConfig();


 }

 void MainWindow::POPThresholdWindow()
 {
         thresholdPopUpWindow=new QDialog(this);
         thresholdPopUpWindow->setGeometry(QRect(70,200,525,410));
         DisplayBase->ThresholdSet->ThresholdWindow->setParent(thresholdPopUpWindow);
         thresholdPopUpWindow->setWindowTitle("Threshold Setting Window");
 }

 void MainWindow::POPTrackWindow()
 {
         trackPopUpWindow=new QDialog(this);
         trackPopUpWindow->setGeometry(QRect(150,10,700,1100));
         DisplayBase->TrackWindowSet->trackWindow->setParent(trackPopUpWindow);
         trackPopUpWindow->setWindowTitle("Track Setting Window");
 }


 void MainWindow::RecordWindowSet()
 {
     QFont font;
     font.setFamily(QString::fromUtf8("Sans Serif"));
     font.setBold(false);
     font.setPointSize(10);
     font.setPixelSize(12);

     RecordDialog=new QDialog(this);
     RecordDialog->setWindowTitle(" RECORDER SETTINGS ");
     RecordDialog->setGeometry(QRect(150,100,600,250));

     RecordFrame=new QFrame(RecordDialog) ;
     RecordFrame->setMouseTracking(true);
     RecordFrame->setFocusPolicy(Qt::StrongFocus);
   //  DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
     RecordFrame->setGeometry(QRect(0,0,600,250));

     RecordFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
     RecordFrame->setFrameShape(QFrame::StyledPanel);
     RecordFrame->setFrameShadow(QFrame::Raised);

     lineEdit = new QLineEdit(RecordFrame);
     lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
     lineEdit->setGeometry(QRect(145, 27, 355, 25));
     lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
     lineEdit->setFont(font);

     RecordName= new QLabel(RecordFrame);
     RecordName->setObjectName(QString::fromUtf8("label"));
     RecordName->setGeometry(QRect(10, 31, 125, 16));
     RecordName->setText("NEW FILE NAME");
     RecordName->setFont(font);

     RecInitcation= new QLabel(RecordFrame);
     RecInitcation->setObjectName(QString::fromUtf8("label"));
     RecInitcation->setGeometry(QRect(10, 65, 600, 16));
     RecInitcation->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(170,170,170);"));
     RecInitcation->setFont(font);

     RecInitcation->setText(RecorderPathSet);
     RecInitcation->hide();
     ReorderButton=new QPushButton(RecordFrame);
     ReorderButton->setStyleSheet(QString::fromUtf8("background-color:rgb(220,220,220)"));
     ReorderButton->setGeometry(280,200,70,30);
     ReorderButton->setFont(font);
     ReorderButton->setText("OK");
 }

 void MainWindow::EnergySaveWindow()
 {
     QFont font;
     font.setFamily(QString::fromUtf8("Sans Serif"));
     font.setBold(false);
     font.setPointSize(10);
     font.setPixelSize(12);

     EnergyDialog=new QDialog(this);
     EnergyDialog->setWindowTitle(" HISTOGRAM SETTINGS ");
     EnergyDialog->setGeometry(QRect(150,100,400,300));

     EnergyFRame=new QFrame(EnergyDialog) ;
     EnergyFRame->setMouseTracking(true);
     EnergyFRame->setFocusPolicy(Qt::StrongFocus);
   //DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
     EnergyFRame->setGeometry(QRect(0,0,400,300));

     EnergyFRame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
     EnergyFRame->setFrameShape(QFrame::StyledPanel);
     EnergyFRame->setFrameShadow(QFrame::Raised);


     EnergyLineEdit = new QLineEdit(EnergyFRame);
     EnergyLineEdit->setObjectName(QString::fromUtf8("lineEdit"));
     EnergyLineEdit->setGeometry(QRect(160, 27, 155, 25));
     EnergyLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
     EnergyLineEdit->setFont(font);

     EnergyFileSave = new QCheckBox(EnergyFRame);
     EnergyFileSave->setObjectName(QString::fromUtf8("checkBox"));
     EnergyFileSave->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     EnergyFileSave->setGeometry(QRect(350,27, 87, 20));


     EnergyFileName= new QLabel(EnergyFRame);
     EnergyFileName->setObjectName(QString::fromUtf8("label"));
     EnergyFileName->setGeometry(QRect(10, 31, 125, 16));
     EnergyFileName->setText(" NEW FILE NAME ");
     EnergyFileName->setFont(font);

     EnergyReplatFile= new QLabel(EnergyFRame);
     EnergyReplatFile->setObjectName(QString::fromUtf8("label"));
     EnergyReplatFile->setGeometry(QRect(10,100,135,16));
     EnergyReplatFile->setFont(font);

     EnergyReplatFile->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     EnergyReplatFile->setText("SELECT ENERGY FILE");

     EnergyCombo=new QComboBox(EnergyFRame);
     EnergyCombo->setGeometry(QRect(160,100,145,20));
     EnergyCombo->setFont(font);
     EnergyCombo->clear();
     EnergyCombo->setWindowModality(Qt::WindowModal);
     EnergyCombo->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                  "color: rgb(0,0,0);"));
     EnergyFileRestore = new QCheckBox(EnergyFRame);
     EnergyFileRestore->setObjectName(QString::fromUtf8("checkBox"));
     EnergyFileRestore->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     EnergyFileRestore->setGeometry(QRect(350,100, 87, 20));

     EnergyFileDelete= new QLabel(EnergyFRame);
     EnergyFileDelete->setObjectName(QString::fromUtf8("label"));
     EnergyFileDelete->setGeometry(QRect(10,180,121, 20));
     EnergyFileDelete->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     EnergyFileDelete->setFont(font);

     EnergyFileDelete->setText("FILE DELETE ");


             EnergyFileDelete7=new QComboBox(EnergyFRame);
             EnergyFileDelete7->setGeometry(QRect(160,180,145,20));
             EnergyFileDelete7->setFont(font);
             EnergyFileDelete7->clear();
             EnergyFileDelete7->setWindowModality(Qt::WindowModal);
             EnergyFileDelete7->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                          "color: rgb(0,0,0);"));

     EnergyFileDelete1 = new QCheckBox(EnergyFRame);
     EnergyFileDelete1->setObjectName(QString::fromUtf8("checkBox"));
     EnergyFileDelete1->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     EnergyFileDelete1->setGeometry(QRect(350,180, 87, 20));


     EnergyButton=new QPushButton(EnergyFRame);
     EnergyButton->setStyleSheet(QString::fromUtf8("background-color:rgb(220,220,220)"));
     EnergyButton->setGeometry(150,250,120,30);
     EnergyButton->setFont(font);
     EnergyButton->setText("OK");
 }


 void MainWindow::ReplayWindowSet()
 {
     QFont font;
     font.setFamily(QString::fromUtf8("Sans Serif"));
     font.setBold(false);
     font.setPointSize(10);
     font.setPixelSize(12);
     ReplayDialog = new QDialog(this);
     ReplayDialog->setWindowTitle(" REPLAY SETTINGS ");
     ReplayDialog->setGeometry(QRect(150,100,380,250));
//ExportCSV_Button_2->setStyleSheet(QString::fromUtf8("background-color:white;\n" "color: black;"
// "border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;"));
    ReplayDialog->setStyleSheet("background-color:white;\n");

     ReplyFrame=new QFrame(ReplayDialog) ;
     ReplyFrame->setMouseTracking(true);
     ReplyFrame->setFocusPolicy(Qt::StrongFocus);
   //DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
     ReplyFrame->setGeometry(QRect(0,0,380,250));

     ReplyFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);"));
     ReplyFrame->setFrameShape(QFrame::StyledPanel);
     ReplyFrame->setFrameShadow(QFrame::Raised);

     ReplayFiles=new QComboBox(ReplayDialog);
     ReplayFiles->setGeometry(QRect(160, 17,200, 20));
     ReplayFiles->setFont(font);
     ReplayFiles->clear();
     ReplayFiles->setWindowModality(Qt::WindowModal);
     ReplayFiles->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                  "color: rgb(0,0,0);"));
     RecLabel= new QLabel(ReplayDialog);
     RecLabel->setObjectName(QString::fromUtf8("label"));
     RecLabel->setGeometry(QRect(15, 17,135, 20));
     RecLabel->setFont(font);

     RecLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     RecLabel->setText("SELECT REPLAY FILE");

     FileDelete= new QLabel(ReplayDialog);
     FileDelete->setObjectName(QString::fromUtf8("label"));
     FileDelete->setGeometry(QRect(15,60,121, 20));
     FileDelete->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     FileDelete->setFont(font);

     FileDelete->setText("FILE DELETE ");
     checkRecBox = new QCheckBox(ReplayDialog);
     checkRecBox->setObjectName(QString::fromUtf8("checkBox"));
     checkRecBox->setStyleSheet(QString::fromUtf8("background-color: rgb(230,230,230);\n"
         "color: rgb(0,0,0);"));
     checkRecBox->setGeometry(QRect(100,60, 87, 20));

     ReplaySTART = new QPushButton(ReplayDialog);
     ReplaySTART->setObjectName(QString::fromUtf8("pushButton"));
     ReplaySTART->setGeometry(QRect(140, 200, 84, 25));
     ReplaySTART->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 160);"));
     ReplaySTART->setText("OK");
 }

  void MainWindow::ScreenShotPicture()
  {
      ScreenShotWindow=new QDialog(this);
      ScreenShotWindow->setGeometry(QRect(150, 50, 1020, 585));
      ScreenShotWindow->setWindowTitle("SnapShot");

      PrintFrame=new QFrame(ScreenShotWindow);
      PrintFrame->setGeometry(QRect(10, 20, 1000, 600));
      PrintFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
      PrintFrame->setFrameShape(QFrame::StyledPanel);
      PrintFrame->setFrameShadow(QFrame::Raised);

      screenshotLabel = new QLabel(PrintFrame);
      screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      screenshotLabel->setAlignment(Qt::AlignCenter);
      screenshotLabel->setMinimumSize(740, 560);
      screenshotLabel->setStyleSheet(QString::fromUtf8("\n""background-color: rgb(0, 0, 0)"));
      screenshotLabel->setGeometry(QRect(0, 0, 1000, 300));

       mainLayout = new QVBoxLayout;

        ScreenShotSaveOpt=new QDialog(ScreenShotWindow);
        ScreenShotSaveOpt->setGeometry(QRect(250, 150,300,100));
        ScreenShotSaveOpt->setStyleSheet(QString::fromUtf8("background-color: rgb(250,250,250);"));
        ScreenShotSaveOpt->setWindowTitle("SetFileName");

       PrintScreen=new QPushButton(ScreenShotSaveOpt);
       PrintScreen->setStyleSheet(QString::fromUtf8("background-color:rgb(220,220,220)"));
       PrintScreen->setGeometry(90,40,65,30);
       PrintScreen->setText("OK");
       PrintCancel=new QPushButton(ScreenShotSaveOpt);
       PrintCancel->setStyleSheet(QString::fromUtf8("background-color:rgb(220,220,220)"));
       PrintCancel->setGeometry(160,40,65,30);
       PrintCancel->setText("Exit");
  }





 void MainWindow::ScreenShotFun()
 {
     originalPixmap = QPixmap();
     originalPixmap = QPixmap::grabWindow(QDesktopWidget().winId());
     mainLayout->addWidget(screenshotLabel);
     screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
                                                      Qt::KeepAspectRatio,
                                                      Qt::SmoothTransformation));

     ScreenShotWindow->show();
     PrintFrame->show();
     screenshotLabel->show();
     ScreenShotSaveOpt->show();
 }



 void MainWindow::ScreenShotSave()
 {
     QString format = "png";
     QString initialPath = QDir::currentPath() +tr("/untitled.") + format;

     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),  initialPath,
                                                     tr("%1 Files (*.%2);;All Files (*)")
                                                     .arg(format.toUpper())
                                                     .arg(format));
     if (!fileName.isEmpty())
       originalPixmap.save(fileName, format.toLatin1().constData());
     screenshotLabel->hide();
      PrintFrame->hide();
     ScreenShotWindow->hide();
     PrintFrame->close();
     screenshotLabel->hide();
     ScreenShotSaveOpt->hide();


 }
void MainWindow::RecorderSetParameter()
{


    PathSettings();
    DisplayBase->RecordEnableFun();
    RecordDialog->hide();

}

void MainWindow::PathSettings()
{
   char Buffer[1024];
   int32_t Size;
   //QString FileName;


  // FileName=lineEdit->text();
   //QString FileDir = "/"+FileName;
 //  strcpy(RecControl.RecorderFilePath,RecControl.RecorderPath);
   strcpy(Buffer,RecordPathSet);
   strcpy(RecControl.RecorderFilePath,Buffer);
   strcat(RecControl.RecorderFilePath,".bin");
   Size=sizeof(RecControl.RecorderFilePath);
   if((Size > 1024))
   {
       printf("\n File Name Error ");
   }

  // printf("\n Aim in path settings %s",RecControl.RecorderFilePath);


  // printf("\n Channel File Name=%s  %d \n",Graph_plt1.RecorderFilePath,sizeof(Graph_plt1.RecorderFilePath));
}


void MainWindow::RecordStart()
{


    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    QString strFileName = dialog.getSaveFileName(this,tr("Set Record File"),RecordPathSet, tr("All Files (*.*)"));

    strcpy(RecordPathSet,strFileName.toStdString().c_str());

    lineEdit->clear();

    lineEdit->setText(RecordPathSet);

  //  printf("\n File Path:%s",RecordPathSet);



RecordDialog->show();


}


 void MainWindow::StorageCapcity()
 {
    // int32_t CurrentFileSize;
   ////  QString NoOfFiles;
   //  QDir Path(RecControl.RecorderPath);
   //  QStringList List=Path.entryList(QDir::Files);
   //  ReplayFiles->clear();
    // ReplayFiles->addItems(List);
    // CurrentFileSize=ReplayFiles->count();
    // ReplayFiles->clear();

  //   DisplayBase->StorageCapacityValue=CurrentFileSize;
    // printf("\n Size of Datafiles=%d",Size);

 }
 void MainWindow::EnergyWindowFileist()
 {
     QString NoOfFiles;
     QDir Path(RecControl.EnergyPath);
     QStringList List=Path.entryList(QDir::Files);
     EnergyCombo->clear();
     EnergyCombo->addItems(List);
     EnergyFileDelete7->clear();
     EnergyFileDelete7->addItems(List);
     //printf("\n Size of Datafiles=%d",Size);


 }
void MainWindow::ReplayStart()
{
//----------------printf("\n Iam in the Replay-------------------");
    checkRecBox->setCheckable(true);
    int32_t Size;
    QString NoOfFiles;
    QDir Path(RecControl.RecorderPath);
    ReplayFiles->clear();
    QStringList List=Path.entryList(QDir::Files);
 // ReplayFiles->clear();
    ReplayFiles->addItems(List);
    Size=ReplayFiles->count();
    DisplayBase->StorageCapacityValue = Size;
    printf("\n Size of Datafiles=%d",Size);

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setStyleSheet("background-color:white");
    QString strFileName = dialog.getOpenFileName(this,tr("Select Replay File"),ReplayPathSet, tr("All Files (*.*)"));

    strcpy(ReplayPathSet,strFileName.toStdString().c_str());
    strcpy(BufferPath,strFileName.toStdString().c_str());
    strcpy(RecControl.ReplayFilePath,BufferPath);

    ReplayFiles->addItem(strFileName);
    ReplayDialog->show();
}

 void MainWindow::ScreenShotSaveClose()
 {
     ScreenShotWindow->hide();
     PrintFrame->hide();
     screenshotLabel->hide();
     ScreenShotSaveOpt->hide();
     PrintFrame->close();
 }

void MainWindow::FileSelect(QString File)
 {
   ReplayFileSelect=File;
 }

void MainWindow::EnergyFileSelect(QString File)
 {
   EnergyReplytFIleSelect=File;
 }

void MainWindow::EnergyDeleteFileSelect(QString File)
 {
   EnergyDeletetFIleSelect=File;
 }

void MainWindow::UnitConvertionClose()
{
UnitCon->close();
}

void MainWindow::SesnorGroupSet()
{
    for(iCount=0;iCount<4;iCount++)
    {
        if(setChange[iCount]==true)
        {
        DisplayBase->MGroup[iCount]=DisplayBase->MSelectGroup[iCount];
        DisplayBase->MGroupSensor[iCount]->setText(Buff[iCount]);
        }
        setChange[iCount]=false;
    }
  SenGroup->close();
}


void MainWindow::OctaveSesnorGroupSet()
{
    int16_t g,b;
    g=0;
    b=0;
    for(iCount=0;iCount<4;iCount++)
    {
        if(octavecheckRecBox[iCount]->isChecked())
        {
            g=1;
            DisplayBase->graphPlot->graph1[iCount]->setVisible(true);
        }
        else
        {
            g=0;
            DisplayBase->graphPlot->graph1[iCount]->setVisible(false);
        }

        if(octaveBarcheckRecBox[iCount]->isChecked())
        {
            b=1;
           DisplayBase->graphPlot->GraphBars[iCount]->setVisible(true);
        }
        else
        {
            b=0;
            DisplayBase->graphPlot->GraphBars[iCount]->setVisible(false);
        }

        if((g==1) || (b==1))
        {
            DisplayBase->OctaveFlag[iCount]=true;
            DisplayBase->OctaveGroupSensor[iCount]->setText(Buff2[iCount]);
        }
        else
        {
            DisplayBase->OctaveFlag[iCount]=false;
            sprintf(Buff2[iCount],"%s","xx");
        }
        if(OctvaesetChange[iCount]==true)
        {
        DisplayBase->OctaveGroup[iCount]=DisplayBase->OctaveSelectGroup[iCount];
        DisplayBase->OctaveGroupSensor[iCount]->setText(Buff2[iCount]);
        OctvaesetChange[iCount]=false;

        }
    }

        OctaveSenGroup->close();
}


void MainWindow::ReplayON()
{
// char Buffer[400];
// qDebug()<< ReplayFileSelect;
 QDir Path(RecControl.RecorderPath);
 //qDebug() << ReplayFileSelect;

  if(checkRecBox->isChecked())
    {
       Path.remove(ReplayFileSelect);
      //qDebug() << ReplayFileSelect;
    }
    else
    {

      //QString FileDir = "/FileStorage/"+ReplayFileSelect;
     // strcpy(RecControl.ReplayFilePath,RecControl.CurrentDir);
     // strcpy(Buffer,FileDir.toStdString().c_str());
      strcpy(RecControl.ReplayFilePath,BufferPath);
 //printf("\n Path=%s",RecControl.ReplayFilePath);
     // printf("\n Path %s",BufferPath);

  DisplayBase->ReplayEnableFun();

    }
ReplayDialog->hide();

}
