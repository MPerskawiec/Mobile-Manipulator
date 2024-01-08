#ifndef PARSOWANIE_H
#define PARSOWANIE_H

typedef unsigned char byte ;
#define POLYNOMIAL_9 0x31

/*!
 * \file
 * \brief Definicja klasy Parsowanie
 *
 * Zawiera definicję klasy Parsowanie
 * W pliku zawarte sa definicje ogolnych klas .
 */

#include <QObject>
#include <QDialog>
#include <QByteArray>
#include <QtWidgets>
#include <QDebug>



/*!
* \brief Modeluje pojęcie Rarsowanie
*
* Klasa jest odpowiedzialna za parsowanie danyc.
* Zaweira funkcję pozwalającą analizować otrzymane
* dane i w zależności od odebranego zestawu danych
* emituje odpowiedni sygnał
*/
class Parsowanie : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief  Parsowanie przekazanych danych
     */
    explicit Parsowanie(QObject *parent = nullptr);

signals:

    /*!
    * \brief Emituje sygnał, odebrano dane z akcelerometru
    *
    * Emituje sygnał, odebrano dane z akcelerometru. Przekazuje wartośc x oraz y
    */
    void dane_acc_gotowe(int dane_x, int dane_y);

    /*!
    * \brief Emituje sygnał, odebrano dane kinematyka prosta
    *
    * Emituje sygnał, odebrano dane kinematyka prosta. Przekazuje wartość odebranych
    * danych dotyczacych polozenia koncowki efektora na osi x,y oraz z
    */
    void dane_kinematyka_gotowe(int dane_x, int dane_y, int dane_z);

    /*!
    * \brief Emituje sygnał, odebrano dane kinematyka odwrotna
    *
    * Emituje sygnał, odebrano dane kinematyka odwrotna. Przekazuje wartość odebranych
    * danych dotyczących kineatyki odwrotnej
    */
    void dane_kinematyka_odwrotna_gotowe(int _q1, int _q2, int _q3);

public slots:
    void parse(QString dane);
};

/*!
 * \brief  wyliczanie CRC8 dla ctablicy o podanej długości
 */
byte CRC8_DataArray(byte *pData, byte Len);
/*!
 * \brief  wyliczanie CRC8 dla pojedynczego bajtu
 */
byte CRC8_SingleByte(byte CRC_prev, byte Data);
#endif // PARSOWANIE_H
