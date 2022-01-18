#include "displaywindow.h"
#include "FilterCoeffients.h"
#include "signalprocessingclass.h"
#include <QRegExp>
#define THRESHOLD_FILE_STATUS 0             // If 0 LaptopVersion else SBC Version

float CompSpectData[3][16384]={0};
int ReplaySpectrumCompFlag=0;
int CompFlagInd[3]={0};
float DispAmpTmp;
FILE *ft,*fr;
extern double SpectrumMainDataPlot[NO_OF_SENSOR][1200];
extern double OctaveMainDataPlot[NO_OF_SENSOR][40];

extern ChannelTrackValue Track_Data[NO_OF_TRACK];
extern ChannelThreshold Threshold;
extern RecordingControls RecControl;
extern double SpectrumZoomDataPlot[1200];
extern int16_t RecordStartFlag;
extern int16_t ReplayStartFlag;
extern int16_t RecordSetFlag;
extern int16_t RecordCompFlag;
int16_t ReplayProgressBar;
int16_t RecordProgressBar;
extern int16_t ReplayCompFlag;
extern int ReplayContFlag;
extern int isCompSpecRdy;
char SpectrumReplayPathSet[1024];
int Selectedfileno;
char tempNam1[100];
char tempNam2[100];
char tempNam3[100];
char txtReplFileNw[200];
QStringList strFileNames;

extern void ClearSpecCompDisplay();

int32_t DelayMaking;
Host_Control_pkt_t control_object;

 struct ThresholgSettingsRead
{
    int16_t THRESOLD_AMP[NO_OF_SENSOR][8];
    int16_t THRESHOLD_FREQ[NO_OF_SENSOR][8];
    int16_t  HISTOGRAM_ENG[HYDROPHONE_SENSOR];
    int8_t  HISTOGRAM_POS[HYDROPHONE_SENSOR];
};

 struct ThresholgSettingWrite
{
    int16_t  THRESOLD_AMP[NO_OF_SENSOR][8];
    int16_t  THRESHOLD_FREQ[NO_OF_SENSOR][8];
    int16_t  HISTOGRAM_ENG[HYDROPHONE_SENSOR];
    int8_t  HISTOGRAM_POS[HYDROPHONE_SENSOR];
};


int16_t ChannelSelected;
ThresholgSettingsRead ReadThreshold;
ThresholgSettingWrite WriteData;
extern Link LinkStatus;
double  EnergyValue[61];
int16_t Energy_Histo_Pos[29];
FILE *ThresholdFileWrite,*ThresholdFileRead;
float SPECTRUM_ALPHA,LOFAR_ALPHA,ENG_ALPHA;
float  SpcetrumBeatFreq,LofarBeatFreq;
int16_t SpectrumZoomCursor,LofarZoomCursor;
bool SpectrumCHChange[NO_OF_SENSOR],SpectrumZoomCursorStatus,LofarZoomCursorStatus,SpectrumZoomEnable,LofarZoomEnable;
char AccelerometerName[ACCELEROMETER_SENSOR][35]={"A1 C1 DK3 PURIFICATION BLOWER","A2 C1 DK3 FAN BLOWER","A3 C1 DK3 ATU","A4 C2 DK1 SMCS-1","A5 C2 DK2 CONVERTER","A6 C2 DK3 AGGRIGATE","A7 C3 DK2 AC REF","A8 C3 DK3 ATU","A9 C3 DK1 FAN BLOWER","A10 C4 DK3 MMG ","A11 C4 DK3 CONVERTER","A12 C4 DK1 FAN BLOWER ",
                                 "A13 C4 DK1 FAN BLOWER","A14 C5 DK3 AC PLANT","A15 C5 DK3 AC PLANT","A16 C5 DK3 MMG/AC  PLANT","A17 C5 DK2 SECRA COMP","A18 C6 DK1 VACCUM COMP","A19 C6 DK2 ATU FAN","A20 C6 DK2 MAKEUP PUMP","A21 C6 DK3 DRAIN PUMP","A22 C7 DK3 MAIN TURBINE","A23 C7 DK3 GEAR BOX","A24 C7 DK2 DIST PLANT",
                                 "A25 C7 DK2 DIST PLANT","A26 C7 DK3 CXP","A27 C7 DK3 TA1 BLOWER","A28 C7 DK3 TA2 BLOWER","A29 C8 DK2 AC PLANT","A30 DK1 HPA COMP","A31 DK1 HPA COMP","A32 DK2 PROPELLER"};
char HydrophoneName[HYDROPHONE_SENSOR][35]={"HMS1","HMS2","HMS3","B&K1","B&K2","H6 SONAR AREA PORT","H7 SONAR AREA(STBD)","H8 SONAR AREA(CENTER TOP)","H9 SONAR AREA(CENTER TOP)","H10(PORT)",
                             "H11 BRIDGE FIN(CENTER)","H12 C-303-C2(STBD)","H13 C-303-C2(PORT)","H14 C-303-C2(STBD)","H15 AFT-FIN(CENTER)","H16 C-303-C3(PORT)","H17 C-303-C3(PORT)","H18 VLM CONTAINER(PORT)","H19 C4(STBD)","H20 DG(CENTER TOP)",
                             "H21 AC PLANT(PORT)","H22 TURBINE(CENTER TOP)","H23 TURBINE(PORT)","H24 TURBINE(CENTER TOP)","H25 TURBINE(CENTER TOP)","H26 TURBINE(CENTER TOP)","H27 RESERVE PROP","H28 RUDDER(STBD)","H29 PROPELLER"};


DisplayWindow::DisplayWindow(QFrame *parent)
{
    sprintf(SpectrumReplayPathSet,"%s","/home");
    DisplayBaseFrame=new QFrame(parent);
    graphPlot=new GraphPlotClass();
    IssInfo=new ISSInfoClass();
    DrawDisplayPage();
    DrawISSInfoFrame();
    DrawISSControlFrame();
    DrawHistorgramFrame();
    DrawProcessingFrame();
    DrawSubmarimeLayoutFrame();
    DrawObservaionFrame();
    DrawAnnotationFrame();
    DrawThresholdWarningFrame();
    DisplayPageInit();
    DrawZoomCursor();
    SetThresholdCursorPos();

    connect(Format_Select,SIGNAL(currentIndexChanged(int)),this,SLOT(on_Formatcombobox_currentIndexChanged()));
    connect(Export_Button,SIGNAL(clicked()),this, SLOT (Export_to_Format_Button()));
    connect(ExportCSV_Button_1,SIGNAL(clicked()),this, SLOT (ExportDelayedSpectrum_to_CSV()));
    connect(ExportCSV_Button_2,SIGNAL(clicked()),this, SLOT (ExportRawData_to_CSV()));
    connect(graphPlot->RawDataDelSpectGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent *)),graphPlot, SLOT (freezeDelSpecPlot(QMouseEvent*)));
    connect(graphPlot->LofarGraphLegend,SIGNAL(mouseDoubleClick(QMouseEvent *)),graphPlot, SLOT (freezeLoFarPlot(QMouseEvent*)));
    connect(ReplayButtonCont,SIGNAL(clicked()),this, SLOT (setReplayCntFlag()));
    connect(graphPlot->SpectrumGraphLegend,SIGNAL(mousePress(QMouseEvent *)),graphPlot, SLOT (showSpectrumValue(QMouseEvent*)));
    connect(graphPlot->CloseSpectrumVal,SIGNAL(clicked()),graphPlot, SLOT (CloseSpectrumvalue()));
    connect(graphPlot->SpectrumGraphLegend,SIGNAL(mousePress(QMouseEvent *)),graphPlot, SLOT (showSpectrumRightWidgetVal(QMouseEvent*)));
    connect(graphPlot->ButtonExportCSV_Spectrum,SIGNAL(clicked()),graphPlot, SLOT (ExportSpectrumData_to_CSV()));
    connect(graphPlot->ButtonExportJPG_Spectrum,SIGNAL(clicked()),graphPlot,SLOT (ExportSpectrumData_to_JPG()));
    connect(graphPlot->ButtonJPG_CSV_Close_Spectrum,SIGNAL(clicked()),graphPlot,SLOT (ButtonJPG_CSV_Close_Spectrum_Tab()));
    connect(graphPlot-> RawDataDelSpectGraphLegend,SIGNAL(mousePress(QMouseEvent *)),graphPlot, SLOT (showRawDataValue(QMouseEvent*)));
    connect(graphPlot->CloseRawDataVal,SIGNAL(clicked()),graphPlot, SLOT (CloseRawDatavalue()));
    connect(graphPlot->RawDataDelSpectGraphLegend,SIGNAL(mousePress(QMouseEvent *)),graphPlot, SLOT (showRawDataRightWidgetVal(QMouseEvent*)));
    connect(graphPlot->ButtonExportCSV_RawData,SIGNAL(clicked()),graphPlot, SLOT (ExportRawData_to_CSV()));
    connect(graphPlot->ButtonExportJPG_RawData,SIGNAL(clicked()),graphPlot,SLOT (ExportRawData_to_JPG()));
    connect(graphPlot->ButtonJPG_CSV_Close_RawData,SIGNAL(clicked()),graphPlot,SLOT (ButtonJPG_CSV_Close_RawData_Tab()));
    connect(graphPlot->SpectrumComparisonGraphLegend,SIGNAL(mousePress(QMouseEvent *)),graphPlot, SLOT (showSpectCompDataValue(QMouseEvent*)));
    connect(graphPlot->CloseSpecCompVal,SIGNAL(clicked()),graphPlot, SLOT (CloseSpecCompvalue()));
    connect(graphPlot->SpectrumComparisonGraphLegend,SIGNAL(mousePress(QMouseEvent *)),graphPlot, SLOT (showSpectCompDataRightWidgetVal(QMouseEvent*)));
    connect(graphPlot->ButtonExportCSV_SpecCompData,SIGNAL(clicked()),graphPlot, SLOT (ExportSpecCompData_to_CSV()));
    connect(graphPlot->ButtonExportJPG_SpecCompData,SIGNAL(clicked()),graphPlot,SLOT (ExportSpecCompData_to_JPG()));
    connect(graphPlot->ButtonJPG_CSV_Close_SpecComp,SIGNAL(clicked()),graphPlot,SLOT (ButtonJPG_CSV_Close_SpecComp_Tab()));
    connect(Replay_Button_Spec_comp,SIGNAL(clicked()),this,SLOT(SelectSpectrumComp()));
    connect(StopReplay_Button_Spec_comp,SIGNAL(clicked()),this,SLOT(StopReplaySpecComp()));


}

void DisplayWindow::DisplayPageInit()
{
    SelectedPageID=0;
    yScale=250;
    yScaleValue=0;
    GainModeSel=0;
    TransmitSocket2=new QUdpSocket();
    TransmitSocket2->bind(QHostAddress::Any,SCU_PORT_NO,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint );

    SpectrumHarmonicCursorStatus = false;
    LofarHarmonicCursorStatus = false;
    Del_Spec_HarmonicCursorStatus = false;

    LofarHarmonicCursorPos = 37;
    SpectrumHarmonicCursorPos = 37;
    Del_Spec_HarmonicCursorPos = 37;

    GainIndB=0;
    StorageCapacityValue=0;
    CHGaindBValue=0;
    CHGainFloatValue=1;
    RecRep=0;
    TrackNumber=0;
    HydrophoneScale=0;
    AccelerometerScale=0;
    ReplayProgressBar=0;
    RecordProgressBar=0;
    ReplayTime(0);
    ChannelSelected=1;
    SpectrumZoomEnable=false;
    LofarZoomEnable=false;
    SpectrumZoomCursorStatus = false;
    LofarZoomCursorStatus=false;
    SetTimer=2000;
    xPos=200;
    TimeValue=0;
    EnergyStage=0;
    SenserRefresh=false;
    ThresholdSet=new ThresholdSettingWindow();
    TrackWindowSet=new TrackWindowSetting();
    CurrentChannelSel=0;
    SelectedChannel=0;
    SensorGroupEnable=false;
    MGroupEnable=false;
    cavitationFlag=false;
    ThresholdSet->CurrentChannelNumber=CurrentChannelSel;
    TrackWindowSet->CurrentChannel=CurrentChannelSel;

    for(iCount=0;iCount<ACCELEROMETER_SENSOR;iCount++)
    {
        ChannelThresholdSet[0][iCount]=false;
        ChannelLinkStatus[0][iCount]=false;
        ChannelThresholdSet[1][iCount]=false;
        ChannelLinkStatus[1][iCount]=false;
        ChannelNarrowBandThresholdSet[0][iCount]=false;
        ChannelNarrowBandThresholdSet[0][iCount]=false;
        ChannelNarrowBandThresholdSet[1][iCount]=false;
        ChannelNarrowBandThresholdSet[1][iCount]=false;
    }

    for(iCount=0;iCount<MAX_SENSOR_GROUP;iCount++)
    {
        MGroup[iCount]=iCount;
        OctaveFlag[iCount]=true;
        OctaveGroup[iCount]=iCount;
    }

    for(iCount=0;iCount<NO_OF_TRACK;iCount++)
    {
    TrackMissCount[iCount]=0;
    }

    for(iCount=0;iCount<NO_OF_SENSOR;iCount++)
    {
      SpectrumCHChange[yCount]=true;
       for(yCount=0;yCount<8;yCount++)
       {
        Threshold.Init_CH_No[iCount][yCount]=0;
       }
    }

    for(iCount=0;iCount<MAX_SCU_NO;iCount++)
    {
    GAIN_SET_FLAG[iCount]=false;
    }

    ZoomCursorWidth = 200;
    showMessageBox();

    for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
    {
        HistogramCursorPos[iCount]=3;
        Energy_Histo_Pos[iCount]=0;
        ChangeEnergyValue(iCount,3);
        HistoBar[iCount][3]->setBrush( Qt::gray);
        HistoBar[iCount][3]->setPen( QPen(Qt::gray) );
     }
}

void DisplayWindow::closePage()
{


}
void DisplayWindow::DrawDisplayPage()
{
        DisplayBaseFrame->setMouseTracking(true);
        DisplayBaseFrame->setFocusPolicy(Qt::StrongFocus);
      //DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
        DisplayBaseFrame->setGeometry(QRect(0,0,1024,768));
        DisplayBaseFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        DisplayBaseFrame->setFrameShape(QFrame::StyledPanel);
        DisplayBaseFrame->setFrameShadow(QFrame::Raised);
      //DisplayBaseFrame->setDisabled()
}

void DisplayWindow::DrawISSInfoFrame()
{
    IssInfo->ISSInfoFrame->setParent(DisplayBaseFrame);
    IssInfo->ISSInfoFrame->raise();
}

void DisplayWindow::DrawISSControlFrame()
{
    IssInfo->ISSButtonFrame->setParent(DisplayBaseFrame);
    IssInfo->ISSButtonFrame->raise();
}

void DisplayWindow::DrawProcessingFrame()
{
    DisplayProcesingFrame=new QFrame(DisplayBaseFrame);
    DisplayProcesingFrame->setMouseTracking(true);
    DisplayProcesingFrame->setGeometry(QRect(0,111,790,500));
    DisplayProcesingFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    DisplayProcesingFrame->setFrameShape(QFrame::StyledPanel);
    DisplayProcesingFrame->setFrameShadow(QFrame::Raised);
  //RawDataFrame->setEnabled(false);

    DrawLOFARFrame();
    DrawSpectrumFrame();
    DrawINTLofarFrame();
    DrawINTSpectrumFrame();
    OctaveBandFrame();
    DrawTrackFrame();
    DrawRawDataFrame();
    DrawSPECTRUMCOMPARISONFrame();


    graphPlot->LofarGraphLegend->setParent(LofarFrame);
    graphPlot->LofarWaterFallLegend->setParent(LofarFrame);
    graphPlot->LofarZoomGraphLegend->setParent(LofarFrame);
    graphPlot->SpectrumGraphLegend->setParent(SpectrumFrame);
    graphPlot->SpectrumWaterFallLegend->setParent(SpectrumFrame);
    graphPlot->SpectrumZoomGraphLegend->setParent(SpectrumFrame);
    graphPlot->OctaveBandLegendPlot->setParent(OctaveFrame);
                //---RAW DATA---//
    graphPlot->RawDataDelSpectGraphLegend->setParent(RawDataFrame);
    graphPlot->RawDataGraphLegend->setParent(RawDataFrame);
                //---RAW DATA---//
              //----SPECTRUM COMPARISON---//
    graphPlot-> SpectrumComparisonGraphLegend->setParent(Spec_Comp_Frame);
               //----SPECTRUM COMPARISON---//

    for(iCount=0;iCount<MAX_SENSOR_GROUP;iCount++)
    {
 //graphPlot->LFTINTWaterFallview[iCount]->setParent(IntLofarFrame);
    graphPlot->LofarINT[iCount]->setParent(IntLofarFrame);
    graphPlot->SpectINT[iCount]->setParent(IntSpectrumFrame);
    MGroupSensor[iCount] = new QLabel(IntLofarFrame);
    MGroupSensor[iCount]->setObjectName(QString::fromUtf8("label"));
    MGroupSensor[iCount]->setGeometry(QRect(10,1,29,12));
    MGroupSensor[iCount]->setText("CH-");
    MGroupSensor[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(255,255, 0);"));
    MGroupSensor[iCount]->show();

    OctaveGroupSensor[iCount] = new QLabel(OctaveFrame);
    OctaveGroupSensor[iCount]->setObjectName(QString::fromUtf8("label"));
    OctaveGroupSensor[iCount]->setGeometry(QRect(10,1,29,12));
    OctaveGroupSensor[iCount]->setText("CH-");
    OctaveGroupSensor[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(255,255, 0);"));
    OctaveGroupSensor[iCount]->show();
    }

    MGroupSensor[0]->setGeometry(QRect(10,50,29,12));
    MGroupSensor[0]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(0,255, 0);"));
    MGroupSensor[1]->setGeometry(QRect(10,175,29,12));
    MGroupSensor[1]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(255,0, 0);"));

    MGroupSensor[2]->setGeometry(QRect(10,300,29,12));
    MGroupSensor[2]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(0,0,255);"));

    MGroupSensor[3]->setGeometry(QRect(10,425,29,12));
    MGroupSensor[3]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(255,255,0);"));

    MGroupSensor[0]->setText("H1");
    MGroupSensor[1]->setText("H2");
    MGroupSensor[2]->setText("H3");
    MGroupSensor[3]->setText("H4");

    OctaveGroupSensor[0]->setGeometry(QRect(10,50,29,12));
    OctaveGroupSensor[0]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(0,255, 0);"));
    OctaveGroupSensor[1]->setGeometry(QRect(10,175,29,12));
    OctaveGroupSensor[1]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(255,0, 0);"));

    OctaveGroupSensor[2]->setGeometry(QRect(10,300,29,12));
    OctaveGroupSensor[2]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(0,0,255);"));

    OctaveGroupSensor[3]->setGeometry(QRect(10,420,29,12));
    OctaveGroupSensor[3]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(255,255,0);"));

    OctaveGroupSensor[0]->setText("H1");
    OctaveGroupSensor[1]->setText("H2");
    OctaveGroupSensor[2]->setText("H3");
    OctaveGroupSensor[3]->setText("H4");
    graphPlot->zoomLofar_yScale_TickLabel2->hide();
    graphPlot->Lofar_yScale_TickLabel2->hide();
}

void DisplayWindow::DrawSubmarimeLayoutFrame()
{
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);

    yCount=10;
    DisplaySubmarineLayoutFrame=new QFrame(DisplayBaseFrame);
    DisplaySubmarineLayoutFrame->setMouseTracking(true);
    DisplaySubmarineLayoutFrame->setGeometry(QRect(0,611,790,120));
 //DisplaySubmarineLayoutFrame->setGeometry(QRect(0,320,790,210));
    DisplaySubmarineLayoutFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0, 0);"));
    DisplaySubmarineLayoutFrame->setFrameShape(QFrame::StyledPanel);
    graphicView=new QGraphicsView( DisplaySubmarineLayoutFrame);
    graphicScene=new QGraphicsScene(graphicView);
    HLayout = new QHBoxLayout(DisplaySubmarineLayoutFrame);
    HLayout->setMargin(0);
    graphicScene->setBackgroundBrush(Qt::black);
    graphicView->setRenderHints(QPainter::Antialiasing);

 //graphicView->setInteractive(true);

    for(iCount=0;iCount<ACCELEROMETER_SENSOR;iCount++)
    {
    ACCSENSOR[iCount] = new QGraphicsRectItem(0,graphicScene);
    ACCSENSOR[iCount]->setFlag(QGraphicsRectItem::ItemIsSelectable);
    ACCSENSOR[iCount]->setPen( QPen(Qt::black) );
    ACCSENSOR[iCount]->setBrush( Qt::gray);
    ACCSENSOR[iCount]->setAcceptedMouseButtons(Qt::LeftButton);
    ACCSENSOR[iCount]->setToolTip(AccelerometerName[iCount]);
    }
    yCount=10;

    for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
    {
    HYDSENSOR[iCount] = new QGraphicsEllipseItem(0,graphicScene);
    HYDSENSOR[iCount]->setFlag(QGraphicsEllipseItem::ItemIsSelectable);
    HYDSENSOR[iCount]->setPen( QPen(Qt::black) );
    HYDSENSOR[iCount]->setBrush( Qt::gray );
    HYDSENSOR[iCount]->setAcceptedMouseButtons(Qt::LeftButton);
    HYDSENSOR[iCount]->setToolTip(HydrophoneName[iCount]);
 //HYDSENSOR[iCount]->hide();
    }

  //HYDSENSOR[0]->setRect(20,73,10,10);
   /* HYDSENSOR[0]->setRect(342,30,12,12);
    HYDSENSOR[1]->setRect(362,30,12,12);
    HYDSENSOR[2]->setRect(382,30,12,12);
    HYDSENSOR[3]->setRect(404,30,12,12);
    HYDSENSOR[4]->setRect(426,30,12,12);
    HYDSENSOR[5]->setRect(448,30,12,12);
    HYDSENSOR[6]->setRect(470,30,12,12);
    HYDSENSOR[7]->setRect(492,30,12,12);
    HYDSENSOR[8]->setRect(342,40,12,12);
    HYDSENSOR[9]->setRect(362,50,12,12);
    HYDSENSOR[10]->setRect(382,30,12,12);
    HYDSENSOR[11]->setRect(404,30,12,12);
    HYDSENSOR[12]->setRect(426,30,12,12);
    HYDSENSOR[13]->setRect(448,30,12,12);
    HYDSENSOR[14]->setRect(470,30,12,12);
    HYDSENSOR[15]->setRect(492,30,12,12);


    HYDSENSOR[16]->setRect(390,130,12,12);
    HYDSENSOR[17]->setRect(480,125,12,12);
    HYDSENSOR[18]->setRect(580,5,12,12);
    HYDSENSOR[19]->setRect(680,25,12,12);
    HYDSENSOR[20]->setRect(620,140,12,12);
    HYDSENSOR[21]->setRect(800,25,12,12);
    HYDSENSOR[22]->setRect(850,140,12,12);
    HYDSENSOR[23]->setRect(870,35,12,12);
    HYDSENSOR[24]->setRect(900,30,12,12);
    HYDSENSOR[25]->setRect(970,26,12,12);
    HYDSENSOR[26]->setRect(1000,5,12,12);
    HYDSENSOR[27]->setRect(1090,20,12,12);
    HYDSENSOR[28]->setRect(1210,70,12,12);


    ACCSENSOR[0]->setRect(514,30,12,12);
    ACCSENSOR[1]->setRect(536,30,12,12);
    ACCSENSOR[2]->setRect(558,30,12,12);
    ACCSENSOR[3]->setRect(580,30,12,12);
    ACCSENSOR[4]->setRect(602,30,12,12);
    ACCSENSOR[5]->setRect(624,30,12,12);
    ACCSENSOR[6]->setRect(644,30,12,12);
    ACCSENSOR[7]->setRect(666,30,12,12);

    ACCSENSOR[8]->setRect(450,7,12,12);
    ACCSENSOR[9]->setRect(490,7,12,12);
    ACCSENSOR[10]->setRect(450,135,12,12);
    ACCSENSOR[11]->setRect(600,135,12,12);
    ACCSENSOR[12]->setRect(600,7,12,12);
    ACCSENSOR[13]->setRect(450,5,12,12);
    ACCSENSOR[14]->setRect(660,20,12,12);
    ACCSENSOR[15]->setRect(690,142,12,12);
    ACCSENSOR[16]->setRect(700,125,12,12);
    ACCSENSOR[17]->setRect(780,110,12,12);
    ACCSENSOR[18]->setRect(820,10,12,12);
    ACCSENSOR[19]->setRect(770,130,12,12);
    ACCSENSOR[20]->setRect(820,140,12,12);
    ACCSENSOR[21]->setRect(960,5,12,12);
    ACCSENSOR[22]->setRect(1020,5,12,12);
    ACCSENSOR[23]->setRect(920,130,12,12);
    ACCSENSOR[24]->setRect(920,13,12,12);
    ACCSENSOR[25]->setRect(960,140,12,12);
    ACCSENSOR[26]->setRect(980,5,12,12);
    ACCSENSOR[27]->setRect(980,140,12,12);
    ACCSENSOR[28]->setRect(1090,120,12,12);
    ACCSENSOR[29]->setRect(1120,110,12,12);
    ACCSENSOR[30]->setRect(1150,55,12,12);
    ACCSENSOR[31]->setRect(1190,65,12,12);
*/
    for(iCount=0;iCount<MAX_SCU_NO;iCount++)
    {
      SCUName[iCount]= new QLabel(DisplaySubmarineLayoutFrame);
      SCUName[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
                                                       "color: rgb(0,0, 0);"));
      SCUName[iCount]->setFont(font);
    //SCUName[iCount]->hide();
    }

    SCUName[0]->setGeometry(QRect(15,8,45,19));
    SCUName[0]->setText("SCU-1");
    SCUName[1]->setGeometry(QRect(15,38,45,19));
    SCUName[1]->setText("SCU-2");
    SCUName[2]->setGeometry(QRect(15,68,45,19));
    SCUName[2]->setText("SCU-3");
    SCUName[3]->setGeometry(QRect(15,97,45,19));
    SCUName[3]->setText("SCU-4");

  /*SCUName[4]->setGeometry(QRect(WIDTH*0.42,HEIGHT*0.3,45,19));
    SCUName[4]->setText("C4-S5");
    SCUName[5]->setGeometry(QRect(WIDTH*0.52,HEIGHT*0.3,45,19));
    SCUName[5]->setText("C5-S6");
    SCUName[6]->setGeometry(QRect(WIDTH*0.62,HEIGHT*0.3,45,19));
    SCUName[6]->setText("C6-S7");
    SCUName[7]->setGeometry(QRect(WIDTH*0.72,HEIGHT*0.6,45,19));
    SCUName[7]->setText("C7-S8");
    SCUName[8]->setGeometry(QRect(WIDTH*0.72,HEIGHT*0.3,45,19));
    SCUName[8]->setText("C7-S9");
    SCUName[9]->setGeometry(QRect(WIDTH*0.82,HEIGHT*0.3,52,19));
    SCUName[9]->setText("C8-S10");*/

    SCUName[4]->hide();
    SCUName[5]->hide();
    SCUName[6]->hide();
    SCUName[7]->hide();
    SCUName[8]->hide();
    SCUName[9]->hide();

    for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
    {
      HydrophoneLabel[iCount]= new QLabel(DisplaySubmarineLayoutFrame);
      HydrophoneLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                       "color: rgb(207,179,255);"));
      HydrophoneLabel[iCount]->setFont(font);
      HydrophoneLabel[iCount]->show();
    }

    for(iCount=0;iCount<ACCELEROMETER_SENSOR;iCount++)
    {
      AcceleometerLabel[iCount]= new QLabel(DisplaySubmarineLayoutFrame);

      AcceleometerLabel[iCount]->setFont(font);
      AcceleometerLabel[iCount]->show();
      AcceleometerLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                       "color: rgb(207,179,255);"));
    }

    yCount=0;
    for(iCount=0;iCount<8;iCount++)
    {
    HydrophoneLabel[iCount]->setGeometry(QRect((80+yCount),8,22,17));
    HydrophoneLabel[iCount]->show();
    yCount=yCount+42;
    }
    yCount=yCount+10;
    for(iCount=0;iCount<8;iCount++)
    {
    AcceleometerLabel[iCount]->setGeometry(QRect((80+yCount),8,22,17));
    AcceleometerLabel[iCount]->show();
    yCount=yCount+42;
    }
    HydrophoneLabel[0]->setText("H1");
    AcceleometerLabel[0]->setText("A1");
    HydrophoneLabel[1]->setText("H2");
    AcceleometerLabel[1]->setText("A2");
    HydrophoneLabel[2]->setText("H3");
    AcceleometerLabel[2]->setText("A3");
    HydrophoneLabel[3]->setText("H4");
    AcceleometerLabel[3]->setText("A4");
    HydrophoneLabel[4]->setText("H5");
    AcceleometerLabel[4]->setText("A5");
    HydrophoneLabel[5]->setText("H6");
    AcceleometerLabel[5]->setText("A6");
    HydrophoneLabel[6]->setText("H7");
    AcceleometerLabel[6]->setText("A7");
    HydrophoneLabel[7]->setText("H8");
    AcceleometerLabel[7]->setText("A8");

    yCount=0;
    for(iCount=0;iCount<8;iCount++)
    {
    HydrophoneLabel[iCount+8]->setGeometry(QRect((80+yCount),38,28,17));
    HydrophoneLabel[iCount+8]->show();
    yCount=yCount+42;
    }
    yCount=yCount+10;
    for(iCount=0;iCount<8;iCount++)
    {
    AcceleometerLabel[iCount+8]->setGeometry(QRect((80+yCount),38,28,17));
    AcceleometerLabel[iCount+8]->show();
    yCount=yCount+42;
    }
    HydrophoneLabel[8]->setText("H9");
    AcceleometerLabel[8]->setText("A9");
    HydrophoneLabel[9]->setText("H10");
    AcceleometerLabel[9]->setText("A10");
    HydrophoneLabel[10]->setText("H11");
    AcceleometerLabel[10]->setText("A11");
    HydrophoneLabel[11]->setText("H12");
    AcceleometerLabel[11]->setText("A12");
    HydrophoneLabel[12]->setText("H13");
    AcceleometerLabel[12]->setText("A13");
    HydrophoneLabel[13]->setText("H14");
    AcceleometerLabel[13]->setText("A14");
    HydrophoneLabel[14]->setText("H15");
    AcceleometerLabel[14]->setText("A15");
    HydrophoneLabel[15]->setText("H16");
    AcceleometerLabel[15]->setText("A16");


    yCount=0;
    for(iCount=0;iCount<8;iCount++)
    {
    HydrophoneLabel[iCount+16]->setGeometry(QRect((80+yCount),68,28,17));
    HydrophoneLabel[iCount+16]->show();
    yCount=yCount+42;
    }
    yCount=yCount+10;
    for(iCount=0;iCount<8;iCount++)
    {
    AcceleometerLabel[iCount+16]->setGeometry(QRect((80+yCount),68,28,17));
    AcceleometerLabel[iCount+16]->show();
    yCount=yCount+42;
    }
    HydrophoneLabel[16]->setText("H17");
    AcceleometerLabel[16]->setText("A17");
    HydrophoneLabel[17]->setText("H18");
    AcceleometerLabel[17]->setText("A18");
    HydrophoneLabel[18]->setText("H19");
    AcceleometerLabel[18]->setText("A20");
    HydrophoneLabel[19]->setText("H21");
    AcceleometerLabel[19]->setText("A21");
    HydrophoneLabel[20]->setText("H22");
    AcceleometerLabel[20]->setText("A22");
    HydrophoneLabel[21]->setText("H22");
    AcceleometerLabel[21]->setText("A22");
    HydrophoneLabel[22]->setText("H23");
    AcceleometerLabel[22]->setText("A23");
    HydrophoneLabel[23]->setText("H24");
    AcceleometerLabel[23]->setText("A24");

    yCount=0;
    for(iCount=0;iCount<5;iCount++)
    {
    HydrophoneLabel[iCount+24]->setGeometry(QRect((80+yCount),98,28,17));
    HydrophoneLabel[iCount+24]->show();
    yCount=yCount+42;
    }
    yCount=336;
    yCount=yCount+10;
    for(iCount=0;iCount<8;iCount++)
    {
    AcceleometerLabel[iCount+24]->setGeometry(QRect((80+yCount),98,28,17));
    AcceleometerLabel[iCount+24]->show();
    yCount=yCount+42;
    }
    HydrophoneLabel[24]->setText("H25");
    AcceleometerLabel[24]->setText("A25");
    HydrophoneLabel[25]->setText("H26");
    AcceleometerLabel[25]->setText("A26");
    HydrophoneLabel[26]->setText("H27");
    AcceleometerLabel[26]->setText("A27");
    HydrophoneLabel[27]->setText("H28");
    AcceleometerLabel[27]->setText("A28");
    HydrophoneLabel[28]->setText("H29");
    AcceleometerLabel[28]->setText("A29");
    AcceleometerLabel[29]->setText("A30");
    AcceleometerLabel[30]->setText("A31");
    AcceleometerLabel[31]->setText("A32");

    graphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicView->setScene(graphicScene);
    HLayout->addWidget(graphicView);
    DisplaySubmarineLayoutFrame->show();
    DisplaySubmarineLayoutFrame->setFrameShadow(QFrame::Raised);
}


void DisplayWindow::DrawThresholdWarningFrame()
{
    DisplayThresholdWarningFrame = new QFrame(DisplayBaseFrame);
    DisplayThresholdWarningFrame->setMouseTracking(true);
    DisplayThresholdWarningFrame->setGeometry(QRect(0,731,790,35));
  //DisplayThresholdWarningFrame->setGeometry(QRect(0,250,790,35));
    DisplayThresholdWarningFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0, 0);"));
    DisplayThresholdWarningFrame->setFrameShape(QFrame::StyledPanel);
    DisplayThresholdWarningFrame->setFrameShadow(QFrame::Raised);
    QFont font;
    font.setFamily(QString::fromUtf8("SansSerif"));
    font.setPointSize(8);
    font.setBold(false);
    font.setPixelSize(12);
    ThresholdWarningLabel= new QLabel(DisplayThresholdWarningFrame);
    ThresholdWarningLabel->setGeometry(QRect(2,2,1360,30));
    ThresholdWarningLabel->setObjectName(QString::fromUtf8("THRTWarning"));
    ThresholdWarningLabel->setFont(font);
  //DisplayThresholdWarningFrame->hide();
}

void DisplayWindow::DrawAnnotationFrame()
{
    DisplayAnnotationFrame = new QFrame(DisplayBaseFrame);
    DisplayAnnotationFrame->setMouseTracking(true);
    DisplayAnnotationFrame->setGeometry(QRect(790,0,233,670));
 //DisplayAnnotationFrame->setGeometry(QRect(450,0,233,1024));
    DisplayAnnotationFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    DisplayAnnotationFrame->setFrameShape(QFrame::StyledPanel);
    DisplayAnnotationFrame->setFrameShadow(QFrame::Raised);
 //DisplayAnnotationFrame->setStyleSheet("border: 5px solid black");
    AnnotationControls();
}

void DisplayWindow::DrawObservaionFrame()
{
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);
    DisplayObservationFrame=new QFrame(DisplayBaseFrame);
    DisplayObservationFrame->setMouseTracking(true);
    DisplayObservationFrame->setGeometry(QRect(790,670,233,176));
  //DisplayObservationFrame->setGeometry(QRect(700,600,233,176));
    DisplayObservationFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    DisplayObservationFrame->setFrameShape(QFrame::StyledPanel);
    DisplayObservationFrame->setFrameShadow(QFrame::Raised);
    DisplayGain = new QLabel(DisplayObservationFrame);
    DisplayGain->setObjectName(QString::fromUtf8("label"));
    DisplayGain->setFont(font);
    DisplayGain->setGeometry(QRect(10,15,100,20));
    DisplayGain->setText("GAIN LEVEL   :");
    DisplayGain->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(207,179,255);"));
    DisplayGain->show();
    DisplayFreq = new QLabel(DisplayObservationFrame);
    DisplayFreq->setObjectName(QString::fromUtf8("label"));
    DisplayFreq->setFont(font);

    DisplayFreq->setGeometry(QRect(10,45,100,20));
    DisplayFreq->setText("FREQUENCY  :");
    DisplayFreq->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(207,179,255);"));
    DisplayFreq->show();
    DisplayAmp = new QLabel(DisplayObservationFrame);
    DisplayAmp->setObjectName(QString::fromUtf8("label"));
    DisplayAmp->setGeometry(QRect(10,75,100,20));
    DisplayAmp->setText("AMPLITUDE   :");
    DisplayAmp->setFont(font);

    DisplayAmp->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(207,179,255);"));
    DisplayAmp->show();
    GainVal = new QLabel(DisplayObservationFrame);
    GainVal->setObjectName(QString::fromUtf8("label"));
    GainVal->setGeometry(QRect(130,15,100,20));
    GainVal->setFont(font);
    GainVal->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(207,179,255);"));
    GainVal->show();
    FreqVal = new QLabel(DisplayObservationFrame);
    FreqVal->setObjectName(QString::fromUtf8("label"));
    FreqVal->setGeometry(QRect(130,45,100,20));
    FreqVal->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(207,179,255);"));
    FreqVal->show();
    FreqVal->setFont(font);

    AmpVal = new QLabel(DisplayObservationFrame);
    AmpVal->setObjectName(QString::fromUtf8("label"));
    AmpVal->setGeometry(QRect(130,75,100,20));
    AmpVal->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(207,179,255);"));
    AmpVal->show();
    AmpVal->setFont(font);
}


void DisplayWindow::AnnotationControls()
{
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);

            PageSelection=new QComboBox(DisplayAnnotationFrame);
            PageSelection->setGeometry(QRect(0,0,233,33));
          //PageSelection->setStyleSheet(QString::fromUtf8("selection-color: rgb(170, 0, 0);"));
          //PageSelection->setFont(font);
            PageSelection->clear();
            PageSelection->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                           "color: rgb(0, 0, 0);"));
            //PageSelection->addItem("LOFAR PAGE");
	    PageSelection->addItem("SPECTRUM PAGE",1);
            PageSelection->addItem("INTEGRATED LOFAR  PAGE",2);
            PageSelection->addItem("INTEGRATED SPECTRUM PAGE",3);
            PageSelection->addItem("1/3 OCTAVE BANDS",4);
            PageSelection->addItem("TRACK PAGE",5);
            PageSelection->addItem("TIME AVERAGED SPECTRUM",6);
	    PageSelection->addItem("SPECTRUM COMPARISON",7);
            PageSelection->addItem("HMI EXIT",8);

            ToolBoxWidget= new QToolBox(DisplayAnnotationFrame);
            ToolBoxWidget->setObjectName(QString::fromUtf8("toolBox"));
            ToolBoxWidget->setGeometry(QRect(0,33,233,640));
            ToolBoxWidget->setAcceptDrops(false);
            ToolBoxWidget->setAutoFillBackground(true);
            ToolBoxWidget->setFrameShadow(QFrame::Raised);
            ToolBoxWidget->setFrameShape(QFrame::StyledPanel);
            ToolBoxWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                           "color: rgb(30,255,41);"));
            Annotation= new QFrame();
            Annotation->setObjectName(QString::fromUtf8("page"));
            Annotation->setGeometry(QRect(0, 0, 79, 79));
            Configuration= new QFrame();
            Configuration->setObjectName(QString::fromUtf8("page_2"));
            Configuration->setGeometry(QRect(0, 0, 201, 79));
            HydrophoneEng= new QFrame();
            HydrophoneEng->setObjectName(QString::fromUtf8("page_2"));
            HydrophoneEng->setGeometry(QRect(0, 0, 201, 79));
	    
	    ZoomWindow= new QFrame();
            ZoomWindow->setObjectName(QString::fromUtf8("page_3"));
            ZoomWindow->setGeometry(QRect(0, 0, 201, 79));
            ToolBoxWidget->addItem( Annotation, QString::fromUtf8("                 Annotation       "));
            ToolBoxWidget->addItem(Configuration, QString::fromUtf8("           Process Configuration    "));
            ToolBoxWidget->addItem(HydrophoneEng, QString::fromUtf8("             Hydrophone Energy    "));
	    ToolBoxWidget->addItem(ZoomWindow, QString::fromUtf8("                  Zoom    "));
            ToolBoxWidget->setCurrentIndex(0);

            HistogramFrame->setParent(HydrophoneEng);

            EnergyCursorSave = new QPushButton(HydrophoneEng);
            EnergyCursorSave->setGeometry(QRect(50,15,155,30));
            EnergyCursorSave->setDefault(true);
            EnergyCursorSave->setFlat(false);
            EnergyCursorSave->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                               "color: rgb(0, 0, 0);"));
            EnergyCursorSave->setText(" Histogram Update");

            POPThresoldWindow = new QPushButton(Annotation);
            POPThresoldWindow->setGeometry(QRect(60, 10,125,30));
            POPThresoldWindow->setDefault(true);
            POPThresoldWindow->setFlat(false);
            POPThresoldWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                               "color: rgb(0, 0, 0);"));

            POPThresoldWindow->setText("Threshold Set");

            POPTrackWindow = new QPushButton(Annotation);
            POPTrackWindow->setGeometry(QRect(60, 50, 125, 30));
            POPTrackWindow->setDefault(true);
            POPTrackWindow->setFlat(false);
            POPTrackWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                            "color: rgb(0, 0, 0);"));
            POPTrackWindow->setText("Track Set");


            RecordButton =new QPushButton(Annotation);
            RecordButton->setObjectName(QString::fromUtf8("RECORD"));
            RecordButton->setGeometry(QRect(60, 90, 125, 30));
            RecordButton->setText(QApplication::translate("Frame", "Record", 0, QApplication::UnicodeUTF8));
            RecordButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
            RecordButton->setFont(font);

            ReplayButton =new QPushButton(Annotation);
            ReplayButton->setObjectName(QString::fromUtf8("REPLAY"));
            ReplayButton->setGeometry(QRect(60, 130, 125, 30));
            ReplayButton->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
            ReplayButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
            ReplayButton->setFont(font);

            SensorGroup =new QPushButton(Annotation);
            SensorGroup->setObjectName(QString::fromUtf8("REPLAY"));
            SensorGroup->setGeometry(QRect(60, 170, 125, 30));
            SensorGroup->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
            SensorGroup->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
            SensorGroup->setFont(font);
            SensorGroup->setText("SEN GROUP");

            OctaveButton =new QPushButton(Annotation);
            OctaveButton->setObjectName(QString::fromUtf8("1/3 OCTAVE"));
            OctaveButton->setGeometry(QRect(60,210, 125, 30));
            OctaveButton->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
            OctaveButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
            OctaveButton->setFont(font);
            OctaveButton->setText("1/3 OCTAVE");

	    RecChnlButton =new QPushButton(Annotation);
            RecChnlButton->setObjectName(QString::fromUtf8("CHNL REC"));
            RecChnlButton->setGeometry(QRect(60,250, 125, 30));
            RecChnlButton->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
            RecChnlButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
            RecChnlButton->setFont(font);
            RecChnlButton->setText("CHNL REC");

	    LFreeze = new QPushButton(Annotation);
            LFreeze->setObjectName(QString::fromUtf8(""));
            LFreeze->setGeometry(QRect(60,290, 125, 30));
            LFreeze->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
            LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
            LFreeze->setFont(font);
            LFreeze->setText("FREEZE");
            LFreeze->show();

	    ReplayButtonCont =new QPushButton(Annotation);
            ReplayButtonCont->setObjectName(QString::fromUtf8("CHNL REC"));
            ReplayButtonCont->setGeometry(QRect(60,330, 125, 30));
            ReplayButtonCont->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
            ReplayButtonCont->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
            ReplayButtonCont->setFont(font);
            ReplayButtonCont->setText("NW REPLAY");



        UnitButton =new QPushButton(Annotation);
        UnitButton->setObjectName(QString::fromUtf8("REPLAY"));
        UnitButton->setGeometry(QRect(60, 280, 125, 30));
        UnitButton->setText(QApplication::translate("Frame", "Replay", 0, QApplication::UnicodeUTF8));
        UnitButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                      "color: rgb(0, 0, 0);"));
        UnitButton->setFont(font);
        UnitButton->setText("METRIC UNITs");
        UnitButton->hide();


            HydLabel = new QLabel(Configuration);
            HydLabel->setObjectName(QString::fromUtf8("label"));
            HydLabel->setGeometry(QRect(10,70,120,33));
            HydLabel->setText("HYD_SEL");
            HydLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(0,255,0);"));
            HydLabel->setFont(font);
        HydrophoneSel=new QComboBox(Configuration);
        HydrophoneSel->setGeometry(QRect(90,70,120,30));
        HydrophoneSel->clear();
        HydrophoneSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                       "color: rgb(0, 0, 0);"));
        HydrophoneSel->addItem("H1");
        HydrophoneSel->addItem("H2");
        HydrophoneSel->addItem("H3");
        HydrophoneSel->addItem("H4");
        HydrophoneSel->addItem("H5");
        HydrophoneSel->addItem("H6");
        HydrophoneSel->addItem("H7");
        HydrophoneSel->addItem("H8");
        HydrophoneSel->addItem("H9");
        HydrophoneSel->addItem("H10");
        HydrophoneSel->addItem("H11");
        HydrophoneSel->addItem("H12");
        HydrophoneSel->addItem("H13");
        HydrophoneSel->addItem("H14");
        HydrophoneSel->addItem("H15");
        HydrophoneSel->addItem("H16");
        HydrophoneSel->addItem("H17");
        HydrophoneSel->addItem("H18");
        HydrophoneSel->addItem("H19");
        HydrophoneSel->addItem("H20");
        HydrophoneSel->addItem("H21");
        HydrophoneSel->addItem("H22");
        HydrophoneSel->addItem("H23");
        HydrophoneSel->addItem("H24");
        HydrophoneSel->addItem("H25");
        HydrophoneSel->addItem("H26");
        HydrophoneSel->addItem("H27");
        HydrophoneSel->addItem("H28");
        HydrophoneSel->addItem("H29");

        AccLabel = new QLabel(Configuration);
        AccLabel ->setObjectName(QString::fromUtf8("label"));
        AccLabel ->setGeometry(QRect(10,110,120,33));
        AccLabel ->setText("ACC_SEL");
        AccLabel ->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
            "color: rgb(0,255,0);"));
        AccLabel ->setFont(font);
        AccelerometerSel=new QComboBox(Configuration);
        AccelerometerSel->setGeometry(QRect(90,110,120,30));
        AccelerometerSel->clear();
        AccelerometerSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
        AccelerometerSel->addItem("A1");
        AccelerometerSel->addItem("A2");
        AccelerometerSel->addItem("A3");
        AccelerometerSel->addItem("A4");
        AccelerometerSel->addItem("A5");
        AccelerometerSel->addItem("A6");
        AccelerometerSel->addItem("A7");
        AccelerometerSel->addItem("A8");
        AccelerometerSel->addItem("A9");
        AccelerometerSel->addItem("A10");
        AccelerometerSel->addItem("A11");
        AccelerometerSel->addItem("A12");
        AccelerometerSel->addItem("A13");
        AccelerometerSel->addItem("A14");
        AccelerometerSel->addItem("A15");
        AccelerometerSel->addItem("A16");
        AccelerometerSel->addItem("A17");
        AccelerometerSel->addItem("A18");
        AccelerometerSel->addItem("A19");
        AccelerometerSel->addItem("A20");
        AccelerometerSel->addItem("A21");
        AccelerometerSel->addItem("A22");
        AccelerometerSel->addItem("A23");
        AccelerometerSel->addItem("A24");
        AccelerometerSel->addItem("A25");
        AccelerometerSel->addItem("A26");
        AccelerometerSel->addItem("A27");
        AccelerometerSel->addItem("A28");
        AccelerometerSel->addItem("A29");
        AccelerometerSel->addItem("A30");
        AccelerometerSel->addItem("A31");
        AccelerometerSel->addItem("A32");

                ZoomLabel = new QLabel(Configuration);
                ZoomLabel->setObjectName(QString::fromUtf8("label"));
                ZoomLabel->setGeometry(QRect(10,150,120,33));
                ZoomLabel->setText("ZOOM_B");
                ZoomLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                ZoomLabel->setFont(font);

                ZoomSpectrum=new QComboBox(Configuration);
                ZoomSpectrum->setGeometry(QRect(90,150,120,30));
                ZoomSpectrum->clear();
                ZoomSpectrum->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                              "color: rgb(0, 0, 0);"));
                ZoomSpectrum->addItem("244");
                ZoomSpectrum->addItem("122");
                ZoomSpectrum->hide();

                ZoomLofar=new QComboBox(Configuration);
                ZoomLofar->setGeometry(QRect(90,150,120,30));
                ZoomLofar->clear();
                ZoomLofar->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                           "color: rgb(0, 0, 0);"));
                ZoomLofar->addItem("200");
                ZoomLofar->addItem("100");
                ZoomLofar->addItem("50");
                ZoomLofar->show();

                Tclabel = new QLabel(Configuration);
                Tclabel->setObjectName(QString::fromUtf8("label"));
                Tclabel->setGeometry(QRect(10,190,120,33));
                Tclabel->setText("TC");
                Tclabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                Tclabel->setFont(font);

                Tc=new QComboBox(Configuration);
                Tc->setGeometry(QRect(90,190,120,30));
                Tc->clear();
                Tc->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                    "color: rgb(0, 0, 0);"));
                Tc->addItem("2");
                Tc->addItem("4");
                Tc->addItem("8");

                AgcLabel = new QLabel(Configuration);
                AgcLabel->setObjectName(QString::fromUtf8("label"));
                AgcLabel->setGeometry(QRect(10,230,120,33));
                AgcLabel->setText("AGC");
                AgcLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                AgcLabel->setFont(font);

                Agc=new QComboBox(Configuration);
                Agc->setGeometry(QRect(90,230,120,30));
                Agc->clear();
                Agc->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                     "color: rgb(0, 0, 0);"));
                Agc->addItem("HIGH");
                Agc->addItem("MEDIUM");
                Agc->addItem("LOW");

                MgcLabel = new QLabel(Configuration);
                MgcLabel->setObjectName(QString::fromUtf8("label"));
                MgcLabel->setGeometry(QRect(10,270,120,33));
                MgcLabel->setText("MGC");
                MgcLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                MgcLabel->setFont(font);

                Mgc=new QComboBox(Configuration);
                Mgc->setGeometry(QRect(90,270,120,30));
                Mgc->clear();
                Mgc->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                     "color: rgb(0, 0, 0);"));
                Mgc->addItem("-13 dB");
                Mgc->addItem("-4  dB");
                Mgc->addItem("-1  dB");
                Mgc->addItem(" 0  dB");
                Mgc->addItem(" 6  dB");
                Mgc->addItem(" 12 dB");
                Mgc->addItem(" 20 dB");
                Mgc->addItem(" 26 dB");
                Mgc->addItem(" 40 dB");
                Mgc->addItem(" 60 dB");
                Mgc->setCurrentIndex(3);

                PresetLabel = new QLabel(Configuration);
                PresetLabel->setObjectName(QString::fromUtf8("label"));
                PresetLabel->setGeometry(QRect(10,310,120,33));
                PresetLabel->setText("PRESET");
                PresetLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                PresetLabel->setFont(font);

                Preset=new QComboBox(Configuration);
                Preset->setGeometry(QRect(90,310,120,30));
                Preset->clear();
                Preset->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                        "color: rgb(0, 0, 0);"));
                Preset->addItem("-13 dB");
                Preset->addItem("-4  dB");
                Preset->addItem("-1  dB");
                Preset->addItem(" 0  dB");
                Preset->addItem(" 6  dB");
                Preset->addItem(" 12 dB");
                Preset->addItem(" 20  dB");
                Preset->addItem(" 26  dB");
                Preset->addItem(" 40 dB");
                Preset->addItem(" 60 dB");
                Preset->setCurrentIndex(3);

              /*
                AudioLabel = new QLabel(Configuration);
                AudioLabel->setObjectName(QString::fromUtf8("label"));
                AudioLabel->setGeometry(QRect(10,350,120,33));
                AudioLabel->setText("AUDIO");
                AudioLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                AudioLabel->setFont(font);
              */


       /*       Audio=new QComboBox(Configuration);
                Audio->setGeometry(QRect(90,350,120,30));
                Audio->clear();
                Audio->setStyleSheet(QString::fromUtf8("background-color: rgb(250, 250, 250);\n"
                                                       "color: rgb(0, 0, 0);"));
                Audio->addItem("HIGH");
                Audio->addItem("MEDIUM");
                Audio->addItem("LOW");
        */

                TrackLabel = new QLabel(Configuration);
                TrackLabel->setObjectName(QString::fromUtf8("label"));
                TrackLabel->setGeometry(QRect(10,230,120,33));
                TrackLabel->setText("TRACKINFO");
                TrackLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                    "color: rgb(0,255,0);"));
                TrackLabel->setFont(font);

                Track=new QComboBox(Configuration);
                Track->setGeometry(QRect(90,230,120,30));
                Track->clear();
                Track->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                       "color: rgb(0, 0, 0);"));
                Track->addItem("1");
                Track->addItem("2");
                Track->addItem("3");
                Track->addItem("4");
                Track->addItem("5");
                Track->addItem("6");
                Track->addItem("7");
                Track->addItem("8");
                Track->addItem("9");
                Track->addItem("10");
                Track->addItem("11");
                Track->addItem("12");
                Track->addItem("13");
                Track->addItem("14");
                Track->addItem("15");
                Track->addItem("16");
                Track->addItem("17");
                Track->addItem("18");
                Track->addItem("19");
                Track->addItem("20");
                Track->addItem("21");
                Track->addItem("22");
                Track->addItem("23");
                Track->addItem("24");
                Track->addItem("25");
                Track->addItem("26");
                Track->addItem("27");
                Track->addItem("28");
                Track->addItem("29");
                Track->addItem("30");
                Track->addItem("31");
                Track->addItem("32");
                Track->addItem("33");
                Track->addItem("34");
                Track->addItem("35");
                Track->addItem("36");
                Track->addItem("37");
                Track->addItem("38");
                Track->addItem("39");
                Track->addItem("40");
                Track->addItem("41");
                Track->addItem("42");
                Track->addItem("43");
                Track->addItem("44");
                Track->addItem("45");
                Track->addItem("46");
                Track->addItem("47");
                Track->addItem("48");
                Track->addItem("49");
                Track->addItem("50");
                Track->addItem("51");
                Track->addItem("52");
                Track->addItem("53");
                Track->addItem("54");
                Track->addItem("55");
                Track->addItem("56");
                Track->addItem("57");
                Track->addItem("58");
                Track->addItem("59");
                Track->addItem("60");
                Track->addItem("61");
                Track->addItem("62");
                Track->addItem("63");
                Track->addItem("64");

   /*
        ScaleLabel = new QLabel(Annotation);
        ScaleLabel->setObjectName(QString::fromUtf8("label"));
        ScaleLabel->setGeometry(QRect(10,345,120,33));
        ScaleLabel->setText("SCA CHANGE");
        ScaleLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
            "color: rgb(0,255,0);"));
        ScaleLabel->setFont(font);

             Scaleadjust=new QComboBox(Annotation);
               Scaleadjust->setGeometry(QRect(90,345,120,30));
               Scaleadjust->setFont(font);
               Scaleadjust->clear();
               Scaleadjust->setWindowModality(Qt::WindowModal);
               Scaleadjust->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                           "color: rgb(0, 0, 0);"));
               Scaleadjust->addItem("250");
               Scaleadjust->addItem("200");
               Scaleadjust->addItem("150");
               Scaleadjust->addItem("100");
            // Scaleadjust->addItem("50");
*/

                  AudioLabel = new QLabel(Annotation);
                  AudioLabel->setObjectName(QString::fromUtf8("label"));
                  AudioLabel->setGeometry(QRect(10,380,120,33));
                  AudioLabel->setText("REPLY SPEED");
                  AudioLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                      "color: rgb(0,255,0);"));
                  AudioLabel->setFont(font);

                TimeAdjust=new QComboBox(Annotation);
                TimeAdjust->setGeometry(QRect(90,380,120,30));
                TimeAdjust->setFont(font);
                TimeAdjust->clear();
                TimeAdjust->setWindowModality(Qt::WindowModal);
                TimeAdjust->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                            "color: rgb(0, 0, 0);"));
                TimeAdjust->addItem("8x");
                TimeAdjust->addItem("6x");
                TimeAdjust->addItem("4x");
                TimeAdjust->addItem("2x");
                TimeAdjust->addItem("1x");


                RecRply = new QLabel(Annotation);
                RecRply->setObjectName(QString::fromUtf8("label"));
                RecRply->setGeometry(QRect(10,415,120,33));
                RecRply->setText("REC/RPLY");
                RecRply->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                              "color: rgb(0,255,0);"));
                RecRply->setFont(font);

                progressbar2 = new QProgressBar(Annotation);
                progressbar2->setObjectName(QString::fromUtf8("progressBar"));
                progressbar2->setGeometry(QRect(90,420,120,20));
                progressbar2->setStyleSheet(QString::fromUtf8("background-color: rgb(170,170,170);"));
                progressbar2->setRange(0,REC_DATA_SEGLEN);
                progressbar2->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 170), stop:1 rgba(0, 0, 0, 0));\n"
                "background-color: rgb(0, 255, 0);"));

                       MemoryOverflow = new QLabel(Annotation);
                       MemoryOverflow->setObjectName(QString::fromUtf8("label"));
                       MemoryOverflow->setGeometry(QRect(10,450,120,33));
                       MemoryOverflow->setText("STORAGE");
                       MemoryOverflow->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                                     "color: rgb(0,255,0);"));
                       MemoryOverflow->setFont(font);

                progressbar = new QProgressBar(Annotation);
                progressbar->setObjectName(QString::fromUtf8("progressBar"));
                progressbar->setGeometry(QRect(90,452,120,20));
                progressbar->setStyleSheet(QString::fromUtf8("background-color: rgb(170,170,170);"));
                progressbar->setRange(0,2400);
                progressbar->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 170), stop:1 rgba(0, 0, 0, 0));\n"
                "background-color: rgb(0, 255, 0);"));


		//ZoomWindow
       Xmin =new QLabel(ZoomWindow);
       Xmin->setObjectName(QString::fromUtf8("label"));
       Xmin->setGeometry(QRect(40,40,50,30));
       Xmin->setText("Xmin");
       Xmin->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                              "color: rgb(0,255,0);"));
       Xmin->setFont(font);

       Xmax =new QLabel(ZoomWindow);
       Xmax->setObjectName(QString::fromUtf8("label"));
       Xmax->setGeometry(QRect(40,70,50,30));
       Xmax->setText("Xmax");
       Xmax->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                              "color: rgb(0,255,0);"));
       Xmax->setFont(font);

       Ymin =new QLabel(ZoomWindow);
       Ymin->setObjectName(QString::fromUtf8("label"));
       Ymin->setGeometry(QRect(40,100,50,30));
       Ymin->setText("Ymin");
       Ymin->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                              "color: rgb(0,255,0);"));
       Ymin->setFont(font);

       Ymax =new QLabel(ZoomWindow);
       Ymax->setObjectName(QString::fromUtf8("label"));
       Ymax->setGeometry(QRect(40,130,50,30));
       Ymax->setText("Ymax");
       Ymax->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                              "color: rgb(0,255,0);"));
       Ymax->setFont(font);
     /*if(SelectedPageID==2)
     {
       Alert =new QLabel(ZoomWindow);
       Alert->setObjectName(QString::fromUtf8("alert_label"));
       Alert->setGeometry(QRect(0,290,300,30));
       Alert->setText("Zoom/ResetZoom is not applicable for this page !!");
       Alert->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                                              "color: rgb(0,255,0);"));
       Alert->setFont(font);
       Alert->show();

     }
	else
	{
	Alert->hide();
	}

     */
     ZoomButton =new QPushButton(ZoomWindow);
       ZoomButton->setObjectName(QString::fromUtf8("Zoonbutton"));
       ZoomButton->setGeometry(QRect(70, 230, 100, 30));
       ZoomButton->setText("ZOOM");
       ZoomButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
       ZoomButton->setFont(font);

       ZoomResetButton =new QPushButton(ZoomWindow);
       ZoomResetButton->setObjectName(QString::fromUtf8("ZoomReset"));
       ZoomResetButton->setGeometry(QRect(70, 260, 100, 30));
       ZoomResetButton->setText("RESET ZOOM");
       ZoomResetButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                          "color: rgb(0, 0, 0);"));
       ZoomResetButton->setFont(font);

      /* Xmin_lineedit = new QLineEdit(ZoomWindow);
       Xmin_lineedit->setObjectName(QString::fromUtf8("Xmin_lineEdit"));
       Xmin_lineedit->move(90, 40);
       Xmin_lineedit->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                          "color: rgb(0, 0, 0);"));*/
        Xmin_Combobox=new QComboBox(ZoomWindow);
        Xmin_Combobox->setGeometry(QRect(90,40,120,30));
        Xmin_Combobox->clear();
        Xmin_Combobox->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                       "color: rgb(0, 0, 0);"));
        Xmin_Combobox->addItem("0");
	Xmin_Combobox->addItem("50");
        Xmin_Combobox->addItem("100");
        Xmin_Combobox->addItem("150");
        Xmin_Combobox->addItem("200");
        Xmin_Combobox->addItem("250");
        Xmin_Combobox->addItem("300");
        Xmin_Combobox->addItem("350");
        Xmin_Combobox->addItem("400");
        Xmin_Combobox->addItem("450");
        Xmin_Combobox->addItem("500");
        Xmin_Combobox->addItem("550");
        Xmin_Combobox->addItem("1000");
        Xmin_Combobox->addItem("1050");
        Xmin_Combobox->addItem("2000");
        Xmin_Combobox->addItem("2050");
        Xmin_Combobox->addItem("3000");
        Xmin_Combobox->addItem("3050");

/*
       Xmax_lineedit = new QLineEdit(ZoomWindow);
       Xmax_lineedit->setObjectName(QString::fromUtf8("Xmax_lineEdit"));
       Xmax_lineedit->move(90, 70);
       Xmax_lineedit->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                          "color: rgb(0, 0, 0);"));*/
	Xmax_Combobox=new QComboBox(ZoomWindow);
        Xmax_Combobox->setGeometry(QRect(90,70,120,30));
        Xmax_Combobox->clear();
        Xmax_Combobox->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                       "color: rgb(0, 0, 0);"));
        Xmax_Combobox->addItem("100");
        Xmax_Combobox->addItem("150");
        Xmax_Combobox->addItem("200");
        Xmax_Combobox->addItem("250");
        Xmax_Combobox->addItem("300");
        Xmax_Combobox->addItem("350");
        Xmax_Combobox->addItem("400");
        Xmax_Combobox->addItem("450");
        Xmax_Combobox->addItem("500");
        Xmax_Combobox->addItem("550");
        Xmax_Combobox->addItem("1000");
        Xmax_Combobox->addItem("1050");
        Xmax_Combobox->addItem("2000");
        Xmax_Combobox->addItem("2050");
        Xmax_Combobox->addItem("3000");
        Xmax_Combobox->addItem("3050");
        Xmax_Combobox->addItem("4000");


/*       Ymin_lineedit = new QLineEdit(ZoomWindow);
       Ymin_lineedit->setObjectName(QString::fromUtf8("Ymin_lineEdit"));
       Ymin_lineedit->move(90, 100);
       Ymin_lineedit->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                          "color: rgb(0, 0, 0);"));*/
	Ymin_Combobox=new QComboBox(ZoomWindow);
        Ymin_Combobox->setGeometry(QRect(90,100,120,30));
        Ymin_Combobox->clear();
        Ymin_Combobox->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                       "color: rgb(0, 0, 0);"));
        Ymin_Combobox->addItem("0");
        Ymin_Combobox->addItem("50");
        Ymin_Combobox->addItem("100");
        Ymin_Combobox->addItem("150");
	Ymin_Combobox->addItem("200");
        Ymin_Combobox->addItem("250");
	Ymin_Combobox->addItem("300");
        Ymin_Combobox->addItem("350");
        Ymin_Combobox->addItem("450");
        Ymin_Combobox->addItem("500");
        Ymin_Combobox->addItem("550");
        



       /*Ymax_lineedit = new QLineEdit(ZoomWindow);
       Ymax_lineedit->setObjectName(QString::fromUtf8("Ymin_lineEdit"));
       Ymax_lineedit->move(90, 130);
       Ymax_lineedit->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                          "color: rgb(0, 0, 0);"));*/
        Ymax_Combobox=new QComboBox(ZoomWindow);
        Ymax_Combobox->setGeometry(QRect(90,130,120,30));
        Ymax_Combobox->clear();
        Ymax_Combobox->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                       "color: rgb(0, 0, 0);"));
        Ymax_Combobox->addItem("50");
        Ymax_Combobox->addItem("100");
        Ymax_Combobox->addItem("150");
        Ymax_Combobox->addItem("200");
	Ymax_Combobox->addItem("250");
        Ymax_Combobox->addItem("300");
	Ymax_Combobox->addItem("350");
        Ymax_Combobox->addItem("400");
        Ymax_Combobox->addItem("450");
        Ymax_Combobox->addItem("500");
        Ymax_Combobox->addItem("550");
        Ymax_Combobox->addItem("600");


       connect(HydrophoneSel,SIGNAL(activated(int)),this,SLOT(HydrophoneSelect(int)));
       connect(AccelerometerSel,SIGNAL(activated(int)),this,SLOT(AccelerometerSelect(int)));
     //connect(GroupSelection,SIGNAL(activated(int)),this,SLOT(SensorGroupSelection(int)));
       connect(ZoomLofar,SIGNAL(activated(int)),this,SLOT(ZoomLofarCursorSet(int)));
       connect(ZoomSpectrum,SIGNAL(activated(int)),this,SLOT(ZoomSpectrumCursorSet(int)));
       connect(TimeAdjust,SIGNAL(activated(int)),this,SLOT(ReplayTime(int)));
     //connect(Scaleadjust,SIGNAL(activated(int)),this,SLOT(SclaeAdujustment(int)));
       connect(Tc,SIGNAL(activated(int)),this,SLOT(SetTimeConstant(int)));
       connect(Xmax_Combobox,SIGNAL(activated(int)),this,SLOT(Xmax_dialogmsg()));
       connect(Ymax_Combobox,SIGNAL(activated(int)),this,SLOT(Ymax_dialogmsg()));
       connect(ZoomButton,SIGNAL(clicked()),this,SLOT(ZoomSet()));//ZoomWindow_ZOOM button
       connect(ZoomResetButton,SIGNAL(clicked()),this,SLOT(ResetSet()));//ZoomWindow_RESET button
}

void DisplayWindow::SelectSpectrumComp(){

    //graphPlot->ClearSpecCompDisplay();
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setStyleSheet("background-color:white");
    QStringList strFileNames = dialog.getOpenFileNames(this,tr("Select up to 3  Replay Files to Compare"),SpectrumReplayPathSet, tr("All Files (*.*)"));
    Selectedfileno=strFileNames.size();
    if (Selectedfileno!=0){
	    Replay_Button_Spec_comp->hide();
	    StopReplay_Button_Spec_comp->show();
    }
    QRegExp rx("[^/]*.txt");

    printf("Selectedfileno : %d\n",Selectedfileno);

    switch(Selectedfileno){
       case 1:
	  CompFlagInd[0]=1;
	  CompFlagInd[1]=0;
	  CompFlagInd[2]=0;
          memcpy( tempNam1, strFileNames[0].toStdString().c_str() ,strFileNames[0].size());
          ReplaySpectrumCompFlag=1;

	  GreenLegend->show();
          rx.indexIn(strFileNames[0]);
	  GreenLegendtext->setText(rx.cap(0));
	  GreenLegendtext->show();
	  RedLegend->hide();
	  BlueLegend->hide();
          RedLegendtext->hide();
          BlueLegendtext->hide();

	  break;
       case 2:
	  CompFlagInd[0]=1;
	  CompFlagInd[1]=1;
	  CompFlagInd[2]=0;
          memcpy( tempNam1, strFileNames[0].toStdString().c_str() ,strFileNames[0].size());
          memcpy( tempNam2, strFileNames[1].toStdString().c_str() ,strFileNames[1].size());
          ReplaySpectrumCompFlag=1;

	  GreenLegend->show();
	  rx.indexIn(strFileNames[0]);
	  GreenLegendtext->setText(rx.cap(0));
          GreenLegendtext->show();

	  RedLegend->show();
	  rx.indexIn(strFileNames[1]);
	  RedLegendtext->setText(rx.cap(0));
          RedLegendtext->show();

	  BlueLegendtext->hide();
          BlueLegend->hide();
	  break;
       case 3:
	  CompFlagInd[0]=1;
	  CompFlagInd[1]=1;
	  CompFlagInd[2]=1;
          memcpy( tempNam1, strFileNames[0].toStdString().c_str() ,strFileNames[0].size());
          memcpy( tempNam2, strFileNames[1].toStdString().c_str() ,strFileNames[1].size());
          memcpy( tempNam3, strFileNames[2].toStdString().c_str() ,strFileNames[2].size());
          ReplaySpectrumCompFlag=1;

	  GreenLegend->show();
	  rx.indexIn(strFileNames[0]);
	  GreenLegendtext->setText(rx.cap(0));
          GreenLegendtext->show();

          RedLegend->show();
	  rx.indexIn(strFileNames[1]);
	  RedLegendtext->setText(rx.cap(0));
          RedLegendtext->show();

	  BlueLegend->show();
	  rx.indexIn(strFileNames[2]);
	  BlueLegendtext->setText(rx.cap(0));
          BlueLegendtext->show();
	  break;
    }

}

void DisplayWindow::StopReplaySpecComp(){
    Replay_Button_Spec_comp->show();
    StopReplay_Button_Spec_comp->hide();
    ReplaySpectrumCompFlag=0;
    CompFlagInd[0]=0;
    CompFlagInd[1]=0;
    CompFlagInd[2]=0;

}

void DisplayWindow::Zoom_XmaxAlertOK(){

	XmaxAlert->accept();

}

void DisplayWindow::Zoom_YmaxAlertOK(){

	YmaxAlert->accept();

}


void DisplayWindow::Xmax_dialogmsg(){


     QString Xmin,Xmax,Ymin,Ymax ;
     unsigned int xmin,xmax,ymin,ymax;
     Xmin =Xmin_Combobox->currentText();
     Xmax =Xmax_Combobox->currentText();
     Ymin =Ymin_Combobox->currentText();
     Ymax =Ymax_Combobox->currentText();

     xmin=Xmin.toDouble();
     xmax=Xmax.toDouble();
     ymin=Ymin.toDouble();
     ymax=Ymax.toDouble();

     if(xmax<=xmin)
   {

       XmaxAlert =new QDialog(ZoomWindow);
       XmaxAlert->setWindowTitle(QString::fromUtf8("Alert"));
       XmaxAlert->setGeometry(QRect(150,250,300,80));
       XmaxAlert->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                               		       "color: rgb(0,0,0);"));

       QFont font;
       font.setFamily(QString::fromUtf8("SansSerif"));
       font.setPointSize(11);
       font.setBold(false);
       Xmaxalert = new QLabel(XmaxAlert);
       Xmaxalert->setGeometry(QRect(20,1,300,70));
       Xmaxalert->setObjectName(QString::fromUtf8("Alert_Xmax"));
       Xmaxalert->setFont(font);
       Xmaxalert->setText("Xmax should be greater than Xmin ");

       ZoomXAlertOK= new QPushButton(XmaxAlert);
       ZoomXAlertOK->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,255;\n"
                                                               "color: rgb(0,0,0);"));
       ZoomXAlertOK->setText("OK");
       ZoomXAlertOK->setGeometry(QRect(90,50,80,28));

       XmaxAlert->show();
   }

}


void DisplayWindow::Ymax_dialogmsg(){


     QString Xmin,Xmax,Ymin,Ymax ;
     unsigned int xmin,xmax,ymin,ymax;
     Xmin =Xmin_Combobox->currentText();
     Xmax =Xmax_Combobox->currentText();
     Ymin =Ymin_Combobox->currentText();
     Ymax =Ymax_Combobox->currentText();

     xmin=Xmin.toDouble();
     xmax=Xmax.toDouble();
     ymin=Ymin.toDouble();
     ymax=Ymax.toDouble();

   if(ymax<=ymin)
   {
       YmaxAlert =new QDialog(ZoomWindow);
       YmaxAlert->setWindowTitle(QString::fromUtf8("Alert"));
       YmaxAlert->setGeometry(QRect(150,200,300,80));
       YmaxAlert->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);\n"
                                                               "color: rgb(0,0,0);"));
       QFont font;
       font.setFamily(QString::fromUtf8("SansSerif"));
       font.setPointSize(11);
       font.setBold(false);
       Ymaxalert = new QLabel(YmaxAlert);
       Ymaxalert->setGeometry(QRect(20,1,300,70));
       Ymaxalert->setObjectName(QString::fromUtf8("Alert_Ymax"));
       Ymaxalert->setFont(font);
       Ymaxalert->setText("Ymax should be greater than Ymin ");

       ZoomYAlertOK= new QPushButton(YmaxAlert);
       ZoomYAlertOK->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,255;\n"
                                                               "color: rgb(0,0,0);"));
       ZoomYAlertOK->setText("OK");
       ZoomYAlertOK->setGeometry(QRect(90,50,80,28));

       YmaxAlert->show();
   }

}



void DisplayWindow::ZoomSet()
{
     QString Xmin,Xmax,Ymin,Ymax ;
     unsigned int xmin,xmax,ymin,ymax;
     Xmin =Xmin_Combobox->currentText();
     Xmax =Xmax_Combobox->currentText();
     Ymin =Ymin_Combobox->currentText();
     Ymax =Ymax_Combobox->currentText();

     xmin=Xmin.toDouble();
     xmax=Xmax.toDouble();
     ymin=Ymin.toDouble();
     ymax=Ymax.toDouble();

     printf("\n xmin=%d,xmax=%d,ymin=%d,ymax=%d \n",xmin,xmax,ymin,ymax);
     printf("\n Selected=%d \n",SelectedPageID);
   if(SelectedPageID==0)
     {

	    graphPlot->LofarGraphLegend->xAxis->setRange(xmin,xmax);
            graphPlot->LofarGraphLegend->yAxis->setRange(ymin,ymax);
     }
   else if(SelectedPageID==1)
     {

            graphPlot->SpectrumGraphLegend->xAxis->setRange(xmin,xmax);
            graphPlot->SpectrumGraphLegend->yAxis->setRange(ymin,ymax);
     }
   else if(SelectedPageID==6)
     {

            graphPlot->RawDataDelSpectGraphLegend->xAxis->setRange(xmin,xmax);
            graphPlot->RawDataDelSpectGraphLegend->yAxis->setRange(ymin,ymax);
     }
   else if(SelectedPageID==7)
     {

            graphPlot->SpectrumComparisonGraphLegend->xAxis->setRange(xmin,xmax);
            graphPlot->SpectrumComparisonGraphLegend->yAxis->setRange(ymin,ymax);
	    graphPlot->SpectrumComparisonGraphLegend->replot();
     }


}

void DisplayWindow::ResetSet()
{

	if(SelectedPageID==0)
     {
            graphPlot->LofarGraphLegend->xAxis->setRange(20,4000);
            graphPlot->LofarGraphLegend->yAxis->setRange(0,600);

     }

     else if (SelectedPageID==1)
     {
            graphPlot->SpectrumGraphLegend->xAxis->setRange(20,4000);
            graphPlot->SpectrumGraphLegend->yAxis->setRange(0,600);

     }

     else if (SelectedPageID==6)
     {
            graphPlot->RawDataDelSpectGraphLegend->xAxis->setRange(20,4000);
            graphPlot->RawDataDelSpectGraphLegend->yAxis->setRange(0,600);

     }
     else if (SelectedPageID==7)
     {
            graphPlot->SpectrumComparisonGraphLegend->xAxis->setRange(20,4000);
            graphPlot->SpectrumComparisonGraphLegend->yAxis->setRange(0,600);

     }





}




void DisplayWindow::DrawLOFARFrame()
{
    LofarFrame = new QFrame(DisplayProcesingFrame);
    LofarFrame->setMouseTracking(true);
    LofarFrame->setGeometry(QRect(0,0,790,500));
    LofarFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0, 0);"));
    LofarFrame->setFrameShape(QFrame::StyledPanel);
    LofarFrame->setFrameShadow(QFrame::Raised);
    DrawMainHLine();
    DrawMainVLine();
    DrawZoomMainHLine();
    DrawZoomMainVLine();
    LofarFrame->hide();
 //printf("\n Spectrum Page is selected---");
}

   void DisplayWindow::DrawSPECTRUMCOMPARISONFrame()
{
    Spec_Comp_Frame = new QFrame(DisplayProcesingFrame);
    Spec_Comp_Frame->setMouseTracking(true);
    Spec_Comp_Frame->setGeometry(QRect(0,0,790,500));
    Spec_Comp_Frame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0, 0);"));
    Spec_Comp_Frame->setFrameShape(QFrame::StyledPanel);
    Spec_Comp_Frame->setFrameShadow(QFrame::Raised);
    Spec_Comp_Frame->hide();

    Replay_Button_Spec_comp = new QPushButton(Spec_Comp_Frame);
    Replay_Button_Spec_comp->setObjectName(QString::fromUtf8("Replay_Button_Spec_comp"));
    Replay_Button_Spec_comp->setStyleSheet(QString::fromUtf8("background-color:white;\n" "color: black;"
     "border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;"));
    Replay_Button_Spec_comp->setText("REPLAY");
    Replay_Button_Spec_comp->setGeometry(350,450,120,30);
    Replay_Button_Spec_comp->show();


    StopReplay_Button_Spec_comp = new QPushButton(Spec_Comp_Frame);
    StopReplay_Button_Spec_comp->setObjectName(QString::fromUtf8("StopReplay_Button_Spec_comp"));
    StopReplay_Button_Spec_comp->setStyleSheet(QString::fromUtf8("background-color:white;\n" "color: black;"
     "border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;"));
    StopReplay_Button_Spec_comp->setText("STOP REPLAY");
    StopReplay_Button_Spec_comp->setGeometry(350,450,120,30);
    StopReplay_Button_Spec_comp->hide();

    GreenLegend = new QLabel(Spec_Comp_Frame);
    GreenLegend->setObjectName(QString::fromUtf8("greenlegendlabel"));
    GreenLegend->setStyleSheet(QString::fromUtf8("background-color:green;\n" "color: black;"));
    GreenLegend->setGeometry(100,8,50,30);
    GreenLegend->hide();
    
    RedLegend = new QLabel(Spec_Comp_Frame);
    RedLegend->setObjectName(QString::fromUtf8("redlegendlabel"));
    RedLegend->setStyleSheet(QString::fromUtf8("background-color:red;\n" "color: black;"));
    RedLegend->setGeometry(300,8,50,30);
    RedLegend->hide();

    BlueLegend = new QLabel(Spec_Comp_Frame);
    BlueLegend->setObjectName(QString::fromUtf8("bluelegendlabel"));
    BlueLegend->setStyleSheet(QString::fromUtf8("background-color:blue;\n" "color: black;"));
    BlueLegend->setGeometry(500,8,50,30);
    BlueLegend->hide();

    GreenLegendtext = new QLabel(Spec_Comp_Frame);
    GreenLegendtext->setStyleSheet("background-color: black; color: white;");
 //  GreenLegendtext->setText("sample");
    GreenLegendtext->setGeometry(155,0,150,33);
    GreenLegendtext->hide();

    RedLegendtext = new QLabel(Spec_Comp_Frame);
    RedLegendtext->setStyleSheet("background-color: black; color: white;");
   // RedLegendtext->setText("sample");
    RedLegendtext->setGeometry(355,0,150,33);
    RedLegendtext->hide();

    BlueLegendtext = new QLabel(Spec_Comp_Frame);
    BlueLegendtext->setStyleSheet("background-color: black; color: white;");
   // BlueLegendtext->setText("sample");
    BlueLegendtext->setGeometry(555,0,220,33);
    BlueLegendtext->hide();

    
    
    printf("\n Spectrum Comparison Page is selected---");
}

void DisplayWindow::DrawRawDataFrame()
{
    RawDataFrame = new QFrame(DisplayProcesingFrame);
    RawDataFrame->setMouseTracking(true);
    RawDataFrame->setGeometry(QRect(0,0,790,500));
    RawDataFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0, 0);"));
    RawDataFrame->setFrameShape(QFrame::StyledPanel);
    RawDataFrame->setFrameShadow(QFrame::Raised);
    RawDataFrame->hide();

    DelTimeAvgSpec = new QLabel(RawDataFrame);
    DelTimeAvgSpec->setObjectName(QString::fromUtf8("delavglabel"));
    DelTimeAvgSpec->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0);\n"
        "color: white;""font: bold 13px;"));
    DelTimeAvgSpec->setText("Delayed Time Average Spectrum");
    DelTimeAvgSpec->hide();

    ExportCSV_Button_1 = new QPushButton(RawDataFrame);
    ExportCSV_Button_1->setObjectName(QString::fromUtf8("export2"));
    ExportCSV_Button_1->setStyleSheet(QString::fromUtf8("background-color:white;\n" "color: black;"
"border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;"));
    ExportCSV_Button_1->setText("Export to CSV");
    ExportCSV_Button_1->hide();

    //connect

    HLayout1 = new QHBoxLayout();
    HLayout1->setMargin(0);
    HLayout1->addWidget(DelTimeAvgSpec);
    HLayout1->addWidget(ExportCSV_Button_1);
    HLayout1->setGeometry(QRect(70,2,480,20));
    setLayout(HLayout1);

    RawSignal = new QLabel(RawDataFrame);
    RawSignal->setObjectName(QString::fromUtf8("rawsignallabel"));
    RawSignal->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0);"
        "color: white;""font: bold 13px;"));
    RawSignal->setText("Raw Signal  \t");
    RawSignal->hide();

    ExportCSV_Button_2 = new QPushButton(RawDataFrame);
    ExportCSV_Button_2->setObjectName(QString::fromUtf8("export2"));
    ExportCSV_Button_2->setStyleSheet(QString::fromUtf8("background-color:white;\n" "color: black;"
"border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;"));
    ExportCSV_Button_2->setText("Export to CSV");
    ExportCSV_Button_2->hide();

          HLayout2 = new QHBoxLayout();
          HLayout2->addWidget(RawSignal);
          HLayout2->addWidget(ExportCSV_Button_2);
          HLayout2->setGeometry(QRect(70,124,335,127));
         setLayout(HLayout2);

         Export_Document = new QLabel(RawDataFrame);
         Export_Document->setObjectName(QString::fromUtf8("expdoclabel"));
         Export_Document->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0);"
             "color: white;""font: bold 13px;"));
         Export_Document->setText("Export Document:  " " "  "Select Format""\t ");
         Export_Document->show();

         Format_Select = new QComboBox(RawDataFrame);
         Format_Select->clear();
         Format_Select->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                        "color: rgb(0, 0, 0);"  "border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;" ));
         Format_Select->addItem("ODT");
         Format_Select->addItem("PDF");
         Format_Select->show();

         Export_Button = new QPushButton(RawDataFrame);
         Export_Button->setObjectName(QString::fromUtf8("export2"));
         Export_Button->setStyleSheet(QString::fromUtf8("background-color:white;\n" "color: black;"
     "border-style: outset;" "border-width: 2px;" "border-radius: 10px;"" font: bold 14px;""min-width: 10em;"));
         Export_Button->setText("Export");
         Export_Button->show();

         HLayout4 = new QHBoxLayout();
         HLayout4->addWidget(Export_Document);
         HLayout4->addWidget(Format_Select);
         HLayout4->addWidget(Export_Button);
         HLayout4->setGeometry(QRect(65,318,600,323));
         setLayout(HLayout4);

         RawTextData = new QTextEdit(RawDataFrame);
         RawTextData->setStyleSheet("QTextEdit {background-color: white; color: black;}");
         RawTextData->textCursor().insertText("sample");
         RawTextData->setMaximumHeight(110);
         RawTextData->setGeometry(60,350,703,460);
         RawTextData->show();

         QScrollBar *v = RawTextData->verticalScrollBar();
         v->setValue(v->maximum());
         v->show();

               HLayout3 = new QHBoxLayout();
               HLayout3->addWidget(RawTextData);
               setLayout(HLayout3);

printf("---\n RAW DATA Page is selected \n---");
}



void DisplayWindow::on_Formatcombobox_currentIndexChanged()
{
    QMessageBox *msgBox = new QMessageBox(this);
    QString value;
    value =  Format_Select->currentText();
    msgBox->setText("Now in "+value);
    msgBox->show();
}

void DisplayWindow::Export_to_Format_Button()
{
    QTextEdit *editor = new QTextEdit();
    QTextDocument *document = new QTextDocument();
    QTextCursor cursor(document);
    format_combo_value = Format_Select->currentText();
    qDebug() << "Selected Format is :"<<format_combo_value;

    if(format_combo_value == "ODT")
    {
       qDebug() << format_combo_value;
       graphPlot->RawDataDelSpectGraphLegend->saveJpg(QString("a.jpg"),600,300);
      // graphPlot->RawDataGraphLegend->saveJpg(QString("b.jpg"),graphPlot->RawDataGraphLegend->width(),graphPlot->RawDataGraphLegend->height());
       QString text = RawTextData->toPlainText();
       editor->setDocument(document);
       editor->append("<img src=\"a.jpg\"/>" "    ");
       //editor->append("<img src=\"b.jpg\"/>" "    ");
       editor->append(text);
       editor->setWindowTitle(QObject::tr("Export to docx"));
       editor->resize(1000,750);
       editor->show();
       QString fileName = QFileDialog::getSaveFileName(nullptr,QObject::tr("Save File"),"output_file.odt",QObject::tr("Open Document ('''.odt)"));
       QTextDocumentWriter fileWriter (fileName);
       fileWriter.setFormat("odt");
       fileWriter.write(document);
    }

    else if(format_combo_value == "PDF"){
       /*QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
       if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
          QPrinter printer(QPrinter::PrinterResolution);
          printer.setOutputFormat(QPrinter::PdfFormat);
          printer.setPaperSize(QPrinter::A4);
             printer.setOutputFileName(fileName);
        graphPlot->RawDataDelSpectGraphLegend->saveJpg(QString("a.jpg"),graphPlot->RawDataDelSpectGraphLegend->width(),graphPlot->RawDataDelSpectGraphLegend->height());
        graphPlot->RawDataGraphLegend->saveJpg(QString("b.jpg"),graphPlot->RawDataGraphLegend->width(),graphPlot->RawDataGraphLegend->height());

        QImage *img1 = new QImage("a.jpg");
        QImage *img2 = new QImage("b.jpg");

         double image_1_SizeWidth = static_cast<double>(img1->width());
         double image_1_SizeHeight = static_cast<double>(img1->height());
         QRectF source_1(0.0, 0.0, image_1_SizeWidth, image_1_SizeHeight);

         double image_2_SizeWidth = static_cast<double>(img2->width());
         double image_2_SizeHeight = static_cast<double>(img2->height());
         QRectF source_2(0.0, 0.0, image_2_SizeWidth, image_2_SizeHeight);


          QTextDocument doc;
          doc.setPageSize(printer.pageRect().size());

          QString text = RawTextData->toPlainText();
          qDebug() << "Inside pdf data  is :"<<text;

           QPainter painter(&printer);
           QRect rect = painter.viewport();
           QSize size_1 = img1->size();
           QSize size_2= img2->size();
        //size.scale(rect.size(), QSize::ScaleMin);
           painter.setViewport(rect.x(), rect.y(),

            size_1.width(), size_1.height());
            painter.setWindow(img1->rect());

            size_2.width(), size_2.height();
            painter.setWindow(img2->rect());
          //painter.drawImage(QRect(100, 50, 100, 100), QImage("/home/sancy/Desktop/ONMASYSTEM/a.jpg"));

            painter.drawText(10,33,"Delayed Time Average Spectrum");
            painter.drawImage(QRect(10, 50, 500, 110),QImage("a.jpg"));
            painter.drawText(10,190,"Raw Signal");   //---Original code---//
            painter.drawImage(QRect(10, 230, 500, 235),QImage("b.jpg")); //---Original code---//
            painter.drawText(10,350,text); //---Original code---//
            doc.print(&printer);*/

	QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
       if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

       QPrinter printer(QPrinter::PrinterResolution);
       printer.setOutputFormat(QPrinter::PdfFormat);
       printer.setPaperSize(QPrinter::A4);
       printer.setOutputFileName(fileName);

       QFont headerFont("Times New Roman", 14);
       QTextCharFormat txtformat = QTextCharFormat();

       QTextDocument doc;
       doc.setPageSize(printer.pageRect().size());

       QTextCursor* cursor = new QTextCursor(&doc);
       cursor->insertText("Delayed Time Average Spectrum:", txtformat);
       graphPlot->RawDataDelSpectGraphLegend->saveJpg("temp_pdf_img.jpg",600,300);
       QImage pictureImage("temp_pdf_img.jpg");
       QString pictureUrl = QString("./");
       doc.addResource(QTextDocument::ImageResource, QUrl(pictureUrl), QVariant(pictureImage));

      // insert the picture in the document
      cursor->insertBlock();
      QTextImageFormat pictureFormat;
      pictureFormat.setName(pictureUrl);
      pictureFormat.setWidth(pictureImage.width()); // 150 pixelfor picture.png
      cursor->insertImage(pictureFormat);
     // insert the text in the document
      QString text = RawTextData->toPlainText();
      cursor->insertText(text, txtformat);
      doc.print(&printer);

    }
}

void DisplayWindow::ExportDelayedSpectrum_to_CSV()
{
   QString filename = QFileDialog::getSaveFileName(this, "DialogTitle", "filename.csv", "CSV files (.csv);;Zip files (.zip, *.7z)", 0, 0); // getting the filename (full path)
   //QFile data(filename);
   QFile file(filename);
   if (file.open(QFile::WriteOnly|QFile::Truncate))
   {
   QTextStream stream(&file);
   for(int m=0; m<1200; m++){
      stream <<graphPlot->xDelFreq_val[m]<< ","<< graphPlot->yDelAmp_val[m]<< "\n";
   }
   file.close();
   }

}

void DisplayWindow::ExportRawData_to_CSV()
{
    float g = graphPlot->DelRawData_X_val;
    float h = graphPlot->DelRawData_Y_val;
    QString filename = QFileDialog::getSaveFileName(this, "DialogTitle", "filename.csv", "CSV files (.csv);;Zip files (.zip, *.7z)", 0, 0); // getting the filename (full path)
  //QFile data(filename);
    QFile file(filename);
    if (file.open(QFile::WriteOnly|QFile::Truncate))
    {
    QTextStream stream(&file);
    stream <<g<< ","<< h;
 //stream <<value1<< "\t"<< value2 <<"\t"<<value3;
    file.close();
    }

}

void DisplayWindow::ReplayTime(int ID)
{
    switch(ID)
    {
    case 0:  DelayMaking = 20000;
             break;
    case 1:  DelayMaking = 40000;
              break;
    case 2:  DelayMaking = 60000;
             break;
    case 3: DelayMaking  = 100000;
              break;
    case 4: DelayMaking  = 120000;
             break;
    default: break;
    };
}


void DisplayWindow::SclaeAdujustment(int ID)
{
    switch(ID)
    {
    case 0:  yScale=2048;
             yScaleValue=0;
             graphPlot->LofarGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->LofarZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->Lofar_yScale_TickLabel1->setText("150");
             graphPlot->Lofar_yScale_TickLabel2->setText("125");
             graphPlot->Lofar_yScale_TickLabel3->setText("96");
             graphPlot->zoomLofar_yScale_TickLabel1->setText("150");
             graphPlot->zoomLofar_yScale_TickLabel2->setText("125");
             graphPlot->zoomLofar_yScale_TickLabel3->setText("96");
             graphPlot->zoomLofar_yScale_TickLabel2->hide();
             graphPlot->Lofar_yScale_TickLabel2->hide();
             //for(iCount=0;iCount<4;iCount)
             {
             //graphPlot->LofarINT[iCount]->yAxis->setRange(0,yScale);
             //graphPlot->SpectINT[iCount]->yAxis->setRange(0,yScale);
             }
             break;

    case 1:  yScale=2048;
             yScaleValue=6;
             graphPlot->LofarGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->LofarZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->Lofar_yScale_TickLabel1->setText("150");
             graphPlot->Lofar_yScale_TickLabel2->setText("125");
             graphPlot->Lofar_yScale_TickLabel3->setText("96");
             graphPlot->zoomLofar_yScale_TickLabel1->setText("150");
             graphPlot->zoomLofar_yScale_TickLabel2->setText("125");
             graphPlot->zoomLofar_yScale_TickLabel3->setText("96");
             graphPlot->zoomLofar_yScale_TickLabel2->hide();
             graphPlot->Lofar_yScale_TickLabel2->hide();
             //for(iCount=0;iCount<4;iCount)
             {
             //graphPlot->LofarINT[iCount]->yAxis->setRange(0,yScale);
             //graphPlot->SpectINT[iCount]->yAxis->setRange(0,yScale);
             }
              break;

    case 2:  yScale=2048;
             yScaleValue=12;

             graphPlot->LofarGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->LofarZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->Lofar_yScale_TickLabel1->setText("150");
             graphPlot->Lofar_yScale_TickLabel2->setText("125");
             graphPlot->Lofar_yScale_TickLabel3->setText("96");
             graphPlot->zoomLofar_yScale_TickLabel1->setText("150");
             graphPlot->zoomLofar_yScale_TickLabel2->setText("125");
             graphPlot->zoomLofar_yScale_TickLabel3->setText("96");
             graphPlot->zoomLofar_yScale_TickLabel2->hide();
             graphPlot->Lofar_yScale_TickLabel2->hide();
             //for(iCount=0;iCount<4;iCount)
             {
               //graphPlot->LofarINT[iCount]->yAxis->setRange(0,yScale);
               //graphPlot->SpectINT[iCount]->yAxis->setRange(0,yScale);
             }
             break;

    case 3: yScale=128;
           yScaleValue=20;

             graphPlot->LofarGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->LofarZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->Lofar_yScale_TickLabel1->setText("100");
             graphPlot->Lofar_yScale_TickLabel2->setText("50");
             graphPlot->Lofar_yScale_TickLabel3->setText("0");
             graphPlot->zoomLofar_yScale_TickLabel1->setText("100");
             graphPlot->zoomLofar_yScale_TickLabel2->setText("50");
             graphPlot->zoomLofar_yScale_TickLabel3->setText("0");
             graphPlot->zoomLofar_yScale_TickLabel2->hide();
             graphPlot->Lofar_yScale_TickLabel2->hide();

             //for(iCount=0;iCount<4;iCount)
             //{
             //graphPlot->LofarINT[iCount]->yAxis->setRange(0,yScale);
             //graphPlot->SpectINT[iCount]->yAxis->setRange(0,yScale);
             //}
              break;

    case 4: yScale=60;
             graphPlot->LofarGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->LofarZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->SpectrumZoomGraphLegend->yAxis->setRange(0,yScale);
             graphPlot->Lofar_yScale_TickLabel1->setText("60");
             graphPlot->Lofar_yScale_TickLabel2->setText("30");
             graphPlot->Lofar_yScale_TickLabel3->setText("0");
             graphPlot->zoomLofar_yScale_TickLabel2->hide();
             graphPlot->Lofar_yScale_TickLabel2->hide();
             //for(iCount=0;iCount<4;iCount)
             {
             //graphPlot->LofarINT[iCount]->yAxis->setRange(0,yScale);
             //graphPlot->SpectINT[iCount]->yAxis->setRange(0,yScale);
             }
             break;
    default: //yScale=250;
             break;
    };
}



void DisplayWindow::DrawMainHLine()
{
    yCount=0;
    MainDisplayHFrame=new QFrame(graphPlot->LofarGraphLegend);
    MainDisplayHFrame->setMouseTracking(true);
    MainDisplayHFrame->setGeometry(QRect(30,50,705,1));
    MainDisplayHFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainDisplayHFrame->setFrameShadow(QFrame::Raised);
    MainDisplayHFrame->show();

    for(iCount=0;iCount<95;iCount++)
    {
        MaskHFrame[iCount]=new QFrame(MainDisplayHFrame);
        MaskHFrame[iCount]->setGeometry(QRect(10+yCount,0,8,1));
        MaskHFrame[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));
      //MaskHFrame[iCount]->setFrameShape(QFrame::VLine);
        MaskHFrame[iCount]->setFrameShadow(QFrame::Raised);
        MaskHFrame[iCount]->show();
        yCount=yCount+10;
    }
    MainDisplayHFrame->move(30,60);
}

void DisplayWindow::DrawMainVLine()
{
    yCount=0;
    MainDisplayVFrame=new QFrame(graphPlot->LofarGraphLegend);
    MainDisplayVFrame->setMouseTracking(true);
    MainDisplayVFrame->setGeometry(QRect(600,15,1,110));
    MainDisplayVFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainDisplayVFrame->setFrameShadow(QFrame::Raised);
    MainDisplayVFrame->show();
    for(iCount=0;iCount<26;iCount++)
    {
        MaskVFrame[iCount]=new QFrame(MainDisplayVFrame);
        MaskVFrame[iCount]->setGeometry(QRect(0,4+yCount,1,6));
        MaskVFrame[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));
    //  MaskHFrame[iCount]->setFrameShape(QFrame::VLine);
        MaskVFrame[iCount]->setFrameShadow(QFrame::Raised);
        MaskVFrame[iCount]->show();
        yCount=yCount+8;
    }

}
void DisplayWindow::DrawZoomMainHLine()
{
    yCount=0;
    ZoomDisplayHFrame=new QFrame(graphPlot->LofarZoomGraphLegend);
    ZoomDisplayHFrame->setMouseTracking(true);
    ZoomDisplayHFrame->setGeometry(QRect(30,50,705,1));
    ZoomDisplayHFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    ZoomDisplayHFrame->setFrameShadow(QFrame::Raised);
    ZoomDisplayHFrame->show();
    for(iCount=0;iCount<95;iCount++)
    {
        MaskZoomHFrame[iCount]=new QFrame(ZoomDisplayHFrame);
        MaskZoomHFrame[iCount]->setGeometry(QRect(10+yCount,0,8,1));
        MaskZoomHFrame[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));
    //  MaskHFrame[iCount]->setFrameShape(QFrame::VLine);
        MaskZoomHFrame[iCount]->setFrameShadow(QFrame::Raised);
        MaskZoomHFrame[iCount]->show();
        yCount=yCount+10;
    }
 ZoomDisplayHFrame->move(30,60);
}

void DisplayWindow::DrawZoomMainVLine()
{
    yCount=0;
    ZoomDisplayVFrame=new QFrame(graphPlot->LofarZoomGraphLegend);
    ZoomDisplayVFrame->setMouseTracking(true);
    ZoomDisplayVFrame->setGeometry(QRect(600,15,1,110));
    ZoomDisplayVFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    ZoomDisplayVFrame->setFrameShadow(QFrame::Raised);
    ZoomDisplayVFrame->show();
    for(iCount=0;iCount<26;iCount++)
    {
        MaskZoomVFrame[iCount]=new QFrame(ZoomDisplayVFrame);
        MaskZoomVFrame[iCount]->setGeometry(QRect(0,4+yCount,1,6));
        MaskZoomVFrame[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255);"));
    //MaskHFrame[iCount]->setFrameShape(QFrame::VLine);
        MaskZoomVFrame[iCount]->setFrameShadow(QFrame::Raised);
        MaskZoomVFrame[iCount]->show();
        yCount=yCount+8;
    }
}


void DisplayWindow::DrawZoomCursor()
{

    ZoomCursourHLine=new QFrame(graphPlot->LofarGraphLegend);
    ZoomCursourHLine->setMouseTracking(true);
    ZoomCursourHLine->setGeometry(QRect(40,120,100,1));
    ZoomCursourHLine->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,0);"));
    ZoomCursourHLine->setFrameShadow(QFrame::Raised);
    ZoomCursourHLine->show();
    ZoomCursorVLine1=new QFrame(graphPlot->LofarGraphLegend);
    ZoomCursorVLine1->setMouseTracking(true);
    ZoomCursorVLine1->setGeometry(QRect(40,115,1,5));
    ZoomCursorVLine1->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,0);"));
    ZoomCursorVLine1->setFrameShadow(QFrame::Raised);
    ZoomCursorVLine1->show();
    ZoomCursorVLine2=new QFrame(graphPlot->LofarGraphLegend);
    ZoomCursorVLine2->setMouseTracking(true);
    ZoomCursorVLine2->setGeometry(QRect(88,115,1,5));
    ZoomCursorVLine2->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,0);"));
    ZoomCursorVLine2->setFrameShadow(QFrame::Raised);
    ZoomCursorVLine2->show();
    ZoomCursorVLine3=new QFrame(graphPlot->LofarGraphLegend);
    ZoomCursorVLine3->setMouseTracking(true);
    ZoomCursorVLine3->setGeometry(QRect(140,115,1,5));
    ZoomCursorVLine3->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,0);"));
    ZoomCursorVLine3->setFrameShadow(QFrame::Raised);
    ZoomCursorVLine3->show();

}


void DisplayWindow::DrawSpectrumFrame()
{
    SpectrumFrame=new QFrame(DisplayProcesingFrame);
    SpectrumFrame->setMouseTracking(true);
    SpectrumFrame->setGeometry(QRect(0,0,790,500));
    SpectrumFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    SpectrumFrame->setFrameShape(QFrame::StyledPanel);
    SpectrumFrame->setFrameShadow(QFrame::Raised);
    SpectrumFrame->show();
  //printf("\n Spectrum Page is selected---");
}


void DisplayWindow::DrawINTLofarFrame()
{
    IntLofarFrame=new QFrame(DisplayProcesingFrame);
    IntLofarFrame->setMouseTracking(true);
    IntLofarFrame->setGeometry(QRect(0,0,790,500));
    IntLofarFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0, 0);"));
    IntLofarFrame->setFrameShape(QFrame::StyledPanel);
    IntLofarFrame->setFrameShadow(QFrame::Raised);
    IntLofarFrame->hide();
  //printf("\n Spectrum Page is selected---");
}

void DisplayWindow::DrawINTSpectrumFrame()
{
    IntSpectrumFrame=new QFrame(DisplayProcesingFrame);
    IntSpectrumFrame->setMouseTracking(true);
    IntSpectrumFrame->setGeometry(QRect(0,0,790,500));
    IntSpectrumFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    IntSpectrumFrame->setFrameShape(QFrame::StyledPanel);
    IntSpectrumFrame->setFrameShadow(QFrame::Raised);
    IntSpectrumFrame->hide();
   //IntSpectrumFrame->setStyleSheet("border: 5px solid black");
   //printf("\n Spectrum Page is Selected---");
}

void DisplayWindow::OctaveBandFrame()
{
    OctaveFrame=new QFrame(DisplayProcesingFrame);
    OctaveFrame->setMouseTracking(true);
    OctaveFrame->setGeometry(QRect(0,0,790,500));
    OctaveFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    OctaveFrame->setFrameShape(QFrame::StyledPanel);
    OctaveFrame->setFrameShadow(QFrame::Raised);
    OctaveFrame->hide();
}

void DisplayWindow::DrawTrackFrame()
{
   QFont font;
   font.setFamily(QString::fromUtf8("Sans Serif"));
   font.setBold(false);
   font.setPointSize(10);
   font.setPixelSize(12);
    SetTrackFrame=new QFrame(DisplayProcesingFrame);
    SetTrackFrame->setMouseTracking(true);
    SetTrackFrame->setGeometry(QRect(0,0,790,500));
    SetTrackFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    SetTrackFrame->setFrameShape(QFrame::StyledPanel);
    SetTrackFrame->setFrameShadow(QFrame::Raised);

            TrackInfoLabel= new QLabel(SetTrackFrame);
            TrackInfoLabel->setObjectName(QString::fromUtf8("label"));
            TrackInfoLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(176,255,142);"));
            TrackInfoLabel->setFont(font);
            TrackInfoLabel->setGeometry(QRect(20,10,100,30));
            TrackInfoLabel->setText("Track Number : ");

            TrackFreq= new QLabel(SetTrackFrame);
            TrackFreq->setObjectName(QString::fromUtf8("label"));
            TrackFreq->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(176,255,142);"));
            TrackFreq->setFont(font);
            TrackFreq->setGeometry(QRect(200,10,120,30));
            TrackFreq->setText("Track Frequency : ");

            TrackAmp= new QLabel(SetTrackFrame);
            TrackAmp->setObjectName(QString::fromUtf8("label"));
            TrackAmp->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(176,255,142);"));
            TrackAmp->setFont(font);
            TrackAmp->setGeometry(QRect(380,10,120,30));
            TrackAmp->setText("Track Amplitude : ");

                    TrackInit= new QLabel(SetTrackFrame);
                    TrackInit->setObjectName(QString::fromUtf8("label"));
                    TrackInit->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                        "color: rgb(176,255,142);"));
                    TrackInit->setFont(font);
                    TrackInit->setGeometry(QRect(600,10,120,30));
                    TrackInit->setText("CH INIT:");

                    TrackInfoLabelNo= new QLabel(SetTrackFrame);
                    TrackInfoLabelNo->setObjectName(QString::fromUtf8("label"));
                    TrackInfoLabelNo->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                        "color: rgb(176,255,142);"));
                    TrackInfoLabelNo->setFont(font);
                    TrackInfoLabelNo->setGeometry(QRect(120,10,50,30));
                    TrackInfoLabelNo->setText("1");

                    TrackFreqVal= new QLabel(SetTrackFrame);
                    TrackFreqVal->setObjectName(QString::fromUtf8("label"));
                    TrackFreqVal->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                        "color: rgb(176,255,142);"));
                    TrackFreqVal->setFont(font);
                    TrackFreqVal->setGeometry(QRect(315,10,60,30));
                    TrackFreqVal->setText("0");

                    TrackAmpVal= new QLabel(SetTrackFrame);
                    TrackAmpVal->setObjectName(QString::fromUtf8("label"));
                    TrackAmpVal->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                        "color: rgb(176,255,142);"));
                    TrackAmpVal->setFont(font);
                    TrackAmpVal->setGeometry(QRect(490,10,80,30));
                    TrackAmpVal->setText("0");

                    TrackInitCh= new QLabel(SetTrackFrame);
                    TrackInitCh->setObjectName(QString::fromUtf8("label"));
                    TrackInitCh->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                        "color: rgb(176,255,142);"));
                    TrackInitCh->setFont(font);
                    TrackInitCh->setGeometry(QRect(660,10,40,30));
                    TrackInitCh->setText("0");

    for(iCount=0;iCount<61;iCount++)
    {
        TrackMarkerLabel[iCount]= new QLabel(SetTrackFrame);
        TrackMarkerLabel[iCount]->setObjectName(QString::fromUtf8("label"));
        TrackMarkerLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
            "color: rgb(228,225,255);"));
        TrackMarkerLabel[iCount]->setFont(font);
    }

    TrackMarkerLabel[0]->setGeometry(QRect(80,80,30,30));
    TrackMarkerLabel[0]->setText("H1");
    TrackMarkerLabel[1]->setGeometry(QRect(120,80,30,30));
    TrackMarkerLabel[1]->setText("H2");
    TrackMarkerLabel[2]->setGeometry(QRect(160,80,30,30));
    TrackMarkerLabel[2]->setText("H3");
    TrackMarkerLabel[3]->setGeometry(QRect(200,80,30,30));
    TrackMarkerLabel[3]->setText("H4");

    TrackMarkerLabel[4]->setGeometry(QRect(240,80,30,30));
    TrackMarkerLabel[4]->setText("H5");
    TrackMarkerLabel[5]->setGeometry(QRect(280,80,30,30));
    TrackMarkerLabel[5]->setText("H6");
    TrackMarkerLabel[6]->setGeometry(QRect(320,80,30,30));
    TrackMarkerLabel[6]->setText("H7");
    TrackMarkerLabel[7]->setGeometry(QRect(360,80,30,30));
    TrackMarkerLabel[7]->setText("H8");

    TrackMarkerLabel[8]->setGeometry(QRect(400,80,30,30));
    TrackMarkerLabel[8]->setText("H9");
    TrackMarkerLabel[9]->setGeometry(QRect(440,80,30,30));
    TrackMarkerLabel[9]->setText("H10");
    TrackMarkerLabel[10]->setGeometry(QRect(480,80,30,30));
    TrackMarkerLabel[10]->setText("H11");
    TrackMarkerLabel[11]->setGeometry(QRect(520,80,30,30));
    TrackMarkerLabel[11]->setText("H12");

    TrackMarkerLabel[12]->setGeometry(QRect(560,80,30,30));
    TrackMarkerLabel[12]->setText("H13");
    TrackMarkerLabel[13]->setGeometry(QRect(600,80,30,30));
    TrackMarkerLabel[13]->setText("H14");
    TrackMarkerLabel[14]->setGeometry(QRect(640,80,30,30));
    TrackMarkerLabel[14]->setText("H15");
    TrackMarkerLabel[15]->setGeometry(QRect(80,120,30,30));
    TrackMarkerLabel[15]->setText("H16");

    TrackMarkerLabel[16]->setGeometry(QRect(120,120,30,30));
    TrackMarkerLabel[16]->setText("H17");

    TrackMarkerLabel[17]->setGeometry(QRect(160,120,30,30));
    TrackMarkerLabel[17]->setText("H18");
    TrackMarkerLabel[18]->setGeometry(QRect(200,120,30,30));
    TrackMarkerLabel[18]->setText("H19");
    TrackMarkerLabel[19]->setGeometry(QRect(240,120,30,30));
    TrackMarkerLabel[19]->setText("H20");

    TrackMarkerLabel[20]->setGeometry(QRect(280,120,30,30));
    TrackMarkerLabel[20]->setText("H21");
    TrackMarkerLabel[21]->setGeometry(QRect(320,120,30,30));
    TrackMarkerLabel[21]->setText("H22");
    TrackMarkerLabel[22]->setGeometry(QRect(360,120,30,30));
    TrackMarkerLabel[22]->setText("H23");
    TrackMarkerLabel[23]->setGeometry(QRect(400,120,30,30));
    TrackMarkerLabel[23]->setText("H24");
    TrackMarkerLabel[24]->setGeometry(QRect(440,120,30,30));
    TrackMarkerLabel[24]->setText("H25");
    TrackMarkerLabel[25]->setGeometry(QRect(480,120,30,30));
    TrackMarkerLabel[25]->setText("H26");
    TrackMarkerLabel[26]->setGeometry(QRect(520,120,30,30));
    TrackMarkerLabel[26]->setText("H27");
    TrackMarkerLabel[27]->setGeometry(QRect(560,120,30,30));
    TrackMarkerLabel[27]->setText("H28");
    TrackMarkerLabel[28]->setGeometry(QRect(600,120,30,30));
    TrackMarkerLabel[28]->setText("H29");
    TrackMarkerLabel[29]->setGeometry(QRect(80,180,30,30));
    TrackMarkerLabel[29]->setText("A1");
    TrackMarkerLabel[30]->setGeometry(QRect(120,180,30,30));
    TrackMarkerLabel[30]->setText("A2");
    TrackMarkerLabel[31]->setGeometry(QRect(160,180,30,30));
    TrackMarkerLabel[31]->setText("A3");
    TrackMarkerLabel[32]->setGeometry(QRect(200,180,30,30));
    TrackMarkerLabel[32]->setText("A4");
    TrackMarkerLabel[33]->setGeometry(QRect(240,180,30,30));
    TrackMarkerLabel[33]->setText("A5");
    TrackMarkerLabel[34]->setGeometry(QRect(280,180,30,30));
    TrackMarkerLabel[34]->setText("A6");
    TrackMarkerLabel[35]->setGeometry(QRect(320,180,30,30));
    TrackMarkerLabel[35]->setText("A7");
    TrackMarkerLabel[36]->setGeometry(QRect(360,180,30,30));
    TrackMarkerLabel[36]->setText("A8");
    TrackMarkerLabel[37]->setGeometry(QRect(400,180,30,30));
    TrackMarkerLabel[37]->setText("A9");
    TrackMarkerLabel[38]->setGeometry(QRect(440,180,30,30));
    TrackMarkerLabel[38]->setText("A10");
    TrackMarkerLabel[39]->setGeometry(QRect(480,180,30,30));
    TrackMarkerLabel[39]->setText("A11");
    TrackMarkerLabel[40]->setGeometry(QRect(520,180,30,30));
    TrackMarkerLabel[40]->setText("A12");
    TrackMarkerLabel[41]->setGeometry(QRect(560,180,30,30));
    TrackMarkerLabel[41]->setText("A13");
    TrackMarkerLabel[42]->setGeometry(QRect(600,180,30,30));
    TrackMarkerLabel[42]->setText("A14");
    TrackMarkerLabel[43]->setGeometry(QRect(640,180,30,30));
    TrackMarkerLabel[43]->setText("A15");
    TrackMarkerLabel[44]->setGeometry(QRect(80,220,30,30));
    TrackMarkerLabel[44]->setText("A16");
    TrackMarkerLabel[45]->setGeometry(QRect(120,220,30,30));
    TrackMarkerLabel[45]->setText("A17");
    TrackMarkerLabel[46]->setGeometry(QRect(160,220,30,30));
    TrackMarkerLabel[46]->setText("A18");
    TrackMarkerLabel[47]->setGeometry(QRect(200,220,30,30));
    TrackMarkerLabel[47]->setText("A19");
    TrackMarkerLabel[48]->setGeometry(QRect(240,220,30,30));
    TrackMarkerLabel[48]->setText("A20");
    TrackMarkerLabel[49]->setGeometry(QRect(280,220,30,30));
    TrackMarkerLabel[49]->setText("A21");
    TrackMarkerLabel[50]->setGeometry(QRect(320,220,30,30));
    TrackMarkerLabel[50]->setText("A22");
    TrackMarkerLabel[51]->setGeometry(QRect(360,220,30,30));
    TrackMarkerLabel[51]->setText("A23");
    TrackMarkerLabel[52]->setGeometry(QRect(400,220,30,30));
    TrackMarkerLabel[52]->setText("A24");
    TrackMarkerLabel[53]->setGeometry(QRect(440,220,30,30));
    TrackMarkerLabel[53]->setText("A25");
    TrackMarkerLabel[54]->setGeometry(QRect(480,220,30,30));
    TrackMarkerLabel[54]->setText("A26");
    TrackMarkerLabel[55]->setGeometry(QRect(520,220,30,30));
    TrackMarkerLabel[55]->setText("A27");
    TrackMarkerLabel[56]->setGeometry(QRect(560,220,30,30));
    TrackMarkerLabel[56]->setText("A28");
    TrackMarkerLabel[57]->setGeometry(QRect(600,220,30,30));
    TrackMarkerLabel[57]->setText("A29");
    TrackMarkerLabel[58]->setGeometry(QRect(640,220,30,30));
    TrackMarkerLabel[58]->setText("A30");
    TrackMarkerLabel[59]->setGeometry(QRect(80,260,30,30));
    TrackMarkerLabel[59]->setText("A31");
    TrackMarkerLabel[60]->setGeometry(QRect(120,260,30,30));
    TrackMarkerLabel[60]->setText("A32");
    SetTrackFrame->hide();
}

void DisplayWindow::PageSelectFunction(int Select_ID)
{
    switch(Select_ID)
    {
    case 0:
             SelectedPageID=0;
             CurrentChannelSel=SelectedChannel;
             ChangeDisplayPageConfiguration(0);                     //Spectrum Page
             LofarFrame->show();
             SpectrumFrame->hide();
             IntLofarFrame->hide();
             IntSpectrumFrame->hide();
             SetTrackFrame->hide();
             OctaveFrame->hide();
             RawDataFrame->hide();
	     Spec_Comp_Frame->hide();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));

             break;
    case 1:
             SelectedPageID=1;
             CurrentChannelSel=SelectedChannel;
             ChangeDisplayPageConfiguration(1);                     //Lofar Page
             SpectrumFrame->show();
             IntLofarFrame->hide();
             IntSpectrumFrame->hide();
             SetTrackFrame->hide();
             OctaveFrame->hide();
             RawDataFrame->hide();
	     Spec_Comp_Frame->hide();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
             break;

    case 2:
                         SelectedPageID=2;
                         SelectedChannel=CurrentChannelSel;
                         IntLofarFrame->show();
                         SpectrumFrame->hide();
                          LofarFrame->hide();
                         IntSpectrumFrame->hide();
                         SetTrackFrame->hide();
                         OctaveFrame->hide();
                         RawDataFrame->hide();
			 Spec_Comp_Frame->hide();
                         ChangeDisplayPageConfiguration(2);       // Track Page
                         MGroupSensor[0]->setParent(IntLofarFrame);
                         MGroupSensor[1]->setParent(IntLofarFrame);
                         MGroupSensor[2]->setParent(IntLofarFrame);
                         MGroupSensor[3]->setParent(IntLofarFrame);
                         MGroupSensor[0]->show();
                         MGroupSensor[1]->show();
                         MGroupSensor[2]->show();
                         MGroupSensor[3]->show();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
                         break;
    case 3:
                         SelectedPageID=3;
                         SelectedChannel=CurrentChannelSel;
                         IntSpectrumFrame->show();
                         IntLofarFrame->hide();
                         SpectrumFrame->hide();
                         LofarFrame->hide();
                         SetTrackFrame->hide();
                         OctaveFrame->hide();
                         RawDataFrame->hide();
			 Spec_Comp_Frame->hide();
                         ChangeDisplayPageConfiguration(3);
                         MGroupSensor[0]->setParent(IntSpectrumFrame);
                         MGroupSensor[1]->setParent(IntSpectrumFrame);
                         MGroupSensor[2]->setParent(IntSpectrumFrame);
                         MGroupSensor[3]->setParent(IntSpectrumFrame);
                         MGroupSensor[0]->show();
                         MGroupSensor[1]->show();
                         MGroupSensor[2]->show();
                         MGroupSensor[3]->show();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
                         break;

    case 4:
                      SelectedPageID=4;   
                      SelectedChannel=CurrentChannelSel;
                      ChangeDisplayPageConfiguration(4);
                      OctaveFrame->show();
                      SetTrackFrame->hide();
                      IntSpectrumFrame->hide();
                      IntLofarFrame->hide();
                      SpectrumFrame->hide();
                      LofarFrame->hide();
                      RawDataFrame->hide();
		      Spec_Comp_Frame->hide();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
                      break;


    case 5:           SelectedPageID=5;
                      SelectedChannel=CurrentChannelSel;
                      ChangeDisplayPageConfiguration(5);
                      SetTrackFrame->show();
                      OctaveFrame->hide();
                      IntSpectrumFrame->hide();
                      IntLofarFrame->hide();
                      SpectrumFrame->hide();
                      LofarFrame->hide();
                      RawDataFrame->hide();
		      Spec_Comp_Frame->hide();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
                      break;

     case 6:         SelectedPageID = 6;
                     SelectedChannel=CurrentChannelSel;
                     ChangeDisplayPageConfiguration(6);
                     RawDataFrame->show();
                     SetTrackFrame->hide();
                     OctaveFrame->hide();
                     IntSpectrumFrame->hide();
                     IntLofarFrame->hide();
                     SpectrumFrame->hide();
                     LofarFrame->hide();
		     Spec_Comp_Frame->hide();
	     graphPlot->LofDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
	     graphPlot->GDyn = 1;
             LFreeze->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);\n"
                                                          "color: rgb(0, 0, 0);"));
                     break;

     case 7:
             SelectedPageID=7;
             SelectedChannel=CurrentChannelSel;
             ChangeDisplayPageConfiguration(7); 
             Spec_Comp_Frame->show();
	     RawDataFrame->hide();
             SetTrackFrame->hide();
             OctaveFrame->hide();
             IntSpectrumFrame->hide();
             IntLofarFrame->hide();
             SpectrumFrame->hide();
             LofarFrame->hide();

	           break;

     default: break;

    };
}

void DisplayWindow::ChangeDisplayPageConfiguration(int16_t x)
{
    switch(x)
    {
    case 0:
                  SenserRefresh=true;
                  ZoomSpectrum->hide();
                  xPos=200;
                  SpectrumZoomCursorStatus=false;
                  LofarZoomCursorStatus=true;
                  ZoomLofar->show();
                  LofarZoomEnable=false;
                  graphPlot->WaterFallscene->clear();
                  RefreshLOFARWaterFallgraph();
                  UpdateSpectrumDisplay();

                  MainDisplayHFrame->setParent(graphPlot->LofarGraphLegend);
                  MainDisplayVFrame->setParent(graphPlot->LofarGraphLegend);
                  ZoomDisplayHFrame->setParent(graphPlot->LofarZoomGraphLegend);
                  ZoomDisplayVFrame->setParent(graphPlot->LofarZoomGraphLegend);
                  graphPlot->LofarGraphLegend->replot();
                  graphPlot->Lofar_Scale_TickLabel1->setText("0");
                  graphPlot->Lofar_Scale_TickLabel2->setText("600");
                  graphPlot->Lofar_Scale_TickLabel3->setText("1200");
                  graphPlot->GraphXScaleFrame->setParent(graphPlot->LofarGraphLegend);
                  graphPlot->GraphYScaleFrame->setParent(graphPlot->LofarGraphLegend);
                  graphPlot->zoomGraphXScaleFrame->setParent(graphPlot->LofarZoomGraphLegend);
                  graphPlot->zoomGraphYScaleFrame->setParent(graphPlot->LofarZoomGraphLegend);
                //graphPlot->WaterFallview->setParent(graphPlot->LofarWaterFallLegend);

                  ZoomCursourHLine->setParent(graphPlot->LofarGraphLegend);
                  ZoomCursorVLine1->setParent(graphPlot->LofarGraphLegend);
                  ZoomCursorVLine2->setParent(graphPlot->LofarGraphLegend);
                  ZoomCursorVLine3->setParent(graphPlot->LofarGraphLegend);
                  ZoomCursorWidth = 200;
                  SetZoomCursorPosition();
                  graphPlot->HarmonicCursorFrame->setParent(graphPlot->LofarWaterFallLegend);
                  graphPlot->HarmonicCursorFrame->hide();

                 //SetSpectrumZoomCursor();

                  graphPlot->LofarGraphLegend->show();
                  graphPlot->LofarWaterFallLegend->show();
                  graphPlot->LofarZoomGraphLegend->show();
                  MainDisplayVFrame->show();
                  MainDisplayHFrame->show();
                  ZoomDisplayHFrame->show();
                  ZoomDisplayVFrame->show();
                  ZoomCursourHLine->show();
                  ZoomCursorVLine1->show();
                  ZoomCursorVLine2->show();
                  ZoomCursorVLine3->show();
                  graphPlot->Lofar_Scale_TickLabel1->show();
                  graphPlot->Lofar_Scale_TickLabel2->show();
                  graphPlot->Lofar_Scale_TickLabel3->show();
                  graphPlot->GraphXScaleFrame->show();
                  graphPlot->GraphYScaleFrame->show();
                  graphPlot->zoomGraphXScaleFrame->show();
                  graphPlot->zoomGraphYScaleFrame->show();
                  break;

    case 1:   SenserRefresh=true;
              ZoomLofar->hide();
              ZoomSpectrum->show();
              SpectrumZoomEnable=false;
              SpectrumZoomCursorStatus=true;
              LofarZoomCursorStatus=false;
              xPos = 300;
              graphPlot->SpectrumWaterFallScene->clear();
              RefreshLOFARWaterFallgraph();
              UpdateSpectrumDisplay();

              MainDisplayHFrame->setParent(graphPlot->SpectrumGraphLegend);
              MainDisplayVFrame->setParent(graphPlot->SpectrumGraphLegend);
              ZoomDisplayHFrame->setParent(graphPlot->SpectrumZoomGraphLegend);
              ZoomDisplayVFrame->setParent(graphPlot->SpectrumZoomGraphLegend);
              graphPlot->SpectrumGraphLegend->replot();
              graphPlot->Lofar_Scale_TickLabel1->setText("0");
              graphPlot->Lofar_Scale_TickLabel2->setText("2000");
              graphPlot->Lofar_Scale_TickLabel3->setText("4000");
              graphPlot->GraphXScaleFrame->setParent(graphPlot->SpectrumGraphLegend);
              graphPlot->GraphYScaleFrame->setParent(graphPlot->SpectrumGraphLegend);
              graphPlot->zoomGraphXScaleFrame->setParent(graphPlot->SpectrumZoomGraphLegend);
              graphPlot->zoomGraphYScaleFrame->setParent(graphPlot->SpectrumZoomGraphLegend);
              //graphPlot->WaterFallview->setParent(graphPlot->SpectrumWaterFallLegend);
              ZoomCursourHLine->setParent(graphPlot->SpectrumGraphLegend);
              ZoomCursorVLine1->setParent(graphPlot->SpectrumGraphLegend);
              ZoomCursorVLine2->setParent(graphPlot->SpectrumGraphLegend);
              ZoomCursorVLine3->setParent(graphPlot->SpectrumGraphLegend);
              ZoomCursorWidth=244;
              SetZoomCursorPosition();
             //SetLofarZoomCursor() ;
              graphPlot->HarmonicCursorFrame->setParent(graphPlot->SpectrumWaterFallLegend);
              graphPlot->HarmonicCursorFrame->hide();
              graphPlot->SpectrumGraphLegend->show();
              graphPlot->SpectrumWaterFallLegend->hide();
              graphPlot->SpectrumZoomGraphLegend->hide();
              MainDisplayVFrame->hide();
              MainDisplayHFrame->hide();
              ZoomDisplayHFrame->show();
              ZoomDisplayVFrame->show();
              ZoomCursourHLine->hide();//Yellow cursor zoom
              ZoomCursorVLine1->hide();
              ZoomCursorVLine2->hide();
              ZoomCursorVLine3->hide();
              graphPlot->Lofar_Scale_TickLabel1->hide();
              graphPlot->Lofar_Scale_TickLabel2->hide();
              graphPlot->Lofar_Scale_TickLabel3->hide();
              graphPlot->GraphXScaleFrame->hide();
              graphPlot->GraphYScaleFrame->hide();
              graphPlot->zoomGraphXScaleFrame->show();
              graphPlot->zoomGraphYScaleFrame->show();
              break;

    case 2:
               SenserRefresh=true;
               HydrophoneSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                              "color: rgb(0, 0, 0);"));
               AccelerometerSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                                 "color: rgb(0, 0, 0);"));
               UpdateSpectrumDisplay();
               break;


     case 3:     SenserRefresh = true;
                 HydrophoneSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                                "color: rgb(0, 0, 0);"));
                 AccelerometerSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                                   "color: rgb(0, 0, 0);"));
                 UpdateSpectrumDisplay();
                 break;


    case 4:      SenserRefresh = true;
                 graphPlot->OctaveBandLegendPlot->replot();
                 graphPlot->OctaveBandLegendPlot->show();
                 break;

    case 5:
                 HydrophoneSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                       "color: rgb(0, 0, 0);"));
                 AccelerometerSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                          "color: rgb(0, 0, 0);"));
                 UpdateSpectrumDisplay();
                 break;

    case 6:     SenserRefresh = true;
                UpdateSpectrumDisplay();
                //---Mouse Moving Event---//
                //MainDisplayHFrame->setParent(graphPlot->RawDataDelSpectGraphLegend);
                //MainDisplayVFrame->setParent(graphPlot->RawDataDelSpectGraphLegend);
                //---Mouse Moving Event---//

//                ZoomCursourHLine->setParent(graphPlot->RawDataDelSpectGraphLegend);
//                ZoomCursorVLine1->setParent(graphPlot->RawDataDelSpectGraphLegend);
//                ZoomCursorVLine2->setParent(graphPlot->RawDataDelSpectGraphLegend);
//                ZoomCursorVLine3->setParent(graphPlot->RawDataDelSpectGraphLegend);
//                ZoomCursorWidth = 244;
//                ZoomCursourHLine->show();
//                ZoomCursorVLine1->show();
//                ZoomCursorVLine2->show();
//                ZoomCursorVLine3->show();
              //SetZoomCursorPosition();
                graphPlot->HarmonicCursorFrame->setParent(graphPlot->RawDataDelSpectGraphLegend);
                graphPlot->HarmonicCursorFrame->hide();

            break;
     case 7: SenserRefresh = true;
           // graphPlot->LofarGraphLegend->show();

            break;
            default :break;
    };
}


void DisplayWindow::CheckEthernetLinkStatus()
{
    for(iCount=0;iCount<MAX_SCU_NO;iCount++)
    {
       if(LinkStatus.SCU_LINK_STATUS[iCount]==true)
       {
           SCUName[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255, 0);\n"
                                                            "color: rgb(0,0, 0);"));
           FDSLink(iCount+1,true);
       }
       else
       {
           SCUName[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
                                                            "color: rgb(0, 0, 0);"));
           FDSLink(iCount+1,false);
       }
    }

       for(iCount=0;iCount<MAX_SCU_NO;iCount++)
       {
            LinkStatus.SCU_LINK_STATUS[iCount]=false;
       }
}

void DisplayWindow::FDSLink(int16_t SCU,bool STATUS)
{
int32_t i,y;
    switch(SCU)
    {
    case 1:    if(STATUS==true)
              {

          /*   ACCSENSOR[0]->setBrush( Qt::yellow);
               //ChannelLinkStatus[1][0]=1;
                  ACCSENSOR[1]->setBrush( Qt::yellow);
                //ChannelLinkStatus[1][1]=1;
                  ACCSENSOR[2]->setBrush( Qt::yellow);
                //ChannelLinkStatus[1][2]=1;
                  HYDSENSOR[0]->setBrush( Qt::yellow);
                //  ChannelLinkStatus[0][0]=1;
                  HYDSENSOR[1]->setBrush( Qt::yellow);
                //ChannelLinkStatus[0][1]=1;
                  HYDSENSOR[2]->setBrush( Qt::yellow);
               //ChannelLinkStatus[0][2]=1;
                  HYDSENSOR[3]->setBrush( Qt::yellow);
                //  ChannelLinkStatus[0][3]=1;
                  HYDSENSOR[4]->setBrush( Qt::yellow);
               //ChannelLinkStatus[0][4]=1;

            */
                  for(i=0;i<8;i++)
                  {

                 HydrophoneLabel[i]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                       "color: rgb(255,255,0);"));
                 AcceleometerLabel[i]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                   "color: rgb(255,255,0);"));
                  }

                  for(i=0;i<8;i++)
                  {
                    ChannelLinkStatus[0][i]=1;
                    ChannelLinkStatus[1][i]=1;
                  }
              }
              else
              {
            /*      ACCSENSOR[0]->setBrush( Qt::gray);
                     ChannelLinkStatus[1][0]=0;
                     ACCSENSOR[1]->setBrush( Qt::gray);
                     ChannelLinkStatus[1][1]=0;
                     ACCSENSOR[2]->setBrush( Qt::gray);
                     ChannelLinkStatus[1][2]=0;
                     HYDSENSOR[0]->setBrush( Qt::gray);
                     ChannelLinkStatus[0][0]=0;
                     HYDSENSOR[1]->setBrush( Qt::gray);
                     ChannelLinkStatus[0][1]=0;
                     HYDSENSOR[2]->setBrush( Qt::gray);
                     ChannelLinkStatus[0][2]=0;
                     HYDSENSOR[3]->setBrush( Qt::gray);
                     ChannelLinkStatus[0][3]=0;
                     HYDSENSOR[4]->setBrush( Qt::gray);
                     ChannelLinkStatus[0][4]=0;
                     */
                     for(i=0;i<8;i++)
                     {

                    HydrophoneLabel[i]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                          "color: rgb(207,179,255);"));
                    AcceleometerLabel[i]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                      "color: rgb(207,179,255);"));
                     }

                     for(i=0;i<8;i++)
                     {
                       ChannelLinkStatus[0][i]=0;
                       ChannelLinkStatus[1][i]=0;
                       Energy_Histo_Pos[i]=0;
                     }
             }
             break;

    case 2:
                     if(STATUS==true)
                     {

      /*                 HYDSENSOR[5]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][5]=1;
                          HYDSENSOR[10]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][10]=1;
                          HYDSENSOR[12]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][12]=1;
                          HYDSENSOR[14]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][14]=1;
                          HYDSENSOR[16]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][16]=1;
                          HYDSENSOR[17]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][17]=1;
      */

                          for(i=0;i<8;i++)
                          {
                         y=i+8;
                         HydrophoneLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                               "color: rgb(255,255,0);"));
                          AcceleometerLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                           "color: rgb(255,255,0);"));
                          }

                          for(i=0;i<8;i++)
                          {
                               y=i+8;
                            ChannelLinkStatus[0][y]=1;
                            ChannelLinkStatus[1][y]=1;
                          }
                     }
                     else
                     {

         /*               HYDSENSOR[5]->setBrush( Qt::gray);
                          ChannelLinkStatus[0][5]=0;
                          HYDSENSOR[10]->setBrush(Qt::gray);
                          ChannelLinkStatus[0][10]=0;
                          HYDSENSOR[12]->setBrush(Qt::gray);
                          ChannelLinkStatus[0][12]=0;
                          HYDSENSOR[14]->setBrush(Qt::gray);
                          ChannelLinkStatus[0][14]=0;
                          HYDSENSOR[16]->setBrush(Qt::gray);
                          ChannelLinkStatus[0][16]=0;
                          HYDSENSOR[17]->setBrush(Qt::gray);
                          ChannelLinkStatus[0][17]=0;
          */

                          for(i=0;i<8;i++)
                          {
                            y=i+8;
                         HydrophoneLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                               "color: rgb(207,179,255);"));
                          AcceleometerLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                           "color: rgb(207,179,255);"));
                          }

                          for(i=0;i<8;i++)
                          {
                                y=i+8;
                            ChannelLinkStatus[0][y]=0;
                            ChannelLinkStatus[1][y]=0;
                            Energy_Histo_Pos[y]=0;
                          }
                       }
                      break;

    case 3:  if(STATUS==true)
             {
      /*             ACCSENSOR[3]->setBrush( Qt::yellow);
                     ChannelLinkStatus[1][3]=1;
                     ACCSENSOR[4]->setBrush( Qt::yellow);
                     ChannelLinkStatus[1][4]=1;
                     ACCSENSOR[5]->setBrush( Qt::yellow);
                     ChannelLinkStatus[1][5]=1;
                     HYDSENSOR[6]->setBrush( Qt::yellow);
                     ChannelLinkStatus[0][6]=1;
                     HYDSENSOR[7]->setBrush( Qt::yellow);
                     ChannelLinkStatus[0][7]=1;
                     HYDSENSOR[8]->setBrush( Qt::yellow);
                     ChannelLinkStatus[0][8]=1;
                     HYDSENSOR[9]->setBrush(Qt::yellow);
                     ChannelLinkStatus[0][9]=1;
                     HYDSENSOR[11]->setBrush( Qt::yellow);
                     ChannelLinkStatus[0][11]=1;
                     HYDSENSOR[13]->setBrush( Qt::yellow);
                     ChannelLinkStatus[0][13]=1;
                     HYDSENSOR[15]->setBrush( Qt::yellow);
                     ChannelLinkStatus[0][15]=1;

*/
                     for(i=0;i<8;i++)
                     {
                     y=i+16;
                    HydrophoneLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                          "color: rgb(255,255,0);"));
                    AcceleometerLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                      "color: rgb(255,255,0);"));
                     }

                     for(i=0;i<8;i++)
                     {
                          y=i+16;
                       ChannelLinkStatus[0][y]=1;
                       ChannelLinkStatus[1][y]=1;
                     }
             }
             else
             {
  /*                ACCSENSOR[3]->setBrush( Qt::gray);
                    ChannelLinkStatus[1][3]=0;
                    ACCSENSOR[4]->setBrush( Qt::gray);
                    ChannelLinkStatus[1][4]=0;
                    ACCSENSOR[5]->setBrush( Qt::gray);
                    ChannelLinkStatus[1][5]=0;

                 HYDSENSOR[6]->setBrush( Qt::gray);
                 ChannelLinkStatus[0][6]=0;
                 HYDSENSOR[7]->setBrush(Qt::gray);
                 ChannelLinkStatus[0][7]=0;
                 HYDSENSOR[8]->setBrush(Qt::gray);
                 ChannelLinkStatus[0][8]=0;
                 HYDSENSOR[9]->setBrush(Qt::gray);
                 ChannelLinkStatus[0][9]=0;
                 HYDSENSOR[11]->setBrush(Qt::gray);
                 ChannelLinkStatus[0][11]=0;
                 HYDSENSOR[13]->setBrush(Qt::gray);
                 ChannelLinkStatus[0][13]=0;
                 HYDSENSOR[15]->setBrush(Qt::gray);
                 ChannelLinkStatus[0][15]=0;
*/

                 for(i=0;i<8;i++)
                 {
                 y=i+16;
                HydrophoneLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                      "color: rgb(207,179,255);"));
                 AcceleometerLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                  "color: rgb(207,179,255);"));
                 }
                 for(i=0;i<8;i++)
                 {
                      y=i+16;
                   ChannelLinkStatus[0][y]=0;
                   ChannelLinkStatus[1][y]=0;
                   Energy_Histo_Pos[y]=0;
                 }
             }
            break;


    case 4:           if(STATUS==true)
                       {
    /*                   ACCSENSOR[6]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][6]=1;
                         ACCSENSOR[7]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][7]=1;
                         ACCSENSOR[8]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][8]=1;
   */
                         for(i=0;i<5;i++)
                         {
                           y=i+24;
                           HydrophoneLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                                  "color: rgb(255,255,0);"));
                           }

                         for(i=0;i<8;i++)
                         {
                             y=i+24;

                         AcceleometerLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                          "color: rgb(255,255,0);"));
                         }

                         for(i=0;i<5;i++)
                         {
                           y=i+24;
                           ChannelLinkStatus[0][y]=1;
                         }

                         for(i=0;i<8;i++)
                         {
                          y=i+24;
                           ChannelLinkStatus[1][y]=1;
                         }


                      }
                         else
                     {

     /*                  ACCSENSOR[6]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][6]=0;
                         ACCSENSOR[7]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][7]=0;
                         ACCSENSOR[8]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][8]=0;
      */

                         for(i=0;i<5;i++)
                         {
                             y=i+24;

                           HydrophoneLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                                  "color: rgb(207,179,255);"));
                           }


                         for(i=0;i<8;i++)
                         {
                             y=i+24;

                         AcceleometerLabel[y]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                          "color: rgb(207,179,255);"));
                         }
                         for(i=0;i<5;i++)
                         {
                             y=i+24;

                           ChannelLinkStatus[0][y]=0;
                         }

                         for(i=0;i<8;i++)
                         {
                             y=i+24;
                           ChannelLinkStatus[1][y]=0;
                           Energy_Histo_Pos[y]=0;
                         }

                     }
                  break;

    case 5:           if(STATUS==1)
                       {
                          ACCSENSOR[9]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][9]=1;
                          ACCSENSOR[10]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][10]=1;
                          ACCSENSOR[11]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][11]=1;
                          ACCSENSOR[12]->setBrush( Qt::yellow);
                      }
                   else
                     {
                         ACCSENSOR[9]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][9]=0;
                         ACCSENSOR[10]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][10]=0;
                         ACCSENSOR[11]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][11]=0;
                         ACCSENSOR[12]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][12]=0;
                     }
                  break;
    case 6:       if(STATUS==1)
                  {

                          ACCSENSOR[13]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][13]=1;
                          ACCSENSOR[14]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][14]=1;
                          ACCSENSOR[15]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][15]=1;
                          ACCSENSOR[16]->setBrush( Qt::yellow);
                          ChannelLinkStatus[1][16]=1;
                     }
                else
                     {
                         ACCSENSOR[13]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][13]=0;
                         ACCSENSOR[14]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][14]=0;
                         ACCSENSOR[15]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][15]=0;
                         ACCSENSOR[16]->setBrush( Qt::gray);
                         ChannelLinkStatus[1][16]=0;
                      }
                        break;
    case 7:       if(STATUS==1)
                  {
                         ACCSENSOR[17]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][17]=1;
                         ACCSENSOR[18]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][18]=1;
                         ACCSENSOR[19]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][19]=1;
                         ACCSENSOR[20]->setBrush( Qt::yellow);
                         ChannelLinkStatus[1][20]=1;
                   }
                else
                     {
                              ACCSENSOR[17]->setBrush( Qt::gray);
                              ChannelLinkStatus[1][17]=0;
                              ACCSENSOR[18]->setBrush( Qt::gray);
                              ChannelLinkStatus[1][18]=0;
                              ACCSENSOR[19]->setBrush( Qt::gray);
                              ChannelLinkStatus[1][19]=0;
                              ACCSENSOR[20]->setBrush( Qt::gray);
                              ChannelLinkStatus[1][20]=0;
                    }
                        break;

    case 8:       if(STATUS==1)
                  {
                         HYDSENSOR[19]->setBrush( Qt::yellow);
                         ChannelLinkStatus[0][19]=1;
                         HYDSENSOR[20]->setBrush( Qt::yellow);
                         ChannelLinkStatus[0][20]=1;
                         HYDSENSOR[21]->setBrush( Qt::yellow);
                         ChannelLinkStatus[0][21]=1;
                         HYDSENSOR[22]->setBrush( Qt::yellow);
                         ChannelLinkStatus[0][22]=1;
                         HYDSENSOR[24]->setBrush( Qt::yellow);
                         ChannelLinkStatus[0][24]=1;

                          }
                else
                     {

                                  HYDSENSOR[19]->setBrush( Qt::gray);
                                  ChannelLinkStatus[0][19]=0;
                                  HYDSENSOR[20]->setBrush( Qt::gray);
                                  ChannelLinkStatus[0][20]=0;
                                  HYDSENSOR[21]->setBrush( Qt::gray);
                                  ChannelLinkStatus[0][21]=0;
                                  HYDSENSOR[22]->setBrush( Qt::gray);
                                  ChannelLinkStatus[0][22]=0;
                                  HYDSENSOR[24]->setBrush( Qt::gray);
                                  ChannelLinkStatus[0][24]=0;

                        ACCSENSOR[23]->setBrush( Qt::gray);
                        ACCSENSOR[25]->setBrush( Qt::gray);
                        ACCSENSOR[26]->setBrush( Qt::gray);
                }
                        break;

    case 9:       if(STATUS==1)
                  {

                          HYDSENSOR[18]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][18]=1;
                          HYDSENSOR[23]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][23]=1;
                          HYDSENSOR[25]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][25]=1;
                          HYDSENSOR[26]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][26]=1;
                          HYDSENSOR[27]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][27]=1;
                          HYDSENSOR[28]->setBrush( Qt::yellow);
                          ChannelLinkStatus[0][28]=1;


                       ACCSENSOR[21]->setBrush( Qt::yellow);
                       ACCSENSOR[22]->setBrush( Qt::yellow);
                       ACCSENSOR[24]->setBrush( Qt::yellow);
                       ACCSENSOR[27]->setBrush( Qt::yellow);

                          }
                else
                     {

                              HYDSENSOR[18]->setBrush( Qt::gray);
                              ChannelLinkStatus[0][18]=0;
                              HYDSENSOR[23]->setBrush( Qt::gray);
                              ChannelLinkStatus[0][23]=0;
                              HYDSENSOR[25]->setBrush( Qt::gray);
                              ChannelLinkStatus[0][25]=0;
                              HYDSENSOR[26]->setBrush( Qt::gray);
                              ChannelLinkStatus[0][26]=0;
                              HYDSENSOR[27]->setBrush( Qt::gray);
                              ChannelLinkStatus[0][27]=0;
                              HYDSENSOR[28]->setBrush( Qt::gray);
                              ChannelLinkStatus[0][28]=0;

                }
                        break;


    case 10:       if(STATUS==1)
                  {
                             ACCSENSOR[28]->setBrush( Qt::yellow);
                             ChannelLinkStatus[1][28]=1;
                             ACCSENSOR[29]->setBrush( Qt::yellow);
                             ChannelLinkStatus[1][29]=1;
                             ACCSENSOR[30]->setBrush( Qt::yellow);
                             ChannelLinkStatus[1][30]=1;
                             ACCSENSOR[31]->setBrush( Qt::yellow);
                             ChannelLinkStatus[1][31]=1;
                          }
                else
                     {
                             ACCSENSOR[28]->setBrush( Qt::gray);
                             ChannelLinkStatus[1][28]=0;

                             ACCSENSOR[29]->setBrush( Qt::gray);
                             ChannelLinkStatus[1][29]=0;

                             ACCSENSOR[30]->setBrush( Qt::gray);
                             ChannelLinkStatus[1][30]=0;

                             ACCSENSOR[31]->setBrush( Qt::gray);
                             ChannelLinkStatus[1][31]=0;
                }
             break;

    default: break;

    };
}

void DisplayWindow::UpdateWaterFallGraph()
{


}

void DisplayWindow::RefreshLOFARWaterFallgraph()
{
    for(iCount=0;iCount<MainWaterfallPixMapSize;iCount++)
    {
        for(yCount=0;yCount<1100;yCount++)
        {
            graphPlot->Histroy[iCount][yCount]=0;
        }
    }
    //UpdateWaterFallGraph();

}
void DisplayWindow::RefreshIntWaterFallgraph()
{
    for(iCount=0;iCount<IntWaterfallPixMapSize;iCount++)
    {
        for(yCount=0;yCount<1024;yCount++)
        {
            graphPlot->IntHistroy[0][iCount][yCount]=0;
            graphPlot->IntHistroy[1][iCount][yCount]=0;
            graphPlot->IntHistroy[2][iCount][yCount]=0;
            graphPlot->IntHistroy[3][iCount][yCount]=0;
            graphPlot->IntHistroy[4][iCount][yCount]=0;
            graphPlot->IntHistroy[5][iCount][yCount]=0;
            graphPlot->IntSpecHistroy[0][iCount][yCount]=0;
            graphPlot->IntSpecHistroy[1][iCount][yCount]=0;
            graphPlot->IntSpecHistroy[2][iCount][yCount]=0;
            graphPlot->IntSpecHistroy[3][iCount][yCount]=0;
            graphPlot->IntSpecHistroy[4][iCount][yCount]=0;
            graphPlot->IntSpecHistroy[5][iCount][yCount]=0;
        }
    }
        //UpdateWaterFallGraph();
}

void DisplayWindow::HydrophoneSelect(int Select_ID)
{
    HydrophoneSel->setStyleSheet(QString::fromUtf8("background-color: rgb(207,179,255);\n"
                                                   "color: rgb(0, 0, 0);"));
    AccelerometerSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,235);\n"
                                                      "color: rgb(0, 0, 0);"));
    SensorGroupEnable=false;
    MGroupEnable=false;
    switch(Select_ID)
    {
    case 0:  CurrentChannelSel=0;
             break;
    case 1:  CurrentChannelSel=1;
             break;
    case 2:  CurrentChannelSel=2;
             break;
    case 3:  CurrentChannelSel=3;
             break;
    case 4:  CurrentChannelSel=4;
             break;
    case 5:  CurrentChannelSel=5;
             break;
    case 6:  CurrentChannelSel=6;
             break;
    case 7:  CurrentChannelSel=7;
             break;
    case 8:  CurrentChannelSel=8;
             break;
    case 9:  CurrentChannelSel=9;
             break;
    case 10:  CurrentChannelSel=10;
             break;
    case 11:  CurrentChannelSel=11;
             break;
    case 12:  CurrentChannelSel=12;
             break;
    case 13:  CurrentChannelSel=13;
             break;
    case 14:  CurrentChannelSel=14;
             break;
    case 15:  CurrentChannelSel=15;
             break;
    case 16:  CurrentChannelSel=16;
             break;
    case 17:  CurrentChannelSel=17;
             break;
    case 18:  CurrentChannelSel=18;
             break;
    case 19:  CurrentChannelSel=19;
             break;
    case 20:  CurrentChannelSel=20;
             break;
    case 21:  CurrentChannelSel=21;
             break;
    case 22:  CurrentChannelSel=22;
             break;
    case 23:  CurrentChannelSel=23;
             break;
    case 24:  CurrentChannelSel=24;
             break;
    case 25:  CurrentChannelSel=25;
             break;
    case 26:  CurrentChannelSel=26;
             break;
    case 27:  CurrentChannelSel=27;
             break;
    case 28:  CurrentChannelSel=28;
             break;
    default: break;
    };

ChannelSelected=CurrentChannelSel+1;
SpectrumCHChange[CurrentChannelSel]=true;

ThresholdSet->CurrentChannelNumber=CurrentChannelSel;
TrackWindowSet->CurrentChannel=CurrentChannelSel;

SenserRefresh=true;
UpdateSpectrumDisplay();
checkGain();

}

void DisplayWindow::AccelerometerSelect(int Select_ID)
{
    HydrophoneSel->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                   "color: rgb(0, 0, 0);"));
    AccelerometerSel->setStyleSheet(QString::fromUtf8("background-color: rgb(207,179,255);\n"
                                                      "color: rgb(0, 0, 0);"));
    SensorGroupEnable=false;
    MGroupEnable=false;
    switch(Select_ID)
    {
    case 0:  CurrentChannelSel=29;
             break;
    case 1:  CurrentChannelSel=30;
             break;
    case 2:  CurrentChannelSel=31;
             break;
    case 3:  CurrentChannelSel=32;
             break;
    case 4:  CurrentChannelSel=33;
             break;
    case 5:  CurrentChannelSel=34;
             break;
    case 6:  CurrentChannelSel=35;
             break;
    case 7:  CurrentChannelSel=36;
             break;
    case 8:  CurrentChannelSel=37;
             break;
    case 9:  CurrentChannelSel=38;
             break;
    case 10:  CurrentChannelSel=39;
             break;
    case 11:  CurrentChannelSel=40;
             break;
    case 12:  CurrentChannelSel=41;
             break;
    case 13:  CurrentChannelSel=42;
             break;
    case 14:  CurrentChannelSel=43;
             break;
    case 15:  CurrentChannelSel=44;
             break;
    case 16:  CurrentChannelSel=45;
             break;
    case 17:  CurrentChannelSel=46;
             break;
    case 18:  CurrentChannelSel=47;
             break;
    case 19:  CurrentChannelSel=48;
             break;
    case 20:  CurrentChannelSel=49;
             break;
    case 21:  CurrentChannelSel=50;
             break;
    case 22:  CurrentChannelSel=51;
             break;
    case 23:  CurrentChannelSel=52;
             break;
    case 24:  CurrentChannelSel=53;
             break;
    case 25:  CurrentChannelSel=54;
             break;
    case 26:  CurrentChannelSel=55;
             break;
    case 27:  CurrentChannelSel=56;
             break;
    case 28:  CurrentChannelSel=57;
             break;
    case 29:  CurrentChannelSel=58;
             break;
    case 30:  CurrentChannelSel=59;
             break;
    case 31:  CurrentChannelSel=60;
             break;

    default: break;
    };

    ChannelSelected=CurrentChannelSel+1;
    SpectrumCHChange[CurrentChannelSel]=true;

    TrackWindowSet->CurrentChannel=CurrentChannelSel;
    ThresholdSet->CurrentChannelNumber=CurrentChannelSel;

    SenserRefresh=true;
    UpdateSpectrumDisplay();
    checkGain();
}

void DisplayWindow::ThresholdCheck()
{
    for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
    {
        if(ChannelThresholdSet[0][iCount]==true )
        {
            HYDSENSOR[iCount]->setBrush(Qt::red);

            HydrophoneLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                             "color: rgb(0,255,0);"));
        }
        else if((ChannelThresholdSet[0][iCount]==false) && (ChannelLinkStatus[0][iCount]==true))
        {
            HYDSENSOR[iCount]->setBrush(Qt::yellow);
            HydrophoneLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                             "color: rgb(255,255,0);"));
        }
       else
        {
                HYDSENSOR[iCount]->setBrush(Qt::gray);

                HydrophoneLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                 "color: rgb(207,179,255);"));
                     }
    }
    for(iCount=0;iCount<ACCELEROMETER_SENSOR;iCount++)
    {
        if(ChannelThresholdSet[1][iCount]==true )
        {
            ACCSENSOR[iCount]->setBrush(Qt::red);


        }
        else if((ChannelThresholdSet[1][iCount]==false) && (ChannelLinkStatus[1][iCount]==true))
        {
            ACCSENSOR[iCount]->setBrush(Qt::yellow);
        }
       else
        {
                ACCSENSOR[iCount]->setBrush(Qt::gray);
        }


    }

        if(CurrentChannelSel < HYDROPHONE_SENSOR)
        {
            if(ChannelThresholdSet[0][CurrentChannelSel]==true)
            {
               HYDSENSOR[CurrentChannelSel]->setBrush(Qt::red);
               HydrophoneLabel[CurrentChannelSel]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                        "color: rgb(255,0,0);"));

            }
            else
            {
             HYDSENSOR[CurrentChannelSel]->setBrush(Qt::green);
             HydrophoneLabel[CurrentChannelSel]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                      "color: rgb(0,255,0);"));
            }
        }
        else
        {
            yCount=CurrentChannelSel-HYDROPHONE_SENSOR;

            if(ChannelThresholdSet[1][yCount]==true)
            {
                ACCSENSOR[yCount]->setBrush(Qt::red);
                AcceleometerLabel[yCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"                                                                           "color: rgb(255,0,0);"));
            }
            else
            {
            ACCSENSOR[yCount]->setBrush(Qt::green);
            AcceleometerLabel[yCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                                       "color: rgb(0,255,0);"));
            }
        }
}

void DisplayWindow::HydEnergyCheck()
{
    int RandomValue;
    for(int count_=0;count_<29;count_++)
    {
        RandomValue=round(10+(rand()%((60-10)+1)));
        graphPlot->HydEnergy[count_]=(float)RandomValue;
        if(count_<5)
        {
          IssInfo->PassiveEnergy[count_]=graphPlot->HydEnergy[count_];
    //printf("\n Energy Level=%f",IssInfo->PassiveEnergy[count_]);
        }
        if(count_==28)
        {
           IssInfo->txcavitationFlag=cavitationFlag;
        }
    //printf("\n count %d %f",count_,graphPlot->EnergyRefLineValue[count_]);
    }

}

void DisplayWindow::SensorGroupSelection(int Select_Group)
{

}


void DisplayWindow::SensorGroupSelect(int16_t CH_ID,bool ACC)
{

}


void DisplayWindow::showMessageBox()
{
    ThresholdMessage=new QDialog(IssInfo->ISSInfoFrame);
    ThresholdMessage->setWindowTitle("Manual Sensor Group");
    ThresholdMessage->setGeometry(QRect(200,450,300,90));
    ThresholdMessage->hide();
    ThresholdMessage->setStyleSheet("background:white");
            QFont font;
            font.setFamily(QString::fromUtf8("SansSerif"));
            font.setPointSize(11);
            font.setBold(false);
            ThesholdMessageLabel= new QLabel(ThresholdMessage);
            ThesholdMessageLabel->setGeometry(QRect(20,1,225,70));
            ThesholdMessageLabel->setObjectName(QString::fromUtf8("THRTWarning"));
            ThesholdMessageLabel->setFont(font);
            ThesholdMessageLabel->setText("         Select Any Six Sensors    ");
            messageBox1 = new QPushButton(ThresholdMessage);
            messageBox1->setStyleSheet(QString::fromUtf8("background-color:rgb(239,228,255)"));
            messageBox1->setText("OK");
            messageBox1->setGeometry(QRect(115,57,80,28));
}

void DisplayWindow::ThresholdAlarmDisplay()
{
    QString str;
    ThresholdWarningLabel->clear();
    for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
    {
        if((ChannelThresholdSet[0][iCount]==true)||(ChannelNarrowBandThresholdSet[0][iCount]==true))
        {
          HydrophoneLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                             "color: rgb(255,0,0);"));
        if(iCount<5)
        {
        switch(iCount)
        {
        case 0: ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" H1")));
                break;
        case 1: ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" H2")));
                 break;
        case 2: ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" H3")));
                break;
        case 3: ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" H4")));
                break;
        case 4: ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" H5")));
                break;
        default:break;
        };

        }
        else
        {
        ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" H%1").arg(iCount+1)));
        if(iCount==28)
        {
        cavitationFlag=true;
        }

        }
    //printf("\n Threshold OK");
        }
        ChannelThresholdSet[0][iCount]=false;
        ChannelNarrowBandThresholdSet[0][iCount]=false;
    }

    for(iCount=0;iCount<ACCELEROMETER_SENSOR;iCount++)
    {
        if((ChannelThresholdSet[1][iCount]==true)||(ChannelNarrowBandThresholdSet[1][iCount]==true))
        {
        AcceleometerLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                             "color: rgb(255,0,0);"));
        ThresholdWarningLabel->setText("<font color='red'>""THRESHOLD WARNING:    " + str.append(QString(" A%1").arg(iCount+1)));
        }
        ChannelThresholdSet[1][iCount]=false;
        ChannelNarrowBandThresholdSet[1][iCount]=false;
    }

    if(StorageCapacityValue>2400)
    {
        ThresholdWarningLabel->setText("<font color='red'>"" ALERT: FILE STORAGE OVERFLOW   ");
    }

}

void DisplayWindow::EnableChannelGroup(bool Manual)
{



}

void DisplayWindow::UpdateSonarInformation()
{
  //  IssInfo->SonarHealthRequest();
  //  IssInfo->SonarHealthStatus();
  //  IssInfo->UpdateISSInformations();
 //   IssInfo->TransmitData();
}


void DisplayWindow::SetZoomCursorPosition()
{
     switch(ZoomCursorWidth)
     {

      case 200:
                             if(ZoomBeatFreq_>1000)
                               {
                                ZoomBeatFreq_=1000;
                               }
                              if(ZoomBeatFreq_<0)
                               {
                                ZoomBeatFreq_=0;
                               }

                      LofarZoomCursorStatus=true;
                      LofarBeatFreq=ZoomBeatFreq_;
                      LofarZoomCursor=200;
                      ZoomCursourHLine->setGeometry(QRect(xPos,120,114,1));
                      ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                      ZoomCursorVLine2->setGeometry(QRect(xPos+58,115,1,5));
                      ZoomCursorVLine3->setGeometry(QRect(xPos+113,115,1,5));

                      sprintf(ZoomScale,"%0.1f",ZoomBeatFreq_);
                      graphPlot->LofarZoom_xScale_TickLabel1->setText(ZoomScale);
                      sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+100));
                      graphPlot->LofarZoom_xScale_TickLabel2->setText(ZoomScale);
                      sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+200));
                      graphPlot->LofarZoom_xScale_TickLabel3->setText(ZoomScale);


                      break;
     case 100:
                            if(ZoomBeatFreq_>1100)
                               {
                                ZoomBeatFreq_=1100;
                               }
                              if(ZoomBeatFreq_<0)
                               {
                                ZoomBeatFreq_=0;
                               }


                     LofarZoomCursorStatus=true;
                     LofarBeatFreq=ZoomBeatFreq_;
                     LofarZoomCursor=100;
                     ZoomCursourHLine->setGeometry(QRect(xPos,120,59,1));
                     ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                     ZoomCursorVLine2->setGeometry(QRect(xPos+28,115,1,5));
                     ZoomCursorVLine3->setGeometry(QRect(xPos+58,115,1,5));
                     sprintf(ZoomScale,"%0.1f",ZoomBeatFreq_);
                     graphPlot->LofarZoom_xScale_TickLabel1->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+50));
                     graphPlot->LofarZoom_xScale_TickLabel2->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+100));
                     graphPlot->LofarZoom_xScale_TickLabel3->setText(ZoomScale);

                     break;
     case 50:
                            if(ZoomBeatFreq_>1150)
                               {
                                ZoomBeatFreq_=1150;
                               }
                              if(ZoomBeatFreq_<0)
                               {
                                ZoomBeatFreq_=0;
                               }
                     LofarZoomCursorStatus=true;
                     LofarBeatFreq = ZoomBeatFreq_;
                     LofarZoomCursor = 50;

                     ZoomCursourHLine->setGeometry(QRect(xPos,120,30,1));
                     ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                     ZoomCursorVLine2->setGeometry(QRect(xPos+16,115,1,5));
                     ZoomCursorVLine3->setGeometry(QRect(xPos+29,115,1,5));
                     sprintf(ZoomScale,"%0.1f",ZoomBeatFreq_);
                     graphPlot->LofarZoom_xScale_TickLabel1->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+25));
                     graphPlot->LofarZoom_xScale_TickLabel2->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+50));
                     graphPlot->LofarZoom_xScale_TickLabel3->setText(ZoomScale);

                     break;

     case 244:

                  //  graphPlot->SpectrumZoomGraphLegend->clearGraphs();
                    CursorChangeFlage();
                    if(ZoomBeatFreq_>3756)
                    {
                       ZoomBeatFreq_=3756;
                    }
                    if(ZoomBeatFreq_<0)
                    {
                     ZoomBeatFreq_=0;
                    }

                    SpectrumZoomCursor=244;
                    SpectrumZoomCursorStatus=true;
                    SpcetrumBeatFreq=ZoomBeatFreq_;

                     ZoomCursourHLine->setGeometry(QRect(xPos,120,40,1));
                     ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                     ZoomCursorVLine2->setGeometry(QRect(xPos+20,115,1,5));
                     ZoomCursorVLine3->setGeometry(QRect(xPos+39,115,1,5));
                     sprintf(ZoomScale,"%0.1f",ZoomBeatFreq_);
                     graphPlot->LofarZoom_xScale_TickLabel1->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+122));
                     graphPlot->LofarZoom_xScale_TickLabel2->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+244));
                     graphPlot->LofarZoom_xScale_TickLabel3->setText(ZoomScale);
                     break;
    case 122:
                      CursorChangeFlage();
                     if(ZoomBeatFreq_>3878)
                      {
                         ZoomBeatFreq_=3878;
                      }
                    if(ZoomBeatFreq_<0)
                      {
                       ZoomBeatFreq_=0;
                     }

                     SpcetrumBeatFreq=ZoomBeatFreq_;
                     SpectrumZoomCursor=122;
                     SpectrumZoomCursorStatus=1;
                    ZoomCursourHLine->setGeometry(QRect(xPos,120,22,1));
                    ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                    ZoomCursorVLine2->setGeometry(QRect(xPos+11,115,1,5));
                    ZoomCursorVLine3->setGeometry(QRect(xPos+21,115,1,5));
                    sprintf(ZoomScale,"%0.1f",ZoomBeatFreq_);
                    graphPlot->LofarZoom_xScale_TickLabel1->setText(ZoomScale);
                    sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+61));
                    graphPlot->LofarZoom_xScale_TickLabel2->setText(ZoomScale);
                    sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+122));
                    graphPlot->LofarZoom_xScale_TickLabel3->setText(ZoomScale);
                    break;

     default: break;
     };

     ZoomCursourHLine->hide();
     ZoomCursorVLine1->hide();
     ZoomCursorVLine2->hide();
     ZoomCursorVLine3->hide();
}



void DisplayWindow::SetZoomCursorPositionChange(float xPos,float xValue)
{

    if(SelectedPageID==0)
    {
    if(xPos<50)
    {
        xPos=40;
    }
    if(ZoomCursorWidth==200)
    {
    if(xPos>621)
    {
        xPos=621;
    }
    }
    else if(ZoomCursorWidth==100)
    {
        if(xPos>675)
        {
            xPos=675;
        }
    }
    else
    {
        if(xPos>705)
        {
            xPos=705;
        }
    }

    }
    else
    {
        if(xPos<27)
        {
            xPos=0;
        }

        if(ZoomCursorWidth==244)
        {
        if(xPos>694)
        {
            xPos=694;
        }
        }
        else
        {
            if(xPos>713)
            {
                xPos=713;
            }
        }
    }


     switch(ZoomCursorWidth)
     {

     case 200:
                            if(xValue>1000)
                              {
                               xValue=1000;
                              }
                             if(xValue<0)
                              {
                               xValue=0;
                              }
                     LofarZoomCursor=200;
                     LofarZoomCursorStatus=true;
                     LofarBeatFreq=ZoomBeatFreq_=xValue;
                     ZoomCursourHLine->setGeometry(QRect(xPos,120,114,1));
                     ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                     ZoomCursorVLine2->setGeometry(QRect(xPos+58,115,1,5));
                     ZoomCursorVLine3->setGeometry(QRect(xPos+113,115,1,5));
                     break;
    case 100:
                           if(xValue>1100)
                              {
                               xValue=1100;
                              }
                             if(xValue<0)
                              {
                               xValue=0;
                              }

                    LofarZoomCursor=100;
                    LofarZoomCursorStatus=true;
                    LofarBeatFreq=ZoomBeatFreq_=xValue;
                    ZoomCursourHLine->setGeometry(QRect(xPos,120,59,1));
                    ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                    ZoomCursorVLine2->setGeometry(QRect(xPos+28,115,1,5));
                    ZoomCursorVLine3->setGeometry(QRect(xPos+58,115,1,5));
                    break;
    case 50:
                           if(xValue>1150)
                              {
                               xValue=1150;
                              }
                             if(xValue<0)
                              {
                               xValue=0;
                              }
                    LofarZoomCursor=50;
                    LofarZoomCursorStatus=true;
                    LofarBeatFreq=ZoomBeatFreq_=xValue;
                    ZoomCursourHLine->setGeometry(QRect(xPos,120,30,1));
                    ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                    ZoomCursorVLine2->setGeometry(QRect(xPos+16,115,1,5));
                    ZoomCursorVLine3->setGeometry(QRect(xPos+29,115,1,5));
                    break;

     case 244:
                    if(xValue>3756)
                    {
                       xValue=3756;
                    }
                    if(xValue<0)
                    {
                     xValue=0;
                    }

                    SpectrumZoomCursor = 244;
                    SpectrumZoomCursorStatus=1;
                    SpcetrumBeatFreq=ZoomBeatFreq_=xValue;
                    CursorChangeFlage();
                    ZoomCursourHLine->setGeometry(QRect(xPos,120,40,1));
                    ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                    ZoomCursorVLine2->setGeometry(QRect(xPos+20,115,1,5));
                    ZoomCursorVLine3->setGeometry(QRect(xPos+39,115,1,5));
                     sprintf(ZoomScale,"%0.1f",ZoomBeatFreq_);
                     graphPlot->LofarZoom_xScale_TickLabel1->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+122));
                     graphPlot->LofarZoom_xScale_TickLabel2->setText(ZoomScale);
                     sprintf(ZoomScale,"%0.1f",(ZoomBeatFreq_+244));
                     graphPlot->LofarZoom_xScale_TickLabel3->setText(ZoomScale);
                     break;
    case 122:
                     if(xValue>3878)
                      {
                         xValue=3878;
                      }
                    if(xValue<0)
                      {
                       xValue=0;
                     }

                    SpcetrumBeatFreq=ZoomBeatFreq_=xValue;
                     SpectrumZoomCursor=122;
                     SpectrumZoomCursorStatus=1;
                     ZoomCursourHLine->setGeometry(QRect(xPos,120,22,1));
                     ZoomCursorVLine1->setGeometry(QRect(xPos,115,1,5));
                     ZoomCursorVLine2->setGeometry(QRect(xPos+11,115,1,5));
                     ZoomCursorVLine3->setGeometry(QRect(xPos+21,115,1,5));
                    break;

     default: break;
     };
     sprintf(freqVal," %.1f Hz ",xValue);
     sprintf(ampVal," %.1f dB ",0.0);
     FreqVal->setText(freqVal);
     AmpVal->setText(ampVal);
     AmpVal->show();
     DisplayAmp->show();
     ZoomCursourHLine->show();
     ZoomCursorVLine1->show();
     ZoomCursorVLine2->show();
     ZoomCursorVLine3->show();
}


 void DisplayWindow::CursorChangeFlage()
 {
     SpectrumZoomCursorStatus=false;
   //printf("\n Iam in The zoom Refresh");
 }


void DisplayWindow::ZoomLofarCursorSet(int Select )
{
    if(Select==0)
    {
        ZoomCursorWidth = 200;
        xPos=200;
    }
    else if(Select==1)
    {
        ZoomCursorWidth = 100;
        xPos=200;
    }
    else
    {
        ZoomCursorWidth = 50;
        xPos=200;
    }
SetZoomCursorPosition();
}

void DisplayWindow::ZoomSpectrumCursorSet(int Select )
{
    if(Select==0)
    {
        ZoomCursorWidth=244;
        xPos=200;
    }
    else
    {
        ZoomCursorWidth=122;
        xPos=200;
    }
SetZoomCursorPosition();
}

void DisplayWindow::UpdateObservationData(float xFreq,float yAmp,float xMousePos,float yMousePos)
{
    if(SelectedPageID==0)
    {
    if(xFreq<0)
    {
        xFreq=0;
    }
    if (xFreq>1200)
    {
      xFreq=1200;
    }
    if(yAmp>500)
    {
        yAmp=500;
    }
    if(yAmp<0)
    {
        yAmp=0;
    }

    }
    if(SelectedPageID==1)
    {
        if(xFreq<0)
        {
            xFreq=0;
        }
        if (xFreq>4000)
        {
          xFreq=4000;
        }
        if(yAmp>500)
        {
            yAmp=500;
        }
        if(yAmp<0)
        {
            yAmp=0;
        }

    }

    if(SelectedPageID==2)
    {
    if(xFreq<0)
    {
        xFreq=0;
    }
    if (xFreq>1200)
    {
      xFreq=1200;
    }
    if(yAmp>500)
    {
        yAmp=500;
    }
    if(yAmp<0)
    {
        yAmp=0;
    }

    }

    if(SelectedPageID==3)
    {
        if(xFreq<0)
        {
            xFreq=0;
        }
        if (xFreq>4000)
        {
          xFreq=4000;
        }
        if(yAmp>500)
        {
            yAmp=500;
        }
        if(yAmp<0)
        {
            yAmp=0;
        }

    }

    if(CurrentChannelSel < 29)
    {
     ScaleCalculation(xFreq,yAmp,0,HydrophoneScale,0);
    }
    else
    {
      ScaleCalculation(xFreq,yAmp,1,AccelerometerScale,0);
    }

    ZoomBeatFreq_=xFreq;
    checkGain();
    FreqVal->setText(freqVal);
    AmpVal->setText(ampVal);
    GainVal->setText(gainVal);
    AmpVal->show();
    GainVal->show();
    DisplayAmp->show();
    DisplayGain->show();
    DisplayFreq->show();
    DisplayAmp->show();
    DisplayGain->show();
    DisplayFreq->show();
    if((xMousePos>37 && xMousePos<735) && (yMousePos>17 && yMousePos<224) )
    {
        MainDisplayVFrame->move(xMousePos,15);
        MainDisplayHFrame->move(30,yMousePos);
        if(SelectedPageID==0)
        {
            if(xPos<27)
            {
                xPos=0;
            }

        if(ZoomCursorWidth==200)
        {
        if(xPos>621)
        {
            xPos=621;
        }
        }
        else if(ZoomCursorWidth==100)
        {
            if(xPos>675)
            {
                xPos=675;
            }
        }
        else
        {
            if(xPos>705)
            {
                xPos=705;
            }
        }

        }
        else
        {
            if(xPos<27)
            {
                xPos=0;
            }

            if(ZoomCursorWidth==244)
            {
            if(xPos>694)
            {
                xPos=694;
            }
            }
            else
            {
                if(xPos>713)
                {
                    xPos=713;
                }
            }

        }

        if(CursorStatus==true)
        {
            if(SelectedPageID==0)
            {
             LofarBeatFreq=ZoomBeatFreq_;
            }
            else
            {
           SpcetrumBeatFreq=ZoomBeatFreq_;
            }
           SetZoomCursorPosition();
           CursorStatus=false;
        }

    }
}

void DisplayWindow::UpdateSpectrumDisplay()
{
    switch(SelectedPageID)
    {
    case 0: graphPlot->LofarGraphLegend->replot();
            graphPlot->LofarWaterFallLegend->replot();
            graphPlot->ShowLofarDisplay(SenserRefresh,ChannelSelected);
            if(LofarHarmonicCursorStatus == true)
            {
            DrawHarmonicCursorLines(0);
            LftCursorEnable == true;
            }
            if(LofarZoomEnable==true)
            {
            graphPlot->LofarZoomGraphLegend->replot();
            graphPlot->ShowLofarZoomDisplay();
            LofarZoomEnable=false;
            }
            UpdateThresholdDisplay(CurrentChannelSel);
            UpdateTrackDisplay();
            break;

    case 1:  graphPlot->SpectrumGraphLegend->replot();
             graphPlot->SpectrumWaterFallLegend->replot();
             graphPlot->ShowSpectrumDisplay(SenserRefresh,ChannelSelected);
             if(SpectrumHarmonicCursorStatus==true)
             {
             DrawHarmonicCursorLines(1);
             SpecCursorEnable == true;
             }
             if(SpectrumZoomEnable==true)
             {
             graphPlot->SpectrumZoomGraphLegend->replot();
             graphPlot->ShowSpectrumZoomDisplay();
             SpectrumZoomEnable=false;
             }
           //UpdateWaterFallGraph();
             break;


    case 2:   //printf("\n Iam in the LOfar PAge select////////////////////////////////");
              for(xCount=0;xCount<4;xCount++)
              {
                graphPlot->LofarINT[xCount]->replot();
                graphPlot->LofarINT[xCount]->show();
                graphPlot->ShowIntLofarDisplay(SenserRefresh,MGroup[xCount],xCount);
              }
              break;

    case 3:
                for(xCount=0;xCount<4;xCount++)
                {
                  graphPlot->SpectINT[xCount]->replot();
                  graphPlot->SpectINT[xCount]->show();
                  graphPlot->ShowIntSpectrumDisplay(SenserRefresh,MGroup[xCount],xCount);
                }
             break;


    case 4:     graphPlot->OctaveBandLegendPlot->replot();
                for(xCount=0;xCount<4;xCount++)
                {
                    if(OctaveFlag[xCount]==true)
                    {
                    graphPlot->ShowOctaveDisplay(SenserRefresh,OctaveGroup[xCount],xCount);
                    }
                }
                graphPlot->OctaveBandLegendPlot->update();
                break;

   case 5:     CheckTrackStatusFun();
               break;

  case 6:     graphPlot->RawDataDelSpectGraphLegend->replot();
              graphPlot->RawDataGraphLegend->replot();
              graphPlot->ShowDelSpectrumDisplay(SenserRefresh,ChannelSelected);
            //graphPlot->ShowSpectrumDisplay(SenserRefresh,ChannelSelected);
              if(Del_Spec_HarmonicCursorStatus == true)
              {
              DrawHarmonicCursorLines(0);
              De_Spec_CursorEnable == true;
              }
              break;
   case 7:
	      graphPlot->ShowSpecCompDisplay();
	      break;

    default:break;
    };
SenserRefresh=false;
}

void DisplayWindow::ResetFlags()
{
    cavitationFlag=false;
}

void DisplayWindow::CheckTrackStatusFun()
{

    for(iCount=0;iCount<61;iCount++)
    {
        if((Track_Data[TrackNumber].CH_NO[iCount]==1) && (Track_Data[TrackNumber].TrackEnable==true))
       {
           TrackMarkerLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
               "color: rgb(255,86,56);"));
       }
       else
       {
           TrackMarkerLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
               "color: rgb(228,225,255);"));
       }
     }
}



void DisplayWindow::DrawHistorgramFrame()
{
    HistogramFrame=new QFrame();
    HistogramFrame->setMouseTracking(true);
    HistogramFrame->setGeometry(QRect(15,5,220,600));
    HistogramFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    HistogramFrame->setFrameShape(QFrame::StyledPanel);
    HistogramFrame->setFrameShadow(QFrame::Raised);
    HistogramFrame->setStyleSheet("border: 5px solid black");
    DrawHistorgram();
}

void DisplayWindow::DrawHistorgram()
{
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setBold(false);
    font.setPointSize(10);
    font.setPixelSize(12);
HistogramView=new QGraphicsView(HistogramFrame);
HistogramScene=new QGraphicsScene(HistogramView);
HistHLayout = new QVBoxLayout(HistogramFrame);

HistHLayout->setMargin(0);
HistogramScene->setBackgroundBrush(Qt::black);
HistogramView->setRenderHints( QPainter::Antialiasing );

xCount=0;
for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
{
    zCount=0;
    for(yCount=0;yCount<6;yCount++)
    {
    HistoBar[iCount][yCount] = new QGraphicsRectItem(0,HistogramScene);
    HistoBar[iCount][yCount]->setFlag(QGraphicsRectItem::ItemIsSelectable);
    HistoBar[iCount][yCount]->setPen( QPen(QColor(230,245,255)) );
    HistoBar[iCount][yCount]->setBrush(QColor(230,245,255));
    HistoBar[iCount][yCount]->setAcceptedMouseButtons(Qt::RightButton);
//HistoBar[iCount][yCount]->setToolTip(AccelerometerName[iCount]);
    HistoBar[iCount][yCount]->setRect(120+zCount,xCount,15,8);
    HistoBar[iCount][yCount]->show();
    zCount=(zCount+16);
    }
HistgramLabel[iCount]= new QLabel(HistogramFrame);
HistgramLabel[iCount]->setGeometry(QRect(0,65+xCount,45,22));
HistgramLabel[iCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(207,179,255);"));
HistgramLabel[iCount]->setObjectName(QString::fromUtf8("THRTWarning"));
HistgramLabel[iCount]->setFont(font);
HistgramLabel[iCount]->setText("HMS1");
HistgramLabel[iCount]->show();
xCount=xCount+16;
}

HistogramView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
HistogramView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
HistogramView->setScene(HistogramScene);
HistHLayout->addWidget(HistogramView);
HistogramFrame->show();
HistogramFrame->setFrameShadow(QFrame::Raised);
HistgramLabel[0]->setText("H1");
HistgramLabel[1]->setText("H2");
HistgramLabel[2]->setText("H3");
HistgramLabel[3]->setText("H4");
HistgramLabel[4]->setText("H5");
HistgramLabel[5]->setText("H6");
HistgramLabel[6]->setText("H7");
HistgramLabel[7]->setText("H8");
HistgramLabel[8]->setText("H9");
HistgramLabel[9]->setText("H10");
HistgramLabel[10]->setText("H11");
HistgramLabel[11]->setText("H12");
HistgramLabel[12]->setText("H13");
HistgramLabel[13]->setText("H14");
HistgramLabel[14]->setText("H15");
HistgramLabel[15]->setText("H16");
HistgramLabel[16]->setText("H17");
HistgramLabel[17]->setText("H18");
HistgramLabel[18]->setText("H19");
HistgramLabel[19]->setText("H20");
HistgramLabel[20]->setText("H21");
HistgramLabel[21]->setText("H22");
HistgramLabel[22]->setText("H23");
HistgramLabel[23]->setText("H24");
HistgramLabel[24]->setText("H25");
HistgramLabel[25]->setText("H26");
HistgramLabel[26]->setText("H27");
HistgramLabel[27]->setText("H28");
HistgramLabel[28]->setText("H29");
}

void DisplayWindow::ChangeHistoThresholdValue(int16_t CH_ID,int16_t Block)
{
    HistogramCursorPos[CH_ID]=Block;
         for(zCount=0;zCount<6;zCount++)
         {
          HistoBar[CH_ID][zCount]->setBrush(QColor(230,245,255));
          HistoBar[CH_ID][zCount]->setPen( QPen(QColor(230,245,255)) );
         }
         for(zCount=0;zCount<Energy_Histo_Pos[CH_ID];zCount++)
         {
          HistoBar[CH_ID][zCount]->setBrush( Qt::green);
          HistoBar[CH_ID][zCount]->setPen( QPen(Qt::green) );
         }
         if(Energy_Histo_Pos[CH_ID]>HistogramCursorPos[CH_ID])
         {
            for(zCount=HistogramCursorPos[CH_ID];zCount<Energy_Histo_Pos[CH_ID];zCount++)
             {
             ChannelNarrowBandThresholdSet[0][CH_ID]=true;
             HistoBar[CH_ID][zCount]->setBrush( Qt::red);
             HistoBar[CH_ID][zCount]->setPen( QPen(Qt::red) );
             }
         }
         HistoBar[CH_ID][Block]->setBrush( Qt::gray);
         HistoBar[CH_ID][Block]->setPen( QPen(Qt::gray) );

  //printf("\n Iam in the %d %d",CH_ID,Block);
}

void DisplayWindow::ChangeEnergyValue(int16_t CH_ID,int16_t Block)
{

    for(zCount=0;zCount<6;zCount++)
    {
     HistoBar[CH_ID][zCount]->setBrush(QColor(230,245,255));
     HistoBar[CH_ID][zCount]->setPen( QColor(230,245,255) );
    }
    for(zCount=0;zCount<Energy_Histo_Pos[CH_ID];zCount++)
    {
     HistoBar[CH_ID][zCount]->setBrush( Qt::green);
     HistoBar[CH_ID][zCount]->setPen( QPen(Qt::green) );
    }
    if(Energy_Histo_Pos[CH_ID]>HistogramCursorPos[CH_ID])
    {
       for(zCount=HistogramCursorPos[CH_ID];zCount<Energy_Histo_Pos[CH_ID];zCount++)
        {
        ChannelNarrowBandThresholdSet[0][CH_ID]=true;
        HistoBar[CH_ID][zCount]->setBrush( Qt::red);
        HistoBar[CH_ID][zCount]->setPen( QPen(Qt::red) );
        }
    }
    HistoBar[CH_ID][HistogramCursorPos[CH_ID]]->setBrush( Qt::gray);
    HistoBar[CH_ID][HistogramCursorPos[CH_ID]]->setPen( QPen(Qt::gray) );
}

void DisplayWindow::SetThresholdCursorPos()
{
    for(xCount=0;xCount<HYDROPHONE_SENSOR;xCount++)
    {
         for(zCount=0;zCount<6;zCount++)
         {
          HistoBar[xCount][zCount]->setBrush(QColor(230,245,255));
          HistoBar[xCount][zCount]->setPen( QPen(QColor(230,245,255)));
          HistoBar[xCount][HistogramCursorPos[xCount]]->setBrush( Qt::darkGray);
          HistoBar[xCount][HistogramCursorPos[xCount]]->setPen( QPen(Qt::darkGray) );
         }
    }
//printf("\n Iam in the %d %d",CH_ID,Block);

}

void DisplayWindow::ReadEnergyFile(int SelectID)
{
    switch(SelectID)
    {
    case 0: EnergyStage=0;
            if(THRESHOLD_FILE_STATUS==1)
            {
            ThresholdFileRead=fopen("/root/ONMASYSTEM/ONMASYSTEM/ThresholdFile1.bin","r+");
            }
            else
            {
             ThresholdFileRead=fopen("ThresholdFile1.bin","r+");
            }

            fread(&ReadThreshold,sizeof(struct ThresholgSettingsRead),1,ThresholdFileRead);
            fclose(ThresholdFileRead);
             break;
    case 1: EnergyStage=1;
              if(THRESHOLD_FILE_STATUS==1)
              {
              ThresholdFileRead=fopen("/root/ONMASYSTEM/ONMASYSTEM/ThresholdFile2.bin","r+");
             }
              else
             {
              ThresholdFileRead=fopen("ThresholdFile2.bin","r+");
                 }
              fread(&ReadThreshold,sizeof(struct ThresholgSettingsRead),1,ThresholdFileRead);
             fclose(ThresholdFileRead);
             break;

    case 2: EnergyStage=2;
                if(THRESHOLD_FILE_STATUS==1)
                {
                ThresholdFileRead=fopen("/root/ONMASYSTEM/ONMASYSTEM/ThresholdFile3.bin","r+");
                }
                else
                {
                 ThresholdFileRead=fopen("ThresholdFile3.bin","r+");
                }
                fread(&ReadThreshold,sizeof(struct ThresholgSettingsRead),1,ThresholdFileRead);
                 fclose(ThresholdFileRead);
             break;
    case 3: EnergyStage=3;
                    if(THRESHOLD_FILE_STATUS==1)
                    {
                    ThresholdFileRead=fopen("/root/ONMASYSTEM/ONMASYSTEM/ThresholdFile4.bin","r+");
                    }
                    else
                    {
                     ThresholdFileRead=fopen("ThresholdFile4.bin","r+");
                    }
                    fread(&ReadThreshold,sizeof(struct ThresholgSettingsRead),1,ThresholdFileRead);
                    fclose(ThresholdFileRead);

             break;
    default: break;

    };

    for(iCount=0;iCount<29;iCount++)
    {
  //HistogramCursorPos[iCount]=ReadThreshold.HISTOGRAM_POS[iCount];
    HistogramCursorPos[iCount]=20;
   //printf("\n Read value %d %d %d",iCount,ReadThreshold.HISTOGRAM_ENG[iCount],ReadThreshold.HISTOGRAM_POS[iCount]);
     }
}

void DisplayWindow::SaveHistogramEnergy()
{
   switch(EnergyStage)
   {
   case 0:
             for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
             {
                WriteData.HISTOGRAM_ENG[iCount]=(int16_t)EnergyValue[iCount];
                WriteData.HISTOGRAM_POS[iCount]=HistogramCursorPos[iCount];
             }
                 ThresholdFileWrite=fopen("ThresholdFile1.bin","wb");
                 fwrite(&WriteData,sizeof(struct ThresholgSettingWrite),1,ThresholdFileWrite);
                 fclose(ThresholdFileWrite);
                 break;
   case 1  :
             for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
             {
                WriteData.HISTOGRAM_ENG[iCount]=(int16_t)EnergyValue[iCount];
                WriteData.HISTOGRAM_POS[iCount]=HistogramCursorPos[iCount];
             }
                 ThresholdFileWrite=fopen("ThresholdFile2.bin","wb");
                 fwrite(&WriteData,sizeof(struct ThresholgSettingWrite),1,ThresholdFileWrite);
                 fclose(ThresholdFileWrite);
                 break;
   case 2:
             for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
             {
                WriteData.HISTOGRAM_ENG[iCount]=(int16_t)EnergyValue[iCount];
                WriteData.HISTOGRAM_POS[iCount]=HistogramCursorPos[iCount];
             }
                 ThresholdFileWrite=fopen("ThresholdFile3.bin","wb");
                 fwrite(&WriteData,sizeof(struct ThresholgSettingWrite),1,ThresholdFileWrite);
                 fclose(ThresholdFileWrite);
                 break;

   case 3:
             for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
             {
                WriteData.HISTOGRAM_ENG[iCount]=(int16_t)EnergyValue[iCount];
                WriteData.HISTOGRAM_POS[iCount]=HistogramCursorPos[iCount];
             }
                 ThresholdFileWrite=fopen("ThresholdFile4.bin","wb");
                 fwrite(&WriteData,sizeof(struct ThresholgSettingWrite),1,ThresholdFileWrite);
                 fclose(ThresholdFileWrite);
                 break;
   default: break;
   };

}

void DisplayWindow::SetTimeConstant(int ID)
{
    switch(ID)
    {
    case 0: SetTimer=2000;
            TimeValue=0;
            SPECTRUM_ALPHA=0.115;
            LOFAR_ALPHA=0.207;

            break;
    case 1: SetTimer=4000;
            TimeValue=0;
            SPECTRUM_ALPHA=0.0615;
            LOFAR_ALPHA=0.116;
            break;
    case 2: SetTimer=8000;
            TimeValue=0;
            SPECTRUM_ALPHA=0.0307;
            LOFAR_ALPHA=0.0615;

            break;
    default: break;

    };
}


void DisplayWindow::DrawHarmonicCursorActive(bool Status, bool PageID)
{
    if(PageID==0)
    {
    if(Status==false)
    {
    LofarHarmonicCursorStatus=false;
    graphPlot->HarmonicCursormenu->hide();
    }
    else
    {
     LofarHarmonicCursorStatus=true;
     LftCursorEnable==false;
     graphPlot->HarmonicCursormenu->hide();
    }
    }

    else if(PageID==1)
    {
        if(Status==false)
        {
        SpectrumHarmonicCursorStatus=false;
        graphPlot->HarmonicCursormenu->hide();
        }
        else
        {
         SpectrumHarmonicCursorStatus=true;
         SpecCursorEnable==false;
         graphPlot->HarmonicCursormenu->hide();
        }
    }

 /*
    else if(PageID== 6)
    {
        if(Status==false)
        {
        Del_Spec_HarmonicCursorStatus=false;
        graphPlot->HarmonicCursormenu->hide();
        }
        else
        {
         Del_Spec_HarmonicCursorStatus=true;
         De_Spec_CursorEnable==false;
         graphPlot->HarmonicCursormenu->hide();
        }
    }
 */

}

void DisplayWindow::DrawHarmonicCursorLines(bool PageID)
{
 //free(graphPlot->HarmonicCursor);
    if(PageID == 0)
    {
    for(iCount=0;iCount<5;iCount++)
    {
    HarmonicsCal=(int16_t)(LofarHarmonicCursorPos-35);
    HarmonicsCal=HarmonicsCal*(iCount+1);
   // printf("\n Position=%d",HarmonicsCal);
    graphPlot->HarmonicCursor[iCount]= new QGraphicsRectItem(0,graphPlot->WaterFallscene);
    graphPlot->HarmonicCursor[iCount]->setRect(HarmonicsCal,2,1,220);
    graphPlot->HarmonicCursor[iCount]->show();
    }
    graphPlot->HarmonicCursor[0]->setPen( QPen(QColor(255,170,0) ));
    graphPlot->HarmonicCursor[1]->setPen( QPen(QColor(255,130,0) ));
    graphPlot->HarmonicCursor[2]->setPen( QPen(QColor(255,90,0) ));
    graphPlot->HarmonicCursor[3]->setPen( QPen(QColor(255,50,0) ));
    graphPlot->HarmonicCursor[4]->setPen( QPen(QColor(255,10,0) ));
    }

    else if(PageID == 1)
    {
        for(iCount=0;iCount<5;iCount++)
        {
        HarmonicsCal=(int16_t)(SpectrumHarmonicCursorPos-35);
        HarmonicsCal=HarmonicsCal*(iCount+1);
     // printf("\n Spectrum Position=%d",HarmonicsCal);
        graphPlot->HarmonicCursor[iCount]= new QGraphicsRectItem(0,graphPlot->SpectrumWaterFallScene);
        graphPlot->HarmonicCursor[iCount]->setRect(HarmonicsCal,2,1,220);
        graphPlot->HarmonicCursor[iCount]->show();
        }
        graphPlot->HarmonicCursor[0]->setPen( QPen(QColor(255,170,0) ));
        graphPlot->HarmonicCursor[1]->setPen( QPen(QColor(255,130,0) ));
        graphPlot->HarmonicCursor[2]->setPen( QPen(QColor(255,90,0) ));
        graphPlot->HarmonicCursor[3]->setPen( QPen(QColor(255,50,0) ));
        graphPlot->HarmonicCursor[4]->setPen( QPen(QColor(255,10,0) ));
    }
 /*
    if(PageID == 6)
    {
    for(iCount=0;iCount<5;iCount++)
    {
    HarmonicsCal=(int16_t)(Del_Spec_HarmonicCursorPos-35);
    HarmonicsCal=HarmonicsCal*(iCount+1);
 //printf("\n Position=%d",HarmonicsCal);
    graphPlot->HarmonicCursor[iCount]= new QGraphicsRectItem(0,graphPlot->WaterFallscene);
    graphPlot->HarmonicCursor[iCount]->setRect(HarmonicsCal,2,1,220);
    graphPlot->HarmonicCursor[iCount]->show();
    }
    graphPlot->HarmonicCursor[0]->setPen( QPen(QColor(255,170,0) ));
    graphPlot->HarmonicCursor[1]->setPen( QPen(QColor(255,130,0) ));
    graphPlot->HarmonicCursor[2]->setPen( QPen(QColor(255,90,0) ));
    graphPlot->HarmonicCursor[3]->setPen( QPen(QColor(255,50,0) ));
    graphPlot->HarmonicCursor[4]->setPen( QPen(QColor(255,10,0) ));
    }
    */
}

void DisplayWindow::UpdateTrackDisplay()
{
    for(xCount=0;xCount<NO_OF_TRACK;xCount++)
    {
        if(Track_Data[xCount].TrackStatus==true)
        {
            IssInfo->TrackSendStatus[xCount]=true;
            IssInfo->TrackSendFreq[xCount]=Track_Data[xCount].Frequency;
            TrackXpos=Track_Data[xCount].xPos;
            TrackYpos=Track_Data[xCount].yPos;
            graphPlot->TrackName[xCount]->move(TrackXpos-10,TrackYpos-20);
            graphPlot->LofaraMainGraphicsView[xCount]->move(TrackXpos-5,TrackYpos);
            TrackMissCount[xCount]=0;
            graphPlot->TrackName[xCount]->show();
            graphPlot->LofaraMainGraphicsView[xCount]->show();
        }
        else if((Track_Data[xCount].TrackMiss==true))
        {
            TrackXpos=Track_Data[xCount].xPos;
            TrackYpos=Track_Data[xCount].yPos;
            graphPlot->TrackName[xCount]->move(TrackXpos-10,TrackYpos-20);
            graphPlot->LofaraMainGraphicsView[xCount]->move(TrackXpos-5,TrackYpos);
            TrackMissCount[xCount]=(TrackMissCount[xCount]+1)%9;

            if(TrackMissCount[xCount]==1)
            {
             graphPlot->TrackName[xCount]->hide();
             graphPlot->LofaraMainGraphicsView[xCount]->hide();

            }
            else if(TrackMissCount[xCount]==2)
            {
             graphPlot->TrackName[xCount]->show();
             graphPlot->LofaraMainGraphicsView[xCount]->show();

            }
            else if(TrackMissCount[xCount]==3)
            {
             graphPlot->TrackName[xCount]->hide();
             graphPlot->LofaraMainGraphicsView[xCount]->hide();

            }
            else if(TrackMissCount[xCount]==4)
            {
             graphPlot->TrackName[xCount]->show();
             graphPlot->LofaraMainGraphicsView[xCount]->show();

            }
           else if (TrackMissCount[xCount]==5)
            {
             graphPlot->TrackName[xCount]->hide();
             graphPlot->LofaraMainGraphicsView[xCount]->hide();

            }
            else if (TrackMissCount[xCount]==6)
             {
              graphPlot->TrackName[xCount]->show();
              graphPlot->LofaraMainGraphicsView[xCount]->show();

             }
            else if (TrackMissCount[xCount]==7)
             {
              graphPlot->TrackName[xCount]->hide();
              graphPlot->LofaraMainGraphicsView[xCount]->hide();

             }
            else if (TrackMissCount[xCount]==8)
             {
              graphPlot->TrackName[xCount]->show();
              graphPlot->LofaraMainGraphicsView[xCount]->show();

             }

           else
            {
             Track_Data[xCount].Frequency=0.0;
             Track_Data[xCount].Amplitude=0.0;
             Track_Data[xCount].yPos=0.0;
             Track_Data[xCount].xPos=0.0;
             TrackXpos=0;
             TrackYpos=0;
             TrackMissCount[xCount]=0;
             graphPlot->TrackName[xCount]->hide();
             graphPlot->LofaraMainGraphicsView[xCount]->hide();
             IssInfo->TrackSendFreq[xCount]=0.0;
             IssInfo->TrackSendStatus[xCount]=false;
             Track_Data[xCount].TrackStatus=false;
             Track_Data[xCount].TrackMiss=false;
            }
// printf("\n Track miss status=%d",TrackMissCount[xCount]);
            }
          else
             {
               IssInfo->TrackSendStatus[xCount]=false;
               IssInfo->TrackSendFreq[xCount]=0.0;
               graphPlot->TrackName[xCount]->hide();
               graphPlot->LofaraMainGraphicsView[xCount]->hide();
             }
        }
}


void DisplayWindow::UpdateThresholdDisplay(int16_t CH_ID)
   {
    for(yCount=0;yCount<NO_OF_SENSOR;yCount++)
    {
    for(xCount=0;xCount<8;xCount++)
    {
        if((Threshold.Init_CH_No[yCount][xCount]==true))
        {
            if(yCount==CH_ID)
            {
                TrackXpos=(ThresholdSet->CH_NO_FreqPos[yCount][xCount]-7);
                TrackYpos=(ThresholdSet->CH_NO_AmpPos[yCount][xCount]);
                graphPlot->LofaraMainThresholdGraphicsView[xCount]->setGeometry(QRect(TrackXpos,TrackYpos,15,(255-ThresholdSet->CH_NO_AmpPos[yCount][xCount])));
               // graphPlot->LofaraMainThresholdGraphicsView[yCount]->resize(TrackXpos,100);
               // graphPlot->LofaraMainThresholdGraphicsView[xCount]->move(TrackXpos,TrackYpos);
                graphPlot->LofaraMainThresholdGraphicsView[xCount]->show();
                //printf("\n Track Setted for CH=%d  Seg=%d Ststus=%d  %d  %d ",yCount,xCount,Threshold.Init_CH_No[yCount][xCount],TrackXpos,TrackYpos);
            }


            if(Threshold.ThresholdWarningFlag[yCount][xCount]==true)
            {

                if(yCount < HYDROPHONE_SENSOR)
                {
                   ChannelNarrowBandThresholdSet[0][yCount]=true;
                }
                else
                {
                  ChannelNarrowBandThresholdSet[1][yCount]=true;
                }
                Threshold.ThresholdWarningFlag[yCount][xCount]=false;

            }
            else
            {

               if(yCount < HYDROPHONE_SENSOR)
                {
                  ChannelNarrowBandThresholdSet[0][yCount]=false;
                }
                else
                {
                  ChannelNarrowBandThresholdSet[1][yCount]=false;
                }

               Threshold.ThresholdWarningFlag[yCount][xCount]=false;
            }
        }

        else
        {
            if(yCount==CH_ID)
            {

            graphPlot->LofaraMainThresholdGraphicsView[xCount]->hide();
            }
        }
    }
    }

   }

void DisplayWindow::checkGain()
{

 int32_t temp,temp2,Value;
    GainValue=LinkStatus.CH_GAIN_Rx[CurrentChannelSel];

    if(GainValue==0xFFFFFFFF)
    {
        sprintf(gainVal,"%s","Invalid");
    }
    else
    {
        temp=LinkStatus.CH_GAIN_Rx[CurrentChannelSel];
        temp2=ntohl(temp<<24);

          switch(temp2)
          {
          case 82:  Value=-13;
                    CHGainFloatValue=0.2238;
                    Mgc->setCurrentIndex(0);
                    Preset->setCurrentIndex(0);
                    break;
          case 113: Value=-4;
                    CHGainFloatValue=0.6309;
                    Mgc->setCurrentIndex(1);
                    Preset->setCurrentIndex(1);
                    break;
          case 124: Value=-1;
                    CHGainFloatValue=0.8912;
                    Mgc->setCurrentIndex(2);
                    Preset->setCurrentIndex(2);
                   break;
          case 128: Value=0;
                    CHGainFloatValue=1;
                    Mgc->setCurrentIndex(3);
                    Preset->setCurrentIndex(3);
                   break;
          case 150: Value=6;
                    CHGainFloatValue=2;
                    Mgc->setCurrentIndex(4);
                    Preset->setCurrentIndex(4);
                    break;
          case 171: Value=12;
                    CHGainFloatValue=4;
                    Mgc->setCurrentIndex(5);
                    Preset->setCurrentIndex(5);
                    break;
          case 195: Value=20;
                    CHGainFloatValue=10;
                    Mgc->setCurrentIndex(6);
                    Preset->setCurrentIndex(6);
                   break;
          case 209: Value=26;
                    CHGainFloatValue=20;
                    Mgc->setCurrentIndex(7);
                    Preset->setCurrentIndex(7);
                   break;
          case 233: Value=40;
                    CHGainFloatValue=100;
                    Mgc->setCurrentIndex(8);
                    Preset->setCurrentIndex(8);
                   break;
          case 248: Value=60;
                   CHGainFloatValue=1000;
                   Mgc->setCurrentIndex(9);
                   Preset->setCurrentIndex(9);

                   break;
          default:break;           //  Graph_plt1.Hydrophone_Scale=0;
              //  printf("\n--- %d \n",NewGainVal);
          };

        sprintf(gainVal," %d dB ",Value);

        if(Value==60)
        {
        CHGaindBValue=(Value);
        }
        else
        {
            CHGaindBValue=Value;

        }
 //printf("\n %X  %X",temp2,temp3);
    }
    GainVal->setText(gainVal);
    GainVal->show();
}

void DisplayWindow::Mgc_SetCmbo(int16_t arg)
{
    int16_t SCUNo,ChannelNo;
    int32_t temp,temp2,Value;
    int16_t z=0,y=0;
    int16_t XData,YData,ZData=0x0000;
    GainModeSel=0;
  //  printf("\n SCU No=%d,ChannelNo:%d,ChannelPos=%d gainStatus=%d AccePos=%d",LinkStatus.CH_SCU_NO[CurrentChannelSel],CurrentChannelSel,LinkStatus.CH_POS_NO[CurrentChannelSel],(LinkStatus.CH_GAIN_Rx[CurrentChannelSel]),CurrentChannelSel-29);

   switch(arg)
    {
    case 0:  NewGainVal=82;
             Mgc->setCurrentIndex(0);
             Value=-13;
             CHGainFloatValue=0.2238;

              break;
    case 1:  NewGainVal=113;
             Mgc->setCurrentIndex(1);
             Value=-4;
             CHGainFloatValue=0.6309;

               // printf("\n---- %d \n",NewGainVal);
              break;
    case 2:  NewGainVal=124;
             Mgc->setCurrentIndex(2);
             Value=-1;
             CHGainFloatValue=0.8912;

             //  printf("\n----- %d \n",NewGainVal);
             break;
    case 3:  NewGainVal=128;
             Mgc->setCurrentIndex(3);
             Value=0;
             CHGainFloatValue=1;

             break;
    case 4:  NewGainVal=150;
             Mgc->setCurrentIndex(4);
             Value=6;
             CHGainFloatValue=2;

                //printf("\n--------- %d \n",NewGainVal);
             break;
    case 5:  NewGainVal=171;
             Mgc->setCurrentIndex(5);
             Value=12;
             CHGainFloatValue=4;

                //printf("\n--------- %d \n",NewGainVal);
             break;
    case 6:  NewGainVal=195;
             Mgc->setCurrentIndex(6);
             Value=20;
             CHGainFloatValue=10;

                //printf("\n--------- %d \n",NewGainVal);
             break;
    case 7:  NewGainVal=209;
             Mgc->setCurrentIndex(7);
             Value=26;
             CHGainFloatValue=20;

                //printf("\n--------- %d \n",NewGainVal);
             break;
    case 8:  NewGainVal=233;
             Mgc->setCurrentIndex(8);
             Value=40;
             CHGainFloatValue=100;

                //printf("\n--------- %d \n",NewGainVal);
             break;
    case 9:  NewGainVal=248;
             Mgc->setCurrentIndex(9);
             Value=60;
             CHGainFloatValue=1000;

                //printf("\n--------- %d \n",NewGainVal);
             break;

    default: break;
    };

   SCUNo=LinkStatus.CH_SCU_NO[CurrentChannelSel];
   if(Value==60)
   {
   CHGaindBValue=Value-10;
   }
   else
   {
       CHGaindBValue=Value;

   }
   sprintf(gainVal," %d dB ",Value);
   GainVal->setText(gainVal);
   for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)
   {
      temp=LinkStatus.CH_GAIN_SCU[SCUNo][iCount];
      temp2=LinkStatus.CH_GAIN_SCU[SCUNo][iCount];
      CH_GAIN_VAL[iCount]=ntohl(temp<<24);
      XData=ntohl(temp2<<16);
      XData=(XData &  0x00FF);
      if(XData==0xFF)
      {
        XData=1;
      }
      else
      {
        XData=0;
      }
      YData=XData<<iCount;
      ZData=ZData | YData;
 //printf("\n SCU No:%d %X  Gain Value=%X GainWord=%X ",SCUNo,ntohl(temp),CH_GAIN_VAL[iCount],CH_GAIN_WORD[iCount]);
   }
    CH_GAIN_WORD[SCUNo]=ZData;
    CH_GAIN_VAL[LinkStatus.CH_POS_NO[CurrentChannelSel]]=NewGainVal;
    GainWord=CH_GAIN_WORD[SCUNo];
   Gain_Set(SCUNo);
  //sprintf(gainVal," %d dB ",Value);
  //GainVal->setText(gainVal);
}

void DisplayWindow::Agc_SetCmbo(int16_t arg)
{
    int16_t SCUNo,ChannelNo;
    int32_t temp,temp2;
    int16_t z=0,y=0;
    int16_t XData,YData,ZData=0x0000;
    GainModeSel=1;
   switch(arg)
    {
    case 0:  NewGainVal=0x002049cd;
           //Graph_plt1.Hydrophone_Scale=0;
           //printf("\n--- %d \n",NewGainVal);
              break;

    case 1:  NewGainVal=0x00127350;
           //printf("\n---- %d \n",NewGainVal);
              break;

    case 2:  NewGainVal=0x000939a8;
           //printf("\n----- %d \n",NewGainVal);
             break;

    default: break;
    };

   SCUNo=LinkStatus.CH_SCU_NO[CurrentChannelSel];

   for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)
   {
      temp=LinkStatus.CH_GAIN_SCU[SCUNo][iCount];
      temp2=LinkStatus.CH_GAIN_SCU[SCUNo][iCount];
      CH_GAIN_VAL[iCount]=ntohl(temp<<24);
      XData=ntohl(temp2<<16);
      XData=(XData &  0x00FF);
      if(XData==0xFF)
      {
        XData=1;
      }
      else
      {
        XData=0;
      }

      YData=XData<<iCount;
      ZData=ZData | YData;

     // printf("\n SCU No:%d %X  Gain Value=%X GainWord=%X ",SCUNo,ntohl(temp),CH_GAIN_VAL[iCount],CH_GAIN_WORD[iCount]);
   }
    CH_GAIN_WORD[SCUNo]=ZData;
    CH_GAIN_VAL[LinkStatus.CH_POS_NO[CurrentChannelSel]]=NewGainVal;
    GainWord=CH_GAIN_WORD[SCUNo];

   Gain_Set(SCUNo);
 }
void DisplayWindow::Preset_SetCmbo(int16_t arg)
{
       int16_t SCUNo,ChannelNo;
       int32_t temp,temp2,Value;
       int16_t z=0,y=0;
       int16_t XData,YData,ZData=0x0000;
       GainModeSel=2;
 //printf("\n SCU No=%d,ChannelNo:%d,ChannelPos=%d gainStatus=%d AccePos=%d",LinkStatus.CH_SCU_NO[CurrentChannelSel],CurrentChannelSel,LinkStatus.CH_POS_NO[CurrentChannelSel],(LinkStatus.CH_GAIN_Rx[CurrentChannelSel]),CurrentChannelSel-29);
      switch(arg)
       {
      case 0:  NewGainVal=82;
               Mgc->setCurrentIndex(0);
               Preset->setCurrentIndex(0);
               Value=-13;
                break;
      case 1:  NewGainVal=113;
               Mgc->setCurrentIndex(1);
               Preset->setCurrentIndex(1);

               Value=-4;
                 // printf("\n---- %d \n",NewGainVal);
                break;
      case 2:  NewGainVal=124;
               Mgc->setCurrentIndex(2);
               Preset->setCurrentIndex(2);

               Value=-1;
               //  printf("\n----- %d \n",NewGainVal);
               break;
      case 3:  NewGainVal=128;
               Mgc->setCurrentIndex(3);
               Preset->setCurrentIndex(3);

               Value=0;
               break;
      case 4:  NewGainVal=150;
               Mgc->setCurrentIndex(4);
               Preset->setCurrentIndex(4);

               Value=6;
                  //printf("\n--------- %d \n",NewGainVal);
               break;
      case 5:  NewGainVal=171;
               Mgc->setCurrentIndex(5);
               Preset->setCurrentIndex(5);

               Value=12;
                  //printf("\n--------- %d \n",NewGainVal);
               break;
      case 6:  NewGainVal=195;
               Mgc->setCurrentIndex(6);
               Preset->setCurrentIndex(6);

               Value=20;
                  //printf("\n--------- %d \n",NewGainVal);
               break;
      case 7:  NewGainVal=209;
               Mgc->setCurrentIndex(7);
               Preset->setCurrentIndex(7);

               Value=26;
                  //printf("\n--------- %d \n",NewGainVal);
               break;
      case 8:  NewGainVal=233;
               Mgc->setCurrentIndex(8);
               Preset->setCurrentIndex(8);

               Value=40;
                  //printf("\n--------- %d \n",NewGainVal);
               break;
      case 9:  NewGainVal=248;
               Mgc->setCurrentIndex(9);
               Preset->setCurrentIndex(9);

               Value=60;
                  //printf("\n--------- %d \n",NewGainVal);
               break;

      default: break;
       };

      SCUNo=LinkStatus.CH_SCU_NO[CurrentChannelSel];


      for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)
      {
         temp=LinkStatus.CH_GAIN_SCU[SCUNo][iCount];
         temp2=LinkStatus.CH_GAIN_SCU[SCUNo][iCount];
         CH_GAIN_VAL[iCount]=ntohl(temp<<24);
         XData=ntohl(temp2<<16);
         XData=(XData &  0x00FF);
         if(XData==0xFF)
         {
           XData=1;
         }
         else
         {
           XData=0;
         }

         YData=XData<<iCount;
         ZData=ZData | YData;

//printf("\n SCU No:%d %X  Gain Value=%X GainWord=%X ",SCUNo,ntohl(temp),CH_GAIN_VAL[iCount],CH_GAIN_WORD[iCount]);
      }
       CH_GAIN_WORD[SCUNo]=ZData;
       for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)
       {
       CH_GAIN_VAL[iCount] = NewGainVal;
       }
       GainWord=CH_GAIN_WORD[SCUNo];
       Gain_Set(SCUNo);
      // sprintf(gainVal," %d dB ",Value);
       //GainVal->setText(gainVal);
}
void DisplayWindow::Gain_Set(int16_t  SCU_ID)
{

    int16_t gain_flag1,z=0,y=0;
    char dmc_buff[1024];
    y=(int16_t)GainWord;

    if(GainModeSel==0)
    {
        z=0x0001<<LinkStatus.CH_POS_NO[CurrentChannelSel];                              //MGC FLAG Setting Operation
        gain_flag1=((y^z)&(~z));

      //  printf(" MGC Flag Settings=%X  %X \n",gain_flag1,y);
        for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)
        {
            control_object.data[iCount]=htonl(CH_GAIN_VAL[iCount]);
        }
    }
    else if(GainModeSel==1)
    {
        z=0x0001<<LinkStatus.CH_POS_NO[CurrentChannelSel];
        gain_flag1=(y | z);
    //printf(" AGC Gain Flag=%X  %X\n",gain_flag,Graph_plt1.GAIN_MODE_FLAG[SCU_ID]);     //AGC  FLAG Setting Operation
        for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)
        {

          control_object.data[iCount]=htonl(CH_GAIN_VAL[iCount]);

        }
    }
    else
    {
        gain_flag1=0x0000;
        for(iCount=0;iCount<NO_OF_CHANNEL;iCount++)                                          //PRESET  FLAG Setting Operation
        {

            control_object.data[iCount]=htonl(CH_GAIN_VAL[iCount]);

        }
    }

   SCU_ID= SCU_ID+1;

    switch(SCU_ID)
    {
case 1:
        UnicastAddress.setAddress("192.168.1.1");

        break;
case 2:
      // TransmitPack2.sin_addr.s_addr=inet_addr("192.168.1.2");
       UnicastAddress.setAddress("192.168.1.2");


           //printf("Ima in 2----------------------------------------- \n");
        break;
case 3:  UnicastAddress.setAddress("192.168.1.3");
     //  printf("Ima in 3----------------------------------------- %d \n",SCU_ID);
        break;

case 4:  UnicastAddress.setAddress("192.168.1.4");
           //printf("Ima in 4----------------------------------------- \n");
        break;

    case 5:
           UnicastAddress.setAddress("192.168.1.5");
              //  printf("Ima in 1----------------------------------------- \n");
            break;
    case 6:
           UnicastAddress.setAddress("192.168.1.6");
              // printf("Ima in 2----------------------------------------- \n");
            break;
    case 7:  UnicastAddress.setAddress("192.168.1.7");
          // printf("Ima in 3----------------------------------------- \n");
            break;

    case 8:  UnicastAddress.setAddress("192.168.1.8");
              // printf("Ima in 4----------------------------------------- \n");
            break;
    case 9: UnicastAddress.setAddress("192.168.1.9");
          // printf("Ima in 3----------------------------------------- \n");
            break;

    case 10: UnicastAddress.setAddress("192.168.1.10");
              // printf("Ima in 4----------------------------------------- \n");
            break;
    case 11: UnicastAddress.setAddress("192.168.1.11");
          // printf("Ima in 3----------------------------------------- \n");
            break;

    case 12:  UnicastAddress.setAddress("192.168.1.12");
               //printf("Ima in 4----------------------------------------- \n");
             break;

default: break;
    };


    Npol_Header_Setting();
    control_object.obj_id=htonl(0x00000000);
    control_object.obj_len=htonl(0x00000000);
    control_object.obj_valid=htonl(0x00000000);
    control_object.save_flag=0x1;
    control_object.scu_id=SCU_ID;
    control_object.agc_mgc_flag=htons(gain_flag1);
    control_object.thrs_flag_act_deact=htons(0x0000);
    control_object.mode_of_op=htons(0x0000);
    control_object.master_flag=htonl(0x0000);
    control_object.reg[0]=htonl(70);
    control_object.reg[1]=htonl(0x00000007);
    control_object.reg[2]=htonl(0x00000017);
    memcpy(dmc_buff,&control_object,sizeof(Host_Control_pkt_t));
    TransmitSocket2->writeDatagram(dmc_buff,sizeof(dmc_buff),UnicastAddress,SCU_PORT_NO);
    printf("\n Signal Transmitted");

}
void DisplayWindow::Npol_Header_Setting()
{
       control_object.npol_hdr.pkt_Length=htonl(0x1A8);
       control_object.npol_hdr.Src=htonl(0x0000001E);
       control_object.npol_hdr.Dst=htonl(0X0000002D);
       control_object.npol_hdr.Catgry=htonl(0X0000FE22);
       control_object.npol_hdr.Protocol=htonl(0X00000001);
       control_object.npol_hdr.No_of_Objs_in_pkt=htonl(0X00000001);
       control_object.npol_hdr.No_of_Pkt_Fragment=htonl(0X00000000);
       control_object.npol_hdr.Pkt_fragment_no=htonl(0X00000001);
       control_object.npol_hdr.Flag=htonl(0X0000020D);
       control_object.npol_hdr.Seq_no=htonl(0X00000000);
       control_object.npol_hdr.Pkt_header=htonl(0XAAAA5555);
       control_object.npol_hdr.Endian_Type=htonl(0X00000002);
       control_object.npol_hdr.Time_Stamp[0]=htonl(0X00000000);
       control_object.npol_hdr.Time_Stamp[1]=htonl(0X00000000);
       control_object.npol_hdr.Time_Stamp[2]=htonl(0X00000000);
       control_object.npol_hdr.Time_Stamp[3]=htonl(0X00000000);
       control_object.npol_hdr.Chk_Sum=htonl(0X00000000);
}
void DisplayWindow::UpdateTimeInfo()
{
IssInfo->Update_Time_Clock();
}



void DisplayWindow::RecordEnableFun()
{
    RecordButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255,141,133);\n"
                                                  "color: rgb(0, 0, 0);"));

    RecordProgressBar=0;
    ReplayCompFlag=0;
    ReplayStartFlag=0;
    RecordSetFlag=1;
    RecordStartFlag=1;
    ReplayProgressBar=0;
    RecRep=1;
}



void DisplayWindow::ReplayEnableFun()
{
    ReplayButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255,141,133);\n"
                                                  "color: rgb(0, 0, 0);"));
    ReplayCompFlag=1;
    RecordSetFlag=1;
    RecordStartFlag=0;
    ReplayStartFlag=1;
    ReplayProgressBar=0;
    RecRep=2;
}

void DisplayWindow::RecordTimeEnableFun()
{
    if(RecRep==1)
    {
    if(RecordStartFlag==1)
    {
        RecordButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255,141,133);\n"
                                                      "color: rgb(0, 0, 0);"));
        progressbar2->setValue(RecordProgressBar);
    }
    else
    {
        RecordButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                      "color: rgb(0, 0, 0);"));
        progressbar2->setValue(0);
        RecRep=0;
    }
    }

    if(RecRep==2)
    {
     if(ReplayCompFlag==1)
    {
        ReplayButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255,141,133);\n"
                                                   "color: rgb(0, 0, 0);"));
      progressbar2->setValue(ReplayProgressBar);
    }
    else
    {
        ReplayButton->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                      "color: rgb(0, 0, 0);"));
        progressbar2->setValue(0);
        RecRep=0;
    }
    }

}

void DisplayWindow::EnergyRecordEnableFun()
{
    for(iCount=0;iCount<29;iCount++)
    {
        HistPos[iCount]=HistogramCursorPos[iCount];
    }
    ft = fopen(RecControl.EnergyFilePath,"wb+");
    if(ft==NULL)
    {
       printf("\n Energy File Creation Error") ;
    }
    fwrite(&HistPos,sizeof(HistPos),1,ft);
    fclose(ft);
  //printf("\n Iam in Energy Save FIle %s",RecControl.EnergyFilePath);
}



void DisplayWindow::EnergyRecordReplayFun()
{
    fr=fopen(RecControl.EnergyReplayFilePath,"rb+");
    if(fr==NULL)
    {
       printf("\n FILE NOT FOUND ......") ;
    }
    else
    {
    fread(&HistPos,sizeof(HistPos),1,fr);
    fclose(fr);
    }

    for(iCount=0;iCount<29;iCount++)
    {
       HistogramCursorPos[iCount]=HistPos[iCount];
    }
   //printf("\n Iam in Energy Save FIle %s",RecControl.EnergyReplayFilePath);
}


void DisplayWindow::MemCheck()
{
    progressbar->setValue(StorageCapacityValue);

}
void DisplayWindow::TrackDataInfo(int32_t ID)
{

char Buffer[40];
if(SelectedPageID==5)
{
TrackNumber=ID;
sprintf(Buffer," %d ",TrackNumber+1);
TrackInfoLabelNo->setText(Buffer);
TrackInfoLabelNo->show();
sprintf(Buffer," %.1fHz ", Track_Data[TrackNumber].Frequency);
TrackFreqVal->setText(Buffer);
TrackFreqVal->show();

sprintf(Buffer," %.1fdB ", Track_Data[TrackNumber].Amplitude);
TrackAmpVal->setText(Buffer);
TrackAmpVal->show();

sprintf(Buffer," %d ",Track_Data[TrackNumber].INT_CH_NO+1);
TrackInitCh->setText(Buffer);
TrackInitCh->show();
}
}

void DisplayWindow::ScaleCalculation(float xFreq,float yAmp,int16_t Page_ID, int16_t SCAL_ID,int16_t Zoom_ID )
{
    float pi=6.28;
    float val;
    float y;

    if(Page_ID==0)
    {
    if(SCAL_ID==1)
    {
        InterValue=((yAmp/250)*3.3*1000);
        DispFreq=xFreq;
        DispAmp=(InterValue/CHGainFloatValue);

        sprintf(freqVal," %.1f Hz ",DispFreq);
     // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(ampVal," %.1f uV ",DispAmp-yScaleValue); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    else
    {
       // CHGaindBValue=60;
        y=(yAmp);
     //InterValue=(110-(60+(20*log10((y)/250))));
     //InterValue=(((20*log10(y))*5)-CHGaindBValue);
     //InterValue=(136.5+(20*log10(y/250))-CHGaindBValue);
     //InterValue=(186.5-CHGaindBValue+(20*log10(y/250)));
     //InterValue=186.5-(86.5+CHGaindBValue-(20*log10(y/250)));
     //fReadOut = (MIN_DB_VALUE + (DB_RANGE*Y)/SPECTRUM_AMPLITUDE_WINDOW_HEIGHT) + 20* log(fMgcFactor);

        InterValue=(87+(114-(20*log10(500/yAmp)))-63);
       // sprintf(DisplayBase->ampVal," %.1f dB ",); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        DispFreq=xFreq;
        DispAmp=(InterValue);
        sprintf(freqVal," %.1f Hz ",DispFreq);
        //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(ampVal," %.1f dB ",DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    }
    else
    {
    if(SCAL_ID==1)
    {
        InterValue=(yAmp/47.2);
        DispFreq=xFreq;
        val=pi*xFreq;
        DispAmp=((InterValue-CHGaindBValue)/pow(val,2));
        sprintf(freqVal," %.1f Hz ",DispFreq);
       //sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(ampVal," %.1f mm ",DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    else if(SCAL_ID==2)
    {
        InterValue=(yAmp/(95.4));
        DispFreq=xFreq;
        val=pi*xFreq;
        DispAmp=((InterValue-CHGaindBValue)/val);
        sprintf(freqVal," %.1f Hz ",DispFreq);
     // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(ampVal," %.1f m/sec ",DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    else
    {
        InterValue=(yAmp/(95.4));
        DispFreq=xFreq;
        DispAmp=(InterValue);
        sprintf(freqVal," %.1f Hz ",DispFreq);
     // sprintf(ampVal," %.1f dB ",(-186.5+((86.5+(60*yAmp)/250)-GainIndB))); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
        sprintf(ampVal," %.3f g ",DispAmp); //MaxDisp Size=250,Max dB for 3.3mV=-136.87.Disp Res=136.87/250=0.24 or 60dB/250;
    }
    }

}

float DisplayWindow::ScaleCalculationRet(float xFreq,float yAmp,int16_t Page_ID, int16_t SCAL_ID,int16_t Zoom_ID )
{
    float pi=6.28;
    float val;
    float y;
    //float DispAmpTmp;

    if(SCAL_ID==1)
    {
        InterValue=(yAmp/47.2);
        val=pi*xFreq;
        DispAmpTmp=((InterValue-CHGaindBValue)/pow(val,2));
    }
    else if(SCAL_ID==2)
    {
        InterValue=(yAmp/(95.4));
        val=pi*xFreq;
        DispAmpTmp=((InterValue-CHGaindBValue)/val);
    }
    else
    {
        InterValue=(yAmp/(95.4));
        DispAmpTmp=(InterValue);
    }
    return DispAmpTmp;

}




void DisplayWindow::setReplayCntFlag(){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setStyleSheet("background-color:white");
    QString strRepFileName = dialog.getOpenFileName(this,tr("Select Replay File"),"/home/", tr("All Files (*.*)")); 
    memcpy( txtReplFileNw, strRepFileName.toStdString().c_str() ,strRepFileName.size());
    ReplayContFlag = 1;
}
