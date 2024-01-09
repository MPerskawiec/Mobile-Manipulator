/*
 * jazda.h
 *
 *  Created on: 25.05.2019
 *      Author: Mariusz
 */

#ifndef JAZDA_H_
#define JAZDA_H_

#include "main.h"
#include "math.h"
#include<stdlib.h>

#define PK_STOP HAL_GPIO_WritePin(PK_1A_GPIO_Port, PK_1A_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(PK_2A_GPIO_Port, PK_2A_Pin, GPIO_PIN_SET)
#define PK_PRZOD HAL_GPIO_WritePin(PK_1A_GPIO_Port, PK_1A_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(PK_2A_GPIO_Port, PK_2A_Pin, GPIO_PIN_RESET)
#define PK_TYL HAL_GPIO_WritePin(PK_1A_GPIO_Port, PK_1A_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(PK_2A_GPIO_Port, PK_2A_Pin, GPIO_PIN_SET)

#define LK_STOP HAL_GPIO_WritePin(LK_1A_GPIO_Port, LK_1A_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(LK_2A_GPIO_Port, LK_2A_Pin, GPIO_PIN_SET)
#define LK_PRZOD HAL_GPIO_WritePin(LK_1A_GPIO_Port, LK_1A_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(LK_2A_GPIO_Port, LK_2A_Pin, GPIO_PIN_RESET)
#define LK_TYL HAL_GPIO_WritePin(LK_1A_GPIO_Port, LK_1A_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(LK_2A_GPIO_Port, LK_2A_Pin, GPIO_PIN_SET)

#define PWM_LK_ON HAL_GPIO_WritePin(PWM_LK_GPIO_Port, PWM_LK_Pin, GPIO_PIN_SET)
#define PWM_LK_OFF HAL_GPIO_WritePin(PWM_LK_GPIO_Port, PWM_LK_Pin, GPIO_PIN_RESET)

#define PWM_PK_ON HAL_GPIO_WritePin(PWM_PK_GPIO_Port, PWM_PK_Pin, GPIO_PIN_SET)
#define PWM_PK_OFF HAL_GPIO_WritePin(PWM_PK_GPIO_Port, PWM_PK_Pin, GPIO_PIN_RESET)

#define WARTOSC_PWM_LEWE_KOLO 31
#define WARTOSC_PWM_PRAWE_KOLO 30

#define TOLERANCJA_KATA 3
#define MALA_TOLERANCJA_TOLERANCJA_KATA 1
#define TOLERANCJA_ODLEGLOSCI 30

#define STOP 0
#define OBROT 1
#define JADZ 2

#define IMPULS_NA_OBROT 1800.0

// 131.95 mm  œrednio
#define DLUGOSC_JAZDY_JEDNEGO_OBROTU M_PI * 42.0
// mm
#define ROZSTAW_KOL 182.0
#define PROMIEN_ROZSTAWU_KOL 91.0

#define DZIALAJ 1
#define CZELAKJ 0

#define BRAK_PRZESZKODY 0
#define LEWO 1
#define PRAWO 2
#define L_90 3

volatile float roznica_kata_obrotu;

int opoznienie_jazdy;
int opoznienie_obrotu_lewo;
int opoznienie_obrotu_prawo;

int flag_jedziemy;

typedef struct  {

	float pos_x_globalna;  		// pozycja x wzgledem momentu startu
 	float pos_y_globalna;  		// pozycja y wzgledem momentu startu
    float kat_obrotu_globalny;	// kat obrotu wzgledem startu

	int pos_x_koncowa;         // pozycja x koncowa
	int pos_y_koncowa; 		   // pozycja y koncowa
	volatile float kat_obrotu_koncowy; // kat obrotu o jaki mamy sie obrocic wzgledem celu

	float odleglosc_do_celu;

	int status_ruchu;
 	int status_pracy; // potrzebna do timera ¿eby odczytywac co 1 ms

 	volatile int PWM_LK;
 	volatile int PWM_PK;

	volatile int16_t impulsy_lewy;  // liczba impulsow z lewego enkodera
	volatile int16_t impulsy_prawy; // liczba impulsow z prawego enkodera



} JAZDA;

void jazda_init(JAZDA* Datastruct);

void licz_kat_obrotu(JAZDA* Datastruct);
void licz_odleglosc_do_celu(JAZDA* Datastruct);

void jedz_do_celu(JAZDA* J);

void obrot_w_prawo();
void obrot_w_lewo();
void jazda_do_przodu();
void zakoncz_ruch();

int wykryto_przeszkode(int L, int SL, int SP, int P);


#endif /* JAZDA_H_ */
