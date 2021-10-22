#include "commonclass.h"

CommonClass::CommonClass(int x)
{

    switch(x)
    {
    case 1: NetworkDataInit(1,1);
             printf("\n Common NetWork Variables Initialisation Completed Succusfully ");
            break;
    case 2: ProcessingDataInit(1,2);
             printf("\n Common Processing Variables Initialisation Completed Succusfully ");
            break;
    case 3: MainWindowDataInit(1,3);
             printf("\n Common MainWindowData Variables Initialisation Completed Succusfully ");
            break;
    case 4: MainWindowDataInit(1,3);
             printf("\n Common Status Variables Initialisation Completed Succusfully ");
            break;

    default: break;

    };


}
void CommonClass::NetworkDataInit(short int x1,short int x2)
{

}
void CommonClass::ProcessingDataInit(short int x1,short int x2)
{

}
void CommonClass::MainWindowDataInit(short int x1,short int x2)
{

}


int CommonClass::EthernetLinkStatus(int x,int y)
{




}




