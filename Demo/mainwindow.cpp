#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 宏定义
#define OUTPUT_LABEL Label_1
#define RECENT_LABEL Label_2
#define SOFT_CTL 1
#define HARD_CTL 2
#define PWM_IOCTL_SET_FREQ 1
#define PWM_IOCTL_STOP 0

// 函数声明
void open_leds();
void close_leds();
void open_buttons();
void close_buttons();
void open_beep();
void close_beep();
void set_beep_freq(int freq);
void stop_beep();

// 系统状态参数
int     control = 0;                    // 控制者：0-无，1-Qt按钮，2-物理按键
int     mode = 0;                       // 当前的模式：1、2、3、4
bool    leds_state[4] = {0, 0, 0, 0};   // LED 状态：0-不亮，1-亮
int     leds_dir = 0;                   // LED 流水灯方向：0-无，1-向右，2-向左
bool    beep_state = 0;                 // Beep 状态：0-不响，1-响
int     led_state = 0;                  // 流水灯计数变量
bool    led_judge = 0;                  // 流水灯状态变量
bool    timer1_state = 0;               // 定时器1状态变量，0-关闭，1-打开
bool    timer2_state = 0;               // 定时器2状态变量，0-关闭，1-打开
bool    timer3_state = 0;               // 定时器3状态变量，0-关闭，1-打开
int     beep_count = 0;                 // 峰鸣器计数变量
int     start_count = 0;                // 自检LED计数变量
char    buttons[6] = {'0', '0', '0', '0', '0', '0'};
int     BEEP_FIRQ = 1000;               // 峰鸣器频率，可变

// 设备定义
static int my_leds;
static int my_buttons;
static int my_beep;

// 提示信息
//QString controlStr_qt = ", 并由Qt按钮控制";
//QString controlStr_py = ", 并由物理按键控制";
//QString modeStr_1 = "当前控制模式为模式一";
//QString modeStr_2 = "当前控制模式为模式二";
//QString modeStr_3 = "当前控制模式为模式三";
//QString modeStr_4 = "当前控制模式为模式四";
QString controlStr_qt = " by Qt";
QString controlStr_py = " by real";
QString modeStr_1 = "Mode 1";
QString modeStr_2 = "Mode 2";
QString modeStr_3 = "Mode 3";
QString modeStr_4 = "Mode 4";


MainWindow* instance;
MainWindow *MainWindow::getInstance() {return instance;}

//// MainWindow构造函数 ////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    instance = this;    // 获取窗口实例，以便在普通函数中使用

    ui->setupUi(this);
    ui->centralWidget->setStyleSheet("QWidget#centralWidget{border-image:url(:/images/A002.jpg)}"); // 加载背景图片

    // LED复选框
    connect(ui->LED_1_chk,  SIGNAL(clicked()), this, SLOT(on_LED_1_chk_clicked()));
    connect(ui->LED_2_chk,  SIGNAL(clicked()), this, SLOT(on_LED_2_chk_clicked()));
    connect(ui->LED_3_chk,  SIGNAL(clicked()), this, SLOT(on_LED_3_chk_clicked()));
    connect(ui->LED_4_chk,  SIGNAL(clicked()), this, SLOT(on_LED_4_chk_clicked()));

    // Beep复选框
    connect(ui->Beep_chk,   SIGNAL(clicked()), this, SLOT( on_Beep_chk_clicked()));

    // Mode按键
    connect(ui->Mode_1_btn, SIGNAL(clicked()), this, SLOT(on_Mode_1_btn_clicked()));
    connect(ui->Mode_2_btn, SIGNAL(clicked()), this, SLOT(on_Mode_2_btn_clicked()));
    connect(ui->Mode_3_btn, SIGNAL(clicked()), this, SLOT(on_Mode_3_btn_clicked()));
    connect(ui->Mode_4_btn, SIGNAL(clicked()), this, SLOT(on_Mode_4_btn_clicked()));
    connect(ui->Clear_btn,  SIGNAL(clicked()), this, SLOT( on_Clear_btn_clicked()));

    // 开机自检，定时器3初始化，定时0.5秒
    timer3_init(500);

    // 启动设备
    open_leds();
    open_buttons();
    open_beep();

    // 按键初始化
    button_init();
}

//// MainWindow析构函数 ////
MainWindow::~MainWindow()
{
    delete ui;
    delete timer1;
    delete timer2;
    delete timer3;
    // 关闭设备
    close_leds();
    close_buttons();
    close_beep();
}

// TODO: 输出提示 （乱码）
void MainWindow::printInfo(int ctl, int mod)
{
    QString controlStr = "";
    QString modeStr = "";
    switch(ctl)
    {
        case 1:     controlStr = controlStr_qt; break;
        case 2:     controlStr = controlStr_py; break;
        default:    controlStr = "";            break;
    }
    switch(mod)
    {
        case 1:     modeStr = modeStr_1; break;
        case 2:     modeStr = modeStr_2; break;
        case 3:     modeStr = modeStr_3; break;
        case 4:     modeStr = modeStr_4; break;
        default:    modeStr = "";        break;
    }
    QString infoStr = modeStr + controlStr;
    ui->OUTPUT_LABEL->setText(infoStr);
}

/**************************************************/
/*******************  Soft事件  ********************/
/**************************************************/
// CheckBox
void MainWindow::on_LED_1_chk_clicked()
{
    ioctl(my_leds, int(ui->LED_1_chk->isChecked()), 0);
}
void MainWindow::on_LED_2_chk_clicked()
{
    ioctl(my_leds, int(ui->LED_2_chk->isChecked()), 1);
}
void MainWindow::on_LED_3_chk_clicked()
{
    ioctl(my_leds, int(ui->LED_3_chk->isChecked()), 2);
}
void MainWindow::on_LED_4_chk_clicked()
{
    ioctl(my_leds, int(ui->LED_4_chk->isChecked()), 3);
}
void MainWindow::on_Beep_chk_clicked()
{
    if(ui->Beep_chk->isChecked())
    {
        set_beep_freq(BEEP_FIRQ);//使峰鸣器鸣叫
    }
    else
    {
        stop_beep();        //关闭峰鸣器
    }
}


// Button
void MainWindow::on_Mode_1_btn_clicked()
{
    do_mode_1(SOFT_CTL);
}
void MainWindow::on_Mode_2_btn_clicked()
{
    do_mode_2(SOFT_CTL);
}
void MainWindow::on_Mode_3_btn_clicked()
{
    do_mode_3(SOFT_CTL);
}
void MainWindow::on_Mode_4_btn_clicked()
{
    do_mode_4(SOFT_CTL);
}
void MainWindow::on_Clear_btn_clicked()
{
    control = SOFT_CTL;
    ui->OUTPUT_LABEL->setText(QString::fromUtf8(""));
}

// TODO: 这是拓展功能
//void MainWindow::on_Extend_btn_clicked()
//{
//    control = SOFT_CTL;
//    ui->OUTPUT_LABEL->setText(QString::fromUtf8("See you again"));
//}

/**************************************************/
/********************  Hard事件  *******************/
/**************************************************/
//定时器1初始化函数，定时0.5秒
void MainWindow::timer1_init(int setTime)
{
    this->timer1 = new QTimer;
    this->timer1->setInterval(setTime);
    connect(this->timer1, SIGNAL(timeout()), this, SLOT(led_display()));
    this->timer1->start();
}
//定时器1关闭函数
void MainWindow::timer1_close()
{
    delete this->timer1;
    this->timer1 = 0;
}
//定时器1的信号槽函数，定时器1定时0.5秒，每隔0.5秒，调用一次此函数
void MainWindow::led_display()
{
    //先熄灭所有LED
    ioctl(my_leds, 0, 0);
    ioctl(my_leds, 0, 1);
    ioctl(my_leds, 0, 2);
    ioctl(my_leds, 0, 3);
    //点亮当前制定的LED
    ioctl(my_leds, 1, led_state);
    switch(led_state)
    {
        case 0: leds_state[0] = true;
                leds_state[1] = false;
                leds_state[2] = false;
                leds_state[3] = false;
                break;
        case 1: leds_state[0] = false;
                leds_state[1] = true;
                leds_state[2] = false;
                leds_state[3] = false;
                break;
        case 2: leds_state[0] = false;
                leds_state[1] = false;
                leds_state[2] = true;
                leds_state[3] = false;
                break;
        case 3: leds_state[0] = false;
                leds_state[1] = false;
                leds_state[2] = false;
                leds_state[3] = true;
                break;
        default:leds_state[0] = false;
                leds_state[1] = false;
                leds_state[2] = false;
                leds_state[3] = false;
                break;
    }
    ui->LED_1_chk->setChecked(leds_state[0]);
    ui->LED_2_chk->setChecked(leds_state[1]);
    ui->LED_3_chk->setChecked(leds_state[2]);
    ui->LED_4_chk->setChecked(leds_state[3]);
    //LED递增
    if(!led_judge)
    {
        led_state++;
    }
    //LED递减
    else
    {
        led_state--;
    }
    //当第四个LED点亮时，反转状态
    if(led_state >= 4)
    {
        led_judge = true;
        led_state = 2;
    }
    //当第一个LED点亮时，反转状态
    if(led_state < 0)
    {
        led_judge = false;
        led_state = 1;
    }
}


//定时器2初始化函数，定时1秒
void MainWindow::timer2_init(int setTime)
{
    this->timer2 = new QTimer;
    this->timer2->setInterval(setTime);
    connect(this->timer2, SIGNAL(timeout()), this, SLOT(beep_display()));
    this->timer2->start();
}
//定时器2关闭函数
void MainWindow::timer2_close()
{
    delete this->timer2;
    this->timer2 = 0;
}
//定时器2的信号槽函数，定时器2定时1秒钟，每隔1秒钟，调用一次此函数
void MainWindow::beep_display()
{
    beep_count++;
    beep_count %= 2;
    if(beep_count)
    {
        set_beep_freq(BEEP_FIRQ);
        beep_state = true;
        ui->Beep_chk->setChecked(beep_state);
    }
    else
    {
        stop_beep();
        beep_state = false;
        ui->Beep_chk->setChecked(beep_state);
    }
}


//定时器3初始化函数，定时0.5秒
void MainWindow::timer3_init(int setTime)
{
    this->timer3 = new QTimer;
    this->timer3->setInterval(setTime);
    connect(this->timer3, SIGNAL(timeout()), this,SLOT(selfpost()));
    this->timer3->start();
}
//定时器3关闭函数
void MainWindow::timer3_close()
{
    delete this->timer3;
    this->timer3 = 0;
}
//这是上电之后的自检程序，同时也是定时器三的信号槽函数，定时器三定时0.5秒，每隔0.5秒，此函数被调用一次
void MainWindow::selfpost()
{
    //因为只需要流水一遍，因此当四个LED都闪烁之后，就关闭定时器
    if(start_count > 8)
    {
        timer3_close();
        ioctl(my_leds, 0, 0);
        ioctl(my_leds, 0, 1);
        ioctl(my_leds, 0, 2);
        ioctl(my_leds, 0, 3);
    }
    else
    {
        //先熄灭所有LED
        ioctl(my_leds, 0,0);
        ioctl(my_leds, 0,1);
        ioctl(my_leds, 0,2);
        ioctl(my_leds, 0,3);
        //亮起对应的LED
        if(start_count % 2)
            ioctl(my_leds, 1, start_count/2);
    }
    start_count++;
}


//按键初始化
void MainWindow::button_init()
{
    //将按键与串口监听绑定
    notifier = new QSocketNotifier(my_buttons, QSocketNotifier::Read, this);
    //将按键与按键响应函数绑定
    connect(notifier, SIGNAL(activated(int)), this, SLOT(onKeyChanged()));
}

//按键的信号槽函数
void MainWindow::onKeyChanged(){
    char current_buttons[6];
    int i = 0;
    //读取当前按键设备数量，若与所有的设备数量不符，则报错，否则将按键端口的当前信息读取到currentbuttons数组内
    if (read(my_buttons, current_buttons, sizeof(current_buttons)) != sizeof(current_buttons))
    {
        perror("read buttons:");
        exit(1);
    }
    //判断哪个按键被按下，其判断依据是，读取到的按键数组中，哪一位与预定义的数组状态不同
    for (; i < (sizeof(buttons) / sizeof(buttons[0])); i++)
    {
        if (buttons[i] != current_buttons[i])
        {
            buttons[i] = current_buttons[i];
            switch(i)
            {
                case 0: do_mode_1(HARD_CTL); break; //若是按键一被按下，则执行模式一的代码
                case 1: do_mode_2(HARD_CTL); break; //若是按键二被按下，则执行模式二的代码
                case 2: do_mode_3(HARD_CTL); break; //若是按键三被按下，则执行模式三的代码
                case 3: do_mode_4(HARD_CTL); break; //若是按键四被按下，则执行模式四的代码
                default: ; break;
            }
        }
    }
}


/**************************************************/
/********************  硬件  ***********************/
/**************************************************/
//// 打开/dev/leds 设备文件 ////
void open_leds()
{
    ::system("kill -s STOP `pidof led-player`");
    my_leds = ::open("/dev/leds", O_RDONLY);
    if (my_leds < 0)
    {
        perror("open device leds");
        exit(1);
    }
}
void close_leds()
{
    close(my_leds);
    my_leds = -1;
}

//// 打开/dev/buttons 设备文件 ////
void open_buttons()
{
    my_buttons = open("/dev/buttons", O_RDONLY | O_NONBLOCK);
    if (my_buttons < 0)
    {
        perror("open device buttons");
        exit(1);
    }
}
void close_buttons()
{
    close(my_buttons);
    my_buttons = -1;
}

//// 打开/dev/pwm 设备文件 ////
void open_beep()
{
    my_beep = open("/dev/pwm", 0);
    if (my_beep < 0)
    {
        perror("open pwm_beep device");
        exit(1);
    }
}
void close_beep()
{
    if (my_beep >= 0)
    {
        ioctl(my_beep, PWM_IOCTL_STOP);
        close(my_beep);
        my_beep = -1;
    }
}
void set_beep_freq(int freq)
{
    int ret = ioctl(my_beep, PWM_IOCTL_SET_FREQ, freq);
    if(ret < 0)
    {
        perror("set beep frequency");
        exit(1);
    }
}
void stop_beep()
{
    int ret = ioctl(my_beep, PWM_IOCTL_STOP);
    if(ret < 0)
    {
        perror("stop the beep");
        exit(1);
    }
}


/**************************************************/
/********************  按键模式  *******************/
/**************************************************/
void MainWindow::do_mode_1(int ctl)
{
    //若有定时器开启，先把它们关闭
    close_timer_if_opened();
    // 设置控制者和模式
    control = ctl; mode = 1;
    printInfo(control, mode);
    // LED全亮
    ioctl(my_leds, 1, 0);
    ioctl(my_leds, 1, 1);
    ioctl(my_leds, 1, 2);
    ioctl(my_leds, 1, 3);
    leds_state[0] = true;
    leds_state[1] = true;
    leds_state[2] = true;
    leds_state[3] = true;
    ui->LED_1_chk->setChecked(leds_state[0]);
    ui->LED_2_chk->setChecked(leds_state[1]);
    ui->LED_3_chk->setChecked(leds_state[2]);
    ui->LED_4_chk->setChecked(leds_state[3]);
}
void MainWindow::do_mode_2(int ctl)
{
    //若有定时器开启，先把它们关闭
    close_timer_if_opened();
    // 设置控制者和模式
    control = ctl; mode = 2;
    printInfo(control, mode);
    //关闭所有峰鸣器与LED
    stop_beep();
    ioctl(my_leds, 0, 0);
    ioctl(my_leds, 0, 1);
    ioctl(my_leds, 0, 2);
    ioctl(my_leds, 0, 3);
    leds_state[0] = false;
    leds_state[1] = false;
    leds_state[2] = false;
    leds_state[3] = false;
    beep_state    = false;
    ui->LED_1_chk->setChecked(leds_state[0]);
    ui->LED_2_chk->setChecked(leds_state[1]);
    ui->LED_3_chk->setChecked(leds_state[2]);
    ui->LED_4_chk->setChecked(leds_state[3]);
    ui->Beep_chk->setChecked(beep_state);
}
void MainWindow::do_mode_3(int ctl)
{
    //若有定时器开启，先把它们关闭
    close_timer_if_opened();
    // 设置控制者和模式
    control = ctl; mode = 3;
    printInfo(control, mode);
    //打开定时器1
    timer1_init(500);
    timer1_state = 1;
}
void MainWindow::do_mode_4(int ctl)
{
    //若有定时器开启，先把它们关闭
    close_timer_if_opened();
    // 设置控制者和模式
    control = ctl; mode = 4;
    printInfo(control, mode);
    //打开定时器2
    timer2_init(1000);
    timer2_state = 1;
}
void MainWindow::close_timer_if_opened()
{
    //若有定时器开启，先把它们关闭
    if(timer1_state)
    {
        timer1_close(); timer1_state = 0;
    }
    if(timer2_state)
    {
        timer2_close(); timer2_state = 0;
    }
    stop_beep();
    ioctl(my_leds, 0, 0);
    ioctl(my_leds, 0, 1);
    ioctl(my_leds, 0, 2);
    ioctl(my_leds, 0, 3);
    leds_state[0] = false;
    leds_state[1] = false;
    leds_state[2] = false;
    leds_state[3] = false;
    beep_state    = false;
    ui->LED_1_chk->setChecked(leds_state[0]);
    ui->LED_2_chk->setChecked(leds_state[1]);
    ui->LED_3_chk->setChecked(leds_state[2]);
    ui->LED_4_chk->setChecked(leds_state[3]);
    ui->Beep_chk->setChecked(beep_state);
}
