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
}