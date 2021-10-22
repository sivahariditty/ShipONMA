#ifndef THRESHOLDSETTINGWINDOW_H
#define THRESHOLDSETTINGWINDOW_H
#include<QFrame>
#include"DataTypes.h"
#include <QWidget>
#include<QLineEdit>
#include<QRadioButton>
#include<QGridLayout>
#include<QLabel>
#include<QPushButton>
#include <QCheckBox>
class ThresholdSettingWindow: public QWidget
{
    Q_OBJECT
public:
    char Freq[50],Amp[50];
    QWidget *gridLayoutWidget;
    QCheckBox *checkBox1;
    QCheckBox *checkBox2;
    QCheckBox *checkBox3;
    QCheckBox *checkBox4;
    QCheckBox *checkBox5;
    QCheckBox *checkBox6;
    QCheckBox *checkBox7;
    QCheckBox *checkBox8;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_16;
    QLabel *label;
    QLabel *label_2;


    QLabel *label11,*label12,*label13,*label14,*label15,*label16,*label17,*label18;

    int32_t CH_NO_FreqPos[NO_OF_SENSOR][8];
    int32_t CH_NO_AmpPos[NO_OF_SENSOR][8];
    int16_t CurrentChannelNumber;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    explicit ThresholdSettingWindow(QWidget *parent = 0);
    QFrame *ThresholdWindow;
    
signals:
    
public slots:
    void setThresholdConfig();
    void cancelThresholdConfig();
    void checkBox1Enable(int);
    void checkBox2Enable(int);
    void checkBox3Enable(int);
    void checkBox4Enable(int);
    void checkBox5Enable(int);
    void checkBox6Enable(int);
    void checkBox7Enable(int);
    void checkBox8Enable(int);



};

#endif // THRESHOLDSETTINGWINDOW_H
