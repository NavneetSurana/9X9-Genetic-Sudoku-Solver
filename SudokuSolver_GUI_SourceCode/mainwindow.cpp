#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QCloseEvent>
#include <iostream>
#include <fstream>
#include <QApplication>
#include "SudokuSolver.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Sudoku Solver");
    list=ui->centralWidget->findChildren<QLabel *>();
    text=ui->MaxItr;
    itrNum=ui->ItrNum;
    fitnessAch=ui->Fitness;
    text->setText("50000");
    text->setAlignment(Qt::AlignCenter);
    itrNum->setText("0");
    fitnessAch->setText("0/972");
    Stop=false;
    Solve=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Load_clicked()
{
    if(Solve) return;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            original[i][j]=0;
        }
    }
    ifstream in;
    string inputFile=QApplication::applicationDirPath().append("/input.md").toStdString();
    in.open(inputFile);
    if(in.good()){
        for(int i=1;i<10;i++){
            for(int j=1;j<10;j++){
                in>>original[i][j];
            }
        }
        for(int p=0;p<list.size();p++){
            if(list[p]->objectName()[0]!='L') continue;
            quint32 i=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[1]).toUInt());
            quint32 j=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[2]).toUInt());
            if(0<i && i<10 && 0<j && j<10){
                if(original[i][j]==0){
                    if( (i>0 && i<4 && j>3 && j<7) || (i>3 && i<7 && (j<4 || j>6) ) || (i>6 && i<10 && j>3 && j<7) ){
                        list[p]->setStyleSheet("QLabel { background-color : rgb(255,255,255) }");
                    }
                    else list[p]->setStyleSheet("QLabel { background-color : rgb(215, 235, 203) }");
                    list[p]->setText(static_cast<QString>(""));
                }
                else{
                    list[p]->setStyleSheet("QLabel { background-color : rgb(192,187,254) }");
                    list[p]->setText(QString::number(original[i][j]));
                }
            }
        }
        in.close();
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    Stop=true;
    event->accept();
}
void MainWindow::on_Save_clicked()
{

    if(Solve) return;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            original[i][j]=0;
        }
    }
    ofstream out;
    string inputFile=QApplication::applicationDirPath().append("/input.md").toStdString();
    QString val;
    for(int p=0;p<list.size();p++){
        if(list[p]->objectName()[0]!='L') continue;
        quint32 i=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[1]).toUInt());
        quint32 j=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[2]).toUInt());
        val=list[p]->text();
        if(val=="") original[i][j]=0;
        else original[i][j]=val.toInt();
    }
    bool flag=0;
    for(int i=1;i<10;i++){
        for(int j=1;j<10;j++){
            if(original[i][j]!=0){
                flag=1;
                break;
            }
        }
        out<<endl;
    }
    if(flag==1){
        out.open(inputFile);
        for(int i=1;i<10;i++){
            for(int j=1;j<10;j++){
                out<<original[i][j]<<' ';
            }
            out<<endl;
        }
        out.close();
    }
}

void MainWindow::on_Reset_clicked()
{
    if(Solve) return;
    for(int p=0;p<list.size();p++){
        if(list[p]->objectName()[0]!='L') continue;
        quint32 i=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[1]).toUInt());
        quint32 j=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[2]).toUInt());
        if(0<i && i<10 && 0<j && j<10){
            if( (i>0 && i<4 && j>3 && j<7) || (i>3 && i<7 && (j<4 || j>6) ) || (i>6 && i<10 && j>3 && j<7) ){
                list[p]->setStyleSheet("QLabel { background-color : rgb(255,255,255) }");
            }
            else list[p]->setStyleSheet("QLabel { background-color : rgb(215, 235, 203) }");
            list[p]->setText("");
        }
    }
    itrNum->setText("0");
    fitnessAch->setText("0/972");
    text->setText("50000");
    text->setAlignment(Qt::AlignCenter);//or another alignment

}

void MainWindow::on_Solve_clicked()
{
    bool check_sudoku=false;
    for(int p=0;p<list.size();p++){
        if(list[p]->objectName()[0]!='L') continue;
        if(list[p]->text()!=static_cast<QString>("")){
            check_sudoku=true;
            break;
        }
    }
    if(check_sudoku==false || Solve==true) return;
    Solve=true;
    QString maxitr=text->toPlainText();
    qint32 maxITR=static_cast<quint32>(static_cast<QString>(maxitr).toUInt());
    string inputFile=QApplication::applicationDirPath().append("/input.md").toStdString();
    string outputFile=QApplication::applicationDirPath().append("/output.md").toStdString();
    SudokuSolver* obj=new SudokuSolver();
    bool flag=obj->t_Ret_GA(inputFile,outputFile,list,maxITR,itrNum,fitnessAch,&Stop);
    if(flag){
        ifstream in;
        in.open(outputFile);
        for(int i=1;i<10;i++){
            for(int j=1;j<10;j++){
                in>> original[i][j];
            }
        }

        for(int p=0;p<list.size();p++){
            if(list[p]->objectName()[0]!='L') continue;
            quint32 i=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[1]).toUInt());
            quint32 j=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[2]).toUInt());
            if(0<i && i<10 && 0<j && j<10){
                if(list[p]->text()=="") list[p]->setStyleSheet("QLabel { background-color : rgb(215, 235, 203) }");
                list[p]->setText(QString::number(original[i][j]));
            }
        }
        fitnessAch->setText("972/972");
        in.close();
    }
    Solve=false;
}

void MainWindow::on_Stop_clicked()
{
    Stop=true;
}
