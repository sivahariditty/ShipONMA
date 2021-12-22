#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>
#include"DataTypes.h"
#include<QToolBox>
#include<QComboBox>
#include"graphplotclass.h"
#include<QPicture>
#include"issinfoclass.h"
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QHBoxLayout>
#include<QGraphicsPolygonItem>
#include<QMouseEvent>
#include "thresholdsettingwindow.h"
#include<QMessageBox>
#include<QPushButton>
#include<QPlainTextEdit>
#include<QScrollBar>
#include<string.h>
#include<trackwindowsetting.h>
#include<QProgressBar>
#include<QFile>
#include<stdio.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<QFileDialog>
#include<QTextDocumentWriter>

#include <QList>
#include <QStringList>
#include <QDir>
#include <QDebug>

//#include "qtcsv/stringdata.h"
//#include "qtcsv/reader.h"
//#include "qtcsv/writer.h"

class DisplayWindow : public QMainWindow
{
    Q_OBJECT

public:

    char TrackLabelVal[50],freqVal[50],ampVal[50],gainVal[50],GroupSensor[50],ZoomScale[50];
    int8_t Count4_,groupCount,HistogramCursorPos[29],HistPos[29];
    int32_t EnergyStage;
    int32_t GainValue;
    int32_t TrackMissCount[NO_OF_TRACK], NewGainVal;
    int32_t TrackXpos,TrackYpos,TrackNumber;
    int8_t GainIndB;
    int32_t CH_GAIN_VAL[NO_OF_CHANNEL],GainWord,CH_GAIN_WORD[MAX_SCU_NO];
    short gain_flag1;

        int16_t GainModeSel,HarmonicsCal,SpectrumHarmonicCursorPos,SetTimer,TimeValue,zCount,xCount,ZoomCursorWidth,iCount,yCount,jCount,SelectedPageID,CurrentChannelSel,SelectedChannel;
        int32_t MGroup[MAX_SENSOR_GROUP],OctaveGroup[MAX_SENSOR_GROUP],MSelectGroup[MAX_SENSOR_GROUP],OctaveSelectGroup[MAX_SENSOR_GROUP];
        bool cavitationFlag,ChannelNarrowBandThresholdSet[2][ACCELEROMETER_SENSOR],ChannelThresholdSet[2][ACCELEROMETER_SENSOR],ChannelLinkStatus[2][ACCELEROMETER_SENSOR],SensorGroupEnable,MGroupEnable,CHEnable[6];
        bool OctaveFlag[MAX_SENSOR_GROUP],SenserRefresh,CursorStatus,LofarHarmonicCursorStatus,SpectrumHarmonicCursorStatus,LftCursorEnable,SpecCursorEnable,De_Spec_CursorEnable;
        float LofarHarmonicCursorPos,HEIGHT,WIDTH,ZoomBeatFreq_,xPos;

        int16_t Del_Spec_HarmonicCursorPos;
        bool Del_Spec_HarmonicCursorStatus;

        ThresholdSettingWindow *ThresholdSet;
        TrackWindowSetting *TrackWindowSet;
        QPushButton *SensorGroupset,*UnitButton,*SensorGroup,*POPTrackWindow,*EnergyCursorSave,*POPThresoldWindow,*messageBox1,*ReplayButton,*ReplayButtonCont,*RecordButton,*OctaveButton, *RecChnlButton, *LFreeze;
        ISSInfoClass *IssInfo;
        explicit DisplayWindow(QFrame *);
        QFrame *MainDisplayHFrame,*MaskHFrame[95],*MainDisplayVFrame,*MaskVFrame[26];
        QFrame *ZoomDisplayHFrame,*MaskZoomHFrame[95],*ZoomDisplayVFrame,*MaskZoomVFrame[26],*ZoomCursourHLine,*ZoomCursorVLine1,*ZoomCursorVLine2,*ZoomCursorVLine3;
        QLabel *MGroupSensor[MAX_SENSOR_GROUP],*OctaveGroupSensor[MAX_SENSOR_GROUP],*DisplayFreq,*DisplayAmp,*DisplayGain,*GainVal,*FreqVal,*AmpVal,*SCUName[MAX_SCU_NO],*SCUNameCon[MAX_SCU_NO],*HydrophoneLabel[HYDROPHONE_SENSOR],*AcceleometerLabel[ACCELEROMETER_SENSOR];
        GraphPlotClass *graphPlot;
        QFrame *DisplayBaseFrame,*DisplaySubmarineLayoutFrame,*DisplayProcesingFrame;
        QFrame *HistogramFrame,*DisplayAnnotationFrame,*DisplayThresholdWarningFrame,*DisplayObservationFrame;
        QToolBox *ToolBoxWidget;
        QFrame *Annotation,*Configuration,*HydrophoneEng,*LofarFrame,*SpectrumFrame,*IntLofarFrame,*OctaveFrame,*IntSpectrumFrame,*DemonFrame,*SetTrackFrame;       
        QFrame *RawDataFrame;
        QTextEdit *RawTextData;
        QPushButton *ExportCSV_Button_1,*ExportCSV_Button_2,*Export_Button;
        QLabel *RawSignal,*DelTimeAvgSpec,*Export_Document,*Select_Format;
        QComboBox *Format_Select;
        QScrollBar *v;
        QHBoxLayout *HLayout1,*HLayout2,*HLayout3,*HLayout4;
        QComboBox *PageSelection,*HydrophoneSel,*AccelerometerSel,*ZoomLofar,*ZoomSpectrum,*Mgc,*Agc,*Preset,*Tc,*Audio,*Track,*TimeAdjust,*Scaleadjust;
        QGraphicsView  *HistogramView,*graphicView;
        QGraphicsScene *graphicScene,*HistogramScene;
        QHBoxLayout *HLayout;
        QVBoxLayout *HistHLayout;
        QGraphicsRectItem *ACCSENSOR[ACCELEROMETER_SENSOR],*HistoBar[HYDROPHONE_SENSOR][6];
        QGraphicsEllipseItem *HYDSENSOR[HYDROPHONE_SENSOR];
        QProgressBar *progressbar,*progressbar2;
        QDialog *ThresholdMessage;
        QLabel *MemoryOverflow,*HistgramLabel[HYDROPHONE_SENSOR],*TrackInfoLabel,*TrackAmp,*TrackInit,*TrackInitCh,*TrackFreq,*TrackAmpVal,*TrackInfoLabelNo,*TrackFreqVal,*TrackMarkerLabel[61],*AudioLabel,*ScaleLabel,*RecRply,*TrackLabel,*AgcLabel,*MgcLabel,*PresetLabel,*ZoomLabel,*Tclabel,*HydLabel,*AccLabel,*ThesholdMessageLabel,*ThresholdWarningLabel;
        QHostAddress UnicastAddress;
        QUdpSocket *TransmitSocket2;
        int32_t StorageCapacityValue;
        float DispFreq,DispAmp,InterValue;
        int16_t HydrophoneScale,AccelerometerScale;
        QString format_combo_value;

        bool GAIN_SET_FLAG[MAX_SCU_NO];
       //bool PageID;

        int16_t PageID;

        void Npol_Header_Setting();
        void DrawDisplayPage();
        void DrawISSInfoFrame();
        void DrawISSControlFrame();
        void DrawAnnotationFrame();
        void DrawSubmarimeLayoutFrame();
        void DrawThresholdWarningFrame();
        void DrawProcessingFrame();
        void DrawObservaionFrame();
        void AnnotationControls();
        void DrawLOFARFrame();

        void  DrawRawDataFrame(); //---RAW DATA---//
        void  DrawSpectrumFrame();
        void  DrawINTSpectrumFrame();
        void  OctaveBandFrame();

        void  DrawINTLofarFrame();
        void  DrawTrackFrame();
        void  ChangeDisplayPageConfiguration(int16_t);
        void  DrawMainHLine();
        void  DrawMainVLine();
        void  DrawZoomMainHLine();
        void  DrawZoomMainVLine();
        void  DrawZoomCursor();
        void  DrawHistorgramFrame();
        void  DrawHistrogramSegments();
        void  DrawHistorgram();
        void  SetZoomCursorPositionChange(float,float);

       //void DrawHarmonicCursorActive(bool,int16_t);
       //void DrawHarmonicCursorLines(int16_t);
         void DrawHarmonicCursorActive(bool,bool);
         void DrawHarmonicCursorLines(bool);

        void  SetZoomCursorPosition();
        void  DisplayPageInit();
        void  CheckEthernetLinkStatus();
      //void  GainUpdate();
        void  FDSLink(int16_t,bool);
        void  ThresholdCheck();
        void  HydEnergyCheck();
        void  MemCheck();

        void  UpdateWaterFallGraph();
        void  RefreshLOFARWaterFallgraph();
        void  RefreshIntWaterFallgraph();
        void  SensorGroupSelect(int16_t,bool);
        void  ThresholdAlarmDisplay();
        void  ResetFlags();
        void  CheckTrackStatusFun();

        void  EnableChannelGroup(bool);
        void  showMessageBox();
        void  UpdateSonarInformation();
        void  UpdateObservationData(float,float,float,float);
        void  UpdateSpectrumDisplay();
        void  UpdateTrackDisplay();
        void  UpdateThresholdDisplay(int16_t);
        void  UpdateTimeInfo();
        void  CursorChangeFlage();
        void  ChangeHistoThresholdValue(int16_t,int16_t);
        void  SetThresholdCursorPos();
        void  ChangeEnergyValue(int16_t,int16_t);
        void  RecordTimeEnableFun();
        void  ScaleCalculation(float,float,int16_t,int16_t,int16_t);

        int16_t RecRep;
        int32_t CHGaindBValue;
        float CHGainFloatValue;
        int16_t yScale,yScaleValue;

   protected:

signals:

    void SignalMouseLeftButtonPress();

public slots:

       void PageSelectFunction(int);
       void ReplayTime(int);
       void SclaeAdujustment(int);
       void HydrophoneSelect(int);
       void ReadEnergyFile(int);
       void AccelerometerSelect(int);
       void SensorGroupSelection(int);
       void ZoomLofarCursorSet(int);
       void ZoomSpectrumCursorSet(int);
       void SaveHistogramEnergy();
       void closePage();
       void SetTimeConstant(int);
       void Mgc_SetCmbo(int16_t arg);
       void checkGain();
       void RecordEnableFun();
       void ReplayEnableFun();
       void EnergyRecordEnableFun();
       void EnergyRecordReplayFun();
       void TrackDataInfo(int32_t);
       void Agc_SetCmbo(int16_t arg);
       void Preset_SetCmbo(int16_t arg);
       void Gain_Set(int16_t);

      void on_Formatcombobox_currentIndexChanged();
      void Export_to_Format_Button();
      void ExportDelayedSpectrum_to_CSV();
      void ExportRawData_to_CSV();
      void setReplayCntFlag();
};


#endif // DISPLAYWINDOW_H
