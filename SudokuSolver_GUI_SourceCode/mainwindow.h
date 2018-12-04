#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        QList<QLabel *> list;
        QTextEdit* text;
        QLabel *itrNum, *fitnessAch;
        bool Stop;
        bool Solve;
    private slots:
        void on_Save_clicked();

        void on_Load_clicked();

        void on_Reset_clicked();

        void on_Solve_clicked();

        void on_Stop_clicked();
        void closeEvent(QCloseEvent *event);

private:
        int original[10][10];
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
