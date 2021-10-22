#ifndef ISSINFOCLASS_H
#define ISSINFOCLASS_H

#include <QMainWindow>
#include<QLabel>
#include<QPushButton>
#include"DataTypes.h"
#include<QHostAddress>
#include<QUdpSocket>
#include <arpa/inet.h>
#include<QMenuBar>
#include<QAction>
#include<QDate>
#include<QTime>
class ISSInfoClass : public QMainWindow
{
    Q_OBJECT
public:
    QFrame *ISSInfoFrame,*ISSButtonFrame;
    int32_t sockfd1,sockfd;
    int32_t iCount,yCount;
    struct sockaddr_in servaddr1,servaddr;
    explicit ISSInfoClass(QWidget *parent = 0);
    void ISSInformationFun();
    QLabel *SP,*CO,*Sv,*D,*HD,*SW,*LAT,*LNG,*deg,*knts,*m,*ms,*PLS,*LEN,*LVL,*BW,*CF,*RL,*SysMod,*Pag,*StopWatchx,*Datex,*Timex;
    QLabel *NPOLImage,*KelImage,*Analon,*SPx,*COx,*Svx,*Dx,*HDx,*SWx,*LATx,*LNGx,*deg1,*deg2,*n,*e,*PLSx,*LENx,*LVLx,*ms1,*db1,*BWx,*CFx,*RLx,*hz1,*hz2,*deg3;
    QPushButton *ShutDown,*Normal1,*ONA,*Start,*Stop,*HMI,*ScreenShot;
    QMenuBar *menuB,*Normal_menuB;
    QDate date;
    QTime newtime,delayTime,timeSec;
    QString DelayTime,dateString;
    void UpdateISSInformations();
    void SonarHealthRequest();
    void SonarHealthStatus();
    void SonarRequestInit();
    void SonarHealthStatusInit();
    void swapBuffer(const char *,unsigned long);
    void Gain_Set();
    void Npol_Header_Setting();
    void Npol_Header_Transmision_Setting();
    void TransmitData();
    void Update_Time_Clock();
    char RequestDataBuffer[1168];
    char StatusDataBuffer[1168];
     QHostAddress healthrequestBroadCast,healthStatusBroadCast;
     QAction *Normal_Action;
     QMenu *Normal_Menu;
     int32_t status,i,kCount;
     bool TrackSendStatus[NO_OF_TRACK];
     float TrackSendFreq[NO_OF_TRACK];
    float PassiveEnergy[29];
    bool txcavitationFlag;

     char databuf[1024];
     QUdpSocket *TransmitSocket1;
     QHostAddress BroadcastSenderAddress,UnicastAddress,ReciverAddr,UnicastAddress2;

signals:
    
public slots:
    
};

#endif // ISSINFOCLASS_H
