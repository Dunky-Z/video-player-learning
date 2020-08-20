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

	//��ʱ������������ʾ
	void timerEvent(QTimerEvent *e);

	//���ڳߴ�仯
	void resizeEvent(QResizeEvent *e);

	//˫��ȫ��
	void mouseDoubleClickEvent(QMouseEvent *e);

	//��ͣ
	void SetPause(bool isPause);

public slots:
	void OpenFile();
	void PlayOrPause();
	void SliderPress();
	void SliderRelease();

private:
	bool isSliderPress = false;
private:
    Ui::XplayerClass ui;
};
