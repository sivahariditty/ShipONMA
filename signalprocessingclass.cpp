#include "signalprocessingclass.h"
#include"QuantisationTable.h"
//#define SpectrumAmplitudeRes 0.000039622

#define SpectrumAmplitudeRes 0.000029802//Calculated for Max ADC value=8388607(Vin*pow(2,24)/Vref).Vref=5Vpp,ADC is 24 bit Resolution.
#define SpectrumZoomAmplitudeRes 0.000024765//Higest Value taken for Zoom1 FFT=10094839.000000

extern ControlData Controls[NO_OF_SENSOR];
extern DelayedSpectrum_Data Del_Controls[NO_OF_SENSOR];

extern ChannelTrackValue Track_Data[NO_OF_TRACK];
extern ChannelThreshold Threshold;
extern int16_t ReplayCompFlag;
extern int32_t DelayMaking;
extern int16_t ReplayProgressBar;

extern Recording Records[REC_DATA_SEGLEN];
extern bool StartProcessing;
extern double SpectrumMainDataPlot[NO_OF_SENSOR][1200];
extern double DelSpectrumMainDataPlot[NO_OF_SENSOR][1200];
extern int IsDelData;
extern  float  RawDataMainDataPlot[16384];
//extern  float  RawDataMainDataPlot[NO_OF_SENSOR][16384];
extern double OctaveMainDataPlot[NO_OF_SENSOR][40];

extern double LofarMainDataPlot[NO_OF_SENSOR][1100];
extern double SpectrumZoomDataPlot[1200];
extern double LofarZoomDataPlot[1100];
extern double DemonMainDataPlot[1100];
extern int16_t Energy_Histo_Pos[29];
extern int16_t ChannelSelected;
extern float  EnergyValue[61];
extern float  SPECTRUM_ALPHA,LOFAR_ALPHA,ENG_ALPHA;
extern float  SpcetrumBeatFreq,LofarBeatFreq;
extern int16_t SpectrumZoomCursor,LofarZoomCursor;
float SPECZOMM_ALPHA,LOFZOOM_ALPHA;
extern bool SpectrumZoomCursorStatus,LofarZoomCursorStatus,SpectrumZoomEnable,LofarZoomEnable;
FILE *frp;


int16_t LowerOctaveBand[30]={11,14,18,22,28,35,44,56,70,88,111,140,177,223,281,354,445,561,707,891,1122,1414,1782,2245,2828,3564,4490,5657,7127,8980};

int16_t UpperOctaveBand[30]={14,18,22,28,35,44,56,70,88,111,140,177,223,281,354,445,561,707,891,1122,1414,1782,2245,2828,3564,4490,5657,7127,8980,11314};

int16_t CenterOctaveBand[30]={12,16,20,25,31,40,50,63,80,100,125,160,200,250,315,400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,8000,10000};

int32_t PROCESS_SCU[10][16]={{1,2,3,4,5,6,7,8,30,31,32,33,34,35,36,37},  // SCU 1
                         {9,10,11,12,13,14,15,16,38,39,40,41,42,43,44,45}, // SCU2
                         {17,18,19,20,21,22,23,24,46,47,48,49,50,51,52},   // SCU 3
                         {25,26,27,28,29,0,0,0,53,54,55,56,57,58,59,60},  // SCU4
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //SCU 5
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //SCU6
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //SCU 7
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //SCU 8
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // SCU9
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //SCU 10
                        };

SignalProcessingClass::SignalProcessingClass()
{
    SPECTRUM_ALPHA = 0.25;
    LOFAR_ALPHA = 0.25;
    SPECZOMM_ALPHA = 0.2076;
    LOFZOOM_ALPHA = 0.2076;
    RecordEnabled = 0;
    RecBlocks=0;
    SpectrumZoomAvgCount=0;
    SpectrumZoomChannelIndex=0;
    LofarZoomAvgCount=0;
    SpectrumZoomSetFlag=0;
    LofarZoomSetFlag=0;
    for(iCount=0;iCount<61;iCount++)
    {
      Track_Data[0].CH_NO[iCount]=0;
      ChannelIndex[iCount]=0;
      LofarChannelIndex[iCount]=0;

      SpectrumExpAvgCount[iCount]=0;
      DelSpectrumExpAvgCount[iCount]=0;
      LofarExpAvgCount[iCount]=0;
      OctaveExpAvgCount[iCount]=0;
    }

    for(iCount=0;iCount<HYDROPHONE_SENSOR;iCount++)
    {
     EnergyData[iCount]=0;
     EnergyAvgCount[iCount]=0;
    }
    delDataCnt = 0;
    IsDelData = 0;
}

void SignalProcessingClass::run()
{
    ReplayCompFlag=0;
    RecOpt=0;    //Normal Signal Processing
    RecMode=0xFE;
    RepMode=0xFE;
    RecEnable=0;
    RepEnable=0;
    RecordStart=0;
    RecInit=0;
    RecClose=0;
    RecordSegmentVal=0;
    delayCounter_=0;
    DelayMaking=0;
    while(1)
    {
             if(ReplayCompFlag==1)
             {
              ReplayInit();
              StartRepalyCont();
             }
             else
             {
               StartInitProcessing();
             }
    }
}

void SignalProcessingClass::ReplayInit()
{
    delayCounter_=0;
    for(zCount=0;zCount<61;zCount++)
    {
      SpectrumExpAvgCount[zCount] = 0;
      OctaveExpAvgCount[zCount] = 0;
      LofarExpAvgCount[zCount] = 0;
 //DelSpectrumExpAvgCount[zCount] = 0;      //---Changed---//
    }
}



void SignalProcessingClass::StartInitProcessing()
{
    for(kCount=0;kCount<NO_OF_SENSOR;kCount++)
    {
       int freq = 1000;
         ChannelID=(kCount+1);
        //if(ChannelID==1)
       if(Controls[kCount].PostStatus[Controls[kCount].RearPointer]==true)
       //if(true)
          {
             for(iCount=0;iCount<16384;iCount++)
             {
                 DataPtr1=(Controls[kCount].RawDataBuffer[Controls[kCount].RearPointer][iCount]);
                 InputData[iCount]=(DataPtr1/ADC_Highest_Value);
                 //float tmpval1 = ((1000*sin(2*3.14*iCount*freq*0.000032)) + (900000000*((rand()/(float)((50000 - 1 + 1)) + 1))/(float)RAND_MAX));
                 //InputData[iCount]=(tmpval1/ADC_Highest_Value);
		 //printf("%f\n",tmpval1);
             }
             Controls[kCount].PostStatus[Controls[kCount].RearPointer]=false;
             Controls[kCount].RearPointer=(Controls[kCount].RearPointer+1)%RawDataBufferSize;
             FIRFilter(&GenralFIR1[0],72,&InputData[0],&BaseInputData[0],16384);
             for (int m =0;m<16384;m++)
             {
                float tmpval2 = ((1000*sin(2*3.14*m*freq*0.000032)) + (900000000*((rand()/(float)((50000 - 1 + 1)) + 1))/(float)RAND_MAX));
                if(delDataCnt == 0){
                   delDataFIR[m] = (float)(BaseInputData[m]/DELFACT_F);
                   //delDataFIR[m] = (tmpval2/DELFACT_F);
                   //delDataFIR[m] = tmpval2;
                }
                else{
                   delDataFIR[m] += (float)(BaseInputData[m]/DELFACT_F);
                   //delDataFIR[m] += (tmpval2/DELFACT_F);
                   //delDataFIR[m] = tmpval2;
                }
		//printf("%f\n",tmpval2);
             }
             SpectrumProcessing(kCount);
             OctaveProcessing(kCount);
             LofarProcessing(kCount);
             delDataCnt++;
             if(delDataCnt == DELFACT_I){
                delSpectrumProcessing(kCount);
                memcpy(RawDataMainDataPlot,delDataFIR,16384);
                IsDelData = 1;
                delDataCnt = 0;
             }
             if(kCount<29)
             {
                 EnergyProcessing(kCount);
             }

             if(kCount==28)
             {
         //DemonProcessing(kCount);
             }
           //printf("\n Ch %d   %d",ChannelSelected,ChannelID);
           //if(RecMode==0xFF)
             {
         //StartRecordProcessing(kCount,RecEnable,&RecordBuffering[0]);
             }
           }
         }
}




void SignalProcessingClass::StartRepaly()
{
     delDataCnt = 0;
    for(zCount=0;zCount<REC_DATA_SEGLEN;zCount++)
    {
       rCount=Records[zCount].ChannelNo;
        ChannelID=(rCount+1);
        for(iCount=0;iCount<16384;iCount++)
        {
            DataPtr7=(float)Records[zCount].RecordDataBuffer1[iCount];
            ReplyData[iCount]=(DataPtr7/ADC_Highest_Value);
            InputData[iCount]= ReplyData[iCount];
	   if(delDataCnt == 0){
              delData[iCount] = (float)((DataPtr7/ADC_Highest_Value));
           }
           else{
              delData[iCount] += (float)((DataPtr7/ADC_Highest_Value));
           }

//printf("\nSegment=%d  %d Channal=%d Replay.....%f ",zCount,iCount,rCount,ReplyData[iCount]);
        }
	//printf("del : %f\n",ReplyData[20]);

        FIRFilter(&GenralFIR1[0],72,&ReplyData[0],&BaseInputData[0],16384);
        FIRFilter(&GenralFIR1[0],72,&delData[0],&delDataFIR[0],16384);
        SpectrumProcessing(rCount);
//---------------------------------------------------------------------------------------------//
	//printf("del %d : %f : %f\n",delDataCnt,BaseInputData[523],RawDataMainDataPlot[523]);
//---------------------------------------------------------------------------------------------//
    delDataCnt ++;
    if(delDataCnt == DELFACT_I){
       delSpectrumProcessing(rCount);
       IsDelData = 1;
       delDataCnt = 0;
    }

        OctaveProcessing(rCount);
        LofarProcessing(rCount);
  //delSpectrumProcessing(rCount); //Replay For Delayed Spectrum
        if(ChannelID<29)
        {
            EnergyProcessing(rCount);
        }

        if(ChannelID==29)
        {
      //DemonProcessing(kCount);
        }
      //printf("\n Ch %d   %d",ChannelSelected,ChannelID);
      //if(RecMode==0xFF)
        {
       // StartRecordProcessing(kCount,RecEnable,&RecordBuffering[0]);
        }
        usleep(DelayMaking);
        ReplayProgressBar=(ReplayProgressBar+1)%REC_DATA_SEGLEN;
    }
    ReplayCompFlag=0;
}

void SignalProcessingClass::StartRepalyCont(){
   printf("calling StartRepalyCont");
   int32_t ReplyPtr[16384]; 
   int32_t testData;
   frp=fopen("/home/sivahari/test_222.bin","rb+");
   if(frp==NULL){
      printf("\n FILE NOT FOUND ......") ;
   }
   delDataCnt = 0;
   rCount=0;
   ChannelID=(rCount+1);
   while(!feof(frp)){
   fread(&ReplyPtr,16384*sizeof(int32_t),1,frp);
   fread(&testData,sizeof(int32_t),1,frp);
   for(iCount=0;iCount<16384;iCount++){
      DataPtr7=(float)ReplyPtr[iCount];
      printf("data : %d\n",testData);
      ReplyData[iCount]=(DataPtr7/ADC_Highest_Value);
      InputData[iCount]= ReplyData[iCount];
      if(delDataCnt == 0){
         delData[iCount] = (float)((DataPtr7/ADC_Highest_Value));
      }
      else{
         delData[iCount] += (float)((DataPtr7/ADC_Highest_Value));
      }
   }
   FIRFilter(&GenralFIR1[0],72,&ReplyData[0],&BaseInputData[0],16384);
   FIRFilter(&GenralFIR1[0],72,&delData[0],&delDataFIR[0],16384);
   SpectrumProcessing(rCount);
   delDataCnt ++;
   if(delDataCnt == DELFACT_I){
      delSpectrumProcessing(rCount);
      IsDelData = 1;
      delDataCnt = 0;
    }
    OctaveProcessing(rCount);
    LofarProcessing(rCount);
  //delSpectrumProcessing(rCount); //Replay For Delayed Spectrum
        if(ChannelID<29)
        {
            EnergyProcessing(rCount);
        }

        if(ChannelID==29)
        {
      //DemonProcessing(kCount);
        }
      //printf("\n Ch %d   %d",ChannelSelected,ChannelID);
      //if(RecMode==0xFF)
        {
       // StartRecordProcessing(kCount,RecEnable,&RecordBuffering[0]);
        }
        usleep(DelayMaking);
      fseek(frp,16384*sizeof(int32_t),SEEK_CUR);
   }
    fclose(frp);
    ReplayCompFlag=0;
}

void SignalProcessingClass::StartRecordProcessing(int16_t CH_ID,int16_t Enable,int32_t *DataBuffer)
{

}


void SignalProcessingClass::StartReplayProcessing()
{


}

void SignalProcessingClass::SpectrumProcessing(int16_t CH_ID)
{
    ComplexFunction(&BaseInputData[0],&SpectrumFilterRealOutput[0],&SpectrumFilterImagOutput[0],16384);
    FFT_Funtion(&SpectrumFilterRealOutput[0],&SpectrumFilterImagOutput[0],8192,8192,false);
    FFTMagnitudeExtraction(&SpectrumFilterRealOutput[0],&SpectrumFilterImagOutput[0],&SpectrumMagnitudeOutput[0],4096,3);
    Exponential_Average(&SpectrumMagnitudeOutput[0],&SpectrumExpAvgOutput[CH_ID][0],4096,0.40);
    SpectrumExpAvgCount[CH_ID]=((SpectrumExpAvgCount[CH_ID]+1)%SPEC_AVG_FACTOR);
    if(SpectrumExpAvgCount[CH_ID]==0)
    {
 //Get_Peak_n_Peak_Position(&SpectrumExpAvgOutput[CH_ID][0],1,4096);
 //Eliminate50Hz(&SpectrumExpAvgOutput[CH_ID][0],,0,4096);
    PeakQunatize(&SpectrumExpAvgOutput[CH_ID][0],&Spectrum50HzEleiminateBuffer[0],1,4096)  ;
    Requantisation(CH_ID,1,&Spectrum50HzEleiminateBuffer[0],&SpectrumQuantisedOutput[0],&SpectrumQuantisationTable[0],1200,1,SpectrumAmplitudeRes);
    SetSpectrumDisplayBuffer(ChannelID);
    }
    if(ChannelSelected==ChannelID)
    {
    SpectrumZoomProcessing(ChannelID,SpectrumZoomCursor,SpcetrumBeatFreq,SpectrumZoomCursorStatus);
    }
}

void SignalProcessingClass::delSpectrumProcessing(int16_t CH_ID)
{
    ComplexFunction(&delDataFIR[0],&DelSpectrumFilterRealOutput[0],&DelSpectrumFilterImagOutput[0],16384);
    //ComplexFunction(&delData[0],&DelSpectrumFilterRealOutput[0],&DelSpectrumFilterImagOutput[0],16384);
    FFT_Funtion(&DelSpectrumFilterRealOutput[0],&DelSpectrumFilterImagOutput[0],8192,8192,false);
    FFTMagnitudeExtraction(&DelSpectrumFilterRealOutput[0],&DelSpectrumFilterImagOutput[0],&DelSpectrumMagnitudeOutput[0],4096,3);
    Exponential_Average(&DelSpectrumMagnitudeOutput[0],&DelSpectrumExpAvgOutput[CH_ID][0],4096,0.40);
    DelSpectrumExpAvgCount[CH_ID]=((DelSpectrumExpAvgCount[CH_ID]+1)%SPEC_AVG_FACTOR);
    if(DelSpectrumExpAvgCount[CH_ID]==0)
    {
 //Get_Peak_n_Peak_Position(&SpectrumExpAvgOutput[CH_ID][0],1,4096);
 //Eliminate50Hz(&SpectrumExpAvgOutput[CH_ID][0],,0,4096);
    PeakQunatize(&DelSpectrumExpAvgOutput[CH_ID][0],&DelSpectrum50HzEleiminateBuffer[0],1,4096)  ;
    Requantisation(CH_ID,1,&DelSpectrum50HzEleiminateBuffer[0],&DelSpectrumQuantisedOutput[0],&DelSpectrumQuantisationTable[0],1200,1,SpectrumAmplitudeRes);
    delSetSpectrumDisplayBuffer(ChannelID);
    }
}




void SignalProcessingClass::OctaveProcessing(int16_t CH_ID)
{
    FIRFilter(&OctaveFIR1[0],38,&InputData[0],&OctaveInputData[0],16384);
    ComplexFunction(&OctaveInputData[0],&OctaveFilterRealOutput[0],&OctaveFilterImagOutput[0],16384);
    FFT_Funtion(&OctaveFilterRealOutput[0],&OctaveFilterImagOutput[0],8192,8192,false);
    FFTMagnitudeExtraction(&OctaveFilterRealOutput[0],&OctaveFilterImagOutput[0],&OctaveMagnitudeOutput[0],4096,2);
    FFTOCTAVEMagnitudeExtraction(&OctaveMagnitudeOutput[0],&OctaveBandOutput[0],4096,1);
    Exponential_Average(&OctaveBandOutput[0],&OctaveExpAvgOutput[CH_ID][0],31,0.60);
    OctaveExpAvgCount[CH_ID]=((OctaveExpAvgCount[CH_ID]+1)%SPEC_AVG_FACTOR);
    if(OctaveExpAvgCount[CH_ID]==0)
    {
 //Get_Peak_n_Peak_Position(&SpectrumExpAvgOutput[CH_ID][0],1,4096);
 //Eliminate50Hz(&OctaveExpAvgOutput[CH_ID][0],&Octave50HzEleiminateBuffer[0],0,4096);
 //Requantisation(CH_ID,0,&OctaveExpAvgOutput[CH_ID][0],&OctaveQuantisedOutput[CH_ID][0],&SpectrumQuantisationTable[0],24,1,SpectrumAmplitudeRes);
    SetOctaveDisplayBuffer(ChannelID);
    }
}

void SignalProcessingClass::LofarProcessing(int16_t CH_ID)
{
    FIRFilter(&LofarFIR1[0],63,&BaseInputData[0],&LofarFilterOutput[0],16384);
    Decimation(&LofarFilterOutput[0],&LofarDec1RealOutput[0],4,16384);
    FIRFilter(&LofarFIR2[0],87,&LofarDec1RealOutput[0],&LofarFilter2RealOutput[0],4096);
    Decimation(&LofarFilter2RealOutput[0],&LofarDec2ZoomRealOutput[0],2,4096);
    ComplexFunction(&LofarDec2ZoomRealOutput[0],&LofarDec2RealOutput[0],&LofarDec2ImagOutput[0],2048);
 //DCRemoval(&LofarDec2RealOutput[0],&LofarDec2ImagOutput[0],&LofarDCRemovalRealOutput[0],&LofarDCRemovalImagOutput[0],2048);
    FFT_Funtion(&LofarDec2RealOutput[0],&LofarDec2ImagOutput[0],2048,2048,false);
    FFTMagnitudeExtraction(&LofarDec2RealOutput[0],&LofarDec2ImagOutput[0],&LofarMagnitudeOutput[0],1024,2);
    Split_Pass_Normaliser(&LofarMagnitudeOutput[0],&LofarNormalisedOutput[0],1024,4,4);
    Exponential_Average(&LofarNormalisedOutput[0],&LofarExpAvgOutput[CH_ID][0],1024,LOFAR_ALPHA);
    LofarExpAvgCount[CH_ID]=((LofarExpAvgCount[CH_ID]+1)%LFT_ZOOM_AVG_FACTOR);
    if(LofarExpAvgCount[CH_ID]==0)
    {
//Lofar_Correction(&LofarNormalisedOutput[0],&LofarCorrectedOutput[0],1200);
//Eliminate50Hz(&LofarCorrectedOutput[0],&Lofar50KzEliminateOutput[0],1,1200);
    PeakQunatize(&LofarExpAvgOutput[CH_ID][0],&Lofar50KzEliminateOutput[0],1,1024)  ;
    Requantisation(CH_ID,0,&Lofar50KzEliminateOutput[0],&LofarQuantisedOutput[CH_ID][0],&SpectrumQuantisationTable[0],1024,350,SpectrumAmplitudeRes);
    ThresholdCheckFun(&LofarQuantisedOutput[CH_ID][0],CH_ID,1024);
    TrackCheckFun(&LofarQuantisedOutput[CH_ID][0],CH_ID,1024);
    SetLofarDisplayBuffer(ChannelID);
    }
    if(ChannelSelected==ChannelID)
    {
     LofarZoomProcessing(ChannelID,LofarZoomCursor,LofarBeatFreq,LofarZoomCursorStatus);
    }
}


void SignalProcessingClass::Lofar_Correction(float *Input,float *Output,int32_t Len)
{
    for(iCount=0;iCount<Len;iCount++)
    {
        Output[iCount]=(Input[iCount]-0.5);
    }
}


void SignalProcessingClass::SpectrumZoomProcessing(int16_t CH_ID,int16_t CursorType,float BeatFreq,bool Status)
{
    for(iCount=0;iCount<16384;iCount++)
     {
      SpectrumZoomBeatRealFreqencyBuffer[iCount]= (BaseInputData[iCount]*cos(2*3.14*BeatFreq*iCount*0.000032));
      SpectrumZoomBeatImagFreqencyBuffer[iCount]= (BaseInputData[iCount]*-sin(2*3.14*BeatFreq*iCount*0.000032));
     }
    FIRFilter(&SpectrumZoomFIR1[0],96,&SpectrumZoomBeatRealFreqencyBuffer[0],&SpectrumZoomFIR1RealFreqencyBuffer[0],16384);
    FIRFilter(&SpectrumZoomFIR1[0],96,&SpectrumZoomBeatImagFreqencyBuffer[0],&SpectrumZoomFIR1ImagFreqencyBuffer[0],16384);
    ComplexDecimation(&SpectrumZoomFIR1RealFreqencyBuffer[0],&SpectrumZoomFIR1ImagFreqencyBuffer[0],&SpectrumZoomDec1RealFreqencyBuffer[0],&SpectrumZoomDec1ImagFreqencyBuffer[0],4,16384);
    FIRFilter(&SpectrumZoomFIR2[0],188,&SpectrumZoomDec1RealFreqencyBuffer[0],&SpectrumZoomFIR2RealFreqencyBuffer[0],4096);
    FIRFilter(&SpectrumZoomFIR2[0],188,&SpectrumZoomDec1ImagFreqencyBuffer[0],&SpectrumZoomFIR2ImagFreqencyBuffer[0],4096);
    ComplexDecimation(&SpectrumZoomFIR2RealFreqencyBuffer[0],&SpectrumZoomFIR2ImagFreqencyBuffer[0],&SpectrumZoomDec2RealFreqencyBuffer[0],&SpectrumZoomDec2ImagFreqencyBuffer[0],4,4096);

    if(CursorType==244)
    {
    for(iCount=0;iCount<1024;iCount++)
    {
        SpectrumZoomAccRealBuffer[SpectrumZoomChannelIndex]=SpectrumZoomDec2RealFreqencyBuffer[iCount];
        SpectrumZoomAccImagBuffer[SpectrumZoomChannelIndex]=SpectrumZoomDec2ImagFreqencyBuffer[iCount];
        SpectrumZoomChannelIndex=(SpectrumZoomChannelIndex+1)%4096;
         if(SpectrumZoomChannelIndex==0)
         {
           SpectrumZoomSetFlag=true;
         }
         else
         {
           SpectrumZoomSetFlag=false;
         }

    }
    }

    if(CursorType==122)
    {
        ComplexDecimation(&SpectrumZoomDec2RealFreqencyBuffer[0],&SpectrumZoomDec2ImagFreqencyBuffer[0],&SpectrumZoomDec3RealFreqencyBuffer[0],&SpectrumZoomDec3ImagFreqencyBuffer[0],2,1024);
        for(iCount=0;iCount<512;iCount++)
        {
        SpectrumZoomAccRealBuffer[SpectrumZoomChannelIndex]=SpectrumZoomDec3RealFreqencyBuffer[iCount];
        SpectrumZoomAccImagBuffer[SpectrumZoomChannelIndex]=SpectrumZoomDec3ImagFreqencyBuffer[iCount];
        SpectrumZoomChannelIndex=(SpectrumZoomChannelIndex+1)%4096;
         if(SpectrumZoomChannelIndex==0)
         {
           SpectrumZoomSetFlag=true;
         }
         else
         {
           SpectrumZoomSetFlag=false;
         }

         }
    }
        if(SpectrumZoomSetFlag==true)
        {
        SpectrumZoomSetFlag=false;
       // WindowFunction(CH_ID,&SpectrumZoomAccRealBuffer[0],&SpectrumZoomAccImagBuffer[0],&SpectrumZoomWindowRealBuffer[0],&SpectrumZoomWindowImagBuffer[0],4096,4);
        FFT_Funtion(&SpectrumZoomAccRealBuffer[0],&SpectrumZoomAccImagBuffer[0],4096,4096,false);
        FFTMagnitudeExtraction(&SpectrumZoomAccRealBuffer[0],&SpectrumZoomAccImagBuffer[0],&SpectrumZoomMagnitudeBuffer[0],2048,6);
       // Exponential_Average(&SpectrumZoomMagnitudeBuffer[0],&SpectrumZoomAvgBuffer[0],4096,0.1120);
        Linear_Average(&SpectrumZoomMagnitudeBuffer[0],&SpectrumZoomAvgBuffer[0],SPEC_ZOOM_AVG_FACTOR,SpectrumZoomAvgCount,2048);
        SpectrumZoomAvgCount=(SpectrumZoomAvgCount+1)%SPEC_ZOOM_AVG_FACTOR;
        if(SpectrumZoomAvgCount==0)
        {
       //CaluclatePeakRatio(&SpectrumQuantisedOutput[CH_ID-1][0],3.81469,244,BeatFreq,4096,2);
       //PeakEnhancement(&SpectrumZoomAvgBuffer[0],&SpectrumZoomPeakEnhanceBuffer[0],4096);
      //Get_Peak_n_Peak_Position(&SpectrumZoomPeakEnhanceBuffer[0],2,4096);
      //Peak_Correction(&SpectrumZoomPeakEnhanceBuffer[0],&SpectrumZoomCorectBuffer[0],SpectrumZoom1PeakAmp,1,2048);
            PeakQunatize(&SpectrumZoomAvgBuffer[0],&SpectrumZoomCorectBuffer[0],1,2048);
            Requantisation(CH_ID,1,&SpectrumZoomCorectBuffer[0],&SpectrumZoomQuantisedOutput[0],&SpectrumQuantisationTable[0],1200,1,SpectrumAmplitudeRes);
        for(iCount=0;iCount<1100;iCount++)
         {
         SpectrumZoomDataPlot[iCount]=(double)sqrt(pow(SpectrumZoomQuantisedOutput[iCount],2));
    //printf("\n Iam in Zoom 2   %f  %lf", SpectrumMainZoomPeakAmp,SpectrumZoomDataPlot[iCount]);
         }
        SpectrumZoomEnable=true;
        }
       }

}

void SignalProcessingClass::LofarZoomProcessing(int16_t CH_ID,int16_t CursorType,float BeatFreq,bool Status)
{

    for(iCount=0;iCount<2048;iCount++)
     {
      LofarZoomBeatRealFreqencyBuffer[iCount]= (LofarDec2ZoomRealOutput[iCount]*cos(2*3.14*BeatFreq*iCount*0.000256));
      LofarZoomBeatImagFreqencyBuffer[iCount]= (LofarDec2ZoomRealOutput[iCount]*-sin(2*3.14*BeatFreq*iCount*0.000256));
      //printf("\n beate Freq=%f",BeatFreq);
     }

    FIRFilter(&LofarZoomFIR1[0],149,&LofarZoomBeatRealFreqencyBuffer[0],&LofarZoomFIR1RealFreqencyBuffer[0],2048);
    FIRFilter(&LofarZoomFIR1[0],149,&LofarZoomBeatImagFreqencyBuffer[0],&LofarZoomFIR1ImagFreqencyBuffer[0],2048);
    ComplexDecimation(&LofarZoomFIR1RealFreqencyBuffer[0],&LofarZoomFIR1ImagFreqencyBuffer[0],&LofarZoomDec1RealFreqencyBuffer[0],&LofarZoomDec1ImagFreqencyBuffer[0],2,2048);
    if(CursorType==200)
    {
     LftZoomValue=200;
    for(iCount=0;iCount<1024;iCount++)
    {
        LofarZoomAccRealBuffer[LofarZoomChannelIndex]=LofarZoomDec1RealFreqencyBuffer[iCount];
        LofarZoomAccImagBuffer[LofarZoomChannelIndex]=LofarZoomDec1ImagFreqencyBuffer[iCount];
        LofarZoomChannelIndex=(LofarZoomChannelIndex+1)%4096;
        if(LofarZoomChannelIndex==0)
        {
            LofarZoomSetFlag=true;
        }
        else
        {
            LofarZoomSetFlag=false;
        }
    }
    }

    if(CursorType==100)
    {
     LftZoomValue=100;
     ComplexDecimation(&LofarZoomDec1RealFreqencyBuffer[0],&LofarZoomDec1ImagFreqencyBuffer[0],&LofarZoomDec2RealFreqencyBuffer[0],&LofarZoomDec2ImagFreqencyBuffer[0],2,1024);
    for(iCount=0;iCount<512;iCount++)
    {
        LofarZoomAccRealBuffer[LofarZoomChannelIndex]=LofarZoomDec2RealFreqencyBuffer[iCount];
        LofarZoomAccImagBuffer[LofarZoomChannelIndex]=LofarZoomDec2ImagFreqencyBuffer[iCount];
        LofarZoomChannelIndex=(LofarZoomChannelIndex+1)%4096;
        if(LofarZoomChannelIndex==0)
        {
            LofarZoomSetFlag=true;
        }
        else
        {
            LofarZoomSetFlag=false;

        }
    }
    }

    if(CursorType==50)
    {
     LftZoomValue=50;
     ComplexDecimation(&LofarZoomDec1RealFreqencyBuffer[0],&LofarZoomDec1ImagFreqencyBuffer[0],&LofarZoomDec3RealFreqencyBuffer[0],&LofarZoomDec3ImagFreqencyBuffer[0],4,1024);
    for(iCount=0;iCount<256;iCount++)
    {
        LofarZoomAccRealBuffer[LofarZoomChannelIndex]=LofarZoomDec3RealFreqencyBuffer[iCount];
        LofarZoomAccImagBuffer[LofarZoomChannelIndex]=LofarZoomDec3ImagFreqencyBuffer[iCount];
        LofarZoomChannelIndex=(LofarZoomChannelIndex+1)%4096;
        if(LofarZoomChannelIndex==0)
        {
            LofarZoomSetFlag=true;
        }
        else
        {
            LofarZoomSetFlag=false;
        }
    }
    }

    if(LofarZoomSetFlag==true)
    {
      //WindowFunction(CH_ID,&LofarZoomAccRealBuffer[0],&LofarZoomAccImagBuffer[0],&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],4096,4);
        FFT_Funtion(&LofarZoomAccRealBuffer[0],&LofarZoomAccImagBuffer[0],4096,4096,false);
        FFTMagnitudeExtraction(&LofarZoomAccRealBuffer[0],&LofarZoomAccImagBuffer[0],&LofarZoomMagnitudeBuffer[0],2048,3);
        Exponential_Average(&LofarZoomMagnitudeBuffer[0],&LofarZoomAvgBuffer[0],2048,LOFZOOM_ALPHA);
      //Linear_Average(&LofarZoomMagnitudeBuffer[0],&LofarZoomAvgBuffer[0],LFT_ZOOM_AVG_FACTOR,LofarZoomAvgCount,2048);
        Split_Pass_Normaliser(&LofarZoomAvgBuffer[0],&LofarZoomNormalisedBuffer[0],2048,2,2);

        LofarZoomAvgCount=(LofarZoomAvgCount+1)%LFT_ZOOM_AVG_FACTOR;
        if(LofarZoomAvgCount==0)
        {
        // CaluclatePeakRatio(&LofarQuantisedOutput[CH_ID-1][0],1.907345,LftZoomValue,BeatFreq,1200,1);
        // PeakEnhancement(&LofarZoomNormalisedBuffer[0],&LofarZoomPeakEnhanceBuffer[0],1200);
        // Get_Peak_n_Peak_Position(&LofarZoomPeakEnhanceBuffer[0],5,1200);
        // Peak_Correction(&LofarZoomPeakEnhanceBuffer[0],&LofarZoomCorectBuffer[0],LofarNormalisedPeakAmp,1,1200);
         PeakQunatize(&LofarZoomNormalisedBuffer[0],&LofarZoomCorectBuffer[0],1,2048);
         Requantisation(CH_ID,0,&LofarZoomNormalisedBuffer[0],&LofarZoomQuantisedOutput[0],&SpectrumQuantisationTable[0],1200,500,SpectrumAmplitudeRes);

         for(iCount=0;iCount<1100;iCount++)
         {
          LofarZoomDataPlot[iCount]=(double)sqrt(pow(LofarZoomQuantisedOutput[iCount],2));
         }
          LofarZoomEnable = true;
        }
    }

/*    else if(CursorType==100)
    {
        FIRFilter(&LofarZoomFIR4[0],43,&LofarZoomDec1RealFreqencyBuffer[0],&LofarZoomFilter2RealFreqencyBuffer[0],1024);
        FIRFilter(&LofarZoomFIR4[0],43,&LofarZoomDec1ImagFreqencyBuffer[0],&LofarZoomFilter2ImagFreqencyBuffer[0],1024);
        ComplexDecimation(&LofarZoomFilter2RealFreqencyBuffer[0],&LofarZoomFilter2ImagFreqencyBuffer[0],&LofarZoomDec3RealFreqencyBuffer[0],&LofarZoomDec3ImagFreqencyBuffer[0],2,1024);
        for(iCount=0;iCount<512;iCount++)
        {
            LofarZoom2AccRealBuffer[LofarZoomChannelIndex]=LofarZoomDec3RealFreqencyBuffer[iCount];
            LofarZoom2AccImagBuffer[LofarZoomChannelIndex]=LofarZoomDec3ImagFreqencyBuffer[iCount];
            LofarZoomChannelIndex=(LofarZoomChannelIndex+1)%4096;
            if(LofarZoomChannelIndex==0)
            {
                WindowFunction(CH_ID,&LofarZoom2AccRealBuffer[0],&LofarZoom2AccImagBuffer[0],&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],4096,2);
                FFT_Funtion(&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],4096,4096,false);
                FFTMagnitudeExtraction(&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],&LofarZoomMagnitudeBuffer[0],2048,1);
                Exponential_Average(&LofarZoomMagnitudeBuffer[0],&LofarZoomAvgBuffer[0],2048,LOFZOOM_ALPHA);
                LofarZoomAvgCount=(LofarZoomAvgCount+1)%4;
                if(LofarZoomAvgCount==0)
                {

                 Split_Pass_Normaliser(&LofarZoomAvgBuffer[0],&LofarZoomNormalisedBuffer[0],2048,2,2);
                 // CaluclatePeakRatio(&LofarZoomAvgBuffer[0],1.907,200,BeatFreq,2047,1);
                 // Get_Peak_n_Peak_Position(&LofarZoomAvgBuffer[0],7,1200);
                    Requantisation(CH_ID,7,&LofarZoomAvgBuffer[0],&LofarZoomQuantisedOutput[0],&SpectrumQuantisationTable[0],1200,250,SpectrumAmplitudeRes);

                for(iCount=0;iCount<1100;iCount++)
                 {
                 LofarZoomDataPlot[iCount]=(double)(LofarZoomQuantisedOutput[iCount]);
                 }
                  LofarZoomEnable=true;
                }
            }


        }
    }
    else
    {
        FIRFilter(&LofarZoomFIR5[0],143,&LofarZoomDec1RealFreqencyBuffer[0],&LofarZoomFilter3RealFreqencyBuffer[0],1024);
        FIRFilter(&LofarZoomFIR5[0],143,&LofarZoomDec1ImagFreqencyBuffer[0],&LofarZoomFilter3ImagFreqencyBuffer[0],1024);
        ComplexDecimation(&LofarZoomFilter3RealFreqencyBuffer[0],&LofarZoomFilter3ImagFreqencyBuffer[0],&LofarZoomDec4RealFreqencyBuffer[0],&LofarZoomDec4ImagFreqencyBuffer[0],4,1024);
        for(iCount=0;iCount<256;iCount++)
        {
            LofarZoom2AccRealBuffer[LofarZoomChannelIndex]=LofarZoomDec4RealFreqencyBuffer[iCount];
            LofarZoom2AccImagBuffer[LofarZoomChannelIndex]=LofarZoomDec4ImagFreqencyBuffer[iCount];

            LofarZoomChannelIndex=(LofarZoomChannelIndex+1)%4096;
            if(LofarZoomChannelIndex==0)
            {
                WindowFunction(CH_ID,&LofarZoom2AccRealBuffer[0],&LofarZoom2AccImagBuffer[0],&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],4096,2);
                FFT_Funtion(&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],4096,4096,false);
                FFTMagnitudeExtraction(&LofarZoomWindowRealBuffer[0],&LofarZoomWindowImagBuffer[0],&LofarZoomMagnitudeBuffer[0],2048,1);
                Exponential_Average(&LofarZoomMagnitudeBuffer[0],&LofarZoomAvgBuffer[0],2048,LOFZOOM_ALPHA);
                LofarZoomAvgCount=(LofarZoomAvgCount+1)%4;
                if(LofarZoomAvgCount==0)
                {

                 Split_Pass_Normaliser(&LofarZoomAvgBuffer[0],&LofarZoomNormalisedBuffer[0],2048,2,2);
                 //CaluclatePeakRatio(&LofarZoomAvgBuffer[0],1.907,200,BeatFreq,2047,1);
                 //Get_Peak_n_Peak_Position(&LofarZoomAvgBuffer[0],7,1200);
                    Requantisation(CH_ID,7,&LofarZoomAvgBuffer[0],&LofarZoomQuantisedOutput[0],&SpectrumQuantisationTable[0],1200,250,SpectrumAmplitudeRes);

                for(iCount=0;iCount<1100;iCount++)
                 {
                 LofarZoomDataPlot[iCount]=(double)(LofarZoomQuantisedOutput[iCount]);
                 }
                  LofarZoomEnable=true;

                }
            }

        }
    }


*/



}

void SignalProcessingClass::DemonProcessing(int16_t CH_ID)
{
//   FIRFilter(&SpectrumFIR1[0],69,&BaseInputData[0],&SpectrumFilterOutput[0],16384);
    //ComplexFunction(&SpectrumFilterOutput[0],&SpectrumFilterRealOutput[0],&SpectrumFilterImagOutput[0],16384);
    FFT_Funtion(&SpectrumFilterRealOutput[0],&SpectrumFilterImagOutput[0],8192,8192,false);
    FFTMagnitudeExtraction(&SpectrumFilterRealOutput[0],&SpectrumFilterImagOutput[0],&SpectrumMagnitudeOutput[0],4096,1);
    SetDemonDisplayBuffer(ChannelID);
}



void SignalProcessingClass::SetOctaveDisplayBuffer(int16_t CH_ID)
{
    float Value;
    for(iCount=0;iCount<31;iCount++)
    {
        //Value=((186.5-1)+(20*log10(OctaveExpAvgOutput[CH_ID-1][iCount])));
        Value=OctaveExpAvgOutput[CH_ID-1][iCount];
       //printf("\n  %f",SpectrumMagnitudeOutput[i]);
         OctaveMainDataPlot[CH_ID-1][iCount]=(double)sqrt(pow(Value,1.4));
        //printf("\n%d  %lf",iCount, OctaveMainDataPlot[CH_ID-1][iCount]);
    }
}


void SignalProcessingClass::SetSpectrumDisplayBuffer(int16_t CH_ID)
{
    for(iCount=0;iCount<1100;iCount++)
    {
         SpectrumMainDataPlot[CH_ID-1][iCount]=(double)sqrt(pow(SpectrumQuantisedOutput[iCount],2));
    }
}

void SignalProcessingClass::delSetSpectrumDisplayBuffer(int16_t CH_ID)
{
    for(iCount=0;iCount<1100;iCount++)
    {
         DelSpectrumMainDataPlot[CH_ID-1][iCount]=(double)sqrt(pow(DelSpectrumQuantisedOutput[iCount],2));
    }
}




void SignalProcessingClass::SetLofarDisplayBuffer(int16_t CH_ID)
{
    for(iCount=0;iCount<1024;iCount++)
    {
       //printf("\n  %f",SpectrumMagnitudeOutput[i]);
         LofarMainDataPlot[CH_ID-1][iCount]=(double)sqrt(pow(LofarQuantisedOutput[CH_ID-1][iCount],2));
        // printf("\n %d %lf",CH_ID,MainDataPlot[iCount]);
    }
}

void SignalProcessingClass::SetDemonDisplayBuffer(int16_t CH_ID)
{
    for(iCount=0;iCount<1100;iCount++)
    {
       // printf("\n  %f",SpectrumMagnitudeOutput[i]);
        // DemonMainDataPlot[iCount]=(double)SpectrumMagnitudeOutput[iCount];
        // printf("\n %d %lf",CH_ID,MainDataPlot[iCount]);
    }
}
void SignalProcessingClass::FIRFilter(const float *coeffs,int filterLength,float *input,float *output,int32_t length)
{
   /* int j, k;
    float y=0.0, Reg[NumTaps];  // This assumes <= 256 taps.
    for(j=0; j<NumTaps; j++)Reg[j] = 0.0; // Init the delay registers.
    for(j=0; j<NumSigPts; j++)
    {
     // Shift the register values down and set Reg[0].
     for(k=NumTaps; k>1; k--)Reg[k-1] = Reg[k-2];
     Reg[0] = Signal[j];

     y = 0.0;
     for(k=0; k<NumTaps; k++)y += FiltCoeff[k] * Reg[k];
     FilteredSignal[j] = y;
    }

    */
        int32_t BufferLength;
        BufferLength=((filterLength-1)+length);
        float insamp[BufferLength];
        float acc;     // accumulator for MACs
        const float *coeffp; // pointer to coefficients    // FIR Filter Equation Y(k)=Sigma(k=0 to N-1)((h(k)*x(n-k)) where N=Filter Length,n=Input sample instant.
        float *inputp; // pointer to input samples
        int32_t n;
        int32_t k;
        // put the new samples at the high end of the buffer
        memcpy(&insamp[filterLength - 1],input,length * sizeof(float) );

        // apply the filter to each input sample
        for ( n = 0; n < length; n++ )                          //Outer Loop
        {
            // calculate output n
            coeffp = coeffs;
            inputp = &insamp[filterLength - 1 + n];
            acc = 0;
            for ( k = 0; k < filterLength; k++ )
            {
                acc += (*coeffp++) * (*inputp--);              //Inner Loop
            }
            output[n] = acc;
        }
      //shift input samples back in time for next time
        memmove( &insamp[0], &insamp[length],(filterLength - 1) * sizeof(float) );
}


void SignalProcessingClass::FFT_Funtion(float *fft_real, float *fft_img, quint32 samples, quint32 fft_window, bool rev)
{
int w;
qint32 NM1= fft_window-1;
qint32 ND2= fft_window/2;
qint32 M= (qint32) (log(fft_window)/log(2));
qint32 J=  ND2;
float TR;
float TI;
qint32 LE=0;
qint32 LE2= 0;
float UR =1;
float UI=0;
float SR =0;

float SI= 0;

qint32 JM1=0;
qint32 IP= 0;

qint32 i;
qint32 L;
qint32 K;
//Bit Reversal
for (i= 1 ;i<= fft_window-2;i++)
{
    if((i <J))
    {
        TR=fft_real[J];
        TI= fft_img[J];
        fft_img[J]=fft_img[i];
        fft_real[J]= fft_real[i];
        fft_real[i]= TR;
        fft_img[i]=TI;
    }
    K=ND2;
    while (K<=J)
    {
        J=J-K;
        K=K/2;
    }
    J=J+K;
}
//M=10;

for(L= 1; L<=M;L++)
{
    LE= (qint32) pow(2,L);
    LE2= LE/2;
    UR =1;
    UI=0;
    SR = qCos(PI/LE2);
    SI= -qSin(PI/LE2);

    for(J=1;J<=LE2;J++)
    {
        JM1=J-1;
        for (i =JM1; i<=NM1;i+=LE)
        {
            qint32 IP= i+LE2;
            TR= fft_real[IP]*UR-fft_img[IP]*UI;
            TI= fft_real[IP]*UI+fft_img[IP]*UR;

            fft_real[IP]=fft_real[i]-TR;
            fft_img[IP]=fft_img[i]-TI;

            fft_real[i]= fft_real[i]+TR;
            fft_img[i]=fft_img[i]+TI;
        }
        TR=UR;
        UR=TR*SR-UI*SI;
        UI=TR*SI+UI*SR;
    }
}
//NORMALIZE

if(rev)
{
    qint32 normal =fft_window;
    for (i = 0 ;i<fft_window; i++)
    {
        fft_real[i]=fft_real[i]/normal;
        fft_img[i]=fft_img[i]/normal;
    }
}
//return 0;
}

void SignalProcessingClass::ComplexFunction(float *INPUT,float *RealPart,float *ImagPart,int16_t Len)
{
    for(iCount=0;iCount<Len;iCount++)
    {
        RealPart[iCount]=INPUT[iCount];     // Accumulation of 2k data
        ImagPart[iCount]=0;                 // Accumulation of 2k data
    }
}
void SignalProcessingClass::DCRemoval(float *RealInput,float *ImagInput,float *RealOut,float *ImagOut,int16_t Len)
{
    float RealAvg,ImagAvg,RealSum,ImagSum;
    RealSum=0;
    ImagSum=0;
    for(iCount=0;iCount<Len;iCount++)
    {
        RealSum=(RealSum+RealInput[iCount]);
        ImagSum=(ImagSum+ImagInput[iCount]);
    }
    RealAvg=(RealSum/Len);
    ImagAvg=(ImagSum/Len);
    for(iCount=0;iCount<Len;iCount++)
    {
        RealOut[iCount]=(RealInput[iCount]-RealAvg);
        ImagOut[iCount]=(ImagInput[iCount]-ImagAvg);
    }
}

void SignalProcessingClass::FFTMagnitudeExtraction(float *REAL_INPUT,float *IMAG_INPUT,float *OUTPUT,int16_t DATA_LENGTH,int16_t AmplitudeCorrectionFactor)
{
/*
    for(iCount=1;iCount<DATA_LENGTH;iCount++)
    {
    SpectrumIntermediate_=sqrt(pow(REAL_INPUT[iCount],2)+pow(IMAG_INPUT[iCount],2));
    OUTPUT[iCount]=((SpectrumIntermediate_*1.41412)/DATA_LENGTH)*AmplitudeCorrectionFactor;
   // OUTPUT[iCount]=pow(OUTPUT[iCount],2);
    }
    OUTPUT[0]=(OUTPUT[0])/DATA_LENGTH;
*/

 /*     for(iCount=1;iCount<DATA_LENGTH;iCount++)
      {
      SpectrumIntermediate_=sqrt(pow(REAL_INPUT[iCount],2)+pow(IMAG_INPUT[iCount],2));
      OUTPUT[iCount]=(AmplitudeCorrectionFactor*2*((SpectrumIntermediate_)/DATA_LENGTH));
//      printf("\n %f", OUTPUT[iCount]);
      }
      OUTPUT[0]=2*(OUTPUT[0]/(DATA_LENGTH-1));
*/
        for(iCount=0;iCount<DATA_LENGTH;iCount++)
        {
        SpectrumIntermediate_=AmplitudeCorrectionFactor*((sqrt(pow(REAL_INPUT[iCount],2)+pow(IMAG_INPUT[iCount],2))));
        OUTPUT[iCount]=(SpectrumIntermediate_)/(DATA_LENGTH)*AmplitudeCorrectionFactor;
        }
        OUTPUT[0]=(sqrt(pow(REAL_INPUT[0],2)+pow(IMAG_INPUT[0],2))/DATA_LENGTH);
}

void SignalProcessingClass::FFTOCTAVEMagnitudeExtraction(float *REAL_INPUT,float *OUTPUT,int16_t DATA_LENGTH,int16_t AmplitudeCorrectionFactor)
{
StartValue=0;
EndValue=0;
AVGVAL=0;
for(jCount=0;jCount<30;jCount++)
{
AVG=0;
StartValue=(int16_t)(LowerOctaveBand[jCount]/3.814);
EndValue=(int16_t)(UpperOctaveBand[jCount]/3.814);
AVGVAL=(EndValue-StartValue);

        for(iCount=StartValue;iCount<EndValue;iCount++)
        {
            AVG=(AVG+pow(REAL_INPUT[iCount],2));
        }
         OctaveDataValue=AVG/AVGVAL;
         OctaveDataValue=AVG;
         //  Buffer[i]=AVG/1;
         OUTPUT[jCount]=OctaveDataValue*500;
     //printf("\n =%d Dat=%f  Start=%d End=%d  %f",jCount,AVG,StartValue,EndValue,OUTPUT[jCount]);
}

}


void SignalProcessingClass::ComplexDecimation(float *InputRealData,float *InputImagData, float *OutputRealData,float *OutputImagData,int32_t DECIMATION_FACTOR,int32_t INPUT_DATA_LENGTH)
{
datalength_=(INPUT_DATA_LENGTH/DECIMATION_FACTOR);    // new data length

        for(iCount=0;iCount<datalength_;iCount++)
        {
        OutputRealData[iCount]= InputRealData[iCount*DECIMATION_FACTOR];
        OutputImagData[iCount]= InputImagData[iCount*DECIMATION_FACTOR];
       }
}


void SignalProcessingClass::Decimation(float *InputRealData, float *OutputRealData,int32_t DECIMATION_FACTOR,int32_t INPUT_DATA_LENGTH)
{
datalength_=(INPUT_DATA_LENGTH/DECIMATION_FACTOR);    // new data length

        for(iCount=0;iCount<datalength_;iCount++)
        {
        OutputRealData[iCount]= InputRealData[iCount*DECIMATION_FACTOR];
       }
}


    void SignalProcessingClass::EnergyProcessing(int16_t CH_ID)
    {
    EnergyExpAvg=0.0;

    FIRFilter(&EnergyFIR1[0],48,&BaseInputData[0],&EnergyInputData[0],16384);
    Decimation(&EnergyInputData[0],&EnergyOutputData[0],2,16384);

        for(iCount=0;iCount<8192;iCount++)
        {
            EnergyLogValue=(EnergyOutputData[iCount]*EnergyOutputData[iCount]);
            EnergyExpAvg=(EnergyExpAvg+EnergyLogValue);

        }
        EnergyAvgVal=(EnergyExpAvg/8192);
        EnergyData[CH_ID]=((EnergyAvgVal*0.90)+(EnergyData[CH_ID]*0.10));
      //  printf("\n channel Id=%d Energye Average=%f  AvegerageVAl=%f   %f",CH_ID+1,EnergyExpAvg,EnergyData[CH_ID],(EnergyData[CH_ID]));

        EnergyLogValue=(EnergyData[CH_ID]);
        EnergyAvgCount[CH_ID]=(EnergyAvgCount[CH_ID]+1)%5;
        if(EnergyAvgCount[CH_ID]==0)
        {
        EnergyQ=(EnergyData[CH_ID]*100);
       if(EnergyQ>100)
       {
          EnergyQ=95;
       }
       if(EnergyQ < 0)
       {
         EnergyQ=0;
       }
        EngQuantisationValue=EnergyQ;


        if(EngQuantisationValue > 90)
        {
            Energy_Histo_Pos[CH_ID]=6;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];
        }
        else if(EngQuantisationValue>80 && EngQuantisationValue<90)
        {
            Energy_Histo_Pos[CH_ID]=5;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];

        }
        else if(EngQuantisationValue>70 && EngQuantisationValue<80)
        {
            Energy_Histo_Pos[CH_ID]=4;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];


        }
        else if(EngQuantisationValue>50 && EngQuantisationValue<70)
        {
            Energy_Histo_Pos[CH_ID]=3;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];
        }
        else if(EngQuantisationValue>30 && EngQuantisationValue<50)
        {
            Energy_Histo_Pos[CH_ID]=2;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];

        }
        else if(EngQuantisationValue>5 && EngQuantisationValue<30)
        {
            Energy_Histo_Pos[CH_ID]=1;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];
        }
        else
        {
            Energy_Histo_Pos[CH_ID]=0;
            EnergyValue[CH_ID]=Energy_Histo_Pos[CH_ID];
        }
    }

    }


void SignalProcessingClass::Exponential_Average(float *pfAvrg_in,float *pfAvrg_out,int32_t LEN ,float fAlpha)
{
    float fBeta=(1-fAlpha);
    for (iCount=0;iCount<LEN;iCount++)
    {
        pfAvrg_out[iCount]=((fAlpha*pfAvrg_in[iCount])+(fBeta*pfAvrg_out[iCount]));
       //printf("\n %f ",pfAvrg_out[iCount]);
    }
}

void SignalProcessingClass::LowPassFreq_Average(float *pfAvrg_in,float *pfAvrg_out,int32_t LEN ,float CutOffRange,float Fs)
{
   float Rc,dt,Alpha;
   Rc=(1.0/(CutOffRange*2*3.14));
   dt= (1.0/Fs);
   Alpha=(dt/(Rc+dt));
   pfAvrg_in[0]=pfAvrg_out[0]=0;
    for (iCount = 1; iCount < LEN;iCount++)
    {
    pfAvrg_out[iCount] =  pfAvrg_out[iCount-1] + (Alpha * (pfAvrg_in[iCount]-pfAvrg_out[iCount-1] ));
    }
}

void SignalProcessingClass::PeakEnhancement(float *Input,float *Output,int16_t DataLen)
{
    for(iCount=1;iCount<DataLen;iCount++)
    {
        Output[iCount]=pow(Input[iCount],2);
       // Output[iCount]=Input[iCount];
      }
}

void SignalProcessingClass::Split_Pass_Normaliser(float *pfInput,float *pfOutput,int iCount,int iWindow_Size,int iGap)
 {
   int32_t i,j;
   int32_t iHalf_Gap = iGap  >> (1);
   int32_t iHalf_Window = iWindow_Size >> (1);
   float mean_in,mean_out [iCount + iGap + iWindow_Size] ,l,m,sp_out1 [iCount + iGap + iWindow_Size];
   float* pfInput1=pfInput-(iHalf_Gap + iHalf_Window);
//-------------- Calculation of the First Mean using the input data--------------------

   for (i = 0; i < (iCount + iGap + iWindow_Size) ;i++)
    {
           //-------  summation of input data values on the left side of the gap------------------
           m = 0;
           for (j =  i - (iHalf_Gap + iHalf_Window); j < (i - iHalf_Gap); j++)
           {
               m = *(pfInput1 + j) +  m;
           }
           //-------  summation of input data values on the right side of the gap------------------
           for (j = i + iHalf_Gap + 1; j <= i + iHalf_Gap + iHalf_Window ; j++)
           {
             m = *(pfInput1 + j) +  m;
           }
           //-------  First Mean Calculation---------------------------------------------------
           mean_in = m / (iWindow_Size);

           //-----------Smoothening the input series by removing the values greater thn the mean
           if (mean_in > pfInput1[i] )
           {
                   sp_out1[i] = pfInput1[i];
           }
           else sp_out1[i] = mean_in;

     }


//-------------- Calculation of the Second Mean using the smoothened data--------------------
    for (i = 0; i < (iCount );i++)
    {
           //-------  summation of smoothened data values on the left side of the gap------------------
                       m = 0;
           for (j = i - (iHalf_Gap + iHalf_Window);j < i - iHalf_Gap; j++)
           {
               m = *(sp_out1 + j + iHalf_Gap + iHalf_Window) +  m;

           }
           //-------  summation of smoothened data values on the right side of the gap------------------
           for (j = i + iHalf_Gap + 1; j <= i + iHalf_Gap + iHalf_Window ; j++)
           {
             m = *(sp_out1 + j + iHalf_Gap + iHalf_Window) + m;
           }
           //-------  Second Mean Calculation------------------
          mean_out[i] = m / (iWindow_Size);

    }
//-----------Normalised output is the difference between the input and Second Mean

     for (i = 0; i < (iCount);i++)
     {
         pfOutput[i ] = *(pfInput + i) - mean_out[i];
       //pfOutput[i ] = sqrt(pfOutput[i ]);
     }
 }


void SignalProcessingClass::WindowFunction(int16_t CH_ID, float *InputReal, float *InputImag, float *OutputReal, float *OutputImag,int16_t SAMPLE_NO, int8_t WindowID )
{
yCount=(SAMPLE_NO-1);
float multiplier;
    switch(WindowID)
    {
    case 1: for(iCount=0; iCount<SAMPLE_NO;iCount++)
            {
            multiplier = 0.5 * (1 - cos(2*PI*iCount/yCount));
            OutputReal[iCount] = multiplier * InputReal[iCount];                //HAN_WINDOW Function
            OutputImag[iCount] = multiplier * InputImag[iCount];

            }
            break;
    case 2:
            for(iCount=0; iCount<SAMPLE_NO;iCount++)
             {
               multiplier = 0.54-0.46*cos(2*PI*iCount/yCount);
               OutputReal[iCount] = multiplier * InputReal[iCount];                //HAMMING_WINDOW Function
               OutputImag[iCount] = multiplier * InputImag[iCount];

              }

            break;
    case 3:    for(iCount=0; iCount<SAMPLE_NO;iCount++)
                {
                 multiplier = 0.62-(0.48*(iCount/yCount)-0.5)-(0.38*cos(2*PI*iCount/yCount));
                 OutputReal[iCount] = multiplier * InputReal[iCount];                //Barlet HANN WINDOW Function
                 OutputImag[iCount] = multiplier * InputImag[iCount];
                }

              break;
    case 4:
             for(iCount=0; iCount<SAMPLE_NO;iCount++)
              {
                 multiplier =( 0.358-(0.488*cos(2*PI*iCount/yCount))+(0.141*cos(4*PI*iCount/yCount))-(0.011*cos(6*PI*iCount/yCount)));
               OutputReal[iCount] = multiplier * InputReal[iCount];                //Blackman Harris WINDOW Function
               OutputImag[iCount] = multiplier * InputImag[iCount];
              }
             break;
    default: break;
    };


}


void SignalProcessingClass::Peak_Correction(float *pfInput,float *Output,float Max_Value,int8_t ID, int32_t DataLen)
{

   /*
    if(ID==2)
    {
        if(SpectrumZoom1PeakAmp>ADC_Highest_Value)
        {
           CorrectionValue=SpectrumZoom1PeakAmp/ADC_Highest_Value;
        }
        else
        {
             CorrectionValue=ADC_Highest_Value/SpectrumZoom1PeakAmp;
        }

       for (iCount=0;iCount <DataLen;iCount++)
       {
           Output[iCount]=pfInput[iCount]*CorrectionValue;
       }
    }
    */

    for (iCount=0;iCount <DataLen;iCount++)
    {
         Output[iCount]=(pfInput[iCount]/Max_Value);
    }


}

void SignalProcessingClass::PeakQunatize(float *pfInput,float *Output,int8_t ID, int32_t DataLen)
{

    for (iCount=0;iCount<DataLen;iCount++)
    {
        if(fPeak_Value < pfInput[iCount])
        {
                fPeak_Value = pfInput[iCount];
                Index=iCount;
        }
    }
// printf("\n Highest Peak=%f",fPeak_Value);
    for (iCount=0;iCount<DataLen;iCount++)
    {
            Output[iCount] = (pfInput[iCount]/fPeak_Value);
    }

}


void SignalProcessingClass::Get_Peak_n_Peak_Position(float *pfInput,int8_t ID, int32_t DataLen)
{
            Index=0;
            fPeak_Value=0;
            if(ID==1)
            {
                for (iCount=1;iCount <DataLen;iCount++)
                {
                    if(fPeak_Value < pfInput[iCount])
                    {
                            fPeak_Value = pfInput[iCount];
                            Index=iCount;
                    }
                }

             SpectrumPeakAmp=fPeak_Value;
  //printf("\n Spectrum  Peak Value=%f %d  %f Position=%d",fPeak_Value,8388608,(8388608/fPeak_Value),Index);
            }
           else if(ID==2)
            {
                for (iCount=0;iCount <DataLen;iCount++)
                {
                    if(fPeak_Value < pfInput[iCount])
                    {
                            fPeak_Value = pfInput[iCount];
                            Index=iCount;
                    }
                }

             SpectrumZoom1PeakAmp=fPeak_Value;
 //printf("\n Spectrum Zoom1 Peak Value=%f Position=%d",fPeak_Value,Index);
            }
           else if(ID==3)
            {

                for (iCount=1;iCount <DataLen;iCount++)
                {
                    if(fPeak_Value < pfInput[iCount])
                    {
                            fPeak_Value = pfInput[iCount];
                            Index=iCount;
                    }
                }

             SpectrumZoom2PeakAmp=fPeak_Value;
//printf("\n Spectrum Zoom2 Peak Value=%f Position=%d",fPeak_Value,Index);
            }
           else if(ID==4)
            {
                for (iCount=8;iCount <DataLen;iCount++)
                {
                    if(fPeak_Value < pfInput[iCount])
                    {
                            fPeak_Value = pfInput[iCount];
                            Index=iCount;
                    }
                }

             LofarPeakAmp=fPeak_Value;
 //printf("\n Lofar Peak Value=%f Position=%d \t",LofarPeakAmp,Index);
            }
            else if(ID==5)
            {

                for (iCount=4;iCount <DataLen;iCount++)
                {
                    if(fPeak_Value < pfInput[iCount])
                    {
                            fPeak_Value = pfInput[iCount];
                            Index=iCount;
                    }
                }

             LofarNormalisedPeakAmp=fPeak_Value;
   //printf("\n Lofar Peak Value=%f Position=%d \t",LofarNormalisedPeakAmp,Index);
            }
            else
            {
            }
       // *(piPeak_Index) = index;
       // return(fPeak_Value);
    }

void SignalProcessingClass::Requantisation(int16_t CH_ID,int16_t PageID,float *fInput_address ,int16_t *nOutput_address,float *fLookup_table_address,int16_t nData_count,float Amp,float Res)
     {

float Ratio=0;
Ratio=Amp/250;

 if(CH_ID<29)
    {
    for(iCount=0;iCount<nData_count;iCount++)
    {
        nOutput_address[iCount]=(fInput_address[iCount]*500);
    }
    }
    else
    {
        for(iCount=0;iCount<nData_count;iCount++)
        {
            nOutput_address[iCount]=(fInput_address[iCount]*500*2);
        }
/*
int16_t Buffer[nData_count];
int16_t h;
float Value;
for(iCount=0;iCount<nData_count;iCount++)
{
    Value=(fInput_address[iCount]*16384);
    Buffer[iCount]=(int16_t)(Value);

    for(int uCount=1;uCount<500;uCount++)
    {
        if((Buffer[iCount]>SpectrumQuantisationTable2[uCount-1]) && (Buffer[iCount]<SpectrumQuantisationTable2[uCount]))
        {
        nOutput_address[iCount]=uCount-1;
        break;
        }
    }
}

*/
 }


   }
void SignalProcessingClass::Linear_Average(float *Input,float *Output,int16_t AvgFact,int16_t Count,int32_t DataLen)
{
    if((Count+1)==AvgFact)
    {
        for(iCount=0;iCount<DataLen;iCount++)
        {
            Output[iCount]=(Input[iCount]/AvgFact);
        }
    }
    else
    {
     for(iCount=0;iCount<DataLen;iCount++)
     {
        Output[iCount]=(Input[iCount]+Output[iCount]);
     }
    }
}

void SignalProcessingClass::CaluclatePeakRatio(int16_t *Input,float Res,int16_t CursorType ,float BeatFreq,int16_t DataLen,int16_t ID)
{
    if(ID==1)
    {
StartPos=(int16_t)(BeatFreq/Res);
EndPos=(int16_t)((BeatFreq+CursorType)/Res);
SampleLen=EndPos-StartPos;

fPeak_Value=Input[StartPos];
Index=0;
for (iCount=StartPos;iCount<EndPos;iCount++)
{
    if(  Input[iCount]>fPeak_Value)
    {
            fPeak_Value = Input[iCount];
            Index=iCount;
    }
}
LofarMainZoomPeakAmp=fPeak_Value;
    }
    else
    {
        StartPos=(int16_t)(BeatFreq/Res);
        EndPos=(int16_t)((BeatFreq+CursorType)/Res);
        SampleLen=EndPos-StartPos;

        fPeak_Value=Input[StartPos];
        Index=0;
        for (iCount=StartPos;iCount<EndPos;iCount++)
        {
            if(  Input[iCount]>fPeak_Value)
            {
                    fPeak_Value = Input[iCount];
                    Index=iCount;
            }
        }
  SpectrumMainZoomPeakAmp=fPeak_Value;
    }
//printf("\n BeatFeq=%f Pos=%d End%d  SampleLen %d  Index %d PeakValue %f",BeatFreq,StartPos,EndPos,SampleLen,Index,fPeak_Value);
}

void SignalProcessingClass::Eliminate50Hz(float *Input,float *Output,int16_t ID,int16_t DataLen)
{
    fPeak_Value=0;
    if(ID==1)
    {
        for (iCount=0;iCount<DataLen;iCount++)
        {
            Output[iCount]=Input[iCount];
        }
        for (iCount=25;iCount<28;iCount++)
        {
             if(Output[iCount]>0.4)
             {
              Output[iCount]=0.20;
             }
        }
    }
    else
    {
        for (iCount=0;iCount<DataLen;iCount++)
        {
            Output[iCount]=Input[iCount];
        }

        for (iCount=12;iCount<15;iCount++)
        {
             if(Output[iCount]>0.4)
             {
              Output[iCount]=0.20;
             }
        }
//printf("\n Spectrum Peak Position=%d PeakValus %f",Index,fPeak_Value);
    }
//printf("\n BeatFeq=%f Pos=%d End%d  SampleLen %d  Index %d PeakValue %f",BeatFreq,StartPos,EndPos,SampleLen,Index,fPeak_Value);
}



void SignalProcessingClass::TrackCheckFun(int16_t *DataInput,int16_t CH_ID,int16_t DATA_LEN)
{
    for(xCount=0;xCount<64;xCount++)
    {
     if((Track_Data[xCount].Frequency>1200))
     {
       Track_Data[xCount].Frequency=0;
       Track_Data[xCount].Amplitude=0;
       Track_Data[xCount].TrackStatus=false;
     }

     if((Track_Data[xCount].TrackStatus==true)&&(Track_Data[xCount].Frequency>2))
    {
     TrackStatusFlag=false;
     fCount=Track_Data[xCount].Frequency;
     aCount=Track_Data[xCount].Amplitude;
     TrackFreqData=(int16_t)(fCount/1.9073);
     TrackAmpData=(int16_t)(aCount/4);
     TrackStartBin=TrackFreqData-2;
     TrackEndBin=TrackFreqData+2;

     CaluclateTrack(DataInput,TrackStartBin,TrackEndBin,TrackAmpData);

     if(TrackStatusFlag==true)
     {
         Track_Data[xCount].CH_NO[CH_ID]=1;
         if(Track_Data[xCount].INT_CH_NO==CH_ID)
         {
         Track_Data[xCount].TrackStatus=true;
         Track_Data[xCount].TrackMiss=false;
         Track_Data[xCount].TrackEnable=true;
         }
//printf("\n Track On Track=%d Ch_Id=%d Status=%d",xCount,CH_ID,Track_Data[xCount].TrackStatus);
     }
     else
     {
         Track_Data[xCount].CH_NO[CH_ID]=0;
         if((Track_Data[xCount].INT_CH_NO==CH_ID) && (Track_Data[xCount].TrackStatus==true))
         {
         Track_Data[xCount].Frequency=0.0;
         Track_Data[xCount].Amplitude=0.0;
         Track_Data[xCount].TrackStatus=false;
         Track_Data[xCount].TrackMiss=true;
         Track_Data[xCount].TrackEnable=false;
         }
//printf("\n Second Track Miss TrackMis=%d Ch_Id=%d MissStatus=%d",xCount,CH_ID,Track_Data[xCount].TrackMiss);
    }
}
//printf("\n Doing Ch=%d %d Track Miss=%d",xCount,CH_ID,Track_Data[xCount].TrackMiss);
    }
}

void SignalProcessingClass::ThresholdCheckFun(int16_t *DataInput,int16_t CH_ID,int16_t DATA_LEN)
{
    for(xCount=0;xCount<8;xCount++)
    {
     if((Threshold.CH_NO_Freq[CH_ID][xCount]>1200))
     {
       Threshold.CH_NO_Freq[CH_ID][xCount]=0;
       Threshold.CH_NO_Amp[CH_ID][xCount]=0;
       Threshold.Init_CH_No[CH_ID][xCount]=false;
     }
     else if((Threshold.CH_NO_Freq[CH_ID][xCount]<0))
     {
         Threshold.CH_NO_Freq[CH_ID][xCount]=0;
         Threshold.CH_NO_Amp[CH_ID][xCount]=0;
         Threshold.Init_CH_No[CH_ID][xCount]=false;
     }
    else
     {
     if((Threshold.Init_CH_No[CH_ID][xCount]==true) && (Threshold.CH_NO_Freq[CH_ID][xCount]>2))
    {
     ThresholdFlag=false;
     ftCount=Threshold.CH_NO_Freq[CH_ID][xCount];
     atCount=Threshold.CH_NO_Amp[CH_ID][xCount];
     ThresholdFreqData=(int16_t)(ftCount/1.9073);
     ThresholdAmpData=(int16_t)(atCount);
     ThresholdStartBin=ThresholdFreqData-2;
     ThresholdEndBin=ThresholdFreqData+2;

     CaluclateThreshold(DataInput,ThresholdStartBin,ThresholdEndBin,ThresholdAmpData);

     if(ThresholdFlag==true)
     {
         Threshold.Init_CH_No[CH_ID][xCount]=true;
         Threshold.ThresholdWarningFlag[CH_ID][xCount]=true;
         Threshold.ThresholdWarningFlagEnable[CH_ID][xCount]=true;
 //printf("\n CH_ID=%d Track No=%d Threshold Set=%f Threshold Set=%d ",CH_ID,xCount,Threshold.CH_NO_Freq[CH_ID][xCount],Threshold.ThresholdWarningFlag[CH_ID][xCount]);
     }
     else
     {
   Threshold.ThresholdWarningFlag[CH_ID][xCount]=false;
      }

    }
    }
    }

}

void SignalProcessingClass::CaluclateTrack(int16_t *Input,int16_t StartPos,int16_t Endpos,int16_t TrackValue)
{
for (iCount=StartPos;iCount<Endpos;iCount++)
{
    if(TrackValue<Input[iCount])
    {
    TrackStatusFlag=true;
    }
}
}

void SignalProcessingClass::CaluclateThreshold(int16_t *Input,int16_t StartPos,int16_t Endpos,int16_t ThresholdValue)
{
for (iCount=StartPos;iCount<Endpos;iCount++)
{
    if(Input[iCount]>ThresholdValue)
    {
            ThresholdFlag = true;
    }
}
}


/*
void SignalProcessingClass::convolve (const float *p_coeffs, int p_coeffs_n,float *p_in, float *p_out, int n)
{
  int i, j, k;
  float tmp;

  for (k = 0; k < n; k++)   //position in output
  {
    tmp = 0;
    for (i = 0; i < p_coeffs_n; i++)  //position in coefficients array
    {
      j = k - i;  //  position in input
      if (j >= 0)  //  bounds check for input buffer
      {
        tmp += p_coeffs [k] * p_in [j];
      }
    }
    p_out [i] = tmp;
    //printf("\n %f",p_out [i]);
  }
}
*/

