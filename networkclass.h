#ifndef NETWORKCLASS_H
#define NETWORKCLASS_H
#include "DataTypes.h"
#include<QUdpSocket>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

class NetworkClass : public QThread
{
    Q_OBJECT
public:
       int16_t xCount,yCount,CHANNEL_NO,Index;
       int16_t jCount,ucJ,ucI;
       QHostAddress group,MyAdress,MyPCAdress;
       QByteArray datagram;
       int32_t DataInt,SampleCount[61];

     NetworkClass(); /**< Network Class Constructor */
     void NetworkInit(); /**< Function for Initiaslisation of Network Class  */
     void run(); /**< Network Thread Automatic Function */
     void DataAccumulation(int16_t);
     ~NetworkClass();/**< Network Class Distructor */

private:

     QUdpSocket  *socket1,*TestSocket1;
     QHostAddress m_groupAddress;
     QHostAddress MyAddress;
     struct ip_mreq group1;
     struct ip_mreq group2;

signals:

public slots:

      void NetworkProcessing(void);
      void ReciveData(void);
};

#endif // NETWORKCLASS_H
