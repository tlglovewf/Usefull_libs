#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tool_Curl.h"

class Tool_Curl : public QMainWindow
{
    Q_OBJECT

public:
    Tool_Curl(QWidget *parent = Q_NULLPTR);

    bool isStop()const { return mbStop; }
	void setLocFileSize(long long sz)
	{
		mlocfilesz = sz;
	}
	long long locFileSize()const
	{
		return mlocfilesz;
	}
signals:
    void finished(int);
    void valueChanged(int);
protected slots :

    void startClicked();

    void stopClicked();

    void onFinished(int result);

    void onProgressValue(int value);




private:
    Ui::Tool_CurlClass ui;

    bool              mbStop = false;

    long long         mlocfilesz = 0;
};
