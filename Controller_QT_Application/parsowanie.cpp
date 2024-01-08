#include "parsowanie.h"
#include <QDataStream>
#include <string.h>
#include <QString>
#include <string.h>


/*!
 * \file
 * \brief Zawiera definicje metod klasy Parsowanie
 *
 * W pliku znajdują się definicje klasy Parsowanie
 * Są one odpowiedzialne za Parsowanie otrzymanej ramki danych
 * oraz przekazaniu danyc poprzez emitowanie odpowiedniego sygnału
 */

Parsowanie::Parsowanie(QObject *parent) : QObject(parent)
{
    //  qDebug() << "parsowanie dziala !\n";

}

/*!
 * Parsuje otrzymana ramke danych. Jezeli jej format jest poprawny i sie
 * wszystko zgadza zostaje emitowany odpoiedni sygnal,  w zaleznosci od
 * odebraej ramki przez ktory przekazywane sa dane.
 * \param[in] dane - Ramka danych przyslana do parsowania
 */
void Parsowanie::parse(QString dane){

   QString dane_buf = dane;   // potem to usunąć

   qDebug() << "odebr: " << dane;

    byte *dane_crc =new byte[20];
    memcpy(dane_crc, dane.toStdString().c_str(),15 );

    int odebrane_crc8;
    int wyliczone_crc8;

    odebrane_crc8 = dane.mid(15 , 3).toInt();
    wyliczone_crc8 = CRC8_DataArray(dane_crc, 15 );

   qDebug() << "od" << odebrane_crc8 << " W: " << wyliczone_crc8;

    QStringList odebrane_dane = dane.split(' ', QString::SkipEmptyParts);  // powoduje pomijanie pustych pól

     //   qDebug() << " MAMY P0: " <<  odebrane_dane.at(0) <<  odebrane_dane.at(1) <<  odebrane_dane.at(2);;


    if(odebrane_dane[0] == 'P' && odebrane_crc8 == wyliczone_crc8 ){

        if(1){ // tutaj dodać sprawdzanie sumy kontrolnej zamiast 1 odwołanie do funkcji


            if(odebrane_dane[1] == 'X'){

                int val_x = odebrane_dane.at(2).toInt();
                int val_y = odebrane_dane.at(3).toInt();

                emit dane_acc_gotowe(val_x, val_y);

            } else if (odebrane_dane[1] == 'k') {

                int val_x = odebrane_dane.at(2).toInt();
                int val_y = odebrane_dane.at(3).toInt();
                int val_z = odebrane_dane.at(4).toInt();

                emit dane_kinematyka_gotowe(val_x, val_y, val_z);

            } else if (odebrane_dane[1] == 'c') {

                int val_x = odebrane_dane.at(2).toInt();
                int val_y = odebrane_dane.at(3).toInt();
                int val_z = odebrane_dane.at(4).toInt();

                emit dane_kinematyka_odwrotna_gotowe(val_x, val_y, val_z);
            }




         //  qDebug() << " MAMY P0: " <<  odebrane_dane.at(0) <<  odebrane_dane.at(1) <<  odebrane_dane.at(2);

        }
    }
}

/*!
 * WYlicza wartość crc dla pojedynczego bitu
 *
 * \param[in] CRC_prev - poprzedni bajt
 * \param[in] CRC_prev - aktualny bajt
 */

byte CRC8_SingleByte(byte CRC_prev, byte Data)
{
    CRC_prev ^= Data;
    for ( byte Idx = 0; Idx<8 ;++Idx){
        if ((CRC_prev & 0x80)!=0){
            CRC_prev = (byte)((CRC_prev << 1) ^ POLYNOMIAL_9);
        }else{
            CRC_prev=(byte)(CRC_prev << 1);
        }
    }
    return CRC_prev;
}


/*!
 * Funckja odpowiedziala za wyliczanie crcu
 *
 * \param[in] *pdata - ramka danych
 * \param[in] Len- długość ramki danych
 */

byte CRC8_DataArray(byte *pData, byte Len)
{
    byte CRCfinal=0xff;

    for(byte Idx = 0; Idx<Len ; ++Idx){
        CRCfinal=CRC8_SingleByte(CRCfinal, pData[Idx]);
    }

    return CRCfinal;
}

















