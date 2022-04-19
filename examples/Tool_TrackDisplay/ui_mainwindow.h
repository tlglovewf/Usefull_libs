/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *bkWidget;
    QPushButton *btnLoad;
    QPushButton *btnClear;
    QPushButton *btnDistance;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setMaximumSize(QSize(800, 600));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        bkWidget = new QWidget(centralWidget);
        bkWidget->setObjectName(QString::fromUtf8("bkWidget"));
        bkWidget->setGeometry(QRect(0, 0, 800, 600));
        bkWidget->setMinimumSize(QSize(800, 600));
        bkWidget->setMaximumSize(QSize(800, 600));
        bkWidget->setAutoFillBackground(false);
        btnLoad = new QPushButton(bkWidget);
        btnLoad->setObjectName(QString::fromUtf8("btnLoad"));
        btnLoad->setGeometry(QRect(0, 0, 80, 25));
        btnClear = new QPushButton(bkWidget);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));
        btnClear->setGeometry(QRect(80, 0, 80, 25));
        btnDistance = new QPushButton(bkWidget);
        btnDistance->setObjectName(QString::fromUtf8("btnDistance"));
        btnDistance->setGeometry(QRect(160, 0, 80, 25));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnLoad->setText(QCoreApplication::translate("MainWindow", "LoadReal", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        btnDistance->setText(QCoreApplication::translate("MainWindow", "OpenDis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
