#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tool_Curl.h"

class Tool_Curl : public QMainWindow
{
    Q_OBJECT

public:
    Tool_Curl(QWidget *parent = Q_NULLPTR);

    bool isStop()const { return mbStop; }
signals:
    void finished();
    void valueChanged(int);
protected slots :

    void startClicked();

    void stopClicked();

    void onFinished();

    void onProgressValue(int value);
private:
    Ui::Tool_CurlClass ui;

    bool              mbStop = false;
};
