#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    Init();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::Init()
{
    setFixedSize(360,150);
    setWindowTitle(tr("Config :"));
    setModal(true);

    if(parent() == NULL) {
        this->done(Rejected);
    }
}

void ConfigDialog::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->spinBox_LocalPort->setEnabled(true);
    } else {
        ui->spinBox_LocalPort->setEnabled(false);
    }
}

void ConfigDialog::on_pushButton_Ok_clicked()
{
    this->done(QDialog::Accepted);
    emit ConfigChanged(QHostAddress(ui->lineEdit_RemoteIP->text()),
                       ui->spinBox_RemotePort->value(),
                       ui->checkBox->isChecked(),
                       ui->spinBox_LocalPort->value(),
                       ui->spinBox_Time->value());
}

void ConfigDialog::on_pushButton_cannel_clicked()
{
    this->done(Rejected);
}

void ConfigDialog::UpdateDisplay(QHostAddress ip, int port,
                                 bool fixed, int localport, int period)
{
    ui->lineEdit_RemoteIP->setText(ip.toString());
    ui->spinBox_RemotePort->setValue(port);
    ui->checkBox->setChecked(fixed);
    if(fixed){
    ui->spinBox_LocalPort->setValue(localport);
    }
    ui->spinBox_Time->setValue(period);
}
