#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSocketNotifier>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow *getInstance();
    void printInfo(int control, int mode);
    // 函数声明
    void do_mode_1(int ctl);
    void do_mode_2(int ctl);
    void do_mode_3(int ctl);
    void do_mode_4(int ctl);
    QTimer* timer1;
    QTimer* timer2;
    QTimer* timer3;
    void timer_init();
    void timer1_init();
    void timer2_init();
    void timer3_init();
    void timer1_close();
    void timer2_close();
    void close_time_1_2_if_opened();
    void timer3_close();
    void button_init();
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

private slots:
    void onKeyChanged();
    void start_display();
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
