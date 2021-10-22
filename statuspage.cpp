#include "statuspage.h"
extern Link LinkStatus;
StatusPage::StatusPage(QFrame *parent) : QMainWindow(parent)
{
    LinkStatusFrame=new QFrame(parent);
    startPage();
    CheckLinkStatus();

}



void StatusPage::startPage()
{
    LinkStatusFrame->setGeometry(QRect(0,0,1024,768));
    LinkStatusFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    LinkStatusFrame->setFrameShape(QFrame::StyledPanel);
    LinkStatusFrame->setFrameShadow(QFrame::Raised);
}


void StatusPage::CheckLinkStatus()
{

    font.setFamily(QString::fromUtf8("Droid Serif"));
    font.setBold(false);
    font.setPixelSize(24);
    Ethernet=new QLabel(LinkStatusFrame);
    Ethernet->setGeometry(QRect(390,40,550,50));
    Ethernet->setText(" ETHERNET LINK STATUS ");
    Ethernet->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
    Ethernet->setFont(font);
    font.setPixelSize(12);
    Byte=new QLabel(LinkStatusFrame);
    Byte->setGeometry(QRect(460,100,400,50));
    Byte->setText(" BYTE IN PROGRESS");
    Byte->setStyleSheet(QString::fromUtf8("color: rgb(0,255,0);"));
    Byte->setFont(font);
    font.setPixelSize(15);
    for(count1_=0;count1_<MAX_SCU_NO;count1_++)
    {

      switch(count1_)
      {
      case 0: SCUName[count1_]=new QLabel(LinkStatusFrame);
              SCUName[count1_]->setGeometry(QRect(400,180,260,50));
              if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
              {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-1  .   .    .   OK");
              }
              else
              {
               SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
               SCUName[count1_]->setText("LINK CHECK SCU-1  .   .    .   NOK");
              }
              SCUName[count1_]->setFont(font);
              break;
      case 1: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,250,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-2  .   .    .   OK");
             }
             else
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-2  .   .    .   NOK");
             }
             SCUName[count1_]->setFont(font);
             break;
      case 2: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,320,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
            SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-3  .   .    .   OK");
             }
             else
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-3  .   .    .   NOK");
             }
             SCUName[count1_]->setFont(font);
             break;
      case 3: SCUName[count1_]=new QLabel(LinkStatusFrame);
              SCUName[count1_]->setGeometry(QRect(400,390,260,50));
              if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
              {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-4  .   .    .   OK");
              }
              else
              {
               SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
               SCUName[count1_]->setText("LINK CHECK SCU-4  .   .    .   NOK");
              }
              SCUName[count1_]->setFont(font);
              break;
      case 4: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,460,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-5  .   .    .   OK");
             }
             else
             {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-5  .   .    .   NOK");
             }
             SCUName[count1_]->setFont(font);
             SCUName[count1_]->hide();

             break;
      case 5: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,530,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-6  .   .    .   OK");
             }
             else
             {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-6  .   .    .   NOK");
             }
             SCUName[count1_]->setFont(font);
             SCUName[count1_]->hide();

             break;
      case 6: SCUName[count1_]=new QLabel(LinkStatusFrame);
              SCUName[count1_]->setGeometry(QRect(400,600,260,50));
              if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
              {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-7  .   .    .   OK");
              }
              else
              {
               SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
               SCUName[count1_]->setText("LINK CHECK SCU-7  .   .    .   NOK");
              }
              SCUName[count1_]->setFont(font);
              SCUName[count1_]->hide();

              break;
      case 7: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,670,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-8  .   .    .   OK");
             }
             else
             {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-8  .   .    .   NOK");
             }
             SCUName[count1_]->setFont(font);
             SCUName[count1_]->hide();

             break;
      case 8: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,740,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-9  .   .    .   OK");
             }
             else
             {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-9  .   .    .   NOK");
             }
             SCUName[count1_]->setFont(font);
             SCUName[count1_]->hide();

             break;

      case 9: SCUName[count1_]=new QLabel(LinkStatusFrame);
             SCUName[count1_]->setGeometry(QRect(400,820,260,60));
             if(LinkStatus.SCU_LINK_STATUS[count1_]==1)
             {
             SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);"));
             SCUName[count1_]->setText("LINK CHECK SCU-10  .   .    .   OK");
             }
             else
             {
              SCUName[count1_]->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
              SCUName[count1_]->setText("LINK CHECK SCU-10  .   .    .   NOK");

             }
             SCUName[count1_]->setFont(font);
             SCUName[count1_]->hide();
             break;

      default: break;

      };
    }





}

StatusPage::~StatusPage()
{
    printf("\n Staus Page Class Deleted...");
    delete StartPage;
    delete LinkStatusFrame;

}
