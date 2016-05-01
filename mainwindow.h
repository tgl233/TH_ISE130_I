#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include "th_ic000_i.h"
#include "th_ise130_i.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void Init();
    void setAllLineEditReadOnly(bool flag);
    void clearAllLineEdit();
    void setDisplayValue(TH_ISE130_I::DataFrame value);

protected:
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_config();
    void on_configChange(QHostAddress remoteip, int remoteport, bool fixed, int localport, int timePeriod);
    void on_startManual();
    void on_timeOut();
    void on_autoStart(bool flag);
    void on_socketRecive();
//    void on_refresh(int index);
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QUdpSocket *socket;
    QHostAddress remoteIP;
    int remotePort;
    int localPort;
    int timePeriod;
    bool fixedLoaclPort;
    QTimer *timer;
    QByteArray lastDataArray;
};

#endif // MAINWINDOW_H
