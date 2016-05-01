#ifndef CONVERT_H
#define CONVERT_H
#include <QApplication>

class Convert
{
public:
    Convert();

    static QByteArray HexStringToByteArray (QString hexString);
    static QString ByteArrayToHexString(QByteArray arry);
};

#endif // CONVERT_H
