#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QThread>
#include <QObject>
#include "komunikacja.h"
#include "parsowanie.h"
#include "QPixmap"
#include "QMessageBox"
#include <QTimer>
#include "myopenglwidget.h"

/*!
 * \file
 * \brief Definicja metody klasy Komunikacja
 *
 * Zawiera definicję klasy Komunikacja
 */

#define SERVO_CZEKAJ 0
#define SERVO_STOP 1
#define SERVO_KIERUNEK_DODATNI 2
#define SERVO_KIERUNEK_UJEMNY 3

#define SERVO_1_MAX 90
#define SERVO_1_MIN -90
#define SERVO_1_NEUTRAL 0

#define SERVO_2_MAX 50
#define SERVO_2_MIN -50
#define SERVO_2_NEUTRAL 0

#define SERVO_3_MAX 90
#define SERVO_3_MIN -90
#define SERVO_3_NEUTRAL 0

#define SERVO_4_MAX 80
#define SERVO_4_MIN -80
#define SERVO_4_NEUTRAL 0

#define SERVO_5_MAX 30
#define SERVO_5_MIN -30
#define SERVO_5_NEUTRAL 0

namespace Ui {
class MainWindow;
}


/*!
* \brief Modeluje pojęcie okna głównego
*
* Klasa modeluje pojęcie okna głównego. Jest odpowiedzialna
* za interakcję z urzytkownikiem. Reaguje na sygnały i wywołuje
* odpowieni slot
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread komunikacjaThread;
    QThread parsowanieThread;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /*!
     * \briefreaguje na zmianę położenia slidera
     */
    void on_Q1_deg_slid_valueChanged(int value);

    /*!
     * \brief reaguje na zmianę położenia slidera
     */
    void on_Q2_deg_slid_valueChanged(int value);

    /*!
    * \brief reaguje na zmianę położenia slidera
     */
    void on_Q3_deg_slid_valueChanged(int value);

    /*!
     * \brief reaguje na zmianę położenia slidera
     */
    void on_Q4_deg_slid_valueChanged(int value);

    /*!
    * \brief reaguje na zmianę położenia slidera
     */
    void on_Q5_deg_slid_valueChanged(int value);

    /*!
     * \brief Wybor przegubu, ktorym sterujemy
     */
    void on_Q1_checked_clicked();

    /*!
     * \brief Wybor przegubu, ktorym sterujemy
     */
    void on_Q2_checked_clicked();

    /*!
     * \brief Wybor przegubu, ktorym sterujemy
     */
    void on_Q5_checked_clicked();

    /*!
     * \brief Wybor przegubu, ktorym sterujemy
     */
    void on_Q4_checked_clicked();

    /*!
     * \brief Wybor przegubu, ktorym sterujemy
     */
    void on_Q3_checked_clicked();

    /*!
     * \brief  Funkcja pobiera dane dla kinematyki odwrotnej
     */
    void on_Z_position_textEdited(const QString &arg1);

    /*!
     * \brief Funkcja pobiera dane dla kinematyki odwrotnej
     */
    void on_Y_position_textEdited(const QString &arg1);

    /*!
     * \brief  Funkcja pobiera dane dla kinematyki odwrotnej
     */
    void on_X_position_textEdited(const QString &arg1);

    /*!
     * \brief  Funkcja emitująca dane dla kinematyki odwrotnej
     */
    void on_Send_forward_kinematics_clicked();

    /*!
     * \brief  Funkcja emitująca sygnał stopu dla manipulatora
     */
    void on_STOP_button_clicked();

    /*!
     * \brief  Funkcja emitująca zmiane prędkości ruchu manipulatora
     */
    void on_speed_SLID_valueChanged(int value);

    /*!
     * \brief  Funkcja odpowiedzialna za połączenie z urządzeniem
     */
    void on_actionPo_cz_triggered();

    /*!
     * \brief  Przełącza się na sterowanie z aplikacja
     */
    void on_application_control_checked_clicked();

    /*!
     * \brief  Przełącza się na sterowanie z pilota
     */
    void on_pilot_control_checked_clicked();

    /*!
     * \brief  Kończy połączenie z urządzeniem
     */
    void on_actionRoz_cz_triggered();

public slots:

    /*!
     * \brief  Reaguje na odebrane dane dotyczące akcelerometru
     */
    void dane_acc(int dane_x, int dane_y);

    /*!
     * \brief  Reaguje na odebrane dane dotyczące kinematyki prostej
     */
    void dane_kinematyka(int dane_x, int dane_y, int dane_z);

    /*!
     * \brief  Reaguje na odebrane dane dotyczące kinematyki odwrotnej
     */
    void dane_kinematyka_odrtona(int q1, int q2, int q3);

    /*!
     * \brief  Sprawdza czy udało się połączyć z urządzeniem
     */
    void status_polaczenia(int status);



signals:
    /*!
    * \brief Emituje sygnał, zmiany wychylenia przegubu
    *
    * Emituje sygnał, zmiany wychylenia przegubu. Po każdrorazowej
    * zmianie położenia suwaka dane są aktualizowane
    */
    void SetQ0_deg(int Q0_deg);
    /*!
    * \brief Emituje sygnał, zmiany wychylenia przegubu
    *
    * Emituje sygnał, zmiany wychylenia przegubu. Po każdrorazowej
    * zmianie położenia suwaka dane są aktualizowane
    */
    void SetQ1_deg(int Q0_deg);
    /*!
    * \brief Emituje sygnał, zmiany wychylenia przegubu
    *
    * Emituje sygnał, zmiany wychylenia przegubu. Po każdrorazowej
    * zmianie położenia suwaka dane są aktualizowane
    */
    void SetQ2_deg(int Q0_deg);
    /*!
    * \brief Emituje sygnał, zmiany wychylenia przegubu
    *
    * Emituje sygnał, zmiany wychylenia przegubu. Po każdrorazowej
    * zmianie położenia suwaka dane są aktualizowane
    */
    void SetQ3_deg(int Q0_deg);
    /*!
    * \brief Emituje sygnał, zmiany wychylenia przegubu
    *
    * Emituje sygnał, zmiany wychylenia przegubu. Po każdrorazowej
    * zmianie położenia suwaka dane są aktualizowane
    */
    void SetQ4_deg(int Q0_deg);

    /*!
    * \brief Emituje sygnał, połącz sie z urządzeniem
    *
    * Emituje sygnał, połączenia się z urządzeniem. W momencie
    * proby nawiązania połączenie emitowany jest sygnał sygnalizujacy
    * chec nawiązania połaczenia
    */
    void connect_to_port();

    /*!
    * \brief Emituje sygnał, zmiany wychylenia przegubu
    *
    * Emituje sygnał, zmiany wychylenia przegubu. W momencie emitowania sygnalu
    * przekazywane sa dane odnoscie ktory przegub ma byc zaktualizowany
    * oraz wartosc jaką zostaje aktualizowany
    *
    */
    void send_servo1_value(char znak, int value);

    /*!
    * \brief Emituje sygnał, nowego położenia manipulatora
    *
    * Emituje sygnał, nowego położenia manipulatora. Wysyłane sa dane
    * odnosnie zadanego polozenia na osiach x, y oraz z
    */
    void send_kinematyka_odwrotna(int value_x, int value_y, int value_z);

    /*!
    * \brief Emituje sygnał odnośnie odległości do pokoniana
    *
    * Emituje sygnał odnośnie odległości do pokoniana. Podajemy o
    * ile chcemy się przemieścić względem osi x oraz y
    */
    void send_poz(int value_x, int value_y);

    /*!
    * \brief Emituje sygnał, rozłącz urządzenie
    *
    * Emituje sygnał, rozłączenia z urządzeniem. Wysyła
    * sygnał, którego zadaniem jesy poinformowanie
    * watku dotyczącego komunikacji o rozlaczeniu urzadzenia
    */
    void disconnect_device();

private:
    /*!
     * \brief  Wyłącza sterowanie z aplikacji
     */
    void disable_application_panel_control();

    /*!
     * \brief  Wlącza sterowanie z aplikacji
     */
    void enable_application_panel_control();

    /*!
     * \brief  Wyłącza sterowanie z pilota
     */
    void disable_pilot_panel_control();

    /*!
     * \brief  Włącza sterowanie z pilota
     */
    void enable_pilot_panel_control();

    Ui::MainWindow *ui;

    /*!
    * \brief Szybkość ruchu manipulatora
    *
    * Pole przechowuje wartość dotyczącą szybkości ruchu manipulatora
    */
    int send_speed;

    /*!
    * \brief Status ruchu
    *
    * Zawiera status czy ruch się wykonuje czy też nie
    */
    int status_ruchu;

    /*!
    * \brief wspólrzędna y kinematyki odwronej
    *
    * Przechowuje dane dotyczące osi y podczas wyliczania
    * kinematyki odwrotnej manipulatora
    */
    int max_y_value;

    /*!
    * \brief wspólrzędna x kinematyki odwronej
    *
    * Przechowuje dane dotyczące osi x podczas wyliczania
    * kinematyki odwrotnej manipulatora
    */
    int x_val;

    /*!
    * \brief odległość efektora od poczatku układu współrzednych
    *
    * Przechowywuje wyliczoną wartość odległości efektora od początku układu współrzednych
    */
    int sqrt_val;

    /*!
    * \brief zmienna pomocnicza do obliczania zakresu kinematyki pdwrotnej
    */
    int z1_l;

    /*!
    * \brief zmienna pomocnicza do obliczania zakresu kinematyki pdwrotnej
    */
    int z1_h;

    /*!
    * \brief zmienna pomocnicza do obliczania zakresu kinematyki pdwrotnej
    */
    int z2_l;

    /*!
    * \brief zmienna pomocnicza do obliczania zakresu kinematyki pdwrotnej
    */
    int z2_h;

    /*!
     * \brief  Zwraca dostępny przedział wartości dla osi z
     */
    QString zakres_z(int value);
};

#endif // MAINWINDOW_H
