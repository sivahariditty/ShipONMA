#ifndef COMMONCLASS_H
#define COMMONCLASS_H
#include<QThread>
#include<QWidget>
#include<stdio.h>
#include<stdlib.h>
#include<QFrame>
#include<QPalette>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include<QLabel>

class CommonClass
{
public:

    CommonClass(int);

    void NetworkDataInit(short int,short int);
    void ProcessingDataInit(short int,short int);
    void MainWindowDataInit(short int,short int);
    int EthernetLinkStatus(int,int);
    int UDPPacketCount;
    bool NetworkLinkStatus[10];
private:
    //   short int x1,x2;
};

#endif // COMMONCLASS_H
