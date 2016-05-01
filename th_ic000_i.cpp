#include "th_ic000_i.h"
#include <QDebug>

TH_IC000_I::TH_IC000_I()
{

}

TH_IC000_I::DataFrame TH_IC000_I::ByteArrayToDataFrame(QByteArray arry)
{
    TH_IC000_I::DataFrame ret;

    ret.type = arry.at(0);
    ret.code = arry.at(1);
    quint16 *temp = (quint16 *)arry.mid(2,2).data();
    ret.count = *temp;
    ret.data = arry.mid(4);

    qDebug("TH_COOO_I->type: 0x%X",ret.type);
    qDebug("TH_COOO_I->code: 0x%X",ret.code);
    qDebug("TH_COOO_I->count: 0x%X",ret.count);
    qDebug()<<"TH_COOO_I->data: "<<ret.data.toHex().toUpper();

    return ret;
}

