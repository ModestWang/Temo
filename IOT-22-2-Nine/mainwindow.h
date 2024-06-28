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

    Ui::MainWindow* getUI() { return this->ui;}

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

typedef struct
{
    unsigned int control;       // 控制者：0-无，1-Qt按钮，2-物理按键
    unsigned int mode;          // 当前的模式：1、2、3、4
    unsigned int led_state;     // LED 状态：0-亮，1-灭
    unsigned int led_dir;       // LED 流水灯方向：0-无，1-向右，2-向左
    unsigned int beep_state;    // Beep 状态：0-不响，1-响
    int count;                  // 记录进入定时器0中断次数
} My_System;
void My_System_Init(My_System *my_system); // 系统属性初始化
