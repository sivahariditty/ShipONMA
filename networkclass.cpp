#include "networkclass.h"
DATA_PACKET_STRUCT *pac_struct;
extern int32_t PROCESS_SCU[10][16];
ControlData Controls[NO_OF_SENSOR];
DelayedSpectrum_Data Del_Controls[NO_OF_SENSOR];
bool StartProcessing=false;
RecordingControls RecControl;
Link LinkStatus;

NetworkClass::NetworkClass()
{
  NetworkInit();
}

void NetworkClass::run()
{

}

void NetworkClass::NetworkInit()
{
    for(xCount=0;xCount<MAX_SCU_NO;xCount++)
    {
        LinkStatus.SCU_LINK_STATUS[xCount]=false;

        for(yCount=0;yCount<NO_OF_CHANNEL;yCount++)
        {
            CHANNEL_NO=PROCESS_SCU[xCount][yCount];
            LinkStatus.CH_IN_SCU[xCount][yCount]=CHANNEL_NO;
            if(CHANNEL_NO!=0)
            {
              Index=CHANNEL_NO-1;
              LinkStatus.CH_GAIN_Rx[Index]=0xFFFFFFFF;
              LinkStatus.CH_SCU_NO[Index]=xCount;
              LinkStatus.CH_POS_NO[Index]=yCount;
            }
        }
    }

    for(xCount=0;xCount<NO_OF_SENSOR;xCount++)
    {
          Controls[xCount].SampleNumber=0;
          Controls[xCount].FrontPointer=0;
          Controls[xCount].RearPointer=0;
          Controls[xCount].FrontRecPointer=0;
          Controls[xCount].RearRecPointer=0;
          Controls[xCount].SampleCountNumber=0;
    }
    qint32 fd;
    qint16 port=(5235);
    group.setAddress("239.192.2.31");
    MyAdress.setAddress("192.168.1.70");
    MyPCAdress.setAddress("10.7.21.61");
    yCount=1;
    socket1=new QUdpSocket();
    socket1->bind(QHostAddress::Any,port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint );  //Data Signal bind
    fd= socket1->socketDescriptor();
    group1.imr_multiaddr.s_addr = htonl(group.toIPv4Address());
    group1.imr_interface.s_addr = htonl(MyAdress.toIPv4Address());
    if(setsockopt(fd,SOL_IP,IP_ADD_MEMBERSHIP,(char *)&group1,sizeof(group1)) < 0)
     {
         perror(" \n Adding Multicast Group error \n");
     }
    else
    {
        printf("\n Multicast Grouping entry Activated");
    }

     TestSocket1=new QUdpSocket();
     TestSocket1->bind(QHostAddress::Any,45707,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
     connect(socket1,SIGNAL(readyRead(void)),this,SLOT(ReciveData(void)));
     connect(TestSocket1,SIGNAL(readyRead()),this,SLOT(NetworkProcessing()));
     printf("\n Network Initialised Completed Succusfully.....");
     StartProcessing=true;
}

void NetworkClass::NetworkProcessing()
{
    yCount=1;
    datagram.resize(TestSocket1->pendingDatagramSize());
    TestSocket1->readDatagram(datagram.data(),datagram.size());
    pac_struct=(DATA_PACKET_STRUCT *)datagram.data();                   // Data Buffering to packet structure
    yCount=htons(pac_struct->SCU_ID);
    LinkStatus.SCU_LINK_STATUS[yCount-1]=true;
    DataAccumulation(yCount);
  //printf("\n Link Status=%d SCU No %d",LinkStatus.SCU_LINK_STATUS[yCount-1],yCount);
  //printf(" \n ------------Iam connected with SCU with data %d ",(int)datagram.size());
}


void NetworkClass::ReciveData(void)
{
       yCount=1;
       datagram.resize(socket1->pendingDatagramSize());
       socket1->readDatagram(datagram.data(),datagram.size());
       pac_struct=(DATA_PACKET_STRUCT *)datagram.data();                   // Data Buffering to packet structure
       yCount=htons(pac_struct->SCU_ID);
       LinkStatus.SCU_LINK_STATUS[yCount-1]=true;
       DataAccumulation(yCount);
    // printf("\n Link Status=%d SCU No %d",LinkStatus.SCU_LINK_STATUS[yCount-1],yCount);
    // printf(" \n ------------Iam connected with SCU with data %d ",(int)datagram.size());
}

void NetworkClass::DataAccumulation(int16_t SCUNO)
{
    ucI=0;

    for(jCount=0;jCount<NO_OF_CH;jCount++)
    {
     CHANNEL_NO=PROCESS_SCU[SCUNO-1][jCount];
     LinkStatus.CH_GAIN_SCU[SCUNO-1][jCount]=(pac_struct->CHANNEL_GAIN_INFO[jCount]);
     Index=(CHANNEL_NO-1);
     if(CHANNEL_NO!=0)
     {
         ucI=(SAMPLES_PER_CH*jCount);
         LinkStatus.CH_GAIN_Rx[Index]=(pac_struct->CHANNEL_GAIN_INFO[jCount]);
//printf("\n SCU No:%d   Ch_No:%d  Gain=%X",SCUNO ,Index,LinkStatus.CH_GAIN_Rx[Index]);
         for(ucJ=0;ucJ<SAMPLES_PER_CH;ucJ++)
         {
           DataInt=ntohl((pac_struct->DataBuffer[ucJ+ucI]));
           Controls[Index].RawDataIntBuffer[Controls[Index].FrontRecPointer][Controls[Index].SampleNumber]=DataInt;
           Controls[Index].RawDataBuffer[Controls[Index].FrontPointer][Controls[Index].SampleNumber]=(float)DataInt;
//printf("\n %d Data=%d %d ",RecControl.RecordNumber,Records[chCount].RecordDataBuffer[RecControl.RecordNumber][Controls[Index].SampleNumber],Controls[Index].SampleNumber);
           Controls[Index].SampleNumber=((Controls[Index].SampleNumber+1)%RawDataBufferSampleSize);
           if(Controls[Index].SampleNumber==0)
            {
             Controls[Index].PostStatus[Controls[Index].FrontPointer]=true;
             Controls[Index].PostRecStatus[Controls[Index].FrontRecPointer]=true;
             Controls[Index].FrontPointer=(Controls[Index].FrontPointer+1)%RawDataBufferSize;
             Controls[Index].FrontRecPointer=(Controls[Index].FrontRecPointer+1)%RawDataBufferSize;
            }
          }

   }
 }


}


NetworkClass::~NetworkClass()
{

}
