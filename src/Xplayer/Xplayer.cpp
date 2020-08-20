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

//��ʱ�� ��������ʾ
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
}

Xplayer::~Xplayer()
{
	dt.Close();
}
