#include "graphplotclass.h"
#include<qcustomplot.h>
extern ControlData Controls[NO_OF_SENSOR];
extern DelayedSpectrum_Data Del_Controls[NO_OF_SENSOR];

double SpectrumMainDataPlot[NO_OF_SENSOR][1200];
double DelSpectrumMainDataPlot[NO_OF_SENSOR][1200];
double SpectrumMainWaterfallDataPlot[NO_OF_SENSOR][1100];
int IsDelData;
double LofarWaterfalHisto[NO_OF_SENSOR][1100];
double OctaveMainDataPlot[NO_OF_SENSOR][40];

double LofarMainDataPlot[NO_OF_SENSOR][1100];
double LofarHisto[NO_OF_SENSOR][1100];
double DelLofarHisto[NO_OF_SENSOR][1100];
double OctaveHisto[NO_OF_SENSOR][40];
double LofarINTHisto[NO_OF_SENSOR][1200];

double LofarIntHisto[NO_OF_SENSOR][1100];
double LofarZoomHisto[1200];
double SpectrumZoomHisto[1200];

double SpectrumZoomDataPlot[1200];
double LofarZoomDataPlot[1200];

//------- RAW DATA DELAYED SPECTRUM ------//
double RawDataZoomHisto[1200];
//float  RawDataMainDataPlot[NO_OF_SENSOR][16384];
float  RawDataMainDataPlot[16384];
double RawDataHisto[NO_OF_SENSOR][1100];
double RawDataWaterfalHisto[NO_OF_SENSOR][1100];
//-------RAW DATA DELAYED SPECTRUM------//

class QCustomPlot;
QVector<double> xFreq(2048),yAmp(2048);
QVector<double> xDelFreq(2048),yDelAmp(2048);
QVector<double> DelRawData_X(16384),DelRawData_Y(16384);

GraphPlotClass::GraphPlotClass(QWidget *parent):QWidget(parent)
{
   LofarGraphLegend = new QCustomPlot(this);
   OctaveBandLegendPlot = new QCustomPlot(this);
   LofarZoomGraphLegend = new QCustomPlot(this);
   LofarWaterFallLegend = new QCustomPlot(this);
   SpectrumGraphLegend = new QCustomPlot(this);
   SpectrumZoomGraphLegend = new QCustomPlot(this);
   SpectrumWaterFallLegend = new QCustomPlot(this);
   DemonWaterFallLegend = new QCustomPlot(this);
   RawDataGraphLegend = new QCustomPlot(this);         //--RAW DATA--//
   RawDataDelSpectGraphLegend = new QCustomPlot(this); //--RAW DATA--//
   LofarDisplaySelect();
   SpectrumDisplaySelect();
   RawDataDisplaySelect();
   DrawINTLOFARWaterFallPoints();
   DrawINTSPECWaterFallPoints();
   DrawOctaveEnergyBandPlot();
   SetHarmonicWaterfallCursor();
   GDyn = 1;
   LofDyn = 1;
   SpecDyn = 1;
}


void GraphPlotClass::LofarDisplaySelect()
{
    DrawGraphWidget(LofarGraphLegend,30,10,LOFAR_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
    DrawThresholdFinFrame();
    DrawTrackFinFrame();
    DrawMainXScale(LofarGraphLegend);
    DrawMainYScale(LofarGraphLegend);
    DrawGraphWidget(LofarWaterFallLegend,30,170,LOFAR_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
    LofarWaterFallLegend->xAxis->setTickLabelColor(Qt::green);
    LofarWaterFallLegend->xAxis->setTicks(false);
    LofarWaterFallLegend->yAxis->ticker()->setTickCount(1);
    DrawGraphWidget(LofarZoomGraphLegend,30,330,420,LOFAR_yGRAPH_RANGE,0,-1);
  //DrawGraphWidget(LofarZoomGraphLegend,170,285,420,LOFAR_yGRAPH_RANGE);
    LofarGraphLegend->xAxis->setRange(0,1200);
    LofarGraphLegend->xAxis->setVisible(true);
    LofarGraphLegend->xAxis->setBasePen(QPen(Qt::white));
    LofarGraphLegend->xAxis->setTickPen(QPen(Qt::white));
    LofarGraphLegend->xAxis->setSubTickPen(QPen(Qt::white));
    LofarGraphLegend->xAxis->setTickLabelColor(Qt::white);
    LofarGraphLegend->xAxis->ticker()->setTickCount(10);
    LofarGraphLegend->xAxis->setTicks(true);
    LofarGraphLegend->xAxis->grid()->setPen(QPen(QColor(30, 30, 30)));
    LofarGraphLegend->setBackground(Qt::black);
    LofarGraphLegend->show();
    LofarGraphLegend->xAxis->setTickLabelColor(Qt::white);
    LofarGraphLegend->xAxis->setVisible(true);
    LofarGraphLegend->xAxis->setTicks(true);
    LofarGraphLegend->xAxis->setTickLabels(true);
    LofarGraphLegend->setInteraction(QCP::iRangeDrag, true);
    LofarGraphLegend->setInteraction(QCP::iRangeZoom, true);;
    LofarGraphLegend->axisRect()->setRangeDrag(Qt::Horizontal);
    LofarGraphLegend->axisRect()->setRangeZoom(Qt::Horizontal);
    DrawZoomMainXScale(LofarZoomGraphLegend);
    DrawZoomMainYScale(LofarZoomGraphLegend);
    DrawLofarZoomScaleLabel() ;
    setWaterfallcolortable();
    DrawLofarWaterFallPoints();
}

void GraphPlotClass::SpectrumDisplaySelect()
{
    DrawGraphWidget(SpectrumGraphLegend,30,10,SPECTRUM_xGRAPH_RANGE,SPECTRUM_yGRAPH_RANGE,0,-1);
    DrawGraphWidget(SpectrumWaterFallLegend,30,170,SPECTRUM_xGRAPH_RANGE,SPECTRUM_yGRAPH_RANGE,0,-1);
    SpectrumWaterFallLegend->xAxis->setTickLabelColor(Qt::black);
    SpectrumWaterFallLegend->xAxis->setTicks(false);
    SpectrumWaterFallLegend->yAxis->ticker()->setTickCount(1);
    DrawGraphWidget(SpectrumZoomGraphLegend,30,330,512,SPECTRUM_yGRAPH_RANGE,0,-1);
    SpectrumGraphLegend->xAxis->setRange(0,4000);
    SpectrumGraphLegend->xAxis->setVisible(true);
    SpectrumGraphLegend->xAxis->setBasePen(QPen(Qt::white));
    SpectrumGraphLegend->xAxis->setTickPen(QPen(Qt::white));
    SpectrumGraphLegend->xAxis->setSubTickPen(QPen(Qt::white));
    SpectrumGraphLegend->xAxis->setTickLabelColor(Qt::white);
    SpectrumGraphLegend->xAxis->ticker()->setTickCount(10);
    SpectrumGraphLegend->xAxis->setTicks(true);
    SpectrumGraphLegend->xAxis->grid()->setPen(QPen(QColor(30, 30, 30)));
    SpectrumGraphLegend->setBackground(Qt::black);
    SpectrumGraphLegend->show();
    SpectrumGraphLegend->xAxis->setTickLabelColor(Qt::white);
    SpectrumGraphLegend->xAxis->setVisible(true);
    SpectrumGraphLegend->xAxis->setTicks(true);
    SpectrumGraphLegend->xAxis->setTickLabels(true);
    SpectrumGraphLegend->setInteraction(QCP::iRangeDrag, true);
    SpectrumGraphLegend->setInteraction(QCP::iRangeZoom, true);;
    SpectrumGraphLegend->axisRect()->setRangeDrag(Qt::Horizontal);
    SpectrumGraphLegend->axisRect()->setRangeZoom(Qt::Horizontal);

    SpectrumWaterFallLegend->xAxis->setRange(0,4000);
  //DrawGraphWidget(SpectrumZoomGraphLegend,170,285,512,SPECTRUM_yGRAPH_RANGE);
    DrawSpectrumWaterFallPoints();
}

void GraphPlotClass::RawDataDisplaySelect()
{
  RawDataDelSpectGraphLegend->setGeometry(QRect(50,28,750,300));
  RawDataDelSpectGraphLegend->addGraph();
  RawDataDelSpectGraphLegend->graph(0)->setPen(QPen(Qt::green));
  RawDataDelSpectGraphLegend->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));
  RawDataDelSpectGraphLegend->xAxis->setVisible(true);
  RawDataDelSpectGraphLegend->xAxis->setBasePen(QPen(Qt::white));
  RawDataDelSpectGraphLegend->xAxis->setTickPen(QPen(Qt::white));
  RawDataDelSpectGraphLegend->xAxis->setSubTickPen(QPen(Qt::white));
  RawDataDelSpectGraphLegend->xAxis->setTickLabelColor(Qt::white);
  RawDataDelSpectGraphLegend->xAxis->ticker()->setTickCount(10);
  RawDataDelSpectGraphLegend->xAxis->setTicks(true);
  RawDataDelSpectGraphLegend->xAxis->grid()->setPen(QPen(QColor(30, 30, 30)));
  RawDataDelSpectGraphLegend->setBackground(Qt::black);
  RawDataDelSpectGraphLegend->yAxis->setTickPen(QPen(Qt::white));
  RawDataDelSpectGraphLegend->yAxis->setTickLabelColor(Qt::white);
  RawDataDelSpectGraphLegend->yAxis->setSubTickPen(QPen(Qt::white));
  RawDataDelSpectGraphLegend->yAxis->setBasePen(QPen(QColor(255,255,255)));
  RawDataDelSpectGraphLegend->yAxis->ticker()->setTickCount(10);
  RawDataDelSpectGraphLegend->yAxis->setSubTicks(true);
  RawDataDelSpectGraphLegend->yAxis->setTicks(true);
  RawDataDelSpectGraphLegend->yAxis->grid()->setPen(QPen(QColor(30, 30, 30)));
  RawDataDelSpectGraphLegend->show();

  RawDataDelSpectGraphLegend->yAxis->setRange(0,400);
  RawDataDelSpectGraphLegend->xAxis->setRange(0,4000);
  RawDataDelSpectGraphLegend->xAxis->setTickLabelColor(Qt::white);
  RawDataDelSpectGraphLegend->xAxis->setVisible(true);
  RawDataDelSpectGraphLegend->xAxis->setTicks(true);
  RawDataDelSpectGraphLegend->xAxis->setTickLabels(true);
  RawDataDelSpectGraphLegend->setInteraction(QCP::iRangeDrag, true);
  RawDataDelSpectGraphLegend->setInteraction(QCP::iRangeZoom, true);;
  RawDataDelSpectGraphLegend->axisRect()->setRangeDrag(Qt::Horizontal);
  RawDataDelSpectGraphLegend->axisRect()->setRangeZoom(Qt::Horizontal);
  RawDataGraphLegend->hide();
}


void GraphPlotClass::DrawGraphWidget(QCustomPlot *customPlot,int16_t Pos_X,int16_t Pos_Y,int16_t X_Range,int16_t Y_Range,int16_t XLW_Range,int16_t YLW_Range)
{
customPlot->setGeometry(QRect(Pos_X,Pos_Y,750,150));
customPlot->addGraph();
customPlot->graph(0)->setPen(QPen(Qt::green));
customPlot->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

customPlot->xAxis->grid()->setVisible(false);
customPlot->xAxis->setVisible(true);
customPlot->xAxis->setBasePen(QPen(Qt::white));
customPlot->xAxis->setTickPen(QPen(Qt::black, 1));
customPlot->xAxis->setSubTickPen(QPen(Qt::black, 1));
customPlot->xAxis->setTickLabelColor(Qt::black);
customPlot->xAxis->ticker()->setTickCount(1);
customPlot->xAxis->setSubTicks(false);
customPlot->xAxis->setRange(XLW_Range, X_Range);
customPlot->xAxis->setTicks(true);
customPlot->xAxis2->grid()->setVisible(false);
customPlot->xAxis2->setVisible(true);
customPlot->xAxis2->setBasePen(QPen(Qt::white));
customPlot->xAxis2->setTickPen(QPen(Qt::white, 1));
customPlot->xAxis2->setSubTickPen(QPen(Qt::white, 1));
customPlot->xAxis2->setTickLabelColor(Qt::white);
customPlot->xAxis2->ticker()->setTickCount(4);
customPlot->xAxis2->setTickLabelColor(Qt::black);
customPlot->xAxis2->setSubTicks(false);
customPlot->xAxis2->setTicks(false);
customPlot->axisRect()->setBackground(Qt::black);
customPlot->setBackground(Qt::black);
customPlot->yAxis->grid()->setVisible(false);
customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
customPlot->yAxis->setTickLabelColor(Qt::black);
customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
customPlot->yAxis->setBasePen(QPen(QColor(255,255,255)));
customPlot->yAxis->ticker()->setTickCount(1);
customPlot->yAxis->setSubTicks(false);
customPlot->yAxis->setRange(YLW_Range,Y_Range);
customPlot->yAxis->setTicks(true);
customPlot->yAxis2->grid()->setVisible(false);
customPlot->yAxis2->setVisible(true);
customPlot->yAxis2->setBasePen(QPen(Qt::white));
customPlot->yAxis2->setSubTickPen(QPen(Qt::white, 1));
customPlot->yAxis2->setSubTicks(false);
customPlot->yAxis2->setTicks(false);
customPlot->yAxis2->setTickPen(QPen(Qt::white, 1));
customPlot->yAxis2->setTickLabelColor(Qt::black);
customPlot->yAxis2->ticker()->setTickCount(2);
customPlot->show();
customPlot->replot();
}

void GraphPlotClass::DrawGraphWidget2(QCustomPlot *customPlot,int16_t Pos_X,int16_t Pos_Y,int16_t X_Range,int16_t Y_Range)
{
customPlot->setGeometry(QRect(Pos_X,Pos_Y,750,450));
customPlot->addGraph();
customPlot->graph(0)->setPen(QPen(Qt::green));
customPlot->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));
customPlot->xAxis->grid()->setVisible(false);
customPlot->xAxis->setVisible(true);
customPlot->xAxis->setBasePen(QPen(Qt::white));
customPlot->xAxis->setTickPen(QPen(Qt::black, 1));
customPlot->xAxis->setSubTickPen(QPen(Qt::black, 1));
customPlot->xAxis->setTickLabelColor(Qt::black);
customPlot->xAxis->ticker()->setTickCount(1);
customPlot->xAxis->setSubTicks(false);
//customPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
customPlot->xAxis->setRange(0,X_Range);
customPlot->xAxis->setTicks(true);
customPlot->xAxis2->grid()->setVisible(false);
//customPlot->xAxis2->setScaleType(QCPAxis::stLogarithmic);

customPlot->xAxis2->setVisible(true);
customPlot->xAxis2->setBasePen(QPen(Qt::white));
customPlot->xAxis2->setTickPen(QPen(Qt::black, 1));
customPlot->xAxis2->setSubTickPen(QPen(Qt::black, 1));
customPlot->xAxis2->setTickLabelColor(Qt::black);
customPlot->xAxis2->ticker()->setTickCount(4);
customPlot->xAxis2->setTickLabelColor(Qt::black);
customPlot->xAxis2->setSubTicks(false);
customPlot->xAxis2->setTicks(false);
customPlot->axisRect()->setBackground(Qt::black);
customPlot->setBackground(Qt::black);
customPlot->yAxis->grid()->setVisible(false);
customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
customPlot->yAxis->setTickLabelColor(Qt::black);
customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
customPlot->yAxis->setBasePen(QPen(QColor(255,255,255)));
customPlot->yAxis->ticker()->setTickCount(1);
customPlot->yAxis->setSubTicks(false);
customPlot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);

customPlot->yAxis->setRange(0,Y_Range);
customPlot->yAxis->setTicks(true);
customPlot->yAxis2->grid()->setVisible(false);
customPlot->yAxis2->setVisible(true);
customPlot->yAxis2->setBasePen(QPen(Qt::white));
customPlot->yAxis2->setSubTickPen(QPen(Qt::white, 1));
customPlot->yAxis2->setSubTicks(false);
customPlot->yAxis2->setTicks(false);
customPlot->yAxis2->setTickPen(QPen(Qt::white, 1));
customPlot->yAxis2->setTickLabelColor(Qt::black);
customPlot->yAxis2->ticker()->setTickCount(2);
customPlot->show();
customPlot->replot();

for(Count4_=0;Count4_<4;Count4_++)
{
graph1[Count4_]= customPlot->addGraph();
GraphBars[Count4_] = new QCPBars(customPlot->xAxis, customPlot->yAxis);
GraphBars[Count4_]->setAntialiased(false); // gives more crisp, pixel aligned bar borders
GraphBars[Count4_]->setStackingGap(1);
GraphBars[Count4_]->setName("Regenerative");
GraphBars[Count4_]->setPen(QPen(QColor(0, 168, 140).lighter(130)));
GraphBars[Count4_]->setBrush(QColor(0, 168, 140));
}

graph1[0]->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,QPen(Qt::green,1),QBrush(Qt::red),5));
graph1[0]->setPen(QPen(Qt::green));
graph1[1]->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot,QPen(Qt::red,1),QBrush(Qt::red),10));
graph1[1]->setPen(QPen(Qt::red));
graph1[2]->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,QPen(Qt::blue,1),QBrush(Qt::red),5));
graph1[2]->setPen(QPen(Qt::blue));
graph1[3]->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond,QPen(Qt::yellow,1),QBrush(Qt::red),5));
graph1[3]->setPen(QPen(Qt::yellow));

GraphBars[0]->setPen(QPen(QColor(0, 255,0).lighter(130)));
GraphBars[0]->setBrush(QColor(0, 255,0));

GraphBars[1]->setPen(QPen(QColor(255,0,0).lighter(130)));
GraphBars[1]->setBrush(QColor(255,0,0));
GraphBars[2]->setPen(QPen(QColor(0, 0,255).lighter(130)));
GraphBars[2]->setBrush(QColor(0, 0,255));

GraphBars[3]->setPen(QPen(QColor(255,255,0).lighter(130)));
GraphBars[3]->setBrush(QColor(255,255,0));

//graph1->setLineStyle(QCPGraph::lsLine);
//graph2=customPlot->addGraph();
//graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,QPen(Qt::black,1.5),QBrush(Qt::red),9));
//graph2->setPen(QPen(Qt::blue, 1, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin));
//graph2->setBrush(QColor(200,200,200,40));
//graph2->setChannelFillGraph(graph1);
}

void GraphPlotClass::DrawMainXScale(QCustomPlot *ScaleDraw)
{
    GraphXScaleFrame=new QFrame(ScaleDraw);
    GraphXScaleFrame->setGeometry(QRect(33,127,705,25));
    GraphXScaleFrame->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
    GraphXScaleFrame->show();
    GraphXScaleFrame->setWindowFlags(Qt::FramelessWindowHint);
    GraphXScale = new QFrame(GraphXScaleFrame);
    GraphXScale->setGeometry(QRect(6,0,697,8));
    GraphXScale->setFrameShape(QFrame::HLine);
    GraphXScale->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
    GraphXScale->setFrameShadow(QFrame::Sunken);
    GraphXScale->show();
    xTicks1=new QFrame(GraphXScaleFrame);
    xTicks1->setGeometry(QRect(2,5,8,6));
    xTicks1->setFrameShape(QFrame::VLine);
    xTicks1->setFrameShadow(QFrame::Sunken);
    xTicks1->show();
    xTicks2=new QFrame(GraphXScaleFrame);
    xTicks2->setGeometry(QRect(350,5,8,6));
    xTicks2->setFrameShape(QFrame::VLine);
    xTicks2->setFrameShadow(QFrame::Sunken);
    xTicks2->show();
    xTicks3=new QFrame(GraphXScaleFrame);
    xTicks3->setGeometry(QRect(698,5,8,6));
    xTicks3->setFrameShape(QFrame::VLine);
    xTicks3->setFrameShadow(QFrame::Sunken);
    xTicks3->show();

    Lofar_Scale_TickLabel1 = new QLabel(GraphXScaleFrame);
    Lofar_Scale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    //Lofar_Scale_TickLabel1->setGeometry(QRect(4,10,40,15));
    //Lofar_Scale_TickLabel1->setText("0");
    //Lofar_Scale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    //"color: rgb(255, 255, 255);"));
    //Lofar_Scale_TickLabel1->show();

    Lofar_Scale_TickLabel2 = new QLabel(GraphXScaleFrame);
    //Lofar_Scale_TickLabel2->setObjectName(QString::fromUtf8("label"));
    //Lofar_Scale_TickLabel2->setGeometry(QRect(337,10,40,15));
    //Lofar_Scale_TickLabel2->setText("600");
    //Lofar_Scale_TickLabel2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    //"color: rgb(255, 255, 255);"));
    //Lofar_Scale_TickLabel2->show();

    Lofar_Scale_TickLabel3 = new QLabel(GraphXScaleFrame);
    Lofar_Scale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    //Lofar_Scale_TickLabel3->setGeometry(QRect(670,10,40,15));
    //Lofar_Scale_TickLabel3->setText("1200");
    //Lofar_Scale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    //"color: rgb(255, 255, 255);"));
    //Lofar_Scale_TickLabel3->show();

    GraphXScaleFrame->show();
}
//-----------RAW DATA DELAYED SPECTRUM LABEL----------//

void GraphPlotClass::DelSpectrumXScale(QCustomPlot *ScaleDraw)
{
   //DelSpectrumXScaleFrame
    DelSpectrumXScaleFrame=new QFrame(ScaleDraw);
    DelSpectrumXScaleFrame->setGeometry(QRect(20,127,705,25));
    DelSpectrumXScaleFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    DelSpectrumXScaleFrame->show();

    GraphXScale = new QFrame(DelSpectrumXScaleFrame);
    GraphXScale->setGeometry(QRect(6,0,697,8));
    GraphXScale->setFrameShape(QFrame::HLine);
    GraphXScale->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    GraphXScale->setFrameShadow(QFrame::Sunken);
    GraphXScale->show();

    DelSpec_Scale_TickLabel1 = new QLabel(DelSpectrumXScaleFrame);
    DelSpec_Scale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    DelSpec_Scale_TickLabel1->setGeometry(QRect(4,10,40,15));
    DelSpec_Scale_TickLabel1->setText("0");
    DelSpec_Scale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    DelSpec_Scale_TickLabel1->show();

    DelSpec_Scale_TickLabel2 = new QLabel(DelSpectrumXScaleFrame);
    DelSpec_Scale_TickLabel2->setObjectName(QString::fromUtf8("label"));
    DelSpec_Scale_TickLabel2->setGeometry(QRect(337,10,40,15));
    DelSpec_Scale_TickLabel2->setText("2000");
    DelSpec_Scale_TickLabel2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    DelSpec_Scale_TickLabel2->show();

    DelSpec_Scale_TickLabel3 = new QLabel(DelSpectrumXScaleFrame);
    DelSpec_Scale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    DelSpec_Scale_TickLabel3->setGeometry(QRect(670,10,40,15));
    DelSpec_Scale_TickLabel3->setText("4000");
    DelSpec_Scale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    DelSpec_Scale_TickLabel3->show();

    DelSpectrumXScaleFrame->show();
}

void GraphPlotClass::DelSpectrumYScale(QCustomPlot *ScaleDraw)
{
    DelSpectrumYScaleFrame=new QFrame(ScaleDraw);
  //DelSpectrumYScaleFrame->setGeometry(QRect(0,10,32,150));   //--original code--//
    DelSpectrumYScaleFrame->setGeometry(QRect(0,10,25,150));
    DelSpectrumYScaleFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));

    MainVLine=new QFrame(DelSpectrumYScaleFrame);
    MainVLine->setGeometry(QRect(28,4,8,110));
    MainVLine->setFrameShape(QFrame::VLine);
    MainVLine->setFrameShadow(QFrame::Sunken);
    MainVLine->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainVLine->show();

    DelSpec_Scale_TickLabel1 = new QLabel(DelSpectrumYScaleFrame);
    DelSpec_Scale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    DelSpec_Scale_TickLabel1->setGeometry(QRect(0,0,26,15));
    DelSpec_Scale_TickLabel1->setText("140");
    DelSpec_Scale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    DelSpec_Scale_TickLabel1->show();

    DelSpec_Scale_TickLabel3 = new QLabel(DelSpectrumYScaleFrame);
    DelSpec_Scale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    DelSpec_Scale_TickLabel3->setGeometry(QRect(5,109,22,15));
    DelSpec_Scale_TickLabel3->setText("90");
    DelSpec_Scale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    DelSpec_Scale_TickLabel3->show();

    DelSpectrumYScaleFrame->show();
}


void GraphPlotClass::DrawOctaveMainYScale(QCustomPlot *ScaleDraw)
{
    OctaveHigh = new QLabel(ScaleDraw);
    OctaveHigh->setObjectName(QString::fromUtf8("label"));
    OctaveHigh->setGeometry(QRect(3,10,27,15));
    OctaveHigh->setText("200");
    OctaveHigh->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    OctaveHigh->show();

    OctaveLow = new QLabel(ScaleDraw);
    OctaveLow->setObjectName(QString::fromUtf8("label"));
    OctaveLow->setGeometry(QRect(12,420,22,15));
    OctaveLow->setText("87");
    OctaveLow->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    OctaveLow->show();
}

void GraphPlotClass::DrawOctaveMainXScale(QCustomPlot *ScaleDraw)
{
    OctaveGraphXScaleFrame=new QFrame(ScaleDraw);
    OctaveGraphXScaleFrame->setGeometry(QRect(33,427,705,25));
    OctaveGraphXScaleFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    OctaveGraphXScaleFrame->show();

    OctaveGraphXScale = new QFrame(OctaveGraphXScaleFrame);
    OctaveGraphXScale->setGeometry(QRect(6,0,697,8));
    OctaveGraphXScale->setFrameShape(QFrame::HLine);
    OctaveGraphXScale->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    OctaveGraphXScale->setFrameShadow(QFrame::Sunken);
    OctaveGraphXScale->show();

    xOctaveTicks1=new QFrame(OctaveGraphXScaleFrame);
    xOctaveTicks1->setGeometry(QRect(2,5,8,6));
    xOctaveTicks1->setFrameShape(QFrame::VLine);
    xOctaveTicks1->setFrameShadow(QFrame::Sunken);
    xOctaveTicks1->show();

    xOctaveTicks2=new QFrame(OctaveGraphXScaleFrame);
    xOctaveTicks2->setGeometry(QRect(435,5,8,6));
    xOctaveTicks2->setFrameShape(QFrame::VLine);
    xOctaveTicks2->setFrameShadow(QFrame::Sunken);
    xOctaveTicks2->show();

    xOctaveTicks3=new QFrame(OctaveGraphXScaleFrame);
    xOctaveTicks3->setGeometry(QRect(698,5,8,6));
    xOctaveTicks3->setFrameShape(QFrame::VLine);
    xOctaveTicks3->setFrameShadow(QFrame::Sunken);
    xOctaveTicks3->show();

    Octave_Scale_TickLabel1 = new QLabel(OctaveGraphXScaleFrame);
    Octave_Scale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    Octave_Scale_TickLabel1->setGeometry(QRect(4,10,40,15));
    Octave_Scale_TickLabel1->setText("10");
    Octave_Scale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    Octave_Scale_TickLabel1->show();

    Octave_Scale_TickLabel2 = new QLabel(OctaveGraphXScaleFrame);
    Octave_Scale_TickLabel2->setObjectName(QString::fromUtf8("label"));
    Octave_Scale_TickLabel2->setGeometry(QRect(415,10,40,15));
    Octave_Scale_TickLabel2->setText("1000");
    Octave_Scale_TickLabel2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    Octave_Scale_TickLabel2->show();

    Octave_Scale_TickLabel3 = new QLabel(OctaveGraphXScaleFrame);
    Octave_Scale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    Octave_Scale_TickLabel3->setGeometry(QRect(660,10,60,15));
    Octave_Scale_TickLabel3->setText("10000");
    Octave_Scale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    Octave_Scale_TickLabel3->show();

    OctaveGraphXScaleFrame->show();
}

void GraphPlotClass::DrawMainYScale(QCustomPlot *ScaleDraw)
{
    GraphYScaleFrame=new QFrame(ScaleDraw);
    GraphYScaleFrame->setGeometry(QRect(0,10,32,150));
    GraphYScaleFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    GraphYScaleFrame->show();

    MainVLine=new QFrame(GraphYScaleFrame);
    MainVLine->setGeometry(QRect(28,4,8,110));
    MainVLine->setFrameShape(QFrame::VLine);
    MainVLine->setFrameShadow(QFrame::Sunken);
    MainVLine->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainVLine->show();

    MainHLine1=new QFrame(GraphYScaleFrame);
    MainHLine1->setGeometry(QRect(26,4,5,3));
    MainHLine1->setFrameShape(QFrame::HLine);
    MainHLine1->setFrameShadow(QFrame::Sunken);
    MainHLine1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainHLine1->show();
    MainHLine2=new QFrame(GraphYScaleFrame);
    MainHLine2->setGeometry(QRect(26,57,5,3));
    MainHLine2->setFrameShape(QFrame::HLine);
    MainHLine2->setFrameShadow(QFrame::Sunken);
    MainHLine2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainHLine2->show();
    MainHLine3=new QFrame(GraphYScaleFrame);
    MainHLine3->setGeometry(QRect(26,113,5,3));
    MainHLine3->setFrameShape(QFrame::HLine);
    MainHLine3->setFrameShadow(QFrame::Sunken);
    MainHLine3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    MainHLine3->show();

    Lofar_yScale_TickLabel1 = new QLabel(GraphYScaleFrame);
    Lofar_yScale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    Lofar_yScale_TickLabel1->setGeometry(QRect(0,0,26,15));
    Lofar_yScale_TickLabel1->setText("140");
    Lofar_yScale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    Lofar_yScale_TickLabel1->show();
    Lofar_yScale_TickLabel2 = new QLabel(GraphYScaleFrame);
    Lofar_yScale_TickLabel2->setObjectName(QString::fromUtf8("label"));
    Lofar_yScale_TickLabel2->setGeometry(QRect(0,52,26,15));
    Lofar_yScale_TickLabel2->setText("125");
    Lofar_yScale_TickLabel2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    Lofar_yScale_TickLabel2->show();

    Lofar_yScale_TickLabel3 = new QLabel(GraphYScaleFrame);
    Lofar_yScale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    Lofar_yScale_TickLabel3->setGeometry(QRect(5,109,22,15));
    Lofar_yScale_TickLabel3->setText("90");
    Lofar_yScale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    Lofar_yScale_TickLabel3->show();
}



void GraphPlotClass::DrawLofarZoomScaleLabel()
{
    LofarZoom_xScale_TickLabel1 = new QLabel(zoomGraphXScaleFrame);
    LofarZoom_xScale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    LofarZoom_xScale_TickLabel1->setGeometry(QRect(4,10,50,15));
    LofarZoom_xScale_TickLabel1->setText("0");
    LofarZoom_xScale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    LofarZoom_xScale_TickLabel1->show();
    LofarZoom_xScale_TickLabel2 = new QLabel(zoomGraphXScaleFrame);
    LofarZoom_xScale_TickLabel2->setObjectName(QString::fromUtf8("label"));
    LofarZoom_xScale_TickLabel2->setGeometry(QRect(330,10,50,15));
    LofarZoom_xScale_TickLabel2->setText("100");
    LofarZoom_xScale_TickLabel2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    LofarZoom_xScale_TickLabel2->show();

    LofarZoom_xScale_TickLabel3 = new QLabel(zoomGraphXScaleFrame);
    LofarZoom_xScale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    LofarZoom_xScale_TickLabel3->setGeometry(QRect(658,10,57,15));
    LofarZoom_xScale_TickLabel3->setText("200");
    LofarZoom_xScale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
    "color: rgb(255, 255, 255);"));
    LofarZoom_xScale_TickLabel3->show();
}

void GraphPlotClass::DrawLofarWaterFallPoints()
{
    WaterFallview=new QGraphicsView(LofarWaterFallLegend);
    WaterFallview->setGeometry(QRect(35,15,700,119));
    WaterFallscene=new QGraphicsScene(LofarWaterFallLegend);
    WaterFallscene->setBackgroundBrush(Qt::black);
    WaterFallview->setRenderHints( QPainter::Antialiasing );
    WaterFallview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    WaterFallview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //  WaterFallview->NoDrag;
    WaterFallview->fitInView(0,0,WaterFallview->width(),WaterFallview->height(),Qt::KeepAspectRatioByExpanding);
    WaterFallview->setScene( WaterFallscene);
}

void GraphPlotClass::DrawSpectrumWaterFallPoints()
{
    SpectrumWaterFallview=new QGraphicsView(SpectrumWaterFallLegend);
    SpectrumWaterFallview->setGeometry(QRect(35,15,700,119));
    SpectrumWaterFallScene=new QGraphicsScene(SpectrumWaterFallLegend);
    SpectrumWaterFallScene->setBackgroundBrush(Qt::black);
    SpectrumWaterFallview->setRenderHints( QPainter::Antialiasing );
    SpectrumWaterFallview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SpectrumWaterFallview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //WaterFallview->NoDrag;
    SpectrumWaterFallview->fitInView(0,0,SpectrumWaterFallview->width(),SpectrumWaterFallview->height(),Qt::KeepAspectRatioByExpanding);
    SpectrumWaterFallview->setScene( SpectrumWaterFallScene);
}


void GraphPlotClass::setWaterfallcolortable()
{
WaterfallCount=MainWaterfallPixMapSize-1;
IntWaterfallCount=IntWaterfallPixMapSize-1;
LofarWaterFallImage=QImage(6000,MainWaterfallPixMapSize,QImage::Format_RGB444);
SpectrumWaterFallImage=QImage(6000,MainWaterfallPixMapSize,QImage::Format_RGB444);
/*
int16_t g=0,b=0,r=0,m=0;

    for(g=0;g<255;g++)
    {
    if(g<10)
    {
     colortable1.push_back(qRgb(0,0,0));
    }
    else if(g>10 && g<20 )
    {
        colortable1.push_back(qRgb(0,10,0));
    }
    else if(g>20 && g<35 )
    {
        colortable1.push_back(qRgb(0,20,0));

    }
    else if(g>35 && g<60 )
    {
        colortable1.push_back(qRgb(0,35,0));
    }
    else if(g>60 && g<100 )
    {
        colortable1.push_back(qRgb(0,60,0));
    }
    else if(g>100 && g<140 )
    {
        colortable1.push_back(qRgb(0,100,0));
    }
    else if(g>140 && g<190 )
    {
        colortable1.push_back(qRgb(0,140,0));
    }
    else if(g>190 && g<230 )
    {
        colortable1.push_back(qRgb(0,190,0));
    }
    else
    {
        colortable1.push_back(qRgb(0,250,0));
    }
    }
*/

}

void GraphPlotClass::PlotWaterFallPoints(int16_t ID,double *Freq,double *INPUT_AMPLITUDE,int16_t Data_Len,float Reslution)
{
int16_t RedVal=0;
    count3_=1;
          for(count_=WaterfallCount;count_>0;count_--)  //WaterfallCount indicates No of Pixmap-1
          {
              for(count2_=0;count2_<Data_Len;count2_++)
              {
                   Histroy[count_][count2_]=Histroy[count_-1][count2_];   // History:Shifting First data element to second position

                   if(count3_==1)
                  {
                   if(INPUT_AMPLITUDE[count2_]<20)
                       {
                      RedVal=0;
                      Min_=0;
                      Max_=13;                                           // Amplitude Peak Color Identification
                      RandomValue_=round(Min_+(rand()%((Max_-Min_)+1)));
                      Max_=RandomValue_;
                   }

                   else if((INPUT_AMPLITUDE[count2_]>20) && (INPUT_AMPLITUDE[count2_]<40) )
                       {
                        Min_=5;
                        Max_=25;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>40) && (INPUT_AMPLITUDE[count2_]<60) )
                       {
                        Min_=15;
                        Max_=30;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>60) && (INPUT_AMPLITUDE[count2_]<80) )
                       {
                        Min_=20;
                        Max_=60;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>80) && (INPUT_AMPLITUDE[count2_]<120) )
                       {
                        Min_=25;
                        Max_=80;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>120) && (INPUT_AMPLITUDE[count2_]<140) )
                       {
                        Min_=50;
                        Max_=100;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>140) && (INPUT_AMPLITUDE[count2_]<180) )
                       {
                        Min_=75;
                        Max_=120;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>180) && (INPUT_AMPLITUDE[count2_]<220) )
                       {
                        Min_=100;
                        Max_=140;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>220) && (INPUT_AMPLITUDE[count2_]<260) )
                       {
                        Min_=125;
                        Max_=160;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>300) && (INPUT_AMPLITUDE[count2_]<360) )
                       {
                        Min_=150;
                        Max_=180;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>360) && (INPUT_AMPLITUDE[count2_]<420) )
                       {
                        Min_=175;
                        Max_=200;
                       }
                   else if((INPUT_AMPLITUDE[count2_]>420) && (INPUT_AMPLITUDE[count2_]<480) )
                       {
                        Min_=200;
                        Max_=220;
                       }
                  else
                       {
                     Min_=230;
                     Max_=254;
                       }
//RandomValue_=round(Min_+(rand()%((Max_-Min_)+1)));
                    Histroy[0][count2_]=Max_;                  // Random Value Generation
                    RGBvalue = qRgb(0,Histroy[0][count2_],0);
                    ResolutionCount_=round(count2_*Reslution);
                  }
                       ResolutionCount_=(count2_*Reslution);
                       SpectrumWaterFallImage.setPixel(ResolutionCount_,count_,qRgb(0,Histroy[count_][count2_],0));      //Making History of old data
              }
      count3_=0;
          }
              pixmap = QPixmap::fromImage(SpectrumWaterFallImage);
              SpectrumWaterFallScene->clear();
              SpectrumWaterFallScene->addPixmap(pixmap);
}


void GraphPlotClass::LofarPlotWaterFallPoints(int16_t ID,double *Freq,double *INPUT_AMPLITUDE,int16_t Data_Len,float Reslution)
{
    count3_=1;
          for(count_=WaterfallCount;count_>0;count_--)  //WaterfallCount indicates No of Pixmap-1
          {
              for(count2_=0;count2_<Data_Len;count2_++)
              {
                   Histroy[count_][count2_]=Histroy[count_-1][count2_];   // History:Shifting First data element to second position

                   if(count3_==1)
                  {
                       if(INPUT_AMPLITUDE[count2_]<20)
                           {
                          Min_=0;
                          Max_=13;                                           // Amplitude Peak Color Identification
                          RandomValue_=round(Min_+(rand()%((Max_-Min_)+1)));
                          Max_=RandomValue_;
                       }

                       else if((INPUT_AMPLITUDE[count2_]>20) && (INPUT_AMPLITUDE[count2_]<40) )
                           {
                            Min_=5;
                            Max_=25;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>40) && (INPUT_AMPLITUDE[count2_]<60) )
                           {
                            Min_=15;
                            Max_=30;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>60) && (INPUT_AMPLITUDE[count2_]<80) )
                           {
                            Min_=20;
                            Max_=60;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>80) && (INPUT_AMPLITUDE[count2_]<120) )
                           {
                            Min_=25;
                            Max_=80;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>120) && (INPUT_AMPLITUDE[count2_]<140) )
                           {
                            Min_=50;
                            Max_=100;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>140) && (INPUT_AMPLITUDE[count2_]<180) )
                           {
                            Min_=75;
                            Max_=120;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>180) && (INPUT_AMPLITUDE[count2_]<220) )
                           {
                            Min_=100;
                            Max_=140;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>220) && (INPUT_AMPLITUDE[count2_]<260) )
                           {
                            Min_=125;
                            Max_=160;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>300) && (INPUT_AMPLITUDE[count2_]<360) )
                           {
                            Min_=150;
                            Max_=180;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>360) && (INPUT_AMPLITUDE[count2_]<420) )
                           {
                            Min_=175;
                            Max_=200;
                           }
                       else if((INPUT_AMPLITUDE[count2_]>420) && (INPUT_AMPLITUDE[count2_]<480) )
                           {
                            Min_=200;
                            Max_=220;
                           }
                      else
                           {
                         Min_=230;
                         Max_=254;
                           }
     //RandomValue_=round(Min_+(rand()%((Max_-Min_)+1)));
                    Histroy[0][count2_]=Max_;                  //Random Value Generation
                    RGBvalue = qRgb(0,Histroy[0][count2_],0);
      //ResolutionCount_=round(count2_*Reslution);
                  }
                   ResolutionCount_=(count2_*Reslution);
                   LofarWaterFallImage.setPixel(ResolutionCount_,count_,qRgb(0,Histroy[count_][count2_],0));      //Making History of old data
                   ResolutionCount_=ResolutionCount_+1;
                   LofarWaterFallImage.setPixel(ResolutionCount_,count_,qRgb(0,Histroy[count_][count2_],0));
                   ResolutionCount_=ResolutionCount_+1;
                   LofarWaterFallImage.setPixel(ResolutionCount_,count_,qRgb(0,Histroy[count_][count2_],0));
                   ResolutionCount_=ResolutionCount_+1;
                   LofarWaterFallImage.setPixel(ResolutionCount_,count_,qRgb(0,Histroy[count_][count2_],0));
                   ResolutionCount_=ResolutionCount_+1;
              }

      count3_=0;
          }
         pixmap = QPixmap::fromImage(LofarWaterFallImage);
         WaterFallscene->clear();
         WaterFallscene->addPixmap(pixmap);
}



void GraphPlotClass::DrawINTLOFARWaterFallPoints()
{

for(count_=0;count_<4;count_++)
    {

    LofarINT[count_]=new QCustomPlot(this);
    LofarINT[count_]->xAxis->setRange(0,1200);
    LofarINT[count_]->yAxis->setRange(0,500);
  /*  LFTINTWaterFallview[count_]=new QGraphicsView();
    LFTINTWaterFallview[count_]->setGeometry(QRect(39,15,946,219));
    LFTINTWaterFallscene[count_]=new QGraphicsScene();
    LFTINTWaterFallscene[count_]->setBackgroundBrush(Qt::black);
    LFTINTWaterFallview[count_]->setRenderHints( QPainter::Antialiasing );
    LFTINTWaterFallview[count_]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    LFTINTWaterFallview[count_]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //  LFTINTWaterFallview[count_]->NoDrag;
    LFTINTWaterFallview[count_]->fitInView(0,0,LFTINTWaterFallview[count_]->width(),LFTINTWaterFallview[count_]->height(),Qt::KeepAspectRatioByExpanding);
    LFTINTWaterFallview[count_]->setScene( LFTINTWaterFallscene[count_]);
    }
    LFTINTWaterFallview[0]->setGeometry(QRect(0,0,790,140));
    LFTINTWaterFallview[1]->setGeometry(QRect(0,140,790,140));
    LFTINTWaterFallview[2]->setGeometry(QRect(0,280,790,140));
    LFTINTWaterFallview[3]->setGeometry(QRect(0,420,790,140));
    LFTINTWaterFallview[4]->setGeometry(QRect(0,560,790,140));
    LFTINTWaterFallview[5]->setGeometry(QRect(0,700,790,140));
    */
}

DrawGraphWidget(LofarINT[0],30,5,LOFAR_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
DrawGraphWidget(LofarINT[1],30,130,LOFAR_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
DrawGraphWidget(LofarINT[2],30,255,LOFAR_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
DrawGraphWidget(LofarINT[3],30,380,LOFAR_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
LofarINT[0]->setGeometry(QRect(10,3,750,145));
LofarINT[1]->setGeometry(QRect(10,124,750,145));
LofarINT[2]->setGeometry(QRect(10,249,750,145));
LofarINT[3]->setGeometry(QRect(10,374,750,145));

 LofarINT[0]->graph(0)->setPen(QPen(Qt::green));
 LofarINT[0]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

 LofarINT[1]->graph(0)->setPen(QPen(Qt::red));
 LofarINT[1]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

 LofarINT[2]->graph(0)->setPen(QPen(Qt::blue));
 LofarINT[2]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

 LofarINT[3]->graph(0)->setPen(QPen(Qt::yellow));
 LofarINT[3]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));
}


void GraphPlotClass::DrawINTSPECWaterFallPoints()
{
for(count_=0;count_<4;count_++)
    {
    SpectINT[count_]=new QCustomPlot(this);
    SpectINT[count_]->xAxis->setRange(0,4000);
    SpectINT[count_]->yAxis->setRange(0,500);

 /* LFTINTSpecWaterFallview[count_]=new QGraphicsView();
    LFTINTSpecWaterFallview[count_]->setGeometry(QRect(39,15,946,219));
    LFTINTSpecWaterFallscene[count_]=new QGraphicsScene();
    LFTINTSpecWaterFallscene[count_]->setBackgroundBrush(Qt::black);
    LFTINTSpecWaterFallview[count_]->setRenderHints( QPainter::Antialiasing );
    LFTINTSpecWaterFallview[count_]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    LFTINTSpecWaterFallview[count_]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//  LFTINTWaterFallview[count_]->NoDrag;
    LFTINTSpecWaterFallview[count_]->fitInView(0,0,LFTINTSpecWaterFallview[count_]->width(),LFTINTSpecWaterFallview[count_]->height(),Qt::KeepAspectRatioByExpanding);
    LFTINTSpecWaterFallview[count_]->setScene( LFTINTSpecWaterFallscene[count_]);
    }
    LFTINTSpecWaterFallview[0]->setGeometry(QRect(0,0,790,140));
    LFTINTSpecWaterFallview[1]->setGeometry(QRect(0,140,790,140));
    LFTINTSpecWaterFallview[2]->setGeometry(QRect(0,280,790,140));
    LFTINTSpecWaterFallview[3]->setGeometry(QRect(0,420,790,140));
    LFTINTSpecWaterFallview[4]->setGeometry(QRect(0,560,790,140));
    LFTINTSpecWaterFallview[5]->setGeometry(QRect(0,700,790,140));
*/
}

DrawGraphWidget(SpectINT[0],30,5,SPECTRUM_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
DrawGraphWidget(SpectINT[1],30,130,SPECTRUM_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
DrawGraphWidget(SpectINT[2],30,255,SPECTRUM_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
DrawGraphWidget(SpectINT[3],30,380,SPECTRUM_xGRAPH_RANGE,LOFAR_yGRAPH_RANGE,0,-1);
SpectINT[0]->setGeometry(QRect(10,3,750,145));
SpectINT[1]->setGeometry(QRect(10,124,750,145));
SpectINT[2]->setGeometry(QRect(10,249,750,145));
SpectINT[3]->setGeometry(QRect(10,374,750,145));

SpectINT[0]->graph(0)->setPen(QPen(Qt::green));
SpectINT[0]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

SpectINT[1]->graph(0)->setPen(QPen(Qt::red));
SpectINT[1]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

SpectINT[2]->graph(0)->setPen(QPen(Qt::blue));
SpectINT[2]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));

SpectINT[3]->graph(0)->setPen(QPen(Qt::yellow));
SpectINT[3]->graph(0)->setBrush(QBrush(QColor(0,0,0,20)));
}

void GraphPlotClass::DrawOctaveEnergyBandPlot()
{
    DrawGraphWidget2(OctaveBandLegendPlot,30,10,4200,500);
    OctaveBandLegendPlot->xAxis->setRange(1,30);
    DrawOctaveMainXScale(OctaveBandLegendPlot) ;
    DrawOctaveMainYScale(OctaveBandLegendPlot) ;
}

void GraphPlotClass::DrawZoomMainXScale(QCustomPlot *ScaleDraw)
{
    zoomGraphXScaleFrame=new QFrame(ScaleDraw);
    zoomGraphXScaleFrame->setGeometry(QRect(33,127,705,25));
    zoomGraphXScaleFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomGraphXScaleFrame->show();
    zoomGraphXScale = new QFrame(zoomGraphXScaleFrame);
    zoomGraphXScale->setGeometry(QRect(6,0,697,8));
    zoomGraphXScale->setFrameShape(QFrame::HLine);
    zoomGraphXScale->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomGraphXScale->setFrameShadow(QFrame::Sunken);
    zoomGraphXScale->show();
    ZoomxTicks1=new QFrame(zoomGraphXScaleFrame);
    ZoomxTicks1->setGeometry(QRect(2,5,8,6));
    ZoomxTicks1->setFrameShape(QFrame::VLine);
    ZoomxTicks1->setFrameShadow(QFrame::Sunken);
    ZoomxTicks1->show();
    ZoomxTicks2=new QFrame(zoomGraphXScaleFrame);
    ZoomxTicks2->setGeometry(QRect(345,5,8,6));
    ZoomxTicks2->setFrameShape(QFrame::VLine);
    ZoomxTicks2->setFrameShadow(QFrame::Sunken);
    ZoomxTicks2->show();
    ZoomxTicks3=new QFrame(zoomGraphXScaleFrame);
    ZoomxTicks3->setGeometry(QRect(698,5,8,6));
    ZoomxTicks3->setFrameShape(QFrame::VLine);
    ZoomxTicks3->setFrameShadow(QFrame::Sunken);
    ZoomxTicks3->show();
    zoomGraphXScaleFrame->show();
}

void GraphPlotClass::DrawZoomMainYScale(QCustomPlot *ScaleDraw)
{
    zoomGraphYScaleFrame=new QFrame(ScaleDraw);
    zoomGraphYScaleFrame->setGeometry(QRect(0,10,32,150));
    zoomGraphYScaleFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomGraphYScaleFrame->show();

    zoomMainVLine=new QFrame(zoomGraphYScaleFrame);
    zoomMainVLine->setGeometry(QRect(28,4,8,110));
    zoomMainVLine->setFrameShape(QFrame::VLine);
    zoomMainVLine->setFrameShadow(QFrame::Sunken);
    zoomMainVLine->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomMainVLine->show();

    zoomMainHLine1=new QFrame(zoomGraphYScaleFrame);
    zoomMainHLine1->setGeometry(QRect(26,4,5,3));
    zoomMainHLine1->setFrameShape(QFrame::HLine);
    zoomMainHLine1->setFrameShadow(QFrame::Sunken);
    zoomMainHLine1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomMainHLine1->show();
    zoomMainHLine2=new QFrame(zoomGraphYScaleFrame);
    zoomMainHLine2->setGeometry(QRect(26,57,5,3));
    zoomMainHLine2->setFrameShape(QFrame::HLine);
    zoomMainHLine2->setFrameShadow(QFrame::Sunken);
    zoomMainHLine2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomMainHLine2->show();
    zoomMainHLine3=new QFrame(zoomGraphYScaleFrame);
    zoomMainHLine3->setGeometry(QRect(26,113,5,3));
    zoomMainHLine3->setFrameShape(QFrame::HLine);
    zoomMainHLine3->setFrameShadow(QFrame::Sunken);
    zoomMainHLine3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);"));
    zoomMainHLine3->show();

    zoomLofar_yScale_TickLabel1 = new QLabel(zoomGraphYScaleFrame);
    zoomLofar_yScale_TickLabel1->setObjectName(QString::fromUtf8("label"));
    zoomLofar_yScale_TickLabel1->setGeometry(QRect(0,0,26,15));
    zoomLofar_yScale_TickLabel1->setText("140");
    zoomLofar_yScale_TickLabel1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    zoomLofar_yScale_TickLabel1->show();
    zoomLofar_yScale_TickLabel2 = new QLabel(zoomGraphYScaleFrame);
    zoomLofar_yScale_TickLabel2->setObjectName(QString::fromUtf8("label"));
    zoomLofar_yScale_TickLabel2->setGeometry(QRect(0,52,2,15));
    zoomLofar_yScale_TickLabel2->setText("125");
    zoomLofar_yScale_TickLabel2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    zoomLofar_yScale_TickLabel2->show();

    zoomLofar_yScale_TickLabel3 = new QLabel(zoomGraphYScaleFrame);
    zoomLofar_yScale_TickLabel3->setObjectName(QString::fromUtf8("label"));
    zoomLofar_yScale_TickLabel3->setGeometry(QRect(5,109,22,15));
    zoomLofar_yScale_TickLabel3->setText("90");
    zoomLofar_yScale_TickLabel3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
        "color: rgb(255, 255, 255);"));
    zoomLofar_yScale_TickLabel3->show();
}


void GraphPlotClass::ShowSpectrumDisplay(bool Status,int16_t CH_ID)
{
    if(SpecDyn == 1){
    if(Status==true)
    {
    for (count_ = 0; count_ <1049; count_++)
    {
     xFreq[count_]=round(3.8146*count_);
     yAmp[count_]=0;
     LofarHisto[CH_ID-1][count_]=0;
     SpectrumMainDataPlot[CH_ID-1][count_]=0;
     SpectrumZoomHisto[count_]=0;
     SpectrumMainWaterfallDataPlot[CH_ID-1][count_]=0;
    }
    SpectrumGraphLegend->graph(0)->setData(xFreq,yAmp);
    SpectrumGraphLegend->show();
    PlotWaterFallPoints(1,&xFreq[0],&yAmp[0],1050,0.6620);
    }
    else
    {
        for (count_ = 0; count_ <1050; count_++)
        {
        xFreq[count_] = round(3.814*count_);
//yAmp[count_] = SpectrumMainDataPlot[CH_ID-1][count_];
//yAmp[count_] =((SpectrumMainDataPlot[CH_ID-1][count_]));        // LofarMainDataPlot[CH_ID-1][count_]=0;
        yAmp[count_] =((SpectrumMainDataPlot[CH_ID-1][count_]*0.40)+((LofarHisto[CH_ID-1][count_]*0.60)));
        SpectrumMainWaterfallDataPlot[CH_ID-1][count_]=(yAmp[count_]*1);
        LofarHisto[CH_ID-1][count_]=yAmp[count_];
	/*if(yAmp[count_] != 0.0){
           printf("%d : %f : %f\n",count_,xFreq[count_],yAmp[count_]);
	}*/
        }
        SpectrumGraphLegend->graph(0)->setData(xFreq,yAmp);
        SpectrumGraphLegend->show();
        PlotWaterFallPoints(1,&xFreq[0],&SpectrumMainWaterfallDataPlot[CH_ID-1][0],1049,0.6620);
    }
    }
}

void GraphPlotClass::ShowDelSpectrumDisplay(bool Status,int16_t CH_ID)
{
    if(IsDelData == 1 && GDyn == 1)
    {
        for(l=0;l<16384;l++)
        {
                      DelRawData_X[l] = l;
                      DelRawData_Y[l] = RawDataMainDataPlot[l];
        }
        for (delcount_ = 0; delcount_ <1200; delcount_++){
           xDelFreq[delcount_] = (float)3.816 * delcount_;
           yDelAmp[delcount_] =((DelSpectrumMainDataPlot[CH_ID-1][delcount_]*0.40)+((DelLofarHisto[CH_ID-1][delcount_]*0.60)));
           DelLofarHisto[CH_ID-1][delcount_] = yDelAmp[delcount_];
           xDelFreq_val[delcount_] = xDelFreq[delcount_];
           yDelAmp_val[delcount_]  = yDelAmp[delcount_];
	   /*if(yDelAmp[delcount_] != 0.0){
              printf("%d : %f : %f\n",delcount_,xDelFreq[delcount_],yDelAmp[delcount_]);
	   }*/
        }
        RawDataDelSpectGraphLegend->graph(0)->setData(xDelFreq,yDelAmp);
        RawDataDelSpectGraphLegend->show();

        //RawDataGraphLegend->graph(0)->setData(DelRawData_X,DelRawData_Y);
        //RawDataGraphLegend->show();
	IsDelData = 0;
    }
//-------------------------------------Obtaining values for CSV File---------------------------------------//
//printf("\n Inside ExportDelayedSpectrumCSV %lf",xDelFreq_val);
//printf("\n Inside ExportDelayedSpectrumCSV %lf",yDelAmp_val);
//printf("\n Inside RawDataCSV %lf",DelRawData_X_val);
//printf("\n Inside RawDataCSV %lf",DelRawData_Y_val);
}

void GraphPlotClass::freezeDelSpecPlot(QMouseEvent* m){
   if(GDyn == 1) {	
      GDyn = 0;
   }
   else{
      GDyn = 1;
   }
}

void GraphPlotClass::freezeLoFarPlot(QMouseEvent* m){
   if(LofDyn == 1) {
      LofDyn = 0;
   }
   else{
      LofDyn = 1;
   }
}


void GraphPlotClass::ShowSpectrumZoomDisplay()
{
        for (count_ = 0; count_ <1050; count_++)
        {
        xFreq[count_] = round((1)*count_);
        yAmp[count_]=((SpectrumZoomDataPlot[count_]*0.20)+(SpectrumZoomHisto[count_]*0.80));
        SpectrumZoomHisto[count_]=yAmp[count_];
        }
        SpectrumZoomGraphLegend->graph(0)->setData(xFreq,yAmp);
        SpectrumZoomGraphLegend->show();
}

void GraphPlotClass::ShowIntSpectrumDisplay(bool Status,int16_t CH_ID,int16_t Count)
{
    if(Status==true)
    {
        for (count_ = 0; count_ <1100; count_++)
         {
            xFreq[count_]=round(3.8146*count_);
            yAmp[count_]=0;
            LofarINTHisto[CH_ID][count_]=0;
         }
        SpectINT[Count]->graph(0)->setData(xFreq,yAmp);
        SpectINT[Count]->show();
    }
    else
    {
       for (count_ = 0; count_ <1100; count_++)
        {
           xFreq[count_] = round(3.8146*count_);
           yAmp[count_] =((SpectrumMainDataPlot[CH_ID][count_]*0.40)+((LofarINTHisto[CH_ID][count_]*0.60)));
           LofarINTHisto[CH_ID][count_]=yAmp[count_];
//printf("\n Iam in Spectrum Int %lf",LofarINTHisto[CH_ID][count_]);
        }
        SpectINT[Count]->graph(0)->setData(xFreq,yAmp);
        SpectINT[Count]->show();
    }
}

void GraphPlotClass::ShowLofarDisplay(bool Status,int16_t CH_ID)
{
   if(LofDyn == 1){
	
    if(Status==true)
    {
        for (count_ = 0; count_ <1100; count_++)
        {
         xFreq[count_]=round((1.9073)*count_);
         yAmp[count_] = 0;
         LofarMainDataPlot[CH_ID-1][count_]=0;
         LofarHisto[CH_ID-1][count_]=0;
         LofarZoomHisto[count_]=0;
         LofarWaterfalHisto[CH_ID-1][count_]=0;
        }
        LofarGraphLegend->graph(0)->setData(xFreq,yAmp);
        LofarGraphLegend->show();
        LofarPlotWaterFallPoints(0,&xFreq[0],&yAmp[0],1100,1.11255);
    }
    else
    {
        for (count_ = 0; count_ <1100; count_++)
        {
         xFreq[count_] = round((1.9073)*count_);
         yAmp[count_] =((LofarMainDataPlot[CH_ID-1][count_]*0.80)+((LofarHisto[CH_ID-1][count_]*0.20)));
         LofarHisto[CH_ID-1][count_]=yAmp[count_];
         LofarWaterfalHisto[CH_ID-1][count_]=yAmp[count_]*1;
        }
        LofarGraphLegend->graph(0)->setData(xFreq,yAmp);
        LofarGraphLegend->show();
        LofarPlotWaterFallPoints(0,&xFreq[0],&LofarWaterfalHisto[CH_ID-1][0],1100,1.11255);
    }
   }
}


void GraphPlotClass::ShowIntLofarDisplay(bool Status,int16_t CH_ID,int16_t Count)
{
  if(Status==true)
    {
        for (count_ = 0; count_ <1100; count_++)
         {
            xFreq[count_]=round((1.9073)*count_);
            yAmp[count_]=0;
            LofarINTHisto[CH_ID][count_]=0;
         }
        LofarINT[Count]->graph(0)->setData(xFreq,yAmp);
        LofarINT[Count]->show();
    }
    else
    {
       for (count_ = 0; count_ <1100; count_++)
        {
           xFreq[count_] = round((1.9073)*count_);
           yAmp[count_] =((LofarMainDataPlot[CH_ID][count_]*0.80)+((LofarINTHisto[CH_ID][count_]*0.20)));
           LofarINTHisto[CH_ID][count_]=yAmp[count_];
        }
        LofarINT[Count]->graph(0)->setData(xFreq,yAmp);
        LofarINT[Count]->show();
    }
}



void GraphPlotClass::ShowLofarZoomDisplay()
{
        for (count_ = 0; count_ <1050; count_++)
        {
        xFreq[count_] = round((1)*count_);       
        yAmp[count_]=((LofarZoomDataPlot[count_]*0.60)+(LofarZoomHisto[count_]*0.40));
        LofarZoomHisto[count_]=yAmp[count_];
//LofarZoomDataPlot[count_]=0;
        }
        LofarZoomGraphLegend->graph(0)->setData(xFreq,yAmp);
        LofarZoomGraphLegend->show();
}


void GraphPlotClass::ShowOctaveDisplay(bool Status,int16_t CH_ID,int16_t Count)
{
if(Status==true)
{
for (count_=0;count_<30;count_++)
{
 xFreq[count_] = 0;
 yAmp[count_] = 0;
 OctaveHisto[CH_ID][count_] = 0;
 OctaveMainDataPlot[CH_ID][count_] = 0;
}
}
else
{
    for (count_ = 0;count_<30;count_++)
    {
    xFreq[count_] =(count_);
    SampleOctaveValue=OctaveMainDataPlot[CH_ID][count_];
    yAmp[count_] =((SampleOctaveValue*0.40)+((OctaveHisto[CH_ID][count_]*0.60)));
    OctaveHisto[CH_ID][count_]=yAmp[count_];
    }

//OctaveBandLegendPlot->graph(0)->setPen(QPen(Qt::green));
//OctaveBandLegendPlot->graph(0)->setData(xFreq,yAmp);
//OctaveBandLegendPlot->graph(1)->setPen(QPen(Qt::red));
//OctaveBandLegendPlot->graph(1)->setData(xFreq,yAmp);
//OctaveBandLegendPlot->graph(0)->data()->clear();

graph1[Count]->setData(xFreq,yAmp);
GraphBars[Count]->setData(xFreq,yAmp);
}
}


void GraphPlotClass::SetHarmonicWaterfallCursor()
{
   HarmonicCursorFrame=new QFrame(LofarWaterFallLegend);
   HarmonicCursorFrame->setMouseTracking(true);
   HarmonicCursorFrame->setGeometry(QRect(10,50,200,82));
   HarmonicCursorFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(170,170,170);"));
   HarmonicCursorFrame->setFrameShape(QFrame::StyledPanel);
   HarmonicCursorFrame->setFrameShadow(QFrame::Raised);

   HarmonicCursormenu=new QMenu(HarmonicCursorFrame);
   FirstAction=HarmonicCursormenu->addAction(("Harmonics Cursor ON"));
   SecondAction=HarmonicCursormenu->addAction(("Harmonics Cursor OFF"));
   ThirdAction=HarmonicCursormenu->addAction(("Harmonics Cursor SET"));
   HarmonicCursorFrame->hide();
}

void GraphPlotClass::DrawTrackFinFrame()
{
    for(xCount=0;xCount<NO_OF_TRACK;xCount++)
    {
    TrackName[xCount] = new QLabel(LofarGraphLegend);
    TrackName[xCount]->setObjectName(QString::fromUtf8("label"));
    TrackName[xCount]->setGeometry(QRect(0,0,30,15));
    sprintf(NameLabel,"T%d",(xCount+1));
    TrackName[xCount]->setText(NameLabel);
    TrackName[xCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                "color: rgb(250,170,0);"));
    TrackName[xCount]->hide();

    LofaraMainGraphicsView[xCount]=new QGraphicsView(LofarGraphLegend);
    LofaraMainGraphicsView[xCount]->setGeometry(QRect(0,0,10,10));
    LofaraMainGraphicsScene[xCount]=new QGraphicsScene(LofarGraphLegend);
    LofaraMainGraphicsScene[xCount]->setBackgroundBrush(Qt::black);
    LofaraMainGraphicsView[xCount]->setRenderHints( QPainter::Antialiasing );
    LofaraMainGraphicsView[xCount]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    LofaraMainGraphicsView[xCount]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    LofaraMainGraphicsView[xCount]->setScene(LofaraMainGraphicsScene[xCount]);
    LofaraMainGraphicsView[xCount]->setStyleSheet("border: 0.25px solid black");

    Line1=new QGraphicsLineItem(0,LofaraMainGraphicsScene[xCount]);
    Line1->setLine(QLineF(0,0,10,10));
    Line1->setPen(QPen(QColor(255,170,0)));
    Line2=new QGraphicsLineItem(0,LofaraMainGraphicsScene[xCount]);
    Line2->setLine(QLineF(0,10,10,0));
    Line2->setPen( QPen(QColor(255,170,0)));
    LofaraMainGraphicsView[xCount]->hide();
    }

//Line1->setBrush( Qt::yellow);
//frame1 = new QGraphicsPolygonItem(QPolygonF( QVector<QPointF>() << QPointF(WIDTH*0.138,(HEIGHT*0.0140))<<QPointF(WIDTH*0.138,(HEIGHT*0.76)))                                                                     //  ,0,graphicScene );
//frame1->setPen( QPen(Qt::darkGray) );
//WaterFallview->NoDrag;
//LofaraMainGraphicsView->fitInView(0,0,LofaraMainGraphicsView->width(),LofaraMainGraphicsView->height(),Qt::KeepAspectRatioByExpanding);

}


void GraphPlotClass::DrawThresholdFinFrame()
{
        for(yCount=0;yCount<8;yCount++)
        {
        LofaraMainThresholdGraphicsView[yCount]=new QGraphicsView(LofarGraphLegend);
        LofaraMainThresholdGraphicsView[yCount]->setGeometry(QRect(100,20,20,20));
        LofaraMainThresholdGraphicsScene[yCount]=new QGraphicsScene(LofarGraphLegend);
        LofaraMainThresholdGraphicsScene[yCount]->setBackgroundBrush(Qt::blue);
        LofaraMainThresholdGraphicsView[yCount]->setRenderHints( QPainter::Antialiasing );
        LofaraMainThresholdGraphicsView[yCount]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        LofaraMainThresholdGraphicsView[yCount]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        LofaraMainThresholdGraphicsView[yCount]->setScene(LofaraMainThresholdGraphicsScene[yCount]);
        LofaraMainThresholdGraphicsView[yCount]->hide();
        LofaraMainThresholdGraphicsView[yCount]->setStyleSheet("border: 0.25px solid black");
        }
       // for(yCount=0;yCount<8;yCount++)
       //  {
     /*   ThresholdFrame[yCount]=new QGraphicsView(LofarGraphLegend);
        ThresholdFrame[yCount]->setGeometry(QRect(100,20,20,20));
        ThresholdFrame[yCount]->hide();
        ThresholdFrame[yCount]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        //LofaraMainThresholdGraphicsView[xCount][yCount]->setStyleSheet("border: 0.25px solid black");
     */
        // }
}



GraphPlotClass::~GraphPlotClass()
{

}

