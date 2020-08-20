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
*@brief  ˫��ȫ������������¼��ຯ��
*@param[out] 
*@param[in]  QMouseEvent * e  ����¼���
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
*@brief  ���ڳߴ�仯
*@param[out] 
*@param[in]  QResizeEvent * e  
*@return     void  
*/
void Xplayer::resizeEvent(QResizeEvent *e)
{
	//���ƻ�����λ��
	ui.playPos->move(50, this->height() - 100);

	ui.playPos->resize(this->width() - 100, ui.playPos->height());
	//���ƴ��ļ���ťλ��
	ui.openFile->move(100, this->height() - 150);
	ui.isPlay->move(ui.openFile->x() + ui.openFile->width() + 10, ui.openFile->y());
	ui.video->resize(this->size());
}


/*!
*@brief  ��ʱ�� ��������ʾ
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
	//ѡ���ļ�
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ����Ƶ�ļ�"));
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
*@brief  ������ͣ�����Ű�ť����
*@param[out] 
*@param[in]  bool isPause  
*@return     void  
*/
void Xplayer::SetPause(bool isPause)
{
	if (isPause)
		ui.isPlay->setText(QString::fromLocal8Bit("�� ��"));
	else
		ui.isPlay->setText(QString::fromLocal8Bit("�� ͣ"));
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
