#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtNetwork/QUdpSocket>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

private slots:
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_Ok_clicked();
    void on_pushButton_cannel_clicked();

public slots:
    void UpdateDisplay(QHostAddress ip = QHostAddress("127.0.0.1"), int port = 2000,
                       bool fixed = true ,int localport = 1000, int period = 3);

signals:
    void ConfigChanged(QHostAddress newAddr , int newRemotePort, bool fixePort, int localPort, int timePeriod);

private:
    void Init();

private:
    Ui::ConfigDialog *ui;

};

#endif // CONFIGDIALOG_H
