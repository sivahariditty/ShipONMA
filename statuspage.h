#ifndef STATUSPAGE_H
#define STATUSPAGE_H
#include <QMainWindow>
#include"DataTypes.h"
#include<QFrame>
class StatusPage : public QMainWindow
{
    Q_OBJECT
public:
   int16_t count1_;
   QFrame *StartPage,*LinkStatusFrame;
   QLabel *SCUName[10],*Ethernet,*Byte;
   QFont font;
 explicit StatusPage(QFrame *parent = 0);  /**< Constructor for  Ethernet Link status.This page pop up just after the program execution.This page contain QLabel "SCU-x....OK" for 10 SCUs.If the link between SCU amd SBC is active then the coresponding label turn to green other wise in yellow. */
 void startPage();                        /**< Function for creating the Ethernet Link status page. */
 void CheckLinkStatus();                 /**< Function for Check the link status.*/

 ~StatusPage();
signals:

public slots:






};

#endif // STATUSPAGE_H
