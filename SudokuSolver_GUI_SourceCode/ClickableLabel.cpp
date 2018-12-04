#include "ClickableLabel.h"
#include <QDebug>

using namespace std;
ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    connect( this, SIGNAL(clicked() ), this, SLOT( setTextLabel() ) );

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
      emit clicked();
}

void ClickableLabel::setTextLabel(){
    if(this->objectName()[0]!='L') return;
    quint32 i=static_cast<quint32>(static_cast<QString>(this->objectName()[1]).toUInt());
    quint32 j=static_cast<quint32>(static_cast<QString>(this->objectName()[2]).toUInt());
    temp=this->text();
    if(temp=="9"){
        temp="";
        if( (i>0 && i<4 && j>3 && j<7) || (i>3 && i<7 && (j<4 || j>6) ) || (i>6 && i<10 && j>3 && j<7) ){
            this->setStyleSheet("QLabel { background-color : rgb(255,255,255) }");
        }
        else this->setStyleSheet("QLabel { background-color : rgb(215, 235, 203) }");
    }
    else if(temp==""){
        temp="1";
        this->setStyleSheet("QLabel { background-color : rgb(192,187,254) }");
    }
    else{
        this->setStyleSheet("QLabel { background-color : rgb(192,187,254) }");
        quint8 number = static_cast<quint8>(temp.toUInt());
        number+=1;
        temp=QString::number(number);
    }
    this->setText(temp);
}
