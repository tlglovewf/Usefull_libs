#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tool_BinMerge.h"
#include <thread>
class Tool_BinMerge : public QMainWindow
{
    Q_OBJECT

public:
    Tool_BinMerge(QWidget *parent = Q_NULLPTR);

    void switchUiStatus(bool status);

    void printLine(const QString& line);

signals:
    void handleLog(const QString& path);
    void endhandle(bool status);
private slots:

    void chooseFiles();
    void chooseTransFile();
    void openExeDir();

    void getLvxData();
    void getpcapData();

    void onHandleLog(const QString& log);
    void onEndHandle(bool status);

    
protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::Tool_BinMergeClass ui;

    int mPgrIndex = 0;
    std::unique_ptr<std::thread>   mpThread;
};
