#pragma once

#include <iostream>
#include <QtWidgets/QWidget>

#include "ui_Xplayer.h"

using std::cout;
using std::endl;

class Xplayer : public QWidget
{
    Q_OBJECT

public:
    Xplayer(QWidget *parent = Q_NULLPTR);
	~Xplayer();

	//定时器，滑动条显示
	void timerEvent(QTimerEvent *e);

public slots:
	void OpenFile();

private:
    Ui::XplayerClass ui;
};
