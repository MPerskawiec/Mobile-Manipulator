#ifndef KOMUNIKACJA_H
#define KOMUNIKACJA_H

/*!
 * \file
 * \brief Definicja klasy Komunikacja
 *
 * Zawiera definicję klasy Komunikacja
 * W pliku zawarte sa definicje ogolnych klas .
 */


#include <QObject>
#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include <QSerialPortInfo>
#include <QtWidgets>
#include <QDebug>

#define vendor_id 1155
#define product_id 14155

#define POLACZONO 1
#define BRAK_POLACZENIA 0


/*!
* \brief Modeluje pojęcie komunikacji
*
* Klasa wykorzystana do połączenia się z urządzeniem za pomocą
* portu szeregowego. Jej atrybutami są zmienne zawierające
* infromacje na temat połączenia
*/
class Komunikacja : public QObject
{
    Q_OBJECT
private:
    /*!
    * \brief Obiekt klasy QSerialPort.
    *
    * Pole tworzy nowy obiekt klasy
    */
    QSerialPort *BTM;

    /*!
    * \brief vendor_id urządznenia
    *
    * Przechowywuje numer id urzadznia
    */
    static const quint16 vendor = vendor_id;

    /*!
    * \brief product_id urządzenia
    *
    * Przechowywuje numer id producenta
    */
    static const quint16 product = product_id;

    /*!
    * \brief Nazwa portu urządzenia
    *
    * Pole przechowywuje nazwe portu do ktorego podpieto urzadzenie
    */
    QString nazwa_portu;

    /*!
    * \brief status dostępności portu
    *
    * Pole zwraca true jeżeli znaleziono urządzenie o podanych parametrach
    * w przeciwnym razie otrzymujemy false
    */
    bool port_dostepny;

    /*!
    * \brief Przechowuje odczytane dane
    *
    * Pole przechowuje tablice odczytanych danych podczas jednego wywolania
    */
    QByteArray serialData;

    /*!
    * \brief Przechowuje odczytane dane
    *
    * Wykorzystywana do skladania odczytanych danych w całą ramkę
    */
    QString serialBuf;

public:
    explicit Komunikacja(QObject *parent = nullptr);
    ~Komunikacja();
    int status_polaczenia;

private slots:
    /*!
    * \brief Szuka dostępnych urządzeń
    */
    void avaliable_ports();

    /*!
    * \brief Sprawdza czy użądzenie o danym ID jest podłączone
    */
    void set_port();       // ustawianie portu

    /*!
    * \brief Nawiazuje polaczenie z wybranym urzadzeniem
    */
    void port_config();    // Konfiguracja modułu - QSerialPort

    /*!
    * \brief Funkcja odbierająca dane
    */
    void read_data();      // Odczytywanie danych z portu

    /*!
    * \brief Funkcja Wysyłająca dane
    */
    void updatedata_data(QString data);

signals:

    /*!
    * \brief Emituje sygnał, odebrano dane
    *
    * Emituje sygnał, odebrano dane. Dane te nastepnie sa przekazywane
    * do funkcji odpowiedzialnej za parsowanie danych
    */   
    void receive_data(QString dane);

    /*!
    * \brief Emituje sygnał, połączono z urządzeniem
    *
    * Emituje sygnał, połączono z urządzeniem w momencie nawiązania
    * komunikacji za pomoca portu szeregowego
    *
    */
    void connect_status(int status);

public slots:

    /*!
    * \brief Slot wywoływujący funkcje odpowiedzialne za połączenie
    */
    void polacz(); // slot do laczenia sie z discovery

    /*!
    * \brief Przygotowywuje ramkę danych do wyslania dla zmiany wychylenia przegubu
    */
    void send_servo1_value(char znak, int value);

    /*!
    * \brief Przygotowywuje ramkę danych do wyslania dla kinematyki odwrotnej
    */
    void send_kinematyka_odwrotna(int value_x, int value_y, int value_z);

    /*!
    * \brief Przygotowywuje ramkę danych do wyslania przemiezczenia robota
    */
    void send_poz(int value_x, int value_y);

    /*!
    * \brief Funkcja do rozłączenia komunikacji
    */
    void disconnect_device();
};

#endif // KOMUNIKACJA_H
