#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hardware.h"

My_System my_system;

void My_System_Init(My_System *my_system)
{
    my_system->control = 0;         // 控制者：0-无，1-Qt按钮，2-物理按键
    my_system->mode = 0;            // 当前的模式：1、2、3、4
    my_system->led_state = 0;       // LED 流水灯状态：0～3对应LED1～4
    my_system->led_dir = 0;         // LED 流水灯方向：0-无，1-向右，2-向左
    my_system->beep_state = 0;      // Beep 状态：0-不响，1-响
    my_system->count = 0;           // 记录进入定时器0中断次数
}


MainWindow* instance;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    instance = this;
    ui->setupUi(this);
    ui->centralWidget->setStyleSheet("QWidget#centralWidget{border-image:url(:/image/A002.jpg)}"); // 加载背景图片
}

MainWindow *MainWindow::getInstance()
{
    return instance;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**************************************************/
/********************  事件  ***********************/
/**************************************************/

void MainWindow::on_pushButton_1_clicked()
{
    my_system.control = 1;
    my_system.mode = 1;
    ui->label->setText(QString::fromUtf8("当前控制模式为模式一，并由Qt按钮控制"));
}

void MainWindow::on_pushButton_2_clicked()
{
    my_system.control = 1;
    my_system.mode = 2;
    ui->label->setText(QString::fromUtf8("当前控制模式为模式二，并由Qt按钮控制"));
}

void MainWindow::on_pushButton_3_clicked()
{
    my_system.control = 1;
    my_system.mode = 3;
    ui->label->setText(QString::fromUtf8("当前控制模式为模式三，并由Qt按钮控制"));
}

void MainWindow::on_pushButton_4_clicked()
{
    my_system.control = 1;
    my_system.mode = 4;
    ui->label->setText(QString::fromUtf8("当前控制模式为模式四，并由Qt按钮控制"));
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
}


/**************************************************/
/********************  硬件  ***********************/
/**************************************************/

void eint_irq_init()
{
    /* 配置GPN0~5引脚为中断功能 */
    GPNCON &= ~(0xff);
    GPNCON |= 0xaa;

    /* 设置中断触发方式为: 下降沿触发 */
    EINT0CON0 &= ~(0xff);
    EINT0CON0 |= 0x33;

    /* 禁止屏蔽中断 */
    EINT0MASK &= ~(0x0f);

    // Select INT_EINT0 mode as irq
    VIC0INTSELECT = 0;

    /* 在中断控制器里使能这些中断 */
    VIC0INTENABLE |= (0x3); /* bit0: eint0~3, bit1: eint4~11 */

    isr** isr_array = (isr**)(0x71200100);

    isr_array[0] = (isr*)do_eint_irq;

    /*将GPK4-GPK7配置为输出口*/
    GPKCON0 = 0x11110000;

    /*熄灭四个LED灯*/
    GPKDATA = 0xf0;
}

void timer0_irq_init()
{
    /* 在中断控制器里使能timer0中断 */
    VIC0INTENABLE |= (1<<23);

    VIC0INTSELECT =0;

    isr** isr_array = (isr**)(0x7120015C);

    isr_array[0] = (isr*)do_timer0_irq;

    /*将GPK4-GPK7配置为输出口*/
    GPKCON0 = 0x11110000;

    /*熄灭四个LED灯*/
    GPKDATA = 0xff;
}

void do_eint_irq()
{
    /* 分辨是哪个中断 */
    // K1:LED1~LED4 全亮
    if(EINT0PEND & (1<<0))
    {
        my_system.control = 2;
        my_system.mode = 1;
        MainWindow::getInstance()->getUI()->label->setText(QString::fromUtf8("当前控制模式为模式一，并由物理按键控制"));
    }
    // K2:LED4~LED1 全灭，Beep 停止鸣叫
    else if(EINT0PEND & (1<<1))
    {
        my_system.control = 2;
        my_system.mode = 2;
        MainWindow::getInstance()->getUI()->label->setText(QString::fromUtf8("当前控制模式为模式二，并由物理按键控制"));
    }
    // K3:LED1~LED4 双向流水灯
    else if(EINT0PEND & (1<<2))
    {
        my_system.control = 2;
        my_system.mode = 3;
        MainWindow::getInstance()->getUI()->label->setText(QString::fromUtf8("当前控制模式为模式三，并由物理按键控制"));
    }
    // K4:Beep 按1秒间隔响
    else if(EINT0PEND & (1<<3))
    {
        my_system.control = 2;
        my_system.mode = 4;
        MainWindow::getInstance()->getUI()->label->setText(QString::fromUtf8("当前控制模式为模式四，并由物理按键控制"));
    }

    /* 清中断 */
    EINT0PEND   = 0x3f;
    VIC0ADDRESS = 0;
}

void do_timer0_irq()
{
    unsigned long uTmp;

    if(my_system.mode == 3) // 模式三：双向LED流水灯
    {
        if(my_system.led_dir == 1)
        {
            my_system.led_state = my_system.count++;
            if(my_system.count>=4)
            {
                my_system.led_dir = 1;
            }
        }
        else if(my_system.led_dir == 2)
        {
            my_system.led_state = my_system.count--;
            if(my_system.count<=0)
            {
                my_system.led_dir = 1;
            }
        }
        else
        {
            my_system.led_state = my_system.count % 4;
        }
    }
    else if(my_system.mode == 4) // 模式四：蜂鸣器
    {
        my_system.count++;
        my_system.count %= 2;
        my_system.beep_state = my_system.count;
    }


    //清timer0的中断状态寄存器
    uTmp = TINT_CSTAT;
    TINT_CSTAT = uTmp;
    VIC0ADDRESS=0x0;
}

// 初始化timer
void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb)
{
    unsigned long temp0;

    // 定时器的输入时钟 = PCLK / ( {prescaler value + 1} ) / {divider value} = PCLK/(65+1)/16=62500hz

    //设置预分频系数为 66
    temp0 = TCFG0;
    temp0 = (temp0 & (~(0xff00ff))) | (uprescaler<<0);
    TCFG0 = temp0;

    // 16 分频
    temp0 = TCFG1;
    temp0 = (temp0 & (~(0xf<<4*utimer))& (~(1<<20))) |(udivider<<4*utimer);
    TCFG1 = temp0;

    // 1s = 62500hz
    TCNTB0 = utcntb;
    TCMPB0 = utcmpb;

    // 手动更新
    TCON |= 1<<1;

    // 清手动更新位
    TCON &= ~(1<<1);

    // 自动加载和启动timer0
    TCON |= (1<<0)|(1<<3);

    // 使能timer0中断
    temp0 = TINT_CSTAT;
    temp0 = (temp0 & (~(1<<utimer)))|(1<<(utimer));
    TINT_CSTAT = temp0;
}

void mode_1()
{
    GPKDATA &= 0x0f;
}

void mode_2()
{
    GPKDATA |= 0xf0;
    GPFDAT &= 1<<13;
}

void mode_3()
{
    my_system.count = 0;
    timer_init(0, 65, 4, 62500 / 2, 0); // 定时0.5s
    GPKDATA = 0xf0 & ~(1<<(my_system.led_state + 4));
}

void mode_4()
{
    my_system.count = 0;
    timer_init(0, 65, 4, 62500, 0);     // 定时1s
    GPFDAT &= 1<<13;
    GPFDAT |= my_system.beep_state << 13;
}
