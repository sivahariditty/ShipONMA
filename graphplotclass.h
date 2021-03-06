#ifndef GRAPHPLOTCLASS_H
#define GRAPHPLOTCLASS_H

#include <QWidget>
#include"DataTypes.h"
#include"qcustomplot.h"
#include<QObject>
#include<QLabel>
#include<QGraphicsView>
#include<QMenu>
#include<QAction>
#include<QGraphicsLineItem>
#include<QGraphicsSimpleTextItem>
#include<QPushButton>
#include<QFileDialog>
#include<QFile>

#define LOFAR_xGRAPH_RANGE 1200
#define LOFAR_yGRAPH_RANGE 500

#define SPECTRUM_xGRAPH_RANGE 4000
#define SPECTRUM_yGRAPH_RANGE 500
#define MainWaterfallPixMapSize 120
#define IntWaterfallPixMapSize 140
#define INTLOFARLegendLen  1200


#define RAWDATA_xGRAPH_RANGE 200
#define RAWDATA_yGRAPH_RANGE 50000
#define RAWLW_xGRAPH_RANGE 0
#define RAWLW_yGRAPH_RANGE -50000
#define DELSPEC_xGRAPH_RANGE 4000
#define DELSPEC_yGRAPH_RANGE 500


//#define RAWDATA_xGRAPH_RANGE 200
//#define RAWDATA_xGRAPH_RANGE 200
//#define RAWDATA_yGRAPH_RANGE 5000
//#define RAWLW_xGRAPH_RANGE 0
//#define RAWLW_xGRAPH_RANGE 0
//#define RAWHG_yGRAPH_RANGE -5000
//#define DELSPEC_xGRAPH_RANGE 1200
//#define DELSPEC_yGRAPH_RANGE 500

class QCustomPlot;
class QwtPlot;
class GraphPlotClass : public QWidget
{
Q_OBJECT

public:

    QImage LofarWaterFallImage;    /**<QImage Object used to create LOFAR Images from QGraphicssceen*/
    QImage SpectrumWaterFallImage; /**<QImage Object used to create Spectrum Images from QGraphicssceen*/
//QImage INTWaterFallImage[6];  /**< QImage Object used to create Integrated Waterfall Images from QGraphicssceen  */
//QImage INTSpecWaterFallImage[6];  /**< QImage Object used to create Integrated Waterfall Images from QGraphicssceen  */
    int32_t IntWaterfallCount,WaterfallCount,count_,count2_,count3_,Count4_,delcount_;
    int32_t Histroy[MainWaterfallPixMapSize][1200]; /**< Waterfall Histroy array used for Waterfall draw */
    int32_t IntHistroy[6][MainWaterfallPixMapSize][2048]; /**< Integrated Waterfall Histroy array used for Waterfall draw */
    int32_t IntSpecHistroy[6][MainWaterfallPixMapSize][2048]; /**< Integrated Spectrum Waterfall Histroy array used for Waterfall draw */
    int16_t RandomValue_,Max_,Min_,xCount,yCount;
    int GDyn, LofDyn,SpecDyn,CompDyn;

    char NameLabel[10];
    QLabel *TrackName[NO_OF_TRACK];
    QPixmap pixmap;
    float ResolutionCount_,HydEnergy[29];
    QVector<QRgb> colortable1;       /*< Color Table required for waterfall window*/
    explicit GraphPlotClass(QWidget *parent = 0);
    QLabel *Octave_Scale_TickLabel1,*Octave_Scale_TickLabel2,*Octave_Scale_TickLabel3,*Lofar_Scale_TickLabel1,*Lofar_Scale_TickLabel2,*Lofar_Scale_TickLabel3,*OctaveHigh,*OctaveLow,*Lofar_yScale_TickLabel1,*Lofar_yScale_TickLabel2,*Lofar_yScale_TickLabel3,*zoomLofar_yScale_TickLabel1,*zoomLofar_yScale_TickLabel2,*zoomLofar_yScale_TickLabel3;
    QLabel *LofarZoom_xScale_TickLabel3,*LofarZoom_xScale_TickLabel1,*LofarZoom_xScale_TickLabel2;
    QLabel *DelSpec_Scale_TickLabel1,*DelSpec_Scale_TickLabel2,*DelSpec_Scale_TickLabel3,*RawData_Scale_TickLabel1,*RawData_Scale_TickLabel2;
    QCustomPlot *LofarGraphLegend,*LofarZoomGraphLegend,*LofarWaterFallLegend,*LofarINT[4],*SpectINT[4],*OctaveBandLegendPlot;
    QCustomPlot *SpectrumGraphLegend,*SpectrumZoomGraphLegend,*SpectrumWaterFallLegend,*DemonWaterFallLegend;
    QCustomPlot *RawDataDelSpectGraphLegend,*RawDataGraphLegend;
    QCustomPlot *customWaterfallPlot;
    QCustomPlot *customZoomfallPlot;
    QCustomPlot *SpectrumComparisonGraphLegend;
    QGraphicsView *SpectrumWaterFallview,*WaterFallview,*DemonWaterfall;
    QGraphicsScene *SpectrumWaterFallScene,*WaterFallscene,*LFTINTWaterFallscene[6],*LFTINTSpecWaterFallscene[6],*DemonWaterfallScene;
    QFrame *HarmonicCursorFrame,*LofarFrame,*OctaveGraphXScale,*GraphXScale,*zoomGraphXScale,*GraphYScale,*OctaveGraphXScaleFrame,*GraphXScaleFrame,*zoomGraphXScaleFrame,*GraphYScaleFrame,*zoomGraphYScaleFrame,*xTicks3,*xTicks2,*xTicks1,*xOctaveTicks3,*xOctaveTicks2,*xOctaveTicks1,*ZoomxTicks3,*ZoomxTicks2,*ZoomxTicks1,*MainVLine,*MainHLine3,*MainHLine2,*MainHLine1,*zoomMainVLine,*zoomMainHLine3,*zoomMainHLine2,*zoomMainHLine1;
  //SpectrumPage  
    QFrame *showValuesSpectrumplot, *showSpectrumRightClickWid;
    QLabel *showSpectrumVal;
    QPushButton  *CloseSpectrumVal;
    QPushButton *ButtonExportCSV_Spectrum,*ButtonExportJPG_Spectrum, *ButtonJPG_CSV_Close_Spectrum;
  //RawDataPage
   QFrame *showValuesRawDataplot,*showRawDataRightClickWid;
   QLabel *showRawDataVal;
   QPushButton  *CloseRawDataVal;
   QPushButton *ButtonExportCSV_RawData, *ButtonExportJPG_RawData, *ButtonJPG_CSV_Close_RawData;
   //SpectrumComparisonPage
   QFrame *showValuesSpectCompDataplot, *showSpectCompDataRightClickWid;
   QLabel *showSpecCompDataVal;
   QPushButton  *CloseSpecCompVal;
   QPushButton *ButtonExportCSV_SpecCompData ,*ButtonExportJPG_SpecCompData,*ButtonJPG_CSV_Close_SpecComp;

    QFrame *DelSpectrumXScaleFrame,*DelSpectrumYScaleFrame;
    QMenu *HarmonicCursormenu;
    QAction *FirstAction,*SecondAction,*ThirdAction;
    QGraphicsRectItem *HarmonicCursor[5],*SpectrumHarmonicCursor[5];
    QRgb RGBvalue;
    QGraphicsPolygonItem *TrackS1,*TrackS2;
    float xDelFreq_val[1200], yDelAmp_val[1200], DelRawData_X_val, DelRawData_Y_val;
    int l;
    QGraphicsLineItem *Line1,*Line2;
    QGraphicsView *LofaraMainGraphicsView[NO_OF_TRACK],*LofaraMainThresholdGraphicsView[8];
    QGraphicsScene *LofaraMainGraphicsScene[NO_OF_TRACK],*LofaraMainThresholdGraphicsScene[8];
    QCPGraph *graph1[4];
    QCPBars *GraphBars[4];
    
    double SampleOctaveValue;

     void  LofarDisplaySelect();
     void  SpectrumDisplaySelect();
     void  RawDataDisplaySelect();
     void SpectrumComparisonDisplaySelect();

     void  DrawGraphWidget(QCustomPlot *,int16_t,int16_t,int16_t,int16_t,int16_t,int16_t);
     void  DrawGraphWidget2(QCustomPlot *,int16_t,int16_t,int16_t,int16_t);
     void  DrawOctaveMainXScale(QCustomPlot *);
     void  DrawOctaveMainYScale(QCustomPlot *);

     void  DrawMainXScale(QCustomPlot *);
     void  DrawMainYScale(QCustomPlot *);

     void  DelSpectrumXScale(QCustomPlot *);
     void  DelSpectrumYScale(QCustomPlot *);

     void  DrawLofarZoomScaleLabel();
     void  DrawSpectrumWaterFallPoints();
     void  DrawLofarWaterFallPoints();
     void  setWaterfallcolortable();
     void  PlotWaterFallPoints(int16_t ,double *,double *,int16_t,float); /**< Plot Waterfall graph. Variable graphResolution represent FFT resolution of each graph(LOFAR & SPECTRUM) */
     void  LofarPlotWaterFallPoints(int16_t ,double *,double *,int16_t,float); /**< Plot Waterfall graph. Variable graphResolution represent FFT resolution of each graph(LOFAR & SPECTRUM) */

     void DrawINTLOFARWaterFallPoints();
     void DrawINTSPECWaterFallPoints();
     void DrawOctaveEnergyBandPlot();
     void SetHarmonicWaterfallCursor();
     void DrawZoomMainXScale(QCustomPlot *);
     void DrawZoomMainYScale(QCustomPlot *);
     void ShowSpectrumDisplay(bool,int16_t);
     void ShowDelSpectrumDisplay(bool,int16_t);
     void ShowSpectrumZoomDisplay();
     void ShowLofarZoomDisplay();
     void ShowOctaveDisplay(bool,int16_t,int16_t);
     void ShowLofarDisplay(bool,int16_t);
     void ShowRawDataDisplay(bool,int16_t);  //--RAW DATA--//
     void ShowIntLofarDisplay(bool,int16_t,int16_t);
     void ShowIntSpectrumDisplay(bool,int16_t,int16_t);
     void DrawTrackFinFrame();
     void DrawThresholdFinFrame();
     void ShowSpecCompDisplay();
     void ClearSpecCompDisplay();
     void ShowCompSpectrumDisplay(int16_t CH_ID);

    ~GraphPlotClass();

signals:

    
public slots:
     void freezeDelSpecPlot(QMouseEvent*);
     void freezeLoFarPlot(QMouseEvent* m);
     //SpectrumPage
     void showSpectrumValue(QMouseEvent*);
     void CloseSpectrumvalue();
     void showSpectrumRightWidgetVal(QMouseEvent* );
     void ExportSpectrumData_to_CSV();
     void ExportSpectrumData_to_JPG();
     void ButtonJPG_CSV_Close_Spectrum_Tab();
     //RawDataPage
     void showRawDataValue(QMouseEvent*);
     void CloseRawDatavalue();
     void showRawDataRightWidgetVal(QMouseEvent*);
     void ExportRawData_to_CSV();
     void ExportRawData_to_JPG();
     void ButtonJPG_CSV_Close_RawData_Tab();
     //SpectrumComparisonPage
     void showSpectCompDataValue(QMouseEvent*);
     void CloseSpecCompvalue();
     void showSpectCompDataRightWidgetVal(QMouseEvent*);
     void ExportSpecCompData_to_CSV();
     void ExportSpecCompData_to_JPG();
     void ButtonJPG_CSV_Close_SpecComp_Tab();


};

#endif // GRAPHPLOTCLASS_H
