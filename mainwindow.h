#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QApplication>
#include<QDesktopWidget>
#include<QWidget>
#include "DataTypes.h"
#include"mainwindow.h"
#include"statuspage.h"
#include"displaywindow.h"
#include<QTimer>
#include<QKeyEvent>
#include<QObject>
#include<QPixmap>
#include<QFileDialog>
#include<QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    int16_t iCount,jCount,ZoomCursorTickerSize_;
    int8_t CursorSet,specZoomCursor,LftZoomCursor;
    float xMousePos,yMousePos,xFreq,yAmp,xPos,xVal;
    explicit MainWindow(QWidget *parent = 0);
    StatusPage  *StartPageExecute;
    DisplayWindow *DisplayBase;
    QFrame *BaseFrame,*PrintFrame,*RecordFrame,*ReplyFrame,*SenGroupFrame,*EnergyFRame,*UnitFrame,*OctaveSenGroupFrame;
    QTimer *UpdateTimer;
    QLabel *lineGraph,*BarGraph,*ONMAName,*screenshotLabel,*EnergyReplatFile,*RecLabel,*SensorGroupLabel[4],*OctaveSensorGroupLabel[4],*HydLabel7,*AccLabel7,*RecordName,*EnergyFileName,*RecInitcation,*FileDelete,*EnergyFileDelete;
    QDialog *EnergyDialog,*thresholdPopUpWindow,*trackPopUpWindow,*ScreenShotWindow,*ScreenShotSaveOpt,*RecordDialog,*ReplayDialog,*SenGroup,*UnitCon,*OctaveSenGroup, *SelectRecChanel;
    QPushButton *PrintScreen,*PrintCancel,*ReorderButton,*ReplaySTART,*SensorGroupButton,*OctaveSensorGroupButton,*UnitOKButton,*EnergyButton;
    QVBoxLayout *mainLayout ;
    QPixmap originalPixmap;
    QLineEdit *lineEdit,*EnergyLineEdit;
    QComboBox *ReplayFiles,*SensorGroupCombo[4],*OctaveSensorGroupCombo[4],*EnergyCombo,*EnergyFileDelete7,*HydUnitCombo,*AccUnitCombo;
    QCheckBox *checkRecBox,*octaveBarcheckRecBox[4],*octavecheckRecBox[4],*EnergyFileDelete1,*EnergyFileSave,*EnergyFileRestore;
    QString ReplayFileSelect,EnergyReplytFIleSelect,EnergyDeletetFIleSelect;
    char Buff[4][20];
    char Buff2[4][10];

    bool setChange[4],PageID,setSpecHarmonicFreq,setLftHarmonicFreq,OctvaesetChange[4];
    void DrawBasePage(QFrame*);
    void DisplayINTObservation(int16_t);
    void MainCursorPosition(float,float);
    void TransmitData();

    ~MainWindow();

protected:
          void mousePressEvent(QMouseEvent *Event);
          void mouseDoubleClickEvent(QMouseEvent *Event);
     //   void wheelEvent ( QWheelEvent * event );
       // void mousePressEvent(QGraphicsSceneMouseEvent *Event);
          void keyPressEvent( QKeyEvent *keyevent );
         // void mouseMoveEvent(QMouseEvent *);



signals:
public slots:

 void SpectrumMainLegendDoubleClickEvent(QMouseEvent *event);
 void SpectrumZoomLegendDoubleClickEvent(QMouseEvent *event);
 void LofarWaterfallLegendDoubleClickEvent(QMouseEvent *event);
 void SpectrumWaterfallLegendDoubleClickEvent(QMouseEvent *event);

 void LofarZoomMainLegendDoubleClickEvent(QMouseEvent *event);
 void LofarMainLegendDoubleClickEvent(QMouseEvent *event);
 void LofarMainLegendPressEvent(QMouseEvent *event);
 void LofarMainLegendPressEvent1(QMouseEvent *event);
 void LofarMainLegendPressEvent2(QMouseEvent *event);
 void LofarMainLegendPressEvent3(QMouseEvent *event);
 void LofarMainLegendPressEvent4(QMouseEvent *event);
 void SpectrumWaterFallLegendPressEvent(QMouseEvent *event);
 void SpectrumIntLegendPressEvent1(QMouseEvent *event);
 void SpectrumIntLegendPressEvent2(QMouseEvent *event);
 void SpectrumIntLegendPressEvent3(QMouseEvent *event);
 void SpectrumIntLegendPressEvent4(QMouseEvent *event);
 void OctaveLegrentPressEvent(QMouseEvent *event);


 void LofarWaterFallLegendPressEvent(QMouseEvent *event);
 void LofarZoomLegendPressEvent(QMouseEvent *event);
 void SpectrumZoomLegendPressEvent(QMouseEvent *event);
 void SpectrumZoomLegendMoveEvent(QMouseEvent *event);

 void LofaronMouseMove(QMouseEvent *event);
 void SpectrumonMouseMove(QMouseEvent *event);

 void Del_Spec_onMouseMove(QMouseEvent *event);      //---Delayed Spectrum---//
 void Del_Spec_LegendPressEvent(QMouseEvent *event); //---Delayed Spectrum---//
 void Del_Spec_LegendDoubleClickEvent(QMouseEvent *event); //---Delayed Spectrum---//

 void LofarZoomonMouseMove(QMouseEvent *event);
 void DemononMouseMove(QMouseEvent *event);
 void SpectrumMainLegendPressEvent(QMouseEvent *event);
 void CallStatusPage();
 void CallStatusPageClose();
 void ReadFileData();
 void CallUpdateFun();
 void UpdateDisplayPage();
 void SetDisplayTimer();
 void showThresoldWindow();
 void showTrackWindow();
 void EnergySaveWindow();
 void HMIChange();
 void SensorGroupWind();
 void OctaveGroupWind();

 void SenosrUnitConv();
void SensorUnitFrame();
void SensorGroupFrame();
void OctaveSensorGroupFrame();
void drawSelRecChDailog();

void PageSelect(int);
 void SetMGroupEnable();
 void CursorPos();
 void HarmonicCursorAction1();
 void HarmonicCursorAction2();
 void HarmonicCursorAction3();
 void MGC_Slot(int);
 void AGC_Slot(int);
 void PRESET_Slot(int);
 void TrackInfo(int);



  void ScreenShotFun();
 void setThresholdConfigButton();
 void CloseTrackConfigButton();
  void cancelThresholdConfigButton();
  void POPThresholdWindow();
  void POPTrackWindow();
  void ScreenShotPicture();
  void ScreenShotSaveClose();
  void ScreenShotSave();
  void RecorderSetParameter();
    void PathSettings();
    void EnergyPathSettings();
    void EnergyFileDeleteSettings();
    void EnergyReplaySettings();
  void RecordStart();
  void ReplayStart();
  void EnergyWindowFileist();
  void StorageCapcity();
  void RecordWindowSet();
  void EnergySaveWindow2();
  void ReplayWindowSet();
  void ReplayON();
  void FileSelect(QString);
  void EnergyFileSelect(QString);
  void EnergyDeleteFileSelect(QString);

  void GroupCHSel1(int ID);
  void GroupCHSel2(int ID);
  void GroupCHSel3(int ID);
  void GroupCHSel4(int ID);
  void OctaveGroupCHSel1(int ID);
  void OctaveGroupCHSel2(int ID);
  void OctaveGroupCHSel3(int ID);
  void OctaveGroupCHSel4(int ID);
  void SesnorGroupSet();
  void OctaveSesnorGroupSet();

  void UnitConvertionClose();
  void EnergySaveWindow3();


  void metricConvertion1(int);
  void metricConvertion2(int);
  void recChanlData();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
