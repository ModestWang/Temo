#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

// 系统状态参数
My_System my_system;
void My_System_Init(My_System *my_system)
{
    my_system->control = 0;         // 控制者：0-无，1-Qt按钮，2-物理按键
    my_system->mode = 0;            // 当前的模式：1、2、3、4
    my_system->led_state[0] = 0;    // LED 流水灯状态：0～3对应LED1～4
    my_system->led_state[1] = 0;    // LED 流水灯状态：0～3对应LED1～4
    my_system->led_state[2] = 0;    // LED 流水灯状态：0～3对应LED1～4
    my_system->led_state[3] = 0;    // LED 流水灯状态：0～3对应LED1～4
    my_system->led_dir = 0;         // LED 流水灯方向：0-无，1-向右，2-向左
    my_system->beep_state = 0;      // Beep 状态：0-不响，1-响
    my_system->count = 0;           // 记录进入定时器0中断次数
}


#define PWM_IOCTL_SET_FREQ 1
#define PWM_IOCTL_STOP 2

static int my_leds;
static int my_buttons;
static int my_beep;
char buttons[6] = {'0', '0', '0', '0', '0', '0'};

// 打开/dev/leds 设备文件 //
static void open_leds()
{
    ::system("kill -s STOP `pidof led-player`");
    my_leds = ::open("/dev/leds", O_RDONLY);
    if (my_leds < 0)
    {
        perror("open device leds");
        exit(1);
    }
}
static void close_leds()
{
    close(my_leds);
    my_leds = -1;
}

// 打开/dev/buttons 设备文件 //
static void open_buttons()
{

    my_buttons = open("/dev/buttons", 0);
    if (my_buttons < 0)
    {
        perror("open device buttons");
        exit(1);
    }
}
void *listen_buttons(void *my_system)
{
    for (;;)
    {
        char current_buttons[6];
        int i;
        int count_of_changed_key;
        if (read(my_buttons, current_buttons, sizeof(current_buttons)) != sizeof(current_buttons))
        {
            perror("read buttons:");
            exit(1);
        }
        for (i = 0, count_of_changed_key = 0; i < (sizeof(buttons) / sizeof(buttons[0])); i++)
        {
            if (buttons[i] != current_buttons[i])
            {
                ((My_System *)my_system)->control = 2;
                buttons[i] = current_buttons[i];
                printf("%skey %d is %s", (count_of_changed_key? ", ": ""), i + 1, ((buttons[i] == '0') ? "up" : "down"));
                count_of_changed_key++;
            }
        }
        if (count_of_changed_key)
        {
            printf("\n");
        }
    }
}
static void close_buttons()
{
    close(my_buttons);
    my_buttons = -1;
}

// 打开/dev/pwm 设备文件 //
static void open_beep()
{
    my_beep = open("/dev/pwm", 0);
    if (my_beep < 0)
    {
        perror("open pwm_beep device");
        exit(1);
    }
//     any function exit call will stop the beep
//     atexit(close_beep);
}
static void close_beep()
{
    if (my_beep >= 0)
    {
        ioctl(my_beep, PWM_IOCTL_STOP);
        close(my_beep);
        my_beep = -1;
    }
}
static void set_beep_freq(int freq)
{
    int ret = ioctl(my_beep, PWM_IOCTL_SET_FREQ, freq);
    if(ret < 0)
    {
        perror("set the frequency of the beep");
        exit(1);
    }
}
static void stop_beep()
{
    int ret = ioctl(my_beep, PWM_IOCTL_STOP);
    if(ret < 0)
    {
        perror("stop the beep");
        exit(1);
    }
}

MainWindow* instance;
MainWindow *MainWindow::getInstance() {return instance;}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    instance = this;    // 获取窗口实例，以便在普通函数中使用
    ui->setupUi(this);
    ui->centralWidget->setStyleSheet("QWidget#centralWidget{border-image:url(:/image/A001.jpg)}"); // 加载背景图片

    // 启动设备
//    open_leds();
//    open_buttons();
//    open_beep();



}

MainWindow::~MainWindow()
{
    delete ui;
    // 关闭
//    close_leds();
//    close_buttons();
//    close_beep();
}



/**************************************************/
/********************  事件  ***********************/
/**************************************************/

void MainWindow::on_pushButton_1_clicked()
{
    my_system.control = 1;
    my_system.mode = 1;

}

void MainWindow::on_pushButton_2_clicked()
{
    my_system.control = 1;
    my_system.mode = 2;
}

void MainWindow::on_pushButton_3_clicked()
{
    my_system.control = 1;
    my_system.mode = 3;
}

void MainWindow::on_pushButton_4_clicked()
{
    my_system.control = 1;
    my_system.mode = 4;
}

void MainWindow::on_pushButton_Reset_clicked()
{
    my_system.control = 1;
    ui->label->setText(QString::fromUtf8("复位"));
}

void MainWindow::on_pushButton_Clear_clicked()
{
    my_system.control = 1;
    ui->label->setText(QString::fromUtf8(""));
}

void MainWindow::on_checkBox_1_clicked()
{
    my_system.control = 1;
    ioctl(my_leds, int(ui->checkBox_1->isChecked()), 0);
    if(ui->checkBox_1->isChecked())
    {
        ui->label->setText(QString::fromUtf8("LED1 ON"));
    }
    else
    {
        ui->label->setText(QString::fromUtf8("LED1 OFF"));
    }
}

void MainWindow::on_checkBox_2_clicked()
{
    my_system.control = 1;
    ioctl(my_leds, int(ui->checkBox_2->isChecked()), 1);
    if(ui->checkBox_2->isChecked())
    {
        ui->label->setText(QString::fromUtf8("LED1 ON"));
    }
    else
    {
        ui->label->setText(QString::fromUtf8("LED1 OFF"));
    }
}

void MainWindow::on_checkBox_3_clicked()
{
    my_system.control = 1;
    ioctl(my_leds, int(ui->checkBox_3->isChecked()), 2);
    if(ui->checkBox_3->isChecked())
    {
        ui->label->setText(QString::fromUtf8("LED1 ON"));
    }
    else
    {
        ui->label->setText(QString::fromUtf8("LED1 OFF"));
    }
}

void MainWindow::on_checkBox_4_clicked()
{
    my_system.control = 1;
    ioctl(my_leds, int(ui->checkBox_4->isChecked()), 3);
    if(ui->checkBox_4->isChecked())
    {
        ui->label->setText(QString::fromUtf8("LED1 ON"));
    }
    else
    {
        ui->label->setText(QString::fromUtf8("LED1 OFF"));
    }
}

void MainWindow::on_checkBox_Beep_clicked()
{
    my_system.control = 1;
    if(ui->checkBox_Beep->isChecked())
    {
        ui->label->setText(QString::fromUtf8("Beep ON"));
    }
    else
    {
        ui->label->setText(QString::fromUtf8("Beep OFF"));
    }
}

// Todo: 这是拓展功能
void MainWindow::on_pushButton_Extend_clicked()
{
    my_system.control = 1;
    ui->label->setText(QString::fromUtf8("See you again"));
    ui->checkBox_1->setCheckState(Qt::Checked);
}


/**************************************************/
/********************  硬件  ***********************/
/**************************************************/

