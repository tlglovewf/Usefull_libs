#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Test_Tree.h"

class Test_Tree : public QMainWindow
{
    Q_OBJECT

public:
    Test_Tree(QWidget *parent = Q_NULLPTR);


protected slots:

    void onClicked();

    void onTimer();

private:
    Ui::Test_TreeClass ui;
};
