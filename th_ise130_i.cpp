#include "th_ise130_i.h"
#include <QDebug>

TH_ISE130_I::TH_ISE130_I()
{

}

float TH_ISE130_I::getValue(int pos)
{
    if(pos>=0 && pos <8)
    {
        return this->__data.Value[pos];
    }else{
        return -1;
    }

}

TH_ISE130_I::DataFrame TH_ISE130_I::getData()
{
    return this->__data;
}

TH_ISE130_I::DataFrame TH_ISE130_I::toDatFrame(QByteArray arry)
{
    TH_ISE130_I::DataFrame ret;

    if(arry.size() != 16 ){
        qDebug()<<"非法的TH_ISE130_I数据包： "<<arry.toHex().toUpper();
    }else{
        ByteData *pdat = (ByteData*)arry.data();
        for(int i=0; i<8 ; i++){
            ret.Value[i] = (float)pdat->Value[i]/100;
            qDebug("TH_ISE130_I->DataFrame->Value[%d]: %.2f",i,ret.Value[i]);
        }
    }

    return ret;
}
