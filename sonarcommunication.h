#ifndef SONARCOMMUNICATION_H
#define SONARCOMMUNICATION_H

#include <QWidget>
#include<QThread>
#include<stdio.h>
#include<QUdpSocket>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include"DataTypes.h"
#include<math.h>
#include<QFile>
#include<stdio.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

class SonarCommunication : public QThread
{
    Q_OBJECT
public:
     SonarCommunication();
     void run();            /**< SonarInterface Thread Automatic Function */
     void NavDataControl();
     void RecordDataFun();
     void ReplayDataFun();
     QUdpSocket *socket2,*Sonarsocket,*Recsocket;
    struct ip_mreq group2;
    unsigned int Category1,Subcategory;
    unsigned int iCount,yCount,kCount;
    int32_t DataPtr2,DataBufferCount;
signals:
    
public slots:
      void PendingData2();
      int  swapBuffer(const char *, int);
      void Sonardataready();
      void Oasread();
      void Quit();
};

#endif // SONARCOMMUNICATION_H
