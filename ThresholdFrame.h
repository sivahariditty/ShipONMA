#ifndef THRESHOLDFRAME_H
#define THRESHOLDFRAME_H

#include <QFrame>

namespace Ui {
class Frame;
}

class Frame : public QFrame
{
    Q_OBJECT
    
public:
    explicit Frame(QWidget *parent = 0);
    ~Frame();
    
private:
    Ui::Frame *ui;
};

#endif // THRESHOLDFRAME_H
