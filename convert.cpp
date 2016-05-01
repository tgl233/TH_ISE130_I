#include "convert.h"
#include <QDebug>
#include <QMessageBox>

Convert::Convert()
{

}

QByteArray Convert::HexStringToByteArray(QString hexString)
{
    bool ok;
    QByteArray ret;

    hexString = hexString.trimmed();//去除首尾空格
    hexString = hexString.simplified(); //却除\n,\t,\r
    QStringList list = hexString.split(" ");//用空格拆分字符串

    foreach (QString str, list) {
        if(! str.isEmpty()) //为非空字符
        {
            if(str.length()>2){
                qDebug()<<"HexStringToByteArray: 空格分隔的16进制字符串长度不能>2 "<<str;
                //                continue;
                ret.clear();
                //                return ret;
                break;
            }
            int i = str.toShort(&ok,16); //把字符串按16进制转为对应的数字
            if(ok){
                ret.append(i);
            }else{
                qDebug()<<"HexStringToByteArray: 非法的16进制字符 "<< str;
                ret.clear();
                //                return ret;
                break;
            }
        }
    }
    return ret;
}

QString Convert::ByteArrayToHexString(QByteArray arry)
{
    QString str(arry.toHex().toUpper()); //先将arry转为与之对应的16进制编码对列，再根据新生成的对列生成字符串

    int len = str.length()/2; //循环长度
    for(int i=1; i<len; i++)
    {
        str.insert(i*2+i-1," ");    //每两个字符间插入1个空格
    }

    qDebug()<<str;
    return str;
}

