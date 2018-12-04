#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <mainwindow.h>
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

signals:
    void clicked();

public slots:
    void setTextLabel();
private:
    QString temp;
protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_H
