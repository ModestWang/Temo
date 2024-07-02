#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSocketNotifier>
#include <QTimer>

namespace Ui 
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow *getInstance();

    // 函数声明
    void printInfo(int ctl, int mod);
    void do_mode_1(int ctl);
    void do_mode_2(int ctl);
    void do_mode_3(int ctl);
    void do_mode_4(int ctl);
    void timer_init();
    void timer1_init(int setTime);
    void timer2_init(int setTime);
    void timer3_init(int setTime);
    void timer1_close();
    void timer2_close();
    void timer3_close();
    void close_timer_if_opened();
    void button_init();
    ~MainWindow();

    // 定时器声明
    QTimer* timer1;
    QTimer* timer2;
    QTimer* timer3;
    
private:
    Ui::MainWindow *ui;

private slots:
    void onKeyChanged();
    void selfpost();
    void led_display();
    void beep_display();
    void on_LED_1_chk_clicked();
    void on_LED_2_chk_clicked();
    void on_LED_3_chk_clicked();
    void on_LED_4_chk_clicked();
    void on_Beep_chk_clicked();
    void on_Mode_1_btn_clicked();
    void on_Mode_2_btn_clicked();
    void on_Mode_3_btn_clicked();
    void on_Mode_4_btn_clicked();
    void on_Clear_btn_clicked();
    //void on_Extend_btn_clicked();
};

#endif // MAINWINDOW_H
