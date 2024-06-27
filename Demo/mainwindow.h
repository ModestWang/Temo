#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* getInstance();
    Ui::MainWindow* getUI(){ return this->ui;}
    ~MainWindow();
    
private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_checkBox_1_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_Beep_clicked();

    void on_pushButton_Reset_clicked();

    void on_pushButton_Clear_clicked();


    void on_pushButton_Extend_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
