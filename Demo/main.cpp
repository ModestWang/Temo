#include <QApplication>
#include <QFont>
#include "mainwindow.h"
#include "hardware.h"

extern My_System my_system;

int main(int argc, char *argv[])
{
    My_System_Init(&my_system); // 初始化 my_system

    // 创建窗体
    QApplication a(argc, argv);
    a.setFont(QFont("serif")); // 设置中文字体，否则界面无法显示中文
    MainWindow w;
    w.show();
    
    return a.exec();
}

