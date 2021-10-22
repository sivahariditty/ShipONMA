#include "issinfoclass.h"
extern SonarDataStructure SonarData;
HmiToFdsHealthRequest healthrequest;
HealthPacket stHealthPacket;
extern SonarInterface Sonarint;
extern Host_Control_pkt_t control_object;
UDPTRXData1 UDPTransmitData1;
extern RecordingControls RecControl;

ISSInfoClass::ISSInfoClass(QWidget *parent) :
    QMainWindow(parent)
{
    ISSInfoFrame=new QFrame();
    ISSInfoFrame=new QFrame();
    ISSInfoFrame->setMouseTracking(true);
    ISSInfoFrame->setGeometry(QRect(0,0,790,70));
    ISSInfoFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    ISSInfoFrame->setFrameShape(QFrame::StyledPanel);
    ISSInfoFrame->setFrameShadow(QFrame::Raised);
    ISSButtonFrame=new QFrame();
    ISSButtonFrame=new QFrame();
    ISSButtonFrame->setMouseTracking(true);
    ISSButtonFrame->setGeometry(QRect(0,70,790,40));
    ISSButtonFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    ISSButtonFrame->setFrameShape(QFrame::StyledPanel);
    ISSButtonFrame->setFrameShadow(QFrame::Raised);
    //TransmitSocket1=new QUdpSocket();
  //  BroadcastSenderAddress.setAddress("10.7.21.255");
   // TransmitSocket1->bind(QHostAddress::Any,TRANS_PORT_NO,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint );
     ISSInformationFun();
  //   SonarHealthStatusInit();
     //SonarRequestInit();

}

void ISSInfoClass::ISSInformationFun()
{

         QFont font;
        font.setPointSize(10);
         font.setPixelSize(12);
         date = QDate::currentDate();
         dateString = date.toString("dd/MM/yyyy");


       sprintf(SonarData.Date,"%s","01/01/15") ;

       sprintf(SonarData.Time,"%s","00:00:00") ;
    /*   StopWatchx = new QLabel(ISSInfoFrame);
       StopWatchx->setObjectName(QString::fromUtf8("label"));
       StopWatchx->setGeometry(QRect(354,10,63,16));
       StopWatchx->setText("00:00:00");
       StopWatchx->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
           "color: rgb(0,255,0);"));
       StopWatchx->setFont(font);

*/






      /* ONA =new QPushButton(ISSInfoFrame);
       ONA->setObjectName(QString::fromUtf8("NORMAL"));
       ONA->setGeometry(QRect(80,45,130,23));
       ONA->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170, 170)"));
       ONA->setFont(font);
       ONA->setText("ONA");*/
/*
       Start =new QPushButton(ISSInfoFrame);
       Start->setObjectName(QString::fromUtf8("NORMAL"));
       Start->setGeometry(QRect(330,30,51,23));
       Start->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170, 170)"));
       Start->setFont(font);
       Start->setText("Start");





       Stop =new QPushButton(ISSInfoFrame);
       Stop->setObjectName(QString::fromUtf8("NORMAL"));
       Stop->setGeometry(QRect(386,30,51,23));
       Stop->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170, 170)"));
       Stop->setFont(font);
       Stop->setText("Reset");  */
       ScreenShot =new QPushButton(ISSButtonFrame);
       ScreenShot->setObjectName(QString::fromUtf8("REPLAY"));
       ScreenShot->setGeometry(QRect(20,9,85,20));
       ScreenShot->setText(QApplication::translate("Frame", "Screenshot", 0, QApplication::UnicodeUTF8));
       ScreenShot->setStyleSheet(QString::fromUtf8("background-color: rgb(235,235,255);\n"
                                                   "color: rgb(0, 0, 0);"));
       ScreenShot->setFont(font);


       Datex = new QLabel(ISSButtonFrame);
       Datex->setObjectName(QString::fromUtf8("label"));
       Datex->setGeometry(QRect(700,7,85,16));
       Datex->setText(dateString);
       Datex->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                              "color: rgb(30,255,41);"));
       Datex->setFont(font);
       Timex = new QLabel(ISSButtonFrame);
       Timex->setObjectName(QString::fromUtf8("label"));
       Timex->setGeometry(QRect(620,7,60,16));
       Timex->setText("00:00:00");
       Timex->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
                                              "color: rgb(30,255,41);"));
       Timex->setFont(font);












    /*  HMI =new QPushButton(ISSButtonFrame);
      HMI->setObjectName(QString::fromUtf8("NORMAL"));
      HMI->setGeometry(QRect(380,8,61,23));
      HMI->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170, 170)"));
      HMI->setFont(font);
      HMI->setText("HMI");
*/
      /* ShutDown =new QPushButton(ISSButtonFrame);
       ShutDown->setObjectName(QString::fromUtf8("SHUTDOWN"));
       ShutDown->setGeometry(QRect(5,5,70,25));
       ShutDown->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170, 170)"));
       ShutDown->setFont(font);
       ShutDown->setText("CLOSE");
       ShutDown->hide();

*/
       KelImage=new QLabel(ISSInfoFrame);
       KelImage->setGeometry(QRect(600,10,250,50));
       strcat(RecControl.KELIMAGPath,"/kel1.jpg");

       QPixmap logo1(RecControl.KELIMAGPath);
       KelImage->setPixmap(logo1);
       KelImage->show();

       NPOLImage=new QLabel(ISSInfoFrame);
       NPOLImage->setGeometry(QRect(10,10,250,50));
       strcat(RecControl.NPOLIMAGPath,"/npol.jpg");

       QPixmap logo2(RecControl.NPOLIMAGPath);
       NPOLImage->setPixmap(logo2);
       NPOLImage->show();




       font.setFamily(QString::fromUtf8("Sans Serif"));
       font.setBold(true);
       font.setPixelSize(24);
       Analon= new QLabel(ISSInfoFrame);
       Analon->setGeometry(QRect(320,10,200,25));
       Analon->setObjectName(QString::fromUtf8("RELval"));
       Analon->setText("ANALON-SS");
       Analon->setStyleSheet(QString::fromUtf8("color: rgb(207,179,255);"));
       Analon->setFont(font);

      // font.setPointSize(10);
       //font.setPixelSize(12);


}

void ISSInfoClass::Update_Time_Clock()
{


    delayTime=QTime::currentTime();
    DelayTime = delayTime.toString("hh:mm:ss");
    Timex->setText(DelayTime);



}

void ISSInfoClass::UpdateISSInformations()
{
 /*
Datex->setText(SonarData.Date);
Timex->setText(SonarData.Time);
COx->setText( SonarData.CourseValue);
SPx->setText(SonarData.SpeedValue);
Dx->setText(SonarData.Diving_Depth);
Svx->setText(SonarData.SoundVelocity);
HDx->setText(SonarData.Heading);
LATx->setText(SonarData.Latitude);
LNGx->setText(SonarData.Longitude);
SWx->setText(SonarData.Logspeed);
HDx->setText(SonarData.Heading);



if(Sonarint.CICS_Health_Value == 1 )
{
    CISC->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0)"));
}
else if(Sonarint.CICS_Health_Value== 2)
{
  CISC->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
}
else if(Sonarint.CICS_Health_Value== 4)
{
  CISC->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,0)"));
}
 else if(Sonarint.CICS_Health_Value == 8)
{
  CISC->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));
}


if(Sonarint.NAV_Health_Value == 1 )
{
    NAV->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0)"));
}
else if(Sonarint.NAV_Health_Value == 2)
{
  NAV->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
}
else if(Sonarint.NAV_Health_Value == 4)
{
  NAV->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,0)"));
}

else if(Sonarint.NAV_Health_Value == 8)
{
  NAV->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));
}
if( Sonarint.CTD_Health_VALUE== 1 )
{
    CTD->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0)"));
}
else if( Sonarint.CTD_Health_VALUE == 2)
{
  CTD->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
}
else if( Sonarint.CTD_Health_VALUE == 4)
{
  CTD->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,0)"));
}
 else if( Sonarint.CTD_Health_VALUE == 8)
{
  CTD->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));
}
if( Sonarint.OAS_Health_Value== 2 )
{
    OAS->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
}
else if( Sonarint.OAS_Health_Value == 1)
{
  OAS->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));
}

if(Sonarint.Active_Health_Value==2)
{
  Active1->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
}
else if(Sonarint.Active_Health_Value==1)
{
  Active1->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));
}

if(Sonarint.Ucs_Health_Value==1)
{
  UCS->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));
}
 else if(Sonarint.Ucs_Health_Value==2)
{
  UCS->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
}


if(Sonarint.Sys_Mode_value==1)
{
    Normal1->setText("NORMAL");
   // if(Graph_plt1.ProcessMode==1)
    {
   //  signal.Normal_Data(signal.Normal,0);
    }

}
else if(Sonarint.Sys_Mode_value==2)
{
    Normal1->setText("ANALOG SNS");

   // Normal_menuB->addMenu(Normal_Menu)->setText("    &ANALOG SNS    ");
  //  if(Graph_plt1.ProcessMode==1)
    {
  //   signal.Normal_Data(signal.Normal,0);
    }


}
else if(Sonarint.Sys_Mode_value==4)
{
    Normal1->setText("DIGITAL SNS");
  //  Normal_menuB->addMenu(Normal_Menu)->setText("     &DIGITAL SNS     ");
  //  if(Graph_plt1.ProcessMode==1)
    {
 //    signal.Normal_Data(signal.Normal,0);
    }


}
else if(Sonarint.Sys_Mode_value==5)
{

    Normal1->setText("REPLAY");
   // Normal_menuB->addMenu(Normal_Menu)->setText("         &REPLAY         ");


   // if(Graph_plt1.ProcessMode==0)
    {
  //   signal.Normal_Data(signal.Record,1);
  //   Graph_plt1.ProcessMode=1;
    }


}
else if(Sonarint.Sys_Mode_value==6)
{
    Normal1->setText("TRAINING SIM");

  //  Normal_menuB->addMenu(Normal_Menu)->setText("   &TRAINING SIM    ");
  //  if(Graph_plt1.ProcessMode==1)
    {
   //  signal.Normal_Data(signal.Normal,0);
    }


}
else if(Sonarint.Sys_Mode_value==7)
{

    Normal1->setText("BEAM PATN CHECK");

   // Normal_menuB->addMenu(Normal_Menu)->setText("&BEAM PATN CHECK");
  //  if(Graph_plt1.ProcessMode==1)
    {
  //   signal.Normal_Data(signal.Normal,0);
    }


}

if( Sonarint.CavitationThresholdFlag==1)
{
    Cavitating->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0)"));

}
else
{
    Cavitating->setStyleSheet(QString::fromUtf8("background-color:rgb(170,170,170)"));

}

*/

}
void ISSInfoClass::SonarHealthRequest()
{
/*    yCount = 0;
    yCount = sendto (sockfd1, &healthrequest, sizeof ( HmiToFdsHealthRequest ),0, ( struct sockaddr * ) &servaddr1,sizeof ( struct sockaddr_in) );
    if(yCount>0)
    {
       // printf("\n SONAR Health request send OK");
    }
    else
    {
       // printf("\n SONAR Health request send NOT OK");

    }
    */

}
void ISSInfoClass::SonarHealthStatus()
{
    yCount = 0;
    yCount = sendto (sockfd, &stHealthPacket, sizeof ( HealthPacket ),0, ( struct sockaddr * ) &servaddr,sizeof ( struct sockaddr_in) );
    if(yCount>0)
    {
      //  printf("\n SONAR Data Health Status OK");
    }
    else
    {
       // printf("\n SONAR Data Health Status NOT OK");
    }

}
void ISSInfoClass::SonarRequestInit()
{
/*
    sockfd1 = socket ( PF_INET, SOCK_DGRAM, 0 );
    if ( sockfd1 < 0 )
    {
        printf ( "\nAP: FDS Socket Error.\n");
    }

    bzero (&servaddr1, sizeof (struct sockaddr_in));
    servaddr1.sin_family = PF_INET;
    servaddr1.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr1.sin_port = htons(FDS_PORT);
    const int on1 = 1;
    inet_pton (PF_INET, FDS_IP_ADDRESS, &servaddr1.sin_addr);
    if(setsockopt(sockfd1, SOL_SOCKET, SO_BROADCAST, (char *)&on1, sizeof(int)))
    {
        sockfd1 = 0;
    }
     //qDebug("Request ");
    healthrequest.stNetHeader.ulCategory=htonl(0xF3);
    healthrequest.stNetHeader.ulSubItem=htonl(0x01);
    healthrequest.stNetHeader.ulPktLen = htonl ( sizeof ( HmiToFdsHealthRequest ) );
    healthrequest.stNetHeader.ulSrc = htonl ( ONA_1 ); // /enter subsystem id
    healthrequest.stNetHeader.stDest.bit.bitDestId = htonl ( FDS );
    healthrequest.stNetHeader.stDest.bit.bitSubDestId = htonl(0);
    healthrequest.stNetHeader.ulProtocol = htonl ( 0x01 );
    healthrequest.stNetHeader.ulNoOfObj = htonl ( 1 );
    healthrequest.stNetHeader.ulNoOfFragments = htonl ( 1 );
    healthrequest.stNetHeader.ulFragmentNo = htonl ( 1 );
    healthrequest.stNetHeader.stFlag.ulVal = htonl ( 0xF );
    healthrequest.stNetHeader.ulAAAA5555 = htonl ( 0xAAAA5555 );
    healthrequest.stNetHeader.ulEndianess = htonl(2);
    healthrequest.stNetHeader.stDate.ucDay = htons(0x07);
    healthrequest.stNetHeader.stDate.ucMonth = htons(0x06);
    healthrequest.stNetHeader.stDate.uiYear = (0x15);
    healthrequest.stNetHeader.stTime.ucSecond = htonl(53);
    healthrequest.stNetHeader.stTime.ucMinute = htonl(47);
    healthrequest.stNetHeader.stTime.uiHour = htonl(17);
    healthrequest.stNetHeader.ulTimeMicroSec = htonl(0);
    healthrequest.stNetHeader.ulTimeSpare = htonl(0);
    healthrequest.stNetHeader.ulVersion = htonl(1.1);
    healthrequest.stNetHeader.ulChecksum = htonl(0xDEADBEEF);
     //printf(" /n  category=%d : \n",ntohl(healthrequest.stNetHeader.ulCategory));

*/
}

void ISSInfoClass::SonarHealthStatusInit()
{


/*
    sockfd = socket ( PF_INET, SOCK_DGRAM, 0 );
    if ( sockfd < 0 )
    {
        printf ( "\nAP: FDS Socket Error.\n");

    }

    bzero (&servaddr, sizeof (struct sockaddr_in));
    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (FDS_PORT);
    const int on1 = 1;
    inet_pton (PF_INET, FDS_IP_ADDRESS, &servaddr.sin_addr);
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&on1, sizeof(int)))
    {
        sockfd = 0;
    }
    stHealthPacket.stNetHeader.ulPktLen = htonl ( sizeof ( HealthPacket ) );
    stHealthPacket.stNetHeader.ulSrc = htonl ( SPMS_1 ); // /enter subsystem id
    stHealthPacket.stNetHeader.stDest.bit.bitDestId = htonl ( FDS );
    stHealthPacket.stNetHeader.stDest.bit.bitSubDestId = htonl(0);
    stHealthPacket.stNetHeader.ulCategory = htonl ( PERIODIC_HEALTH_DATA );
    stHealthPacket.stNetHeader.ulSubItem = htonl( FDS_ONA_1); //Enter subitem
    stHealthPacket.stNetHeader.ulProtocol = htonl ( 0x01 );
    stHealthPacket.stNetHeader.ulNoOfObj = htonl ( 1 );
    stHealthPacket.stNetHeader.ulNoOfFragments = htonl ( 1 );
    stHealthPacket.stNetHeader.ulFragmentNo = htonl ( 1 );
    stHealthPacket.stNetHeader.stFlag.ulVal = htonl ( 0xF );
    stHealthPacket.stNetHeader.ulAAAA5555 = htonl ( 0xAAAA5555 );
    stHealthPacket.stNetHeader.ulEndianess = htonl(2);
    stHealthPacket.stNetHeader.stDate.ucDay = htons(0x07);
    stHealthPacket.stNetHeader.stDate.ucMonth = htons(0x06);
    stHealthPacket.stNetHeader.stDate.uiYear = (0x15);
    stHealthPacket.stNetHeader.stTime.ucSecond = htonl(53);
    stHealthPacket.stNetHeader.stTime.ucMinute = htonl(47);
    stHealthPacket.stNetHeader.stTime.uiHour = htonl(17);
    stHealthPacket.stNetHeader.ulTimeMicroSec = htonl(0);
    stHealthPacket.stNetHeader.ulTimeSpare = htonl(0);
    stHealthPacket.stNetHeader.ulVersion = htonl(1.1);
    stHealthPacket.stNetHeader.ulChecksum = htonl(0xDEADBEEF);
    stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth.ulMajVer=(1);
    stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth.ulMinVer=(3);
    stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth.ulBuildNum=(4);
    stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth.stAppBuildDate.ucDay=18;
    stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth.stAppBuildDate.ucMonth=7;
    stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth.stAppBuildDate.uiYear=2015;

    for(iCount=0;iCount<10;iCount++)
    {
      stHealthPacket.stPeriodicHealth.stSpmsHealth.stAppSpecHealth.uiLinkStatus[iCount]=0x01;
    }

    swapBuffer((const char *)(void *)&(stHealthPacket.stPeriodicHealth.stSpmsHealth.stHardHealth), sizeof(InfoProcHardHealth));
    swapBuffer((const char *)(void *)&(stHealthPacket.stPeriodicHealth.stSpmsHealth.stSoftHealth), sizeof(InfoProcSoftHealth));
    swapBuffer((const char *)(void *)&(stHealthPacket.stPeriodicHealth.stSpmsHealth.stAppSpecHealth), sizeof(SpmsAppSepcHealth));






*/



}
void ISSInfoClass::Npol_Header_Setting()
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
void ISSInfoClass::Npol_Header_Transmision_Setting()
{
    UDPTransmitData1.NPOL_HEADER[0]=htonl(0x2F8);
    UDPTransmitData1.NPOL_HEADER[1]=htonl(0x00000030);
    UDPTransmitData1.NPOL_HEADER[2]=htonl(0X00000031);
    UDPTransmitData1.NPOL_HEADER[3]=htonl(0X00000071);
    UDPTransmitData1.NPOL_HEADER[4]=htonl(0X00000001);
    UDPTransmitData1.NPOL_HEADER[5]=htonl(0X00000001);
    UDPTransmitData1.NPOL_HEADER[6]=htonl(0X00000003);
    UDPTransmitData1.NPOL_HEADER[7]=htonl(0X00000001);
    UDPTransmitData1.NPOL_HEADER[8]=htonl(0X00000001);
    UDPTransmitData1.NPOL_HEADER[9]=htonl(0X0000000F);
    UDPTransmitData1.NPOL_HEADER[10]=htonl(0X00000000);
    UDPTransmitData1.NPOL_HEADER[11]=htonl(0XAAAA5555);
    UDPTransmitData1.NPOL_HEADER[12]=htonl(0X00000002);
    UDPTransmitData1.NPOL_HEADER[13]=htonl(0X00000000);
    UDPTransmitData1.NPOL_HEADER[14]=htonl(0X00000000);
    UDPTransmitData1.NPOL_HEADER[15]=htonl(0X00000000);
    UDPTransmitData1.NPOL_HEADER[16]=htonl(0X00000000);
    UDPTransmitData1.NPOL_HEADER[31]=htonl(0xBABACACA);
    for(iCount=17;iCount<30;iCount++)
    {
      UDPTransmitData1.NPOL_HEADER[iCount]=htonl(0x00000000);
    }


}

void ISSInfoClass::swapBuffer(const char *data, unsigned long len)
{
    unsigned long *pulData = (unsigned long *)data;

    if (htons(0xAA55) == 0xAA55)
        return;

    for (unsigned int i = 0; i < len; i += 4)
    {
    *(pulData) = htonl(*(pulData));
    pulData ++;
    }
}


void ISSInfoClass::TransmitData()
{
Npol_Header_Transmision_Setting();
kCount=0;

     //  printf("\n Iam in the Track Data Setup-----------------------------------%f \n",TR_Info[k].Trfreq);
        UDPTransmitData1.FreqTrackObjectID=htonl(0xC00001);
        UDPTransmitData1.PassiveTrackObjectID=htonl(0xC00002);
        UDPTransmitData1.CavitationTrackObjectID=htonl(0xC00003);
        UDPTransmitData1.Object1_Length=htonl(0x00000210);
        UDPTransmitData1.Object1_Status=htonl(0x00000001);
        UDPTransmitData1.Object2_Length=htonl(0x00000024);
        UDPTransmitData1.Object2_Status=htonl(0x00000001);
        UDPTransmitData1.Object3_Length=htonl(0x00000014);
        UDPTransmitData1.Object3_Status=htonl(0x00000001);
        UDPTransmitData1.Object1_Trailer=htonl(0xBABACACA);
        UDPTransmitData1.Object2_Trailer=htonl(0xBABACACA);
        UDPTransmitData1.Object3_Trailer=htonl(0xBABACACA);

        for(i=0;i<NO_OF_TRACK;i++)
        {



              if(TrackSendStatus[i]!=0)
              {

                UDPTransmitData1.TRACK1_FREQ[kCount]=htonl(0x00000001);
              }
              else
              {

               UDPTransmitData1.TRACK1_FREQ[kCount]=htonl(0x00000000);

              }

              kCount=kCount+1;
              UDPTransmitData1.TRACK1_FREQ[kCount]=(unsigned int)(htonl(TrackSendFreq[i]));
           //   printf("\n Iam in the Track Data Setup-----------------------------------%d %d %d %f \n",i,UDPTransmitData1.TRACK1_FREQ[kCount],kCount,TrackSendFreq[i]);

              kCount=kCount+1;


        }
        for(i=0;i<4;i++)
        {
          UDPTransmitData1.SPARE1[i]=0x00000000;
          UDPTransmitData1. SPARE2[i]=0x00000000;
          UDPTransmitData1.SPARE3[i]=0x00000000;
        }



        for(i=0;i<5;i++)
        {
        UDPTransmitData1.PassiveEnergyLevel[i]=(int)(htonl(PassiveEnergy[i]));
        }

         if(txcavitationFlag==1)
         {

             UDPTransmitData1.CavitationThresholdFlag=htonl(0x00000001);
             txcavitationFlag=0;
            // printf("\n Cavitating Error---------%d",ntohl(UDPTransmitData1.CavitationThresholdFlag));
         }
         else
         {

             UDPTransmitData1.CavitationThresholdFlag=htonl(0x00000000);
             txcavitationFlag=0;
            // printf("\n Cavitating Out---------%d",UDPTransmitData1.CavitationThresholdFlag);
         }



        memcpy(&databuf,&UDPTransmitData1,sizeof(UDPTRXData1));
        TransmitSocket1->writeDatagram(databuf,sizeof(UDPTRXData1),BroadcastSenderAddress,TRANS_PORT_NO);
       //  printf("Transmitted OK sizeof %d \n",sizeof(UDPTRXData1));




}

void ISSInfoClass::Gain_Set()
{


}


