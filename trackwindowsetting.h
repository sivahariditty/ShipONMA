#ifndef TRACKWINDOWSETTING_H
#define TRACKWINDOWSETTING_H

#include <QWidget>
#include<QFrame>
#include"DataTypes.h"
#include <QWidget>
#include<QLineEdit>
#include<QRadioButton>
#include<QGridLayout>
#include<QLabel>
#include<QPushButton>
#include <QCheckBox>
class TrackWindowSetting : public QWidget
{
    Q_OBJECT
public:
    char Freq[50],Amp[50];
    int16_t CurrentChannel;

    char TrackNo[50];
    int16_t count_,yCount,xCount;
    QFrame *trackWindow;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QCheckBox *trackcheckBox[64];
    QLineEdit *lineEdit_a1[64],*lineEdit_b1[64];
    QLabel *label[64];

    QPushButton *OK_BUTTON;




    explicit TrackWindowSetting(QWidget *parent = 0);
    
signals:
    
public slots:

    void checkBox1Enable(int);
    void checkBox2Enable(int);
    void checkBox3Enable(int);
    void checkBox4Enable(int);
    void checkBox5Enable(int);
    void checkBox6Enable(int);
    void checkBox7Enable(int);
    void checkBox8Enable(int);

    void checkBox9Enable(int);
    void checkBox10Enable(int);
    void checkBox11Enable(int);
    void checkBox12Enable(int);
    void checkBox13Enable(int);
    void checkBox14Enable(int);
    void checkBox15Enable(int);
    void checkBox16Enable(int);

    void checkBox17Enable(int);
    void checkBox18Enable(int);
    void checkBox19Enable(int);
    void checkBox20Enable(int);
    void checkBox21Enable(int);
    void checkBox22Enable(int);
    void checkBox23Enable(int);
    void checkBox24Enable(int);

    void checkBox25Enable(int);
    void checkBox26Enable(int);
    void checkBox27Enable(int);
    void checkBox28Enable(int);
    void checkBox29Enable(int);
    void checkBox30Enable(int);
    void checkBox31Enable(int);
    void checkBox32Enable(int);

    void checkBox33Enable(int);
    void checkBox34Enable(int);
    void checkBox35Enable(int);
    void checkBox36Enable(int);
    void checkBox37Enable(int);
    void checkBox38Enable(int);
    void checkBox39Enable(int);
    void checkBox40Enable(int);

    void checkBox41Enable(int);
    void checkBox42Enable(int);
    void checkBox43Enable(int);
    void checkBox44Enable(int);
    void checkBox45Enable(int);
    void checkBox46Enable(int);
    void checkBox47Enable(int);
    void checkBox48Enable(int);

    void checkBox49Enable(int);
    void checkBox50Enable(int);
    void checkBox51Enable(int);
    void checkBox52Enable(int);
    void checkBox53Enable(int);
    void checkBox54Enable(int);
    void checkBox55Enable(int);
    void checkBox56Enable(int);

    void checkBox57Enable(int);
    void checkBox58Enable(int);
    void checkBox59Enable(int);
    void checkBox60Enable(int);
    void checkBox61Enable(int);
    void checkBox62Enable(int);
    void checkBox63Enable(int);
    void checkBox64Enable(int);

    void WindowHide();

};

#endif // TRACKWINDOWSETTING_H
