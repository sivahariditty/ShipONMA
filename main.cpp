#include "mainwindow.h"
#include <QApplication>
#include"networkclass.h"
#include"signalprocessingclass.h"
#include "commonclass.h"
#include<QPicture>
#include"sonarcommunication.h"
#include"DataTypes.h"
extern RecordingControls RecControl;
extern char RecorderPathSet[400];

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    getcwd(RecControl.CurrentDir,sizeof(RecControl.CurrentDir));
    strcat(RecControl.RecorderPath,RecControl.CurrentDir);
    strcat(RecControl.KELIMAGPath,RecControl.CurrentDir);
    strcat(RecControl.NPOLIMAGPath,RecControl.CurrentDir);
    strcat(RecControl.RecorderPath,"/FileStorage");
    strcat(RecControl.EnergyPath,RecControl.CurrentDir);
    strcat(RecControl.EnergyPath,"/EnergyStorage");

    strcat(RecorderPathSet,"FILE PATH :");
    strcat(RecorderPathSet,RecControl.RecorderPath);

   // printf("\n  Curent Dir=%s ",RecControl.EnergyPath);

    MainWindow w;
    w.show();
    w.setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    NetworkClass *NetwokThread;
    NetwokThread=new NetworkClass();
    NetwokThread->start(QThread::HighestPriority);
    SignalProcessingClass *ProcessingThread;
    ProcessingThread=new SignalProcessingClass();
    ProcessingThread->start(QThread::NormalPriority);
    SonarCommunication *SonarInterfaceThread;
    SonarInterfaceThread=new SonarCommunication();
    SonarInterfaceThread->start(QThread::NormalPriority);
    // NetwokThread->wait();
    // ProcessingThread->wait();
    /// SonarInterface->wait();
    
    return a.exec();
}
