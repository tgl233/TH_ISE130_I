#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include <QKeyEvent>
#include <QStatusBar>
#include "convert.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    setWindowTitle(tr("4路交道 TH_ISE130_I "));
//    setFixedSize(320,240);
    setAllLineEditReadOnly(true);
    //    ui->spinBox->setFocus();
    this->remoteIP = QHostAddress::LocalHost;
    this->remotePort = 1000;
    this->fixedLoaclPort = true;
    this->localPort = 2000;
    this->timePeriod = 5;
    this->timer = new QTimer(this->parent());

    this->socket = new QUdpSocket(this);
    this->ui->statusBar->showMessage("按回车键开始测试！");

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::on_config);
    connect(this->timer,&QTimer::timeout,this,&MainWindow::on_timeOut);
    connect(this->ui->checkBox,&QCheckBox::clicked,this,&MainWindow::on_autoStart);
    connect(this->socket,&QUdpSocket::readyRead,this,&MainWindow::on_socketRecive);
//    connect(this->ui->spinBox,&QSpinBox::valueChanged,this,&MainWindow::on_refresh);
}

void MainWindow::setAllLineEditReadOnly(bool flag)
{
    ui->IN_1_AC->setReadOnly(flag);
    ui->IN_1_DC->setReadOnly(flag);
    ui->IN_2_AC->setReadOnly(flag);
    ui->IN_2_DC->setReadOnly(flag);
    ui->IN_3_AC->setReadOnly(flag);
    ui->IN_3_DC->setReadOnly(flag);
    ui->IN_4_AC->setReadOnly(flag);
    ui->IN_4_DC->setReadOnly(flag);
}

void MainWindow::on_config()
{
    qDebug()<<"push Button is clicked !";
    ConfigDialog *cfg = new ConfigDialog(this); //在堆上创建ConfigDialog对象
    cfg->setAttribute(Qt::WA_DeleteOnClose);    //设备关闭窗口时注销对对象
    connect(cfg,&ConfigDialog::ConfigChanged,this,&MainWindow::on_configChange);
    cfg->UpdateDisplay(remoteIP,remotePort,fixedLoaclPort,localPort,timePeriod);
    if(this->socket->isValid())
    {
        this->socket->close();
    }
    cfg->exec();
}

void MainWindow::on_configChange(QHostAddress remoteip, int remoteport, bool fixed, int localport,
                                 int timePeriod)
{
    qDebug()<<remoteip.toString()<<remoteport<<fixed<<localport<<timePeriod;
    if(fixed)
    {
        this->socket->bind(QHostAddress::Any, localport);
    }
    socket->connectToHost(remoteip,remoteport);
    this->timer->setInterval(timePeriod*1000);

    this->remoteIP = remoteip;
    this->remotePort = remoteport;
    this->fixedLoaclPort = fixed;
    this->localPort = localport;
    this->timePeriod = timePeriod;

    qDebug() << "socke is vaild: "<<socket->isValid();
    if(this->socket->isValid()){
        qDebug()<<"socket 为活动状态重启一次手动测试";
        on_startManual();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(this->ui->checkBox->isChecked()) return;
    if(event->key() == Qt::Key_Return && !this->timer->isActive())
    {
        qDebug()<<"Enter press";
        on_startManual();
    }
}

void MainWindow::on_startManual()
{
    if(!this->socket->isValid())
    {
        qDebug()<<"socket is invalid !";
        ui->statusBar->showMessage(tr(" UDP通信为非活动状态,请先配置！"));
        ui->pushButton->clicked();
    }else {
        this->ui->statusBar->showMessage(tr("正在测试……"));
        qDebug()<<socket->state();
        qDebug()<<"socket is valid "<<"发送测试命令码 01 BB";
        socket->writeDatagram(Convert::HexStringToByteArray("01 BB"),remoteIP,remotePort);
        qDebug()<<"开启定时器……";
        this->timer->start();
    }
}

void MainWindow::on_timeOut()
{
    this->ui->statusBar->showMessage(tr("远程主机超时未响应！回车重新开始测试……"));
    clearAllLineEdit();
    if(this->ui->checkBox->isChecked())
    {
        qDebug()<<"重新开始1次测试";
        on_startManual();
    }
    else
    {
        qDebug()<<"定时器关闭";
        this->timer->stop();

    }
}

void MainWindow::clearAllLineEdit()
{
    this->ui->IN_1_AC->clear();
    this->ui->IN_1_DC->clear();
    this->ui->IN_2_AC->clear();
    this->ui->IN_2_DC->clear();
    this->ui->IN_3_AC->clear();
    this->ui->IN_3_DC->clear();
    this->ui->IN_4_AC->clear();
    this->ui->IN_4_DC->clear();
}

void MainWindow::on_autoStart(bool flag)
{
    if(flag)
    {
        this->ui->pushButton->setEnabled(false);
        on_startManual();
    }
    else
    {
        this->ui->pushButton->setEnabled(true);
    }
}

void MainWindow::on_socketRecive()
{
    QByteArray rec;
    rec.resize(socket->pendingDatagramSize());
    QHostAddress ip;
    quint16 port;

    socket->readDatagram(rec.data(),socket->pendingDatagramSize(),&ip,&port);
    if(!rec.size())
    {
        qDebug()<<"接收到来自："<<ip.toString()<<":"<<port<<"的非正常数据";
        return;
    }

    TH_IC000_I::DataFrame data = TH_IC000_I::ByteArrayToDataFrame(rec);
    if(data.type != 0)
    {
        qDebug()<<"来自："<<ip<<":"<<port<<"的非测试数据："<<Convert::ByteArrayToHexString(rec);
        return;
    }
    if(data.code != 0xBB)
    {
        qDebug()<<"非道岔表示电压数据包";
        return;
    }
    if(data.count*2 != data.data.size())
    {
        qDebug()<<"非完整的道岔表示电压数据包";
        return;
    }
    this->ui->spinBox->setMaximum(data.count>>3);
    //循环测试始终显示最新的测试值，不对值进行暂存
    if(ui->checkBox->isChecked())
    {
    TH_ISE130_I::DataFrame val = TH_ISE130_I::toDatFrame(data.data.mid((ui->spinBox->value()-1)*16,16));
    setDisplayValue(val);
    lastDataArray.clear();
    }else{
        this->lastDataArray = data.data;
        this->on_spinBox_valueChanged(this->ui->spinBox->value());
    }

    qDebug()<<"sender: "<<ip.toString()<<"Port: "<<port<<Convert::ByteArrayToHexString(rec);
    this->ui->statusBar->showMessage(tr("测试完成！回车重新开始测试……"));
    qDebug()<<"定时器终止";
    this->timer->stop();
    if(this->ui->checkBox->isChecked())
    {
        qDebug("重新开始下1次测试!");
        on_startManual();
    }
}

void MainWindow::setDisplayValue(TH_ISE130_I::DataFrame value)
{
    this->ui->IN_1_DC->setText(QString::asprintf("%.2f",value.sVale.IN_1_DC));
    this->ui->IN_1_AC->setText(QString::asprintf("%.2f",value.sVale.IN_1_AC));
    this->ui->IN_2_DC->setText(QString::asprintf("%.2f",value.sVale.IN_2_DC));
    this->ui->IN_2_AC->setText(QString::asprintf("%.2f",value.sVale.IN_2_AC));
    this->ui->IN_3_DC->setText(QString::asprintf("%.2f",value.sVale.IN_3_DC));
    this->ui->IN_3_AC->setText(QString::asprintf("%.2f",value.sVale.IN_3_AC));
    this->ui->IN_4_DC->setText(QString::asprintf("%.2f",value.sVale.IN_4_DC));
    this->ui->IN_4_AC->setText(QString::asprintf("%.2f",value.sVale.IN_4_AC));
}

//void MainWindow::on_refresh(int index)
//{
//}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if(lastDataArray.size() != 0)
    {
    TH_ISE130_I::DataFrame val = TH_ISE130_I::toDatFrame(lastDataArray.mid((arg1-1)*16,16));
    setDisplayValue(val);
    }
}
