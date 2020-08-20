#include "Xplayer.h"
#include "XDemuxThread.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

static XDemuxThread dt;

Xplayer::Xplayer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	dt.Start();
	startTimer(40);
}

/*!
*@brief  双击全屏，重载鼠标事件类函数
*@param[out] 
*@param[in]  QMouseEvent * e  鼠标事件类
*@return     void  
*/
void Xplayer::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (isFullScreen())
		this->showNormal();
	else
		this->showFullScreen();
}


/*!
*@brief  窗口尺寸变化
*@param[out] 
*@param[in]  QResizeEvent * e  
*@return     void  
*/
void Xplayer::resizeEvent(QResizeEvent *e)
{
	//控制滑动条位置
	ui.playPos->move(50, this->height() - 100);

	ui.playPos->resize(this->width() - 100, ui.playPos->height());
	//控制打开文件按钮位置
	ui.openFile->move(100, this->height() - 150);
	ui.isPlay->move(ui.openFile->x() + ui.openFile->width() + 10, ui.openFile->y());
	ui.video->resize(this->size());
}


/*!
*@brief  定时器 滑动条显示
*@param[out] 
*@param[in]  QTimerEvent * e  
*@return     void  
*/
void Xplayer::timerEvent(QTimerEvent *e)
{
	long long total = dt.totalMs;
	if (total > 0)
	{
		double pos = (double)dt.pts / (double)total;
		int v = ui.playPos->maximum() * pos;
		ui.playPos->setValue(v);
	}
}

void Xplayer::OpenFile()
{
	//选择文件
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
	qDebug() << name << endl;
	if (name.isEmpty())return;
	this->setWindowTitle(name);
	if (!dt.Open(name.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(0, "error", "open file failed!");
		return;
	}
	SetPause(dt.isPause);
}


/*!
*@brief  设置暂停，播放按钮文字
*@param[out] 
*@param[in]  bool isPause  
*@return     void  
*/
void Xplayer::SetPause(bool isPause)
{
	if (isPause)
		ui.isPlay->setText(QString::fromLocal8Bit("播 放"));
	else
		ui.isPlay->setText(QString::fromLocal8Bit("暂 停"));
}

void Xplayer::PlayOrPause()
{
	bool isPause = !dt.isPause;
	SetPause(isPause);
	dt.SetPause(isPause);
}
Xplayer::~Xplayer()
{
	dt.Close();
}
