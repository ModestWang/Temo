/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCheckBox *LED_1_chk;
    QCheckBox *LED_2_chk;
    QCheckBox *LED_3_chk;
    QCheckBox *LED_4_chk;
    QCheckBox *Beep_chk;
    QPushButton *Mode_1_btn;
    QPushButton *Mode_2_btn;
    QPushButton *Mode_3_btn;
    QPushButton *Mode_4_btn;
    QPushButton *Clear_btn;
    QLabel *Label_1;
    QLabel *Label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        LED_1_chk = new QCheckBox(centralWidget);
        LED_1_chk->setObjectName(QString::fromUtf8("LED_1_chk"));
        LED_1_chk->setGeometry(QRect(10, 0, 85, 22));
        LED_2_chk = new QCheckBox(centralWidget);
        LED_2_chk->setObjectName(QString::fromUtf8("LED_2_chk"));
        LED_2_chk->setGeometry(QRect(10, 30, 85, 22));
        LED_3_chk = new QCheckBox(centralWidget);
        LED_3_chk->setObjectName(QString::fromUtf8("LED_3_chk"));
        LED_3_chk->setGeometry(QRect(10, 60, 85, 22));
        LED_4_chk = new QCheckBox(centralWidget);
        LED_4_chk->setObjectName(QString::fromUtf8("LED_4_chk"));
        LED_4_chk->setGeometry(QRect(10, 90, 85, 22));
        Beep_chk = new QCheckBox(centralWidget);
        Beep_chk->setObjectName(QString::fromUtf8("Beep_chk"));
        Beep_chk->setGeometry(QRect(10, 120, 85, 22));
        Mode_1_btn = new QPushButton(centralWidget);
        Mode_1_btn->setObjectName(QString::fromUtf8("Mode_1_btn"));
        Mode_1_btn->setGeometry(QRect(110, 0, 85, 27));
        Mode_2_btn = new QPushButton(centralWidget);
        Mode_2_btn->setObjectName(QString::fromUtf8("Mode_2_btn"));
        Mode_2_btn->setGeometry(QRect(110, 30, 85, 27));
        Mode_3_btn = new QPushButton(centralWidget);
        Mode_3_btn->setObjectName(QString::fromUtf8("Mode_3_btn"));
        Mode_3_btn->setGeometry(QRect(110, 60, 85, 27));
        Mode_4_btn = new QPushButton(centralWidget);
        Mode_4_btn->setObjectName(QString::fromUtf8("Mode_4_btn"));
        Mode_4_btn->setGeometry(QRect(110, 90, 85, 27));
        Clear_btn = new QPushButton(centralWidget);
        Clear_btn->setObjectName(QString::fromUtf8("Clear_btn"));
        Clear_btn->setGeometry(QRect(110, 120, 85, 27));
        Label_1 = new QLabel(centralWidget);
        Label_1->setObjectName(QString::fromUtf8("Label_1"));
        Label_1->setGeometry(QRect(10, 160, 261, 31));
        Label_2 = new QLabel(centralWidget);
        Label_2->setObjectName(QString::fromUtf8("Label_2"));
        Label_2->setGeometry(QRect(10, 200, 261, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        LED_1_chk->setText(QApplication::translate("MainWindow", "LED 1", 0, QApplication::UnicodeUTF8));
        LED_2_chk->setText(QApplication::translate("MainWindow", "LED 2", 0, QApplication::UnicodeUTF8));
        LED_3_chk->setText(QApplication::translate("MainWindow", "LED 3", 0, QApplication::UnicodeUTF8));
        LED_4_chk->setText(QApplication::translate("MainWindow", "LED 4", 0, QApplication::UnicodeUTF8));
        Beep_chk->setText(QApplication::translate("MainWindow", "Beep", 0, QApplication::UnicodeUTF8));
        Mode_1_btn->setText(QApplication::translate("MainWindow", "\346\250\241\345\274\217\344\270\200", 0, QApplication::UnicodeUTF8));
        Mode_2_btn->setText(QApplication::translate("MainWindow", "\346\250\241\345\274\217\344\272\214", 0, QApplication::UnicodeUTF8));
        Mode_3_btn->setText(QApplication::translate("MainWindow", "\346\250\241\345\274\217\344\270\211", 0, QApplication::UnicodeUTF8));
        Mode_4_btn->setText(QApplication::translate("MainWindow", "\346\250\241\345\274\217\345\233\233", 0, QApplication::UnicodeUTF8));
        Clear_btn->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
        Label_1->setText(QString());
        Label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
