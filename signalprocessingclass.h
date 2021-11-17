#ifndef SIGNALPROCESSINGCLASS_H
#define SIGNALPROCESSINGCLASS_H
#include "DataTypes.h"
#include"FilterCoeffients.h"
#include"math.h"
#include"qmath.h"
#include<complex.h>
#define PI 3.14159265358979323846
#define	ENERGY_MAX 6000000000.0
#define SPEC_AVG_FACTOR 3

#define SPEC_ZOOM_AVG_FACTOR 2
#define LFT_ZOOM_AVG_FACTOR 2

#define DELFACT_F 23.0
#define DELFACT_I 23

#define ADC_Highest_Value 8388607
#include <iostream>
#include <fstream>
using namespace std;

class SignalProcessingClass : public QThread
{
    Q_OBJECT

public:

     SignalProcessingClass();
     ifstream RepFile; 
     int16_t LftZoomValue,SpectrumZoomValue,iCount,yCount,zCount,rCount,kCount,jCount,xCount,ChannelID,datalength_,ChannelIndex[61],SpectrumZoomChannelIndex,LofarZoomChannelIndex,LofarChannelIndex[61];
     int16_t StartPos,EndPos,SampleLen,TrackFreqData,ThresholdAmpData,ThresholdFreqData,TrackAmpData,TrackStartBin,TrackEndBin,ThresholdStartBin,ThresholdEndBin;
     float fCount,aCount,ftCount,atCount,EngQuantisationValue;
     bool SpectrumZoomSetFlag,LofarZoomSetFlag,RecordEnabled;
     int16_t RecordStart,RecInit,RecClose,ReplayReady,ReplayClose,ReplayStart,RecOpt,RecMode,RecEnable,RepMode,RepEnable;
     int32_t RecordBuffering[16384],delayCounter_;
     unsigned int RecordSegmentVal,RecBlocks;
     int16_t StartValue,EndValue,AVGVAL;
     float AVG,OctaveDataValue;
     float delData[16384];
     float delDataFIR[16384];
     int delDataCnt;
     void run();
     void StartInitProcessing();
     void ReplayInit();
     void StartReplayProcessing();
     void StartRepalyCont();
     void StartRecordProcessing(int16_t,int16_t,int32_t*);
     void SpectrumProcessing(int16_t);
     void delSpectrumProcessing(int16_t);
     void OctaveProcessing(int16_t);

     void DemonProcessing(int16_t);
     void SetSpectrumDisplayBuffer(int16_t);
     void delSetSpectrumDisplayBuffer(int16_t);
     void SetOctaveDisplayBuffer(int16_t);
     void SetDemonDisplayBuffer(int16_t);
     void SetLofarDisplayBuffer(int16_t);
     void LofarProcessing(int16_t);
     void LofarZoomProcessing(int16_t,int16_t,float,bool);
     void SpectrumZoomProcessing(int16_t,int16_t,float,bool);
     void EnergyProcessing(int16_t);
     void FIRFilter(const float*,int,float*,float*,int32_t); /**< FIR Filter routine.Usally equiripple filter with pass band ripples 0.1dB and stop band ripple 120dB  */
     void FFT_Funtion(float *, float *,quint32,quint32,bool);        /**< FFT Funtion  */
     void ComplexFunction(float*,float*,float*,int16_t);
     void DCRemoval(float*,float*,float*,float*,int16_t);
     void FFTMagnitudeExtraction(float*,float*,float*,int16_t,int16_t);   /**< Perform Magnitude of FFT  (|FFT(x)|=sqrt(pow(REAL,2)+pow(IMAG,2))  */
     void FFTOCTAVEMagnitudeExtraction(float*,float*,int16_t,int16_t);   /**< Perform Magnitude of FFT  (|FFT(x)|=sqrt(pow(REAL,2)+pow(IMAG,2))  */

     void ComplexDecimation(float *,float *,float *,float *,int32_t,int32_t);  /**< Decimation function used to reduce sampling rate.The OUTPUT of this function contain DECIMATION_FACTOR times reduced sampling rate data.DATA_LENGTH is the length of INPUT  */
     void Decimation(float*,float*,int32_t,int32_t);
     void Exponential_Average(float*,float*,int32_t,float);
     void LowPassFreq_Average(float*,float*,int32_t,float,float);
     void Linear_Average(float*,float*,int16_t,int16_t,int32_t);
     void PeakEnhancement(float*,float*,int16_t);
     void Split_Pass_Normaliser(float*,float*,int,int,int);
     void WindowFunction(int16_t,float*,float*,float*,float*,int16_t,int8_t);
     void Get_Peak_n_Peak_Position(float*,int8_t,int32_t);
     void PeakQunatize(float*,float*,int8_t,int32_t);
     void Peak_Correction(float*,float*,float,int8_t,int32_t);
     void Lofar_Correction(float*,float*,int32_t);
     void StartRepaly();
     void TrackCheckFun(int16_t*,int16_t,int16_t);
     void ThresholdCheckFun(int16_t*,int16_t,int16_t);
     void Requantisation(int16_t,int16_t,float*,int16_t*,float*,int16_t,float,float);
     void CaluclatePeakRatio(int16_t *,float,int16_t,float,int16_t,int16_t);
     void Eliminate50Hz(float*,float*,int16_t,int16_t);

     void CaluclateTrack(int16_t*,int16_t,int16_t,int16_t);
     void CaluclateThreshold(int16_t*,int16_t,int16_t,int16_t);

      bool TrackStatusFlag,ThresholdFlag;
     float SpectrumIntermediate_;
     float EnergyQ,EnergyLogValue,EnergyAvgVal,EnergyExpAvg,EnergyData[HYDROPHONE_SENSOR];
     float BaseInputData[16384];
     float InputData[16384],ReplyData[16384];
     float DataPtr1;
     int16_t LofarQuantisedOutput[NO_OF_SENSOR][1200],SpectrumExpAvgCount[NO_OF_SENSOR],OctaveExpAvgCount[NO_OF_SENSOR],Index,LofarExpAvgCount[NO_OF_SENSOR];
     int16_t DelSpectrumExpAvgCount[NO_OF_SENSOR];
     float LofarDisplayCorrect,fPeak_Value,CorrectionValue,PeakRatioFactor,LofarMainZoomPeakAmp,SpectrumMainZoomPeakAmp,SpectrumPeakAmp,LofarPeakAmp,LofarNormalisedPeakAmp,LofarZoomPeakQuantise,LofarPeakQuantise,SpectrumPeakQuantise,SpectrumZoom1PeakAmp,SpectrumZoom2PeakAmp;
     float SpectrumFilterRealOutput[16384];
     float DelSpectrumFilterRealOutput[16384];
     float SpectrumFilterImagOutput[16384];
     float DelSpectrumFilterImagOutput[16384];
     float OctaveFilterRealOutput[16384];
     float OctaveFilterImagOutput[16384];
     float SpectrumMagnitudeOutput[8192];
     float DelSpectrumMagnitudeOutput[8192];
     float OctaveMagnitudeOutput[8192];
     float OctaveBandOutput[40];

     float SpectrumExpAvgOutput[NO_OF_SENSOR][4096];
     float DelSpectrumExpAvgOutput[NO_OF_SENSOR][4096];
     float OctaveExpAvgOutput[NO_OF_SENSOR][40];

     float Spectrum50HzEleiminateBuffer[4096];
     float DelSpectrum50HzEleiminateBuffer[4096];
     float Octave50HzEleiminateBuffer[4096];

     int16_t SpectrumQuantisedOutput[1200],EnergyAvgCount[HYDROPHONE_SENSOR];
     int16_t DelSpectrumQuantisedOutput[1200];
     float LofarExpAvgOutput[NO_OF_SENSOR][1200];

     float LofarFilterOutput[16384];
     float LofarDec1RealOutput[4096];
     float LofarMagnitudeOutput[2048];
     float LofarNormalisedOutput[2048];
     float LofarCorrectedOutput[2048];
     float Lofar50KzEliminateOutput[2048];
     float LofarFilter2RealOutput[4096];
     float LofarDec2RealOutput[2048];
     float LofarDec2ZoomRealOutput[2048];
     float LofarDec2ImagOutput[2048];

     float LofarDCRemovalRealOutput[2048];
     float LofarDCRemovalImagOutput[2048];

     float LofarWindowImagOutput[2048];
     float LofarWindowRealOutput[2048];

     float EnergyInputData[16384];
     float OctaveInputData[16384];

     float EnergyOutputData[8192];
     float SpectrumZoomBeatRealFreqencyBuffer[16384];
     float SpectrumZoomBeatImagFreqencyBuffer[16384];
     float SpectrumZoomFIR1RealFreqencyBuffer[16384];
     float SpectrumZoomFIR1ImagFreqencyBuffer[16384];
     float SpectrumZoomDec1ImagFreqencyBuffer[4096];

     float SpectrumZoomDec1RealFreqencyBuffer[4096];
     float SpectrumZoomFIR2RealFreqencyBuffer[4096];
     float SpectrumZoomFIR2ImagFreqencyBuffer[4096];

     float SpectrumZoomDec2RealFreqencyBuffer[2048];
     float SpectrumZoomDec2ImagFreqencyBuffer[2048];

     float SpectrumZoomDec3RealFreqencyBuffer[512];
     float SpectrumZoomDec3ImagFreqencyBuffer[512];
     float SpectrumZoomAccRealBuffer[4096];
     float SpectrumZoomAccImagBuffer[4096];
     float SpectrumZoomWindowRealBuffer[4096];
     float SpectrumZoomWindowImagBuffer[4096];
     float SpectrumZoomMagnitudeBuffer[4096];

  // float SpectrumZoomNormalisedBuffer[2048];
  // float SpectrumZoomEnhancementBuffer[2048];

     int16_t SpectrumZoomQuantisedOutput[2048];
     int16_t LofarZoomQuantisedOutput[2048];
     float SpectrumZoomEnhancementBuffer[2048];
     float SpectrumZoomAvgBuffer[4096];
     float SpectrumZoomPeakEnhanceBuffer[4096];

     float SpectrumZoomCorectBuffer[2048];
     float LofarZoomBeatRealFreqencyBuffer[2048];
     float LofarZoomBeatImagFreqencyBuffer[2048];
     float LofarZoomFIR1RealFreqencyBuffer[2048];
     float LofarZoomFIR1ImagFreqencyBuffer[2048];
     float LofarZoomDec1RealFreqencyBuffer[1024];
     float LofarZoomDec1ImagFreqencyBuffer[1024];

     float LofarZoomAccRealBuffer[4096];
     float LofarZoomAccImagBuffer[4096];
     float LofarZoomMagnitudeBuffer[4096];
     float LofarZoomWindowRealBuffer[4096];
     float LofarZoomWindowImagBuffer[4096];
     float LofarZoomNormalisedBuffer[4096];
     float LofarZoomAvgBuffer[4096];
     float LofarZoomDec2RealFreqencyBuffer[512];
     float LofarZoomDec2ImagFreqencyBuffer[512];
     float LofarZoomDec3RealFreqencyBuffer[256];
     float LofarZoomDec3ImagFreqencyBuffer[256];

     float LofarZoomPeakEnhanceBuffer[4096];
     float LofarZoomCorectBuffer[4096];

     int8_t SpectrumZoomAvgCount,LofarZoomAvgCount;
     float DataPtr7;

//void  convolve (const float*, int,float*, float*, int) ;

signals:
    
public slots:
    
};

#endif // SIGNALPROCESSINGCLASS_H
