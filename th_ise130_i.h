#ifndef TH_ISE130_I_H
#define TH_ISE130_I_H

#include <QApplication>

class TH_ISE130_I
{
public:
    typedef struct
    {
        union
        {
            float Value[8];
            struct
            {
                float IN_1_DC;
                float IN_1_AC;
                float IN_2_DC;
                float IN_2_AC;
                float IN_3_DC;
                float IN_3_AC;
                float IN_4_DC;
                float IN_4_AC;
            }sVale;
        };
    }DataFrame;

public:
    TH_ISE130_I();

    static DataFrame toDatFrame(QByteArray arry);
    DataFrame getData();
    float getValue(int pos);

private:
    DataFrame __data;
    QByteArray __arry;

private:
    typedef struct {
        union {
            struct {
                quint16 in_1_dc;
                quint16 in_1_ac;
                quint16 in_2_dc;
                quint16 in_2_ac;
                quint16 in_3_dc;
                quint16 in_3_ac;
                quint16 in_4_dc;
                quint16 in_4_ac;
            }svalue;
            quint16 Value[8];
        };
    }ByteData;

};

#endif // TH_ISE130_I_H
