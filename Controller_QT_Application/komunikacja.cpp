#include "komunikacja.h"


/*!
 * \file
 * \brief Zawiera definicje metod klasy Komunikacja
 *
 * W pliku znajdują się definicje klasy Komunikacja
 * Są one odpowiedzialne za komunikację, odbieranie
 * oraz wysyłanie danych za pomocą portu szeregoego
 */





Komunikacja::Komunikacja(QObject *parent) : QObject(parent)
{
    BTM = new QSerialPort(this);
    port_dostepny = false;  // STM niedostepny
    nazwa_portu = "";       // brak nazwy portu
    serialBuf = "";

}

/*!
 * Destruktor. Jeże port jest otwarty, zostaje zamkniety. Przerywana jest komunikacja
 */
Komunikacja::~Komunikacja()
{
    if(BTM->isOpen()){
        BTM->close();
    }
}


/*!
 * Wyszukuje urządzenia podłączone do portów
 * oraz wyświetla ich vendor ID oraz Product ID
 */
void Komunikacja::avaliable_ports(){
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << Info.description() << "\n";
        qDebug() << "Has vendor id?: " << Info.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << Info.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << Info.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << Info.productIdentifier() << "\n";
    }
}

/*!
 * Sprawdza czy użądzenie o danym ID jest podłączone
 * Jeżeli tak przypisuję jego informacje o porcie do zmiennej nazwa_portu
 */
void Komunikacja::set_port(){
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        if(info.hasVendorIdentifier() && info.hasProductIdentifier()){
            if(info.vendorIdentifier() == vendor && info.productIdentifier() == product){
                nazwa_portu = info.portName();
                port_dostepny = true;
            }
        }
    }
}

/*!
 * Nawiazuje polaczenie z wybranym urzadzeniem, a także
 * ustawia parametry w jakim ma przebiegać komunikacja
 */
void Komunikacja::port_config(){
    if(port_dostepny){
        BTM->setPortName(nazwa_portu);
        BTM->open(QSerialPort::ReadWrite);        // Ustawiamy jako port do odczytu i zapisu
        BTM->setBaudRate(QSerialPort::Baud9600);  // Prędkość transmisji
        BTM->setDataBits(QSerialPort::Data8);
        BTM->setFlowControl(QSerialPort::NoFlowControl);
        BTM->setParity(QSerialPort::NoParity);
        BTM->setStopBits(QSerialPort::OneStop);
        QObject::connect(BTM, SIGNAL(readyRead()), this, SLOT(read_data()));
        status_polaczenia = POLACZONO;
    }else{
        status_polaczenia = BRAK_POLACZENIA;
     //   qDebug() << "Brak portu z ustalonymi indeksami!\n";
    }

    emit connect_status(status_polaczenia);
}

/*!
 * Slot wywolujacy metody zwiazane z nawizaniem polaczenia
 */
void Komunikacja::polacz(){
    set_port();
    port_config();

 //   qDebug() << "polacz - ciekawe czy zadziala !\n";
}


/*!
 * Funkcja odbierająca dane. Po odebraniu całej ramki danych zostaje emitowany sygnal, poprzez
 * który dane są dalej przekazywane do wątka parsującego
 */
void Komunikacja::read_data(){
  //    qDebug() << "tyyyyyyyyyyyyyyyi!\n";
/*
   QByteArray serialData2;// = BTM->readAll();
    while(BTM->waitForReadyRead(10)){
        serialData2 += BTM->readAll();
    }

    serialBuf = QString::fromStdString(serialData2.toStdString());
    emit receive_data(serialBuf);
    qDebug() << "co wyszlo? " << serialBuf ;
    serialBuf = "";
*/

      QStringList ramka_danych = serialBuf.split("\n");
      if(ramka_danych.length() <= 1){
          serialData = BTM->readAll();
          serialBuf += QString::fromStdString(serialData.toStdString());
      }
      else{

       //  qDebug() << " " << serialBuf ;
          emit receive_data(serialBuf);
          serialBuf = "";

      }


 }

/*!
 * Funkcja Wysyłająca za pomocą portu szeregowego ramkę danych o długości 20 znaków
 * \param[in] data - dane w formacie QString gotowe do wysłania
 */
// Funkcja Wysyłająca za pomocą portu szeregowego ramkę danych o długości 20 znaków
void Komunikacja::updatedata_data(QString data){


   BTM->write(data.toStdString().c_str(),20);
}

/*!
 * Przygotowywuje ramkę danych do wysłania dla zmiany wartosci obrotu
 * \param[in] znak - litery od a-e odpowiadające poszczególnym przegubom
 * \param[in] value - Wartosc kata w stopniach
 */
void Komunikacja::send_servo1_value(char znak, int value){

    Komunikacja::updatedata_data(QString("P s %1 %2                    ").arg(znak).arg(value));

}

/*!
 * Przygotowywuje ramkę danych do wysłania danych odnosnie nowego polozenia efektora
 * \param[in] value_x - nowa wartosc na osi X
 * \param[in] value_y - nowa wartosc na osi Y
 * \param[in] value_z - nowa wartosc na osi Z
 */
void Komunikacja::send_kinematyka_odwrotna(int value_x, int value_y, int value_z){

     Komunikacja::updatedata_data(QString("P o %1 %2 %3                ").arg(value_x).arg(value_y).arg(value_z));
}

void Komunikacja::send_poz(int value_x, int value_y){
    Komunikacja::updatedata_data(QString("P e %1 %2                  ").arg(value_x).arg(value_y));
}

void Komunikacja::disconnect_device()
{
    if(BTM->isOpen()){
        BTM->close();
    }
}



