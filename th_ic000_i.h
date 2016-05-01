#ifndef TH_IC000_I_H
#define TH_IC000_I_H
#include <QApplication>

class TH_IC000_I
{
public:
   typedef struct
    {
        quint8 type;
        quint8 code;
        quint16 count;
        QByteArray data;
    }DataFrame;

public:
    TH_IC000_I();

    static DataFrame ByteArrayToDataFrame(QByteArray arry);
};

#endif // TH_IC000_I_H
