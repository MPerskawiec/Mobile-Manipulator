#include "mainwindow.h"
#include "ui_mainwindow.h"


/*!
 * \file
 * \brief Zawiera definicje metod klasy Manipulator
 *
 * W pliku znajdują się definicje klasy Manipulator
 * Są one odpowiedzialne za komunikację z GUI.
 * W momencie wykrycia reakcju ze strony uzytkownika
 * emitowany jest odpowieni sygnal.
 */


/*!
 * Konstruktor - inicjalizuje stan poczatkowy aplikacji,
 * a także torzy dodatkowe watki odowiedzialne za parsowanie
 * oraz komunikacje danych
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    disable_pilot_panel_control();
    disable_application_panel_control();

    ui->application_control_checked->setEnabled(false);
    ui->pilot_control_checked->setEnabled(false);
    ui->actionRoz_cz->setEnabled(false);
    send_speed = 999;
    status_ruchu = SERVO_CZEKAJ;


    QPixmap stop_pix(":/led/img/stop.png");
    QIcon ButtonIcon(stop_pix);
    ui->STOP_button->setIcon(ButtonIcon);
    int w = ui->STOP_button->width();
    int h = ui->STOP_button->height();
    w = w - 8;
    h = h - 8;
    ui->STOP_button->setIconSize(QSize(h,w));

    QPixmap pix_battery(":/led/img/bateria_red.png");
     w = ui->Bateria->width();
     h = ui->Bateria->height();
    ui->Bateria -> setPixmap(pix_battery.scaled(w,h,Qt::KeepAspectRatio));

    QPixmap pix_speed(":/led/img/speed.png");
     w = ui->speed_label->width();
     h = ui->speed_label->height();
    ui->speed_label -> setPixmap(pix_speed.scaled(w,h,Qt::KeepAspectRatio));




    ui->plot->addGraph(); // blue line
   ui->plot->graph(0)->setPen(QPen(QColor(0, 0, 255)));
    ui->plot->addGraph(); // red line
    ui->plot->graph(1)->setPen(QPen(QColor(255, 0, 0)));
    ui->plot->addGraph(); // green line
    ui->plot->graph(2)->setPen(QPen(QColor(0, 255, 0)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    ui->plot->xAxis->setTicker(timeTicker);
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->yAxis->setRange(-100, 100);
    // make left and bottom axes transfer their ranges to right and top axes:

    connect( ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->plot->yAxis2, SLOT(setRange(QCPRange)));


    //  QTimer dataTimer;
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
   // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  //  dataTimer.start(0); // Interval 0 means to refresh as fast as possible



//****** watek od komunikacji
    Komunikacja *komunikacja = new Komunikacja;

    komunikacja->moveToThread(&komunikacjaThread);
    connect(this, &MainWindow::connect_to_port, komunikacja, &Komunikacja::polacz);
    connect(this, &MainWindow::send_servo1_value, komunikacja, &Komunikacja::send_servo1_value);
    connect(this, &MainWindow::send_kinematyka_odwrotna, komunikacja, &Komunikacja::send_kinematyka_odwrotna);
    connect(this, &MainWindow::send_poz, komunikacja, &Komunikacja::send_poz);
    connect(this, &MainWindow::disconnect_device, komunikacja, &Komunikacja::disconnect_device);

//****** watek parsowania danych
    Parsowanie *parsowanie = new Parsowanie;
    parsowanie->moveToThread(&parsowanieThread);
    connect(parsowanie, &Parsowanie::dane_acc_gotowe, this, &MainWindow::dane_acc);  // dane od wychylenia gotowe
    connect(parsowanie, &Parsowanie::dane_kinematyka_gotowe, this, &MainWindow::dane_kinematyka);  // Dane od kinematyki gotowe do aktualizacji
    connect(parsowanie, &Parsowanie::dane_kinematyka_odwrotna_gotowe, this, &MainWindow::dane_kinematyka_odrtona);  // Dane od kinematyki odwrotrnej gotowe do aktualizacji

    connect(komunikacja, &Komunikacja::receive_data, parsowanie, &Parsowanie::parse);
    connect(komunikacja, &Komunikacja::connect_status,  this, &MainWindow::status_polaczenia);


    connect(this, &MainWindow::SetQ0_deg, ui->widget, &MyOpenGLWidget::SetQ0_deg);
    connect(this, &MainWindow::SetQ1_deg, ui->widget, &MyOpenGLWidget::SetQ1_deg);
    connect(this, &MainWindow::SetQ2_deg, ui->widget, &MyOpenGLWidget::SetQ2_deg);
    connect(this, &MainWindow::SetQ3_deg, ui->widget, &MyOpenGLWidget::SetQ3_deg);
    connect(this, &MainWindow::SetQ4_deg, ui->widget, &MyOpenGLWidget::SetQ4_deg);


  //  ui->widget->SetQ0_deg(0);
  //  ui->widget->SetQ1_deg(-10);
  //  ui->widget->SetQ2_deg(-20);


    komunikacjaThread.start();
    parsowanieThread.start();




}

/*!
 * Destruktor - kasuje wątki
 */
MainWindow::~MainWindow()
{
    komunikacjaThread.quit();
    komunikacjaThread.wait();
    parsowanieThread.quit();
    parsowanieThread.wait();

    delete ui;
}

/*!
 * W momencie odebrania danych emitowany jest sygnal,
 * ktoremu przekazywane sa parametry w zaleznosci
 * od informacji na ktorym suwaku wykryto przemieszczenie
 * \param[in] value - Wartosc konta w stopniach
 */
void MainWindow::on_Q1_deg_slid_valueChanged(int value)
{
    ui->Q1_deg_label->setNum(value);
    emit send_servo1_value('a', value);
    emit SetQ0_deg(value);
}

/*!
 * W momencie zmiany polozenia suwaka emitowany jest sygnal,
 * ktoremu przekazywane sa parametry w zaleznosci
 * od informacji na ktorym suwaku wykryto przemieszczenie
 * \param[in] value - Wartosc konta w stopniach
 */
void MainWindow::on_Q2_deg_slid_valueChanged(int value)
{
    ui->Q2_deg_label->setNum(value);
    emit send_servo1_value('b', value);
    emit SetQ1_deg(value);
}

/*!
 * W momencie zmiany polozenia suwaka emitowany jest sygnal,
 * ktoremu przekazywane sa parametry w zaleznosci
 * od informacji na ktorym suwaku wykryto przemieszczenie
 * \param[in] value - Wartosc konta w stopniach
 */
void MainWindow::on_Q3_deg_slid_valueChanged(int value)
{
    ui->Q3_deg_label->setNum(value);
    emit send_servo1_value('c', value);
    emit SetQ2_deg(value);
}

/*!
 * W momencie zmiany polozenia suwaka emitowany jest sygnal,
 * ktoremu przekazywane sa parametry w zaleznosci
 * od informacji na ktorym suwaku wykryto przemieszczenie
 * \param[in] value - Wartosc konta w stopniach
 */
void MainWindow::on_Q4_deg_slid_valueChanged(int value)
{
    ui->Q4_deg_label->setNum(value);
    emit send_servo1_value('d', value);
    emit SetQ3_deg(value);
}

/*!
 * W momencie zmiany polozenia suwaka emitowany jest sygnal,
 * ktoremu przekazywane sa parametry w zaleznosci
 * od informacji na ktorym suwaku wykryto przemieszczenie
 * \param[in] value - Wartosc konta w stopniach
 */
void MainWindow::on_Q5_deg_slid_valueChanged(int value)
{
    ui->Q5_deg_label->setNum(value);
    emit send_servo1_value('e', value);
    emit SetQ4_deg(value);
}

/*!
 * Wybor przegubu, ktorym sterujemy
 */
void MainWindow::on_Q1_checked_clicked()
{
    emit send_servo1_value('s', 0);
    status_ruchu = SERVO_CZEKAJ;
}

/*!
 * Wybor przegubu, ktorym sterujemy
 */
void MainWindow::on_Q2_checked_clicked()
{
    emit send_servo1_value('s', 0);
    status_ruchu = SERVO_CZEKAJ;
}

/*!
 * Wybor przegubu, ktorym sterujemy
 */
void MainWindow::on_Q3_checked_clicked()
{
    emit send_servo1_value('s', 0);
    status_ruchu = SERVO_CZEKAJ;
}

/*!
 * Wybor przegubu, ktorym sterujemy
 */
void MainWindow::on_Q4_checked_clicked()
{
    emit send_servo1_value('s', 0);
    status_ruchu = SERVO_CZEKAJ;
}

/*!
 * Wybor przegubu, ktorym sterujemy
 */
void MainWindow::on_Q5_checked_clicked()
{
    emit send_servo1_value('s', 0);
    status_ruchu = SERVO_CZEKAJ;
}

/*!
 * Pobiera dane dla kinematyki odwrotnej odnosnie polozenia na osi x
 * W momencie podania wartosci sprawdzane sa warunki czy wartosc ta miesci
 * sie w strefie roboczej robota.
 * \param[in] arg1 - Dane odnosnie polozenia
 */
void MainWindow::on_X_position_textEdited(const QString &arg1)
{

    x_val = arg1.toInt();
    if (x_val >=0 && x_val <=24){
        ui->Y_position->setEnabled(true);
        max_y_value = static_cast<int>(sqrt(576 - x_val*x_val));
        QString zakres;
        zakres = "-";
        QString Number;
        Number = QString::number(max_y_value);
        zakres += Number + " : " + Number;
        ui->Y_FK_range->setText(zakres);

    } else {
        ui->Y_position->setEnabled(false);
    }
}

/*!
 * Pobiera dane dla kinematyki odwrotnej odnosnie polozenia na osi y
 * W momencie podania wartosci sprawdzane sa warunki czy wartosc ta miesci
 * sie w strefie roboczej robota.
 * \param[in] arg1 - Dane odnosnie polozenia
 */
void MainWindow::on_Y_position_textEdited(const QString &arg1)
{
    int val = arg1.toInt();
    ui->X_position->setEnabled(false);
    if (val >=-max_y_value && val <=max_y_value){
        ui->Z_position->setEnabled(true);
        sqrt_val = static_cast<int>(round(sqrt(pow(val,2)+ pow(x_val,2))));
        ui->Z_FK_range->setText(zakres_z(sqrt_val));
    } else {
         ui->Z_position->setEnabled(false);
    }
}

/*!
 * Pobiera dane dla kinematyki odwrotnej odnosnie polozenia na osi z
 * W momencie podania wartosci sprawdzane sa warunki czy wartosc ta miesci
 * sie w strefie roboczej robota.
 * \param[in] arg1 - Dane odnosnie polozenia
 */
void MainWindow::on_Z_position_textEdited(const QString &arg1)
{
    int val = arg1.toInt();
    ui->Y_position->setEnabled(false);
    if((val >= z1_l && val <= z1_h) || (val >= z2_l && val <= z2_h) ){
        ui->Send_forward_kinematics->setEnabled(true);
    } else{
        ui->Send_forward_kinematics->setEnabled(false);
    }
}

/*!
 * W momencie nacisniecia przycisku emitowany jest sygnal,
 * ktoremu przekazywane sa parametry odnosnie zadanej pozycji
 */
void MainWindow::on_Send_forward_kinematics_clicked()
{
    int val_x = ui->X_position->text().toInt();
    int val_y = ui->Y_position->text().toInt();
    int val_z = ui->Z_position->text().toInt();

    if(sqrt_val >= 21 && sqrt_val <= 23 && val_z == 20) val_z ++; // ominięcie martwej strefy
    emit send_kinematyka_odwrotna(val_x, val_y, val_z);

    ui->X_position->setText("0");
    ui->Y_position->setText("0");
    ui->Z_position->setText("0");

    ui->X_position->setEnabled(true);
    ui->Y_position->setEnabled(false);
    ui->Z_position->setEnabled(false);

    ui->Send_forward_kinematics->setEnabled(false);


}

/*!
 * Emituje sygnal zwiazany z awaryjnym zatrzymaniem robota
 */
void MainWindow::on_STOP_button_clicked()
{
      emit send_servo1_value('s', 0);
}

/*!
 * Zmiana predkosci ruchu manipulatora.
 * W momencie zmiany polozenia suwaka emitowany jest sygnal,
 * ktoremu przekazywane sa parametry w zaleznosci
 * od informacji pochodzacych z pozycji suwaka
 * \param[in] value - Wartosc konta w stopniach
 */
void MainWindow::on_speed_SLID_valueChanged(int value)
{
    ui->SPEED_VALUE_LCD->display(value);
    send_speed = value;
     emit send_servo1_value('p', value);
}

/*!
 * Reakcja na wcisniecie przycisku jest emitowanie sygnalu
 * zwiazanego z proba nawiazania polaczenia
 */
void MainWindow::on_actionPo_cz_triggered()
{

    emit connect_to_port();
}

/*!
 * Sprawdza czy udało się połączyć z manipulatorem;
 * Jeżeli nie wyświetla komunikat,
 * w przeciwnym razie uaktywnia panel sterowania,
 * \param[in] status - Informacja czy udało sie połączyć
 */
void MainWindow::status_polaczenia(int status){
status = POLACZONO;
if(status == POLACZONO){

    QPixmap pix_battery(":/led/img/bateria.png");
     int w = ui->Bateria->width();
     int h = ui->Bateria->height();
    ui->Bateria -> setPixmap(pix_battery.scaled(w,h,Qt::KeepAspectRatio));


    ui->actionPo_cz->setEnabled(false);
    ui->actionRoz_cz->setEnabled(true);
    enable_application_panel_control();
    ui->application_control_checked->setEnabled(true);
    ui->pilot_control_checked->setEnabled(true);
    ui->application_control_checked->setChecked(true);


    } else if(status == BRAK_POLACZENIA ){
       // qDebug() << "Nie znalezionu urządzenia docelowego!\n";
        QMessageBox::warning(this, tr("BŁĄD POŁĄCZENIA"),tr("Nie udało się połączyć z urządzeniem !!!"));
    }

}

/*!
 * Przetwarza dane dotyczace wychylenia zyroskopu oraz akcelerometru.
 * Jezeli wybrana jest opcja sterowania przegubami przy wykorzystaniu wychylenia
 * jest odpowiedzialna ze przetwarzanie tych danych
 * \param[in] dane_x - Wartosc kata w stopniach
 * \param[in] dane_y - Wartosc kata w stopniach
 */
void MainWindow::dane_acc(int dane_x, int dane_y){


    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plot->graph(0)->addData(key, dane_x);
      ui->plot->graph(1)->addData(key, dane_y);
      ui->plot->graph(2)->addData(key, 0);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot->xAxis->setRange(key, 10, Qt::AlignRight);
    ui->plot->replot();



    ui->ACC_VALUE_X_LCD->display(dane_x);
  //  ui->Dane_X_Prog->setValue(dane_x);

    ui->ACC_VALUE_Y_LCD->display(dane_y);
   // ui->Dane_Y_Prog->setValue(dane_y);

    // Cała obsługa sterowania wychyleniem


    if(ui->pilot_control_checked->isChecked()){  // Jeżeli wybrano sterowanie wychyleniem
         // qDebug() << "steroanie wychyleniem \n";

        int val = 0;
        if(ui->Q1_checked->isChecked() || ui->Q4_checked->isChecked() || ui->Q5_checked->isChecked()){

         //   qDebug() << "steroanie wychyleniem \n";
            val = abs(dane_x/5);
            if(val>10) val = 10;

            if(send_speed != val){
                send_speed = val;
                emit send_servo1_value('p', val);
                 ui->speed_SLID->setValue(val);
            }



        } else if(ui->Q2_checked->isChecked()|| ui->Q3_checked->isChecked()){

            val = abs(dane_y/5);
            if(val>10) val = 10;

            if(send_speed != val){
                send_speed = val;
                ui->speed_SLID->setValue(val);
                emit send_servo1_value('p', val);
            }


        }


        if(ui->Q1_checked->isChecked()){

            if((dane_x > 10) && (status_ruchu!=SERVO_KIERUNEK_DODATNI)){

                status_ruchu=SERVO_KIERUNEK_DODATNI;
                emit send_servo1_value('a', SERVO_1_MAX);

            } else if((dane_x < -10) && (status_ruchu!=SERVO_KIERUNEK_UJEMNY)){

                status_ruchu=SERVO_KIERUNEK_UJEMNY;
                emit send_servo1_value('a', SERVO_1_MIN);

            } else if((status_ruchu!=SERVO_STOP) && (dane_x <= 10) && (dane_x >= -10)){

                status_ruchu=SERVO_STOP;
                emit send_servo1_value('s', 0);
            }

        } else  if(ui->Q2_checked->isChecked()){

            if((dane_y > 10) && (status_ruchu!=SERVO_KIERUNEK_DODATNI)){

                status_ruchu=SERVO_KIERUNEK_DODATNI;
                emit send_servo1_value('b', SERVO_2_MAX);

            } else if((dane_y < -10) && (status_ruchu!=SERVO_KIERUNEK_UJEMNY)){

                status_ruchu=SERVO_KIERUNEK_UJEMNY;
                emit send_servo1_value('b', SERVO_2_MIN);

            } else if((status_ruchu!=SERVO_STOP) && (dane_y <= 10) && (dane_y >= -10)){

                status_ruchu=SERVO_STOP;
                emit send_servo1_value('s', 0);
            }

        } else  if(ui->Q3_checked->isChecked()){

            if((dane_y > 10) && (status_ruchu!=SERVO_KIERUNEK_DODATNI)){

                status_ruchu=SERVO_KIERUNEK_DODATNI;
                emit send_servo1_value('c', SERVO_3_MAX);

            } else if((dane_y < -10) && (status_ruchu!=SERVO_KIERUNEK_UJEMNY)){

                status_ruchu=SERVO_KIERUNEK_UJEMNY;
                emit send_servo1_value('c', SERVO_3_MIN);

            } else if((status_ruchu!=SERVO_STOP) && (dane_y <= 10) && (dane_y >= -10)){

                status_ruchu=SERVO_STOP;
                emit send_servo1_value('s', 0);
            }


        } else  if(ui->Q4_checked->isChecked()){

            if((dane_x > 10) && (status_ruchu!=SERVO_KIERUNEK_DODATNI)){

                status_ruchu=SERVO_KIERUNEK_DODATNI;
                emit send_servo1_value('d', SERVO_4_MAX);

            } else if((dane_x < -10) && (status_ruchu!=SERVO_KIERUNEK_UJEMNY)){

                status_ruchu=SERVO_KIERUNEK_UJEMNY;
                emit send_servo1_value('d', SERVO_4_MIN);

            } else if((status_ruchu!=SERVO_STOP) && (dane_x <= 10) && (dane_x >= -10)){

                status_ruchu=SERVO_STOP;
                emit send_servo1_value('s', 0);
            }

        } else  if(ui->Q5_checked->isChecked()){

            if((dane_x > 10) && (status_ruchu!=SERVO_KIERUNEK_DODATNI)){

                status_ruchu=SERVO_KIERUNEK_DODATNI;
                emit send_servo1_value('e', SERVO_5_MAX);

            } else if((dane_x < -10) && (status_ruchu!=SERVO_KIERUNEK_UJEMNY)){

                status_ruchu=SERVO_KIERUNEK_UJEMNY;
                emit send_servo1_value('e', SERVO_5_MIN);

            } else if((status_ruchu!=SERVO_STOP) && (dane_x <= 10) && (dane_x >= -10)){

                status_ruchu=SERVO_STOP;
                emit send_servo1_value('s', 0);
            }

        }


    }  // jeżeli wychyleniem

}


/*!
 * Aktualizuje na LCD odebrane dane dotyczace polozenia efektora
 * \param[in] dane_x - Wartosc konta w stopniach
 * \param[in] dane_y - Wartosc konta w stopniach
 * \param[in] dane_z - Wartosc konta w stopniach
 */
void MainWindow::dane_kinematyka(int dane_x, int dane_y, int dane_z){
    ui->X_EFFECTOR_LCD->display(dane_x);
    ui->Y_EFFECTOR_LCD->display(dane_y);
    ui->Z_EFFECTOR_LCD->display(dane_z);

}

/*!
 * Aktualizuje odebrane dane dotyczace wychylenia poszczegolnyh
 * serw w momenie realizowania zadania kinematyki odwrotnej.
 * Wychylenia te sa aktualizowane na suwakach.
 * \param[in] dane_x - Wartosc konta w stopniach
 * \param[in] dane_y - Wartosc konta w stopniach
 * \param[in] dane_ z- Wartosc konta w stopniach
 */
void MainWindow::dane_kinematyka_odrtona(int q1, int q2, int q3){
    ui->Q1_deg_slid->setValue(q1);
    ui->Q2_deg_slid->setValue(q2);
    ui->Q3_deg_slid->setValue(q3);
}


/*!
 * Wyłącza panel sterowania manipulatora z pozimu aplikacji
 */
void MainWindow::disable_application_panel_control()
{
    ui->Q1_deg_slid->setEnabled(false);
    ui->Q2_deg_slid->setEnabled(false);
    ui->Q3_deg_slid->setEnabled(false);
    ui->Q4_deg_slid->setEnabled(false);
    ui->Q5_deg_slid->setEnabled(false);

    ui->Q1_deg_label->setEnabled(false);
    ui->Q2_deg_label->setEnabled(false);
    ui->Q3_deg_label->setEnabled(false);
    ui->Q4_deg_label->setEnabled(false);
    ui->Q5_deg_label->setEnabled(false);

    ui->X_position->setEnabled(false);
    ui->Y_position->setEnabled(false);
    ui->Z_position->setEnabled(false);

    ui->speed_SLID->setEnabled(false);
    ui->Send_forward_kinematics->setEnabled(false);

    QPixmap pix_led_red(":/led/img/RED.PNG");

    int  w = ui->LED_APPLICATION->width();
    int  h = ui->LED_APPLICATION->height();
    ui->LED_APPLICATION -> setPixmap(pix_led_red.scaled(w,h,Qt::KeepAspectRatio));


}

/*!
 * Włącza panel sterowania manipulatora z pozimu aplikacji
 */
void MainWindow::enable_application_panel_control()
{
    ui->Q1_deg_slid->setEnabled(true);
    ui->Q2_deg_slid->setEnabled(true);
    ui->Q3_deg_slid->setEnabled(true);
    ui->Q4_deg_slid->setEnabled(true);
    ui->Q5_deg_slid->setEnabled(true);

    ui->Q1_deg_label->setEnabled(true);
    ui->Q2_deg_label->setEnabled(true);
    ui->Q3_deg_label->setEnabled(true);
    ui->Q4_deg_label->setEnabled(true);
    ui->Q5_deg_label->setEnabled(true);

    ui->X_position->setEnabled(true);
    ui->speed_SLID->setEnabled(true);


    QPixmap pix_led_green(":/led/img/GREEN.PNG");
    int w = ui->LED_APPLICATION->width();
    int h = ui->LED_APPLICATION->height();
    ui->LED_APPLICATION -> setPixmap(pix_led_green.scaled(w,h,Qt::KeepAspectRatio));


}

/*!
 * Wyłącza panel sterowania manipulatora z pozimu pilota
 */
void MainWindow::disable_pilot_panel_control()
{
    ui->Q1_checked->setEnabled(false);
    ui->Q2_checked->setEnabled(false);
    ui->Q3_checked->setEnabled(false);
    ui->Q4_checked->setEnabled(false);
    ui->Q5_checked->setEnabled(false);

    QPixmap pix_led_red(":/led/img/RED.PNG");
    int w = ui->LED_PILOT->width();
    int h = ui->LED_PILOT->height();
    ui->LED_PILOT -> setPixmap(pix_led_red.scaled(w,h,Qt::KeepAspectRatio));

}

/*!
 * Włącza panel sterowania manipulatora z pozimu pilota
 */
void MainWindow::enable_pilot_panel_control()
{
    ui->Q1_checked->setEnabled(true);
    ui->Q2_checked->setEnabled(true);
    ui->Q3_checked->setEnabled(true);
    ui->Q4_checked->setEnabled(true);
    ui->Q5_checked->setEnabled(true);

    QPixmap pix_led_green(":/led/img/GREEN.PNG");
    int w = ui->LED_PILOT->width();
    int h = ui->LED_PILOT->height();
    ui->LED_PILOT -> setPixmap(pix_led_green.scaled(w,h,Qt::KeepAspectRatio));

}

/*!
 * Zmiana trybu pracy na sterowanie przegubami
 */
void MainWindow::on_application_control_checked_clicked()
{
    emit send_servo1_value('s', 0);
    enable_application_panel_control();
    disable_pilot_panel_control();
}

/*!
 * Zmienia tryb pracy na sterowanie wychyleniem
 */
void MainWindow::on_pilot_control_checked_clicked()
{
    disable_application_panel_control();
    enable_pilot_panel_control();

    send_speed = 999;
    status_ruchu = SERVO_CZEKAJ;


    int val = 0;
    ui->SPEED_VALUE_LCD->display(val);
    ui->speed_SLID->setValue(val);
}

/*!
 * Wylicza zakres roboczy dla osi z uzglednajac polozenie na osiach x oraz y
 * \param[in] value - odleglosc zadanego punktu na plaszczyznie x,y od poczatku ukladu wspolrzednych
 * \return zakres strefy roboczej dla kierunku na osi Z
 */

QString MainWindow::zakres_z(int value){

    z1_l = 100;
    z1_h = 100;
    z2_l = 100;
    z2_h = 100;

    if(value == 0){
        z1_l = 39;
        z1_h = 42;
    } else if (value == 1){
        z1_l = 39;
        z1_h = 42;
    } else if (value == 2){
        z1_l = 39;
        z1_h = 42;
    } else if (value == 3){
        z1_l = 39;
        z1_h = 42;
    } else if (value == 4){
        z1_l = 39;
        z1_h = 42;
    } else if (value == 5){
        z1_l = 39;
        z1_h = 42;
    } else if (value == 6){
        z1_l = 38;
        z1_h = 42;
    } else if (value == 7){
        z1_l = 38;
        z1_h = 42;
    } else if (value == 8){
        z1_l = 38;
        z1_h = 42;
    } else if (value == 9){
        z1_l = 37;
        z1_h = 42;
    } else if (value == 10){
        z1_l = 36;
        z1_h = 41;
    } else if (value == 11){
        z1_l = 36;
        z1_h = 41;
    } else if (value == 12){
        z1_l = 35;
        z1_h = 40;
        z2_l = 0;
        z2_h = 5;
    } else if (value == 13){
        z1_l = 34;
        z1_h = 40;
        z2_l = 0;
        z2_h = 6;
    } else if (value == 14){
        z1_l = 33;
        z1_h = 39;
        z2_l = 1;
        z2_h = 7;
    } else if (value == 15){
        z1_l = 32;
        z1_h = 39;
        z2_l = 2;
        z2_h = 8;
    } else if (value == 16){
        z1_l = 30;
        z1_h = 38;
        z2_l = 2;
        z2_h = 10;
    } else if (value == 17){
        z1_l = 29;
        z1_h = 37;
        z2_l = 3;
        z2_h = 12;
    } else if (value == 18){
        z1_l = 27;
        z1_h = 36;
        z2_l = 4;
        z2_h = 13;
    } else if (value == 19){
        z1_l = 25;
        z1_h = 34;
        z2_l = 6;
        z2_h = 15;
    } else if (value == 20){
        z1_l = 23;
        z1_h = 33;
        z2_l = 7;
        z2_h = 17;
    } else if (value == 21){
        z1_l = 9;
        z1_h = 31;
    } else if (value == 22){
        z1_l = 11;
        z1_h = 29;
    } else if (value == 23){
        z1_l = 13;
        z1_h = 27;
    } else if (value == 24){
        z1_l = 19;
        z1_h = 21;
    }



    if(value >= 12 && value <= 20){

        QString zakres;
        zakres = "";
        QString Number;
        Number = QString::number(z2_l);
        zakres += Number + " : ";
        Number = QString::number(z2_h);
        zakres += Number + " U ";
        Number = QString::number(z1_l);
        zakres += Number + " : ";
        Number = QString::number(z1_h);
        zakres += Number;
        return  zakres;

    } else {

        QString zakres;
        zakres = "";
        QString Number;
        Number = QString::number(z1_l);
        zakres += Number + " : ";
        Number = QString::number(z1_h);
        zakres += Number;

        return  zakres;
    }


}


/*!
 * Rozłącza komunikację z urządzeniem
 * Wyłącza panel sterowania manipulatora z pozimu aplikacji
 * oraz Wyłącza panel sterowania manipulatora z pozimu pilota
 */
void MainWindow::on_actionRoz_cz_triggered()
{
    disable_pilot_panel_control();
    disable_application_panel_control();
    ui->application_control_checked->setEnabled(false);
    ui->pilot_control_checked->setEnabled(false);
    ui->ACC_VALUE_X_LCD->display(0);
    ui->ACC_VALUE_Y_LCD->display(0);

    ui->actionPo_cz->setEnabled(true);
    ui->actionRoz_cz->setEnabled(false);


    QPixmap pix_battery(":/led/img/bateria_red.png");
     int w = ui->Bateria->width();
     int h = ui->Bateria->height();
    ui->Bateria -> setPixmap(pix_battery.scaled(w,h,Qt::KeepAspectRatio));


    emit disconnect_device();
}
