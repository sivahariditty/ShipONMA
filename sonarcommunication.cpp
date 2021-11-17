#include "sonarcommunication.h"
#include"DataTypes.h"

FILE *fp,*fs;


ISSNavDataPacketType *nav_pac_struct;
FdsBriefHealthPacketType *stfdsHealthData;
OasConfigPacketType *oasdata;
ActiveConfigPacketType *Activedata;
UcsConfigPacketType *Ucsdata;
SystemModeConfigPacketType *Sysmoddata;
NetworkPacketHeaderType *SonarHeader;
SonarInterface Sonarint;
int16_t RecordStartFlag;
int16_t RecordStartFlagCont;
int16_t RecordStopFlagCont;
int IsPtrSet = 0;
int16_t RecordCompFlag;
int16_t RecordCompFlagCont;
extern int16_t RecordProgressBar;
extern QString ContRecFileName;
extern int ContRecSelCh;

 Recording Records[REC_DATA_SEGLEN];
 long int *RecordsPtr;
 SonarDataStructure SonarData;
 extern RecordingControls RecControl;
 int16_t ReplayCompFlag;
 int16_t ReplayStartFlag;

 extern ControlData Controls[NO_OF_SENSOR];
 extern DelayedSpectrum_Data Del_Controls[NO_OF_SENSOR];
 int16_t RecordSetFlag;
 int16_t RecordSetFlagCont;

SonarCommunication::SonarCommunication()
{
    qint32 ESI_SOCK;
    qint16 port2=(52102);      //Data Server;
    qint16 port3=(52115);      //Data Server;
    qint16 port4=(55555);      //Data Server;
    QHostAddress ESIgroup;
    ESIgroup.setAddress("239.7.21.2");
    QHostAddress MyPCAdress;
    MyPCAdress.setAddress("10.7.21.61");
    QHostAddress SonarAddr;
    SonarAddr.setAddress("10.7.21.27");
    QHostAddress BrodcastRecpAddr;
    BrodcastRecpAddr.setAddress("10.7.21.255");
    socket2=new QUdpSocket(this);
    Sonarsocket=new QUdpSocket(this);
    Recsocket=new QUdpSocket(this);

    socket2->bind(QHostAddress::Any,port2,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint );
    Sonarsocket->bind(QHostAddress::Any,50061,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    Recsocket->bind(BrodcastRecpAddr,55555,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    group2.imr_multiaddr.s_addr=htonl(ESIgroup.toIPv4Address());
    group2.imr_interface.s_addr=htonl(MyPCAdress.toIPv4Address());

    ESI_SOCK= socket2->socketDescriptor();

    if(setsockopt(ESI_SOCK,SOL_IP,IP_ADD_MEMBERSHIP,(char *)&group2,sizeof(group2)) < 0)
    {
        perror(" \n Adding SONAR Communication ESI multicast group error \n");
        //exit(1);
    }
    else
    {
        printf(" \n Adding SONAR Communication ESI multicast group succuss \n");
    }

}

void SonarCommunication::PendingData2()
{




}

void SonarCommunication::NavDataControl()
{

     swapBuffer((const char *)&nav_pac_struct->stISSNavDataObject.stIssNavData.ulDateTimeMode,sizeof(IssNavDataType));
     SonarData.Year=(nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Year & 0x0F);
     sprintf(SonarData.Date,"%.2d/%.2d/%.2d",( nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Day),(nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Month),  SonarData.Year) ;
     sprintf(SonarData.Time,"%.2d:%.2d:%.2d",( nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Hour),(nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Minute),(nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Second)) ;
     sprintf(SonarData.WaterfallTime1,"%.2d:%.2d:%.2d",( nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Hour),(nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Minute),(nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Second)) ;

     SonarData.CurrentHour=nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Hour;
     SonarData.CurrentMin=nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Minute;
     SonarData.CurrentSec=nav_pac_struct->stISSNavDataObject.stIssNavData.stNavTime.Second;

     SonarData.ChangeHour=(SonarData.CurrentHour);
     SonarData.ChangeMin= (SonarData.CurrentMin-8)%60;
     SonarData.ChangeSec=(SonarData.CurrentSec);
     sprintf(SonarData.WaterfallTime2,"%.2d:%.2d:%.2d",( SonarData.ChangeHour),(SonarData.ChangeMin),(SonarData.ChangeSec)) ;
     SonarData.ChangeHour=(SonarData.CurrentHour);
     SonarData.ChangeMin= (SonarData.CurrentMin-16)%60;
     SonarData.ChangeSec=(SonarData.CurrentSec);
     sprintf(SonarData.WaterfallTime3,"%.2d:%.2d:%.2d",( SonarData.ChangeHour),(SonarData.ChangeMin),(SonarData.ChangeSec)) ;
     SonarData.ChangeHour=(SonarData.CurrentHour);
     SonarData.ChangeMin= (SonarData.CurrentMin-24)%60;
     SonarData.ChangeSec=(SonarData.CurrentSec);
     sprintf(SonarData.WaterfallTime4,"%.2d:%.2d:%.2d",( SonarData.ChangeHour),(SonarData.ChangeMin),(SonarData.ChangeSec)) ;

     if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulCourseOverGroundVld==1)
     {
       //  printf("\n iam in Course over ground=%.2f",nav_pac_struct->stISSNavDataObject.stIssNavData.fHeading);
         if(nav_pac_struct->stISSNavDataObject.stIssNavData.fCourseOverGround >= 100)
         {
           sprintf( SonarData.CourseValue,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fCourseOverGround);
         }
         else if(nav_pac_struct->stISSNavDataObject.stIssNavData.fCourseOverGround>= 10)
         {
            sprintf( SonarData.CourseValue,"0%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fCourseOverGround);
         }
         else
         {
            sprintf( SonarData.CourseValue,"00%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fCourseOverGround);

         }
     }
     else
     {
        // sprintf( SonarData.CourseValue,"%.2f",nav_pac_struct->stISSNavDataObject.stIssNavData.fCourseOverGround);

     }



     if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulSpeedOverGroundVld==1)
     {
         if(nav_pac_struct->stISSNavDataObject.stIssNavData.fSpeedOverGround >= 10)
         {
            sprintf(SonarData.SpeedValue,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fSpeedOverGround);
         }
         else
         {
             sprintf(SonarData.SpeedValue,"0%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fSpeedOverGround);
         }
     }
      else
       {
         //sprintf( SonarData.SpeedValue,"%s \n","000");
       }



     if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulDivingDepthVld==1)
     {
         if(nav_pac_struct->stISSNavDataObject.stIssNavData.fDivingDepth >= 100)
         {
          sprintf(SonarData.Diving_Depth,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fDivingDepth);
         }
        else if(nav_pac_struct->stISSNavDataObject.stIssNavData.fDivingDepth >= 10)
         {
          sprintf(SonarData.Diving_Depth,"0%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fDivingDepth);
         }
         else
         {

               sprintf(SonarData.Diving_Depth,"00%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fDivingDepth);

         }
     }

     else
     {
        // sprintf( SonarData.Diving_Depth,"%s \n","000");
     }



     if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulWaterSoundVelocityVld==1)
     {
         if(nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity>=1000)
         {
     sprintf(SonarData.SoundVelocity,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity);
         }
         else if(nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity>=100)
         {
     sprintf(SonarData.SoundVelocity,"0%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity);
         }
         else if(nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity>=10)
         {
     sprintf(SonarData.SoundVelocity,"00%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity);
         }
        else
         {
     sprintf(SonarData.SoundVelocity,"000%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity);
         }
       // printf("\n %.2f",nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity);
     }

     else
     {

     // sprintf( SonarData.SoundVelocity,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fWaterSoundVelocity);
     }


      if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulLatitudeVld==1)
      {
          if(nav_pac_struct->stISSNavDataObject.stIssNavData.fLatitude>=10)
          {
     sprintf(SonarData.Latitude,"%.2f.000 N \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLatitude);
          }

      else
      {
          sprintf(SonarData.Latitude,"0%.2f.000 N \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLatitude);
      }
    }

      if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulLatitudeVld==0)
      {
      // sprintf( SonarData.Latitude,"%s \n","xxx.xx");
      }





       //printf("\n %.2f",nav_pac_struct->stISSNavDataObject.stIssNavData.fLatitude);
       if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulLongitudeVld==1)
       {

           if(nav_pac_struct->stISSNavDataObject.stIssNavData.fLongitude>=100)
           {
      sprintf(SonarData.Longitude,"%.2f.000 E \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLongitude);
           }


           if(nav_pac_struct->stISSNavDataObject.stIssNavData.fLongitude>=10)
           {
      sprintf(SonarData.Longitude,"0%.2f.000 E \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLongitude);
           }

       else
       {
           sprintf(SonarData.Longitude,"00%.2f.000 E \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLongitude);
       }
       }
       if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulLongitudeVld==0)
       {
       //  sprintf( SonarData.Longitude,"%s \n","xxx.xx");
       }



       if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulLogSpeedVld==1)
       {


           if(nav_pac_struct->stISSNavDataObject.stIssNavData.fLogSpeed>= 100)
           {
            sprintf(SonarData.Logspeed,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLogSpeed);
           }
          else if(nav_pac_struct->stISSNavDataObject.stIssNavData.fLogSpeed >= 10)
           {
            sprintf(SonarData.Logspeed,"0%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLogSpeed);
           }
           else
           {
            sprintf(SonarData.Logspeed,"00%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fLogSpeed);
           }

         }

       if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulLogSpeedVld==0)
       {

        // sprintf( SonarData.Logspeed,"%s \n","xx");

       }



      if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulHeadingVld==1)
           {


               if(nav_pac_struct->stISSNavDataObject.stIssNavData.fHeading >= 100)
               {
                sprintf(SonarData.Heading,"%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fHeading);
               }
              else if(nav_pac_struct->stISSNavDataObject.stIssNavData.fDivingDepth >= 10)
               {
                sprintf(SonarData.Heading,"0%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fHeading);
               }
               else
               {

                     sprintf(SonarData.Heading,"00%.2f \n",nav_pac_struct->stISSNavDataObject.stIssNavData.fHeading);

               }
           }

      if(nav_pac_struct->stISSNavDataObject.stIssNavData.ulHeadingVld == 0)
           {
   //sprintf( SonarData.Heading,"%s \n","xxx");

           }
 //           }




      SonarData.NavFlag=1;

}
int SonarCommunication::swapBuffer(const char *data, int len)
{
    unsigned long *pulData = (unsigned long *)data;

    if (htons(0xAA55) == 0xAA55)
        return 0;
    else
    {

        for (unsigned int i = 0; i < len; i += 4)
        {
            *(pulData) = htonl(*(pulData));
            pulData ++;
            //printf("--------------- %d \n",*(pulData));

        }
    }



}

void SonarCommunication::Oasread()
{
   // printf(" \n ----------------------------------------------------------------oasdata................ \n\n");
   QByteArray oas1;
   oas1.resize(Recsocket->pendingDatagramSize());
   Recsocket->readDatagram(oas1.data(),oas1.size());
   SonarHeader=(NetworkPacketHeaderType*)oas1.data();
   Category1=ntohl(SonarHeader->ulCategory);
   Subcategory=ntohl(SonarHeader->ulSubItem);
   if((Category1==0x90) && (Subcategory==0x12))
   {
     oasdata=(OasConfigPacketType*)oas1.data();

    Sonarint.OAS_Health_Value=ntohl(oasdata->stOasConfigObject.stOasConfig.ulTransmission);
   //printf("  Entered in Active  Sector--------------------------------------------------- %x   %x\n",Category,Subcategory);
   }
    if(Category1==0x90 && (Subcategory==0x0D))
   {
   Activedata=(ActiveConfigPacketType*)oas1.data();
   Sonarint.Active_Health_Value=ntohl(Activedata->stActiveConfigObject.stActiveConfig.stActiveTxParam.ulPingStatus);
 //printf("  Entered in Active  Sector--------------------------------------------------- %d   %x\n",ntohl(Activedata->stActiveConfigObject.stActiveConfig.stActiveTxParam.ulPingStatus),Subcategory);

   }
   if(Category1==0x90 && (Subcategory==0x13))
   {
       Ucsdata=(UcsConfigPacketType*)oas1.data();
       Sonarint.Ucs_Health_Value=ntohl(Ucsdata->stUcsConfigObject.stUcsConfig.ulUcsXmnMode);
   }
   if(Category1==0x90 && (Subcategory==0x40))
   {
       Sysmoddata=(SystemModeConfigPacketType *)oas1.data();
       Sonarint.Sys_Mode_value=ntohl(Sysmoddata->stSystemModeConfigObject.stSystemModeConfig.ulSystemMode);
     //  printf("\n Sysmode=%d",Sonarint.Sys_Mode_value);

   }
   if(Category1==0xF02 && (Subcategory==0x01))
   {
     Quit();
   }
}

void SonarCommunication::Quit()
{
    // close();
    qApp->quit();
    system("poweroff");
}

void SonarCommunication::Sonardataready()
{
    QByteArray Sonar;
    Sonar.resize(Sonarsocket->pendingDatagramSize());
        Sonarsocket->readDatagram(Sonar.data(),Sonar.size());
        stfdsHealthData=(FdsBriefHealthPacketType *)Sonar.data();
        // printf("%d  data=\n",htonl(stfdsHealthData->stFdsBriefHealthData.ulSonarHealth));
        Sonarint.CICS_Health_Value=ntohl(stfdsHealthData->stFdsBriefHealthData.stExtAndCriticalSysHealth.ulCicsHealth);
        Sonarint.NAV_Health_Value=ntohl(stfdsHealthData->stFdsBriefHealthData.stExtAndCriticalSysHealth.ulNavHealth);
        Sonarint.CTD_Health_VALUE=ntohl(stfdsHealthData->stFdsBriefHealthData.stExtAndCriticalSysHealth.ulCtdHealth);
        //soanr_value=htonl(stfdsHealthData->stFdsBriefHealthData.ulSonarHealth);
        Sonarint.SONAR_Health_Value=htonl(stfdsHealthData->stFdsBriefHealthData.ulSonarHealth);
      //  printf(" data=   %d\n",Sonarint.CICS_Health_Value);
}



void SonarCommunication::run()
{
   yCount=0;
   DataBufferCount=0;
   RecordStartFlag=0;
   RecordStartFlagCont=0;
   RecordStopFlagCont=0;

   //SonarCommunication();
    while(1)
    {
    if(ReplayStartFlag==1)
    {
      ReplayDataFun(2);
    }
    else
    {
      //RecordDataFun();
      RecordDataFunCont();
      //stopRecCont();
    }

    }

}

void SonarCommunication:: RecordDataFun(){
   for(kCount=0;kCount<NO_OF_SENSOR;kCount++){
      if(RecordSetFlag==1){
         DataBufferCount=0;
         RecordSetFlag=0;
      }
      if((RecordStartFlag==1)){
         if(Controls[kCount].PostRecStatus[Controls[kCount].RearRecPointer]==true){
            Records[DataBufferCount].ChannelNo=kCount;
            Records[DataBufferCount].SegmentNo=DataBufferCount;
            for(iCount=0;iCount<16384;iCount++){
               DataPtr2=(Controls[kCount].RawDataIntBuffer[Controls[kCount].RearRecPointer][iCount]);
               Records[DataBufferCount].RecordDataBuffer1[iCount]=DataPtr2;
            }
            Controls[kCount].PostRecStatus[Controls[kCount].RearRecPointer]=false;
            Controls[kCount].RearRecPointer=(Controls[kCount].RearRecPointer+1)%RawDataBufferSize;
            RecordProgressBar=DataBufferCount;
            DataBufferCount=(DataBufferCount+1)%REC_DATA_SEGLEN;
            if(DataBufferCount==0){
               fp=fopen(RecControl.RecorderFilePath,"wb+");
               if(fp==NULL){
                  printf("\n File Creation Error") ;
               }
               fwrite(&Records,sizeof(Records),1,fp);
               fclose(fp);
               RecordStartFlag=0;
               RecordCompFlag=0;
            }
         }
      }
   }
}

void SonarCommunication:: RecordDataFunCont(){
      if(RecordSetFlagCont==1){
       DataBufferCountCont=0;
       RecordSetFlagCont=0;
      //fp=fopen(ContRecFileName.toStdString().c_str(),"wb+");
      RecFile.open(ContRecFileName.toStdString().c_str());
      }
      if((RecordStartFlagCont==1)){
         if(Controls[ContRecSelCh].PostRecStatus[Controls[ContRecSelCh].RearRecPointer]==true){
            for(iCount=0;iCount<16384;iCount++){
               DataPtr2=(Controls[ContRecSelCh].RawDataIntBuffer[Controls[ContRecSelCh].RearRecPointer][iCount]);
	       RecFile << DataPtr2 <<"\n";
            }
            Controls[ContRecSelCh].PostRecStatus[Controls[ContRecSelCh].RearRecPointer]=false;
            Controls[ContRecSelCh].RearRecPointer=(Controls[ContRecSelCh].RearRecPointer+1)%RawDataBufferSize;
            DataBufferCountCont=(DataBufferCountCont+1);
         }
      }
      if(DataBufferCountCont == 2048)
         stopRecCont();
}

void SonarCommunication::stopRecCont(){
   if(RecordStopFlagCont == 1){
      //fp=fopen(ContRecFileName.toStdString().c_str(),"wb+");
      if(fp==NULL){
         printf("\n File Creation Error") ;
      }
      //fwrite(RecordsPtr,DataBufferCountCont*16384*sizeof(int32_t),1,fp);
      RecFile.close();
      RecordStartFlagCont=0;
      RecordCompFlagCont=0;
      DataBufferCountCont=0;
      IsPtrSet = 0;
      RecordStopFlagCont = 0;
   }
}

void SonarCommunication:: ReplayDataFun(int opt){
   if(opt == 1){
      fs=fopen(RecControl.ReplayFilePath,"rb+");
      if(fs==NULL){
         printf("\n FILE NOT FOUND ......") ;
      }
      else{
         fread(&Records,sizeof(Records),1,fs);
         fclose(fs);
         RecordStartFlag=0;
         ReplayStartFlag=0;
      }
   }
   else{
      fs=fopen(RecControl.ReplayFilePath,"rb+");
      if(fs==NULL){
         printf("\n FILE NOT FOUND ......") ;
      }
      else{
         fread(&Records,sizeof(Records),1,fs);
         fclose(fs);
         RecordStartFlag=0;
         ReplayStartFlag=0;
      }
   }
}
