/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kinematyka.h"
#include "jazda.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ROZMIAR_RAMKI 30
typedef unsigned char byte;
#define POLYNOMIAL_9 0x31
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

SERVO S; // struktura przechowuj¹ca dane o serwomechanizmach

JAZDA J;
volatile int licznik;

volatile int L;
volatile int SL;

volatile int SP;
volatile int P;

volatile int tmp;

volatile int liczba_przeszkod;

volatile float dist;
volatile float angle;

volatile float dist_test;

volatile int start_pomiar_1;
volatile int start_value_pomiar_1;

volatile int start_pomiar_2;
volatile int start_value_pomiar_2;

volatile int start_pomiar_3;
volatile int start_value_pomiar_3;

volatile int start_pomiar_4;
volatile int start_value_pomiar_4;

volatile int opoznienie_jazdy_robota;
volatile int opoznienie_skretu_w_prawo;
volatile int opoznienie_skretu_w_lewo;

int rodzaj_przeszkody;
int rodzaj_przeszkody_poprzedniej;
int dist_omijania_przeszkody;
int szeroka_przeszkoda_przed_robotem;
int kat_omijania_przeszkody;
// volatile int pos_x;
// volatile int pos_y;

int flag_kinematyka_prosta;
int flag_kinematyka_odwrotna;
int flag_dane_czujniki_gotowe;
int flag_koniec_ruchu;

char Data[40];
char Data2[40]; // Tablica przechowujaca wysylana wiadomosc.
char Data_CRC8[10];
char Received[ROZMIAR_RAMKI];

volatile uint16_t pulse_count_1; // Licznik impulsow
volatile uint16_t pulse_count_2; // Licznik impulsow
volatile uint16_t positions_1; // Licznik przekreconych pozycji
volatile uint16_t positions_2; // Licznik przekreconych pozycji

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

byte CRC8_SingleByte(byte CRC_prev, byte Data) {
	CRC_prev ^= Data;
	for (byte Idx = 0; Idx < 8; ++Idx) {
		if ((CRC_prev & 0x80) != 0) {
			CRC_prev = (byte) ((CRC_prev << 1) ^ POLYNOMIAL_9);
		} else {
			CRC_prev = (byte) (CRC_prev << 1);
		}
	}
	return CRC_prev;
}
byte CRC8_DataArray(byte *pData, byte Len) {
	byte CRCfinal = 0xff;
	for (byte Idx = 0; Idx < Len; ++Idx) {
		CRCfinal = CRC8_SingleByte(CRCfinal, pData[Idx]);
	}
	return CRCfinal;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == HC_1_ECHO_Pin) {  // ECHO ZACZYNA ODPOWIADAÆ PO 480 ns

		if (start_pomiar_1 == 1) {
			start_value_pomiar_1 = licznik;
			start_pomiar_1 = 2;
		}

		else if (start_pomiar_1 == 2) {

			tmp = ((licznik - start_value_pomiar_1) * 10) / 58;

			L = 200;
			if (tmp <= 200 && tmp >= 5)
				L = tmp;
			start_pomiar_1 = 0;
		}

	}

	if (GPIO_Pin == HC_2_ECHO_Pin) {  // ECHO ZACZYNA ODPOWIADAÆ PO 480 ns

		if (start_pomiar_2 == 1) {
			start_value_pomiar_2 = licznik;
			start_pomiar_2 = 2;
		}

		else if (start_pomiar_2 == 2) {

			tmp = ((licznik - start_value_pomiar_2) * 10) / 58;

			SL = 200;
			if (tmp <= 200 && tmp >= 5)
				SL = tmp;
			start_pomiar_2 = 0;
		}

	}

	if (GPIO_Pin == HC_3_ECHO_Pin) {  // ECHO ZACZYNA ODPOWIADAÆ PO 480 ns

		if (start_pomiar_3 == 1) {
			start_value_pomiar_3 = licznik;
			start_pomiar_3 = 2;
		}

		else if (start_pomiar_3 == 2) {

			tmp = ((licznik - start_value_pomiar_3) * 10) / 58;

			SP = 200;
			if (tmp <= 200 && tmp >= 5)
				SP = tmp;
			start_pomiar_3 = 0;
		}

	}

	if (GPIO_Pin == HC_4_ECHO_Pin) {  // ECHO ZACZYNA ODPOWIADAÆ PO 480 ns

		if (start_pomiar_4 == 1) {
			start_value_pomiar_4 = licznik;
			start_pomiar_4 = 2;
		}

		else if (start_pomiar_4 == 2) {

			tmp = ((licznik - start_value_pomiar_4) * 10) / 58;

			P = 200;
			if (tmp <= 200 && tmp >= 5)
				P = tmp;
			start_pomiar_4 = 0;
		}

	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim) {

	static uint32_t cnt;
	static uint32_t cnt_distans;
	static uint16_t cnt_pwm;
	static uint32_t count;
	//static uint32_t licznik;

	if (htim == &htim4) {  // Przerwanie jest generowane co 0.01 ms !!!

		if (cnt > 40 && cnt < 250) {
			if (cnt == S.serwo1)
				HAL_GPIO_WritePin(SERVO_6_PWM_GPIO_Port, SERVO_6_PWM_Pin,
						GPIO_PIN_RESET);
			if (cnt == S.serwo2)
				HAL_GPIO_WritePin(SERVO_5_PWM_GPIO_Port, SERVO_5_PWM_Pin,
						GPIO_PIN_RESET);
			if (cnt == S.serwo3)
				HAL_GPIO_WritePin(SERVO_4_PWM_GPIO_Port, SERVO_4_PWM_Pin,
						GPIO_PIN_RESET);
			if (cnt == S.serwo4)
				HAL_GPIO_WritePin(SERVO_7_PWM_GPIO_Port, SERVO_7_PWM_Pin,
						GPIO_PIN_RESET);
			if (cnt == S.serwo5)
				HAL_GPIO_WritePin(SERVO_3_PWM_GPIO_Port, SERVO_3_PWM_Pin,
						GPIO_PIN_RESET);
		}
		licznik++;
		cnt++;

		if (cnt >= 1999) { // ca³a pêtla trwa 20 ms
			cnt = 0;
			HAL_GPIO_WritePin(SERVO_6_PWM_GPIO_Port, SERVO_6_PWM_Pin,
					GPIO_PIN_SET);
			HAL_GPIO_WritePin(SERVO_5_PWM_GPIO_Port, SERVO_5_PWM_Pin,
					GPIO_PIN_SET);
			HAL_GPIO_WritePin(SERVO_4_PWM_GPIO_Port, SERVO_4_PWM_Pin,
					GPIO_PIN_SET);
			HAL_GPIO_WritePin(SERVO_7_PWM_GPIO_Port, SERVO_7_PWM_Pin,
					GPIO_PIN_SET);
			HAL_GPIO_WritePin(SERVO_3_PWM_GPIO_Port, SERVO_3_PWM_Pin,
					GPIO_PIN_SET);
		}

		//	if (licznik == 1){
		//	HAL_GPIO_WritePin(HC_1_TRIG_GPIO_Port, HC_1_TRIG_Pin, GPIO_PIN_RESET);
		//		start_pomiar = 1;
		//	HAL_GPIO_WritePin(HC_2_TRIG_GPIO_Port, HC_2_TRIG_Pin, GPIO_PIN_RESET);
		//	HAL_GPIO_WritePin(HC_3_TRIG_GPIO_Port, HC_3_TRIG_Pin, GPIO_PIN_RESET);
		//	HAL_GPIO_WritePin(HC_4_TRIG_GPIO_Port, HC_4_TRIG_Pin, GPIO_PIN_RESET);

		//	}

		switch (licznik) {

		case 1:

			HAL_GPIO_WritePin(HC_1_TRIG_GPIO_Port, HC_1_TRIG_Pin, GPIO_PIN_SET);
			start_pomiar_1 = 1;
			break;

		case 10:
			HAL_GPIO_WritePin(HC_1_TRIG_GPIO_Port, HC_1_TRIG_Pin,
					GPIO_PIN_RESET);
			break;

		case 5001:
			HAL_GPIO_WritePin(HC_2_TRIG_GPIO_Port, HC_2_TRIG_Pin, GPIO_PIN_SET);
			start_pomiar_2 = 1;
			break;

		case 5010:
			HAL_GPIO_WritePin(HC_2_TRIG_GPIO_Port, HC_2_TRIG_Pin,
					GPIO_PIN_RESET);
			break;

		case 10001:
			HAL_GPIO_WritePin(HC_3_TRIG_GPIO_Port, HC_3_TRIG_Pin, GPIO_PIN_SET);
			start_pomiar_3 = 1;
			break;

		case 10010:
			HAL_GPIO_WritePin(HC_3_TRIG_GPIO_Port, HC_3_TRIG_Pin,
					GPIO_PIN_RESET);
			break;

		case 1501:
			HAL_GPIO_WritePin(HC_4_TRIG_GPIO_Port, HC_4_TRIG_Pin, GPIO_PIN_SET);
			start_pomiar_4 = 1;
			break;

		case 15010:
			HAL_GPIO_WritePin(HC_4_TRIG_GPIO_Port, HC_4_TRIG_Pin,
					GPIO_PIN_RESET);
			break;

		case 20000:

			licznik = 0;
			break;

		}

		if (cnt_pwm == J.PWM_LK)
			PWM_LK_OFF;
		if (cnt_pwm == J.PWM_PK)
			PWM_PK_OFF;

		if (cnt_pwm >= 100) {
			cnt_pwm = 0;
			PWM_LK_ON;
			PWM_PK_ON;
		}
		cnt_pwm++;

	} // koniec od htim4
	if (htim == &htim3) { // przeranie generowane co 0.01s

		J.status_pracy = DZIALAJ;  // CO 1MS KONTROLA PRACY ENKODEROW

		if (count >= 10 - S.speed) { // ustawiamy predkosc serwa

			if (S.serwo1 != S.serwo1_val) {
				if (S.serwo1 > S.serwo1_val)
					S.serwo1--;
				else
					S.serwo1++;

			}
			if (S.serwo2 != S.serwo2_val) {
				if (S.serwo2 > S.serwo2_val)
					S.serwo2--;
				else
					S.serwo2++;
			}
			if (S.serwo3 != S.serwo3_val) {
				if (S.serwo3 > S.serwo3_val)
					S.serwo3--;
				else
					S.serwo3++;
			}
			if (S.serwo4 != S.serwo4_val) {
				if (S.serwo4 > S.serwo4_val)
					S.serwo4--;
				else
					S.serwo4++;
			}
			if (S.serwo5 != S.serwo5_val) {
				if (S.serwo5 > S.serwo5_val)
					S.serwo5--;
				else
					S.serwo5++;
			}

			count = 0;
		}

		if (cnt_distans >= 40) {

			cnt_distans = 0;
			flag_dane_czujniki_gotowe = 1;

		}

		count++;
		cnt_distans++;

	} // koniec od htim 3
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

// parsowanie danych
	char *wsk;

	wsk = strtok(Received, " ");

	if (!strcmp(wsk, "P")) {

		wsk = strtok(NULL, " ");

		if (!strcmp(wsk, "s")) {

			wsk = strtok(NULL, " ");
			if (!strcmp(wsk, "a")) {  // sterowanie poszczególnymi przegubami
				wsk = strtok(NULL, " ");
				S.serwo1_deg = atoi(wsk);
				S.serwo1_val = S_1_VALUE_START + S.serwo1_deg * VARTOSC_STOPNIA;
			} else if (!strcmp(wsk, "b")) {
				wsk = strtok(NULL, " ");
				S.serwo2_deg = atoi(wsk);
				S.serwo2_val = S_2_VALUE_START + S.serwo2_deg * VARTOSC_STOPNIA;
			} else if (!strcmp(wsk, "c")) {
				wsk = strtok(NULL, " ");
				S.serwo3_deg = -atoi(wsk);
				S.serwo3_val = S_3_VALUE_START + S.serwo3_deg * VARTOSC_STOPNIA;
			} else if (!strcmp(wsk, "d")) {
				wsk = strtok(NULL, " ");
				S.serwo4_deg = atoi(wsk);
				S.serwo4_val = S_4_VALUE_START + S.serwo4_deg * VARTOSC_STOPNIA;
			} else if (!strcmp(wsk, "e")) {
				wsk = strtok(NULL, " ");
				S.serwo5_deg = atoi(wsk);
				S.serwo5_val = S_5_VALUE_START + S.serwo5_deg * VARTOSC_STOPNIA;
			}

			else if (!strcmp(wsk, "s")) {
				wsk = strtok(NULL, " ");

				S.serwo1_val = S.serwo1;
				S.serwo2_val = S.serwo2;
				S.serwo3_val = S.serwo3;
				S.serwo4_val = S.serwo4;
				S.serwo5_val = S.serwo5;

				S.serwo1_deg = round(
						(S.serwo1_val - S_1_VALUE_START) / (VARTOSC_STOPNIA));
				S.serwo2_deg = round(
						(S.serwo2_val - S_2_VALUE_START) / (VARTOSC_STOPNIA));
				S.serwo3_deg = round(
						(S.serwo3_val - S_3_VALUE_START) / (VARTOSC_STOPNIA));
				S.serwo4_deg = round(
						(S.serwo4_val - S_4_VALUE_START) / (VARTOSC_STOPNIA));
				S.serwo5_deg = round(
						(S.serwo5_val - S_5_VALUE_START) / (VARTOSC_STOPNIA));

			}

			else if (!strcmp(wsk, "p")) {
				wsk = strtok(NULL, " ");
				S.speed = atoi(wsk);
			}

			flag_kinematyka_prosta = 1; // wywo³anie liczenia kinematyki prostej

		} else if (!strcmp(wsk, "o")) {   // kienmatyka odwrotna
			wsk = strtok(NULL, " ");
			S.x_odwrotna = atoi(wsk);
			wsk = strtok(NULL, " ");
			S.y_odwrotna = atoi(wsk);
			wsk = strtok(NULL, " ");
			S.z_odwrotna = atoi(wsk);

			flag_kinematyka_odwrotna = 1; // liczymy kinematyke odwrotn¹

		} else if (!strcmp(wsk, "e")) {   // liczenie odleglosci
			wsk = strtok(NULL, " ");
			J.pos_x_koncowa += atoi(wsk) * 10;
			wsk = strtok(NULL, " ");
			J.pos_y_koncowa += atoi(wsk) * 10;
			liczba_przeszkod = 0;
			flag_koniec_ruchu = 1;

		}
	}

	//sprintf(Data2, "Jest \n\r");

//	sprintf(Data2, "P X %d %d %d \r\n     ", i,j, crc8);

	// HAL_UART_Transmit_IT(&huart3,  (uint8_t*)Data2, ROZMIAR_RAMKI); // Rozpoczecie nadawania danych z wykorzystaniem przerwan
	HAL_UART_Receive_IT(&huart3, (uint8_t*) Received, ROZMIAR_RAMKI); // Ponowne w³¹czenie nas³uchiwania

}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	flag_dane_czujniki_gotowe = 0;
	flag_koniec_ruchu = 0;
	start_pomiar_1 = 0;
	start_pomiar_2 = 0;
	start_pomiar_3 = 0;
	start_pomiar_4 = 0;

	start_value_pomiar_1 = 0;
	start_value_pomiar_2 = 0;
	start_value_pomiar_3 = 0;
	start_value_pomiar_4 = 0;

	dist_omijania_przeszkody = 0;
	szeroka_przeszkoda_przed_robotem = 0;
	kat_omijania_przeszkody = 0;
	opoznienie_jazdy_robota = 0;

	opoznienie_skretu_w_prawo = 0;
	opoznienie_skretu_w_lewo = 0;
	rodzaj_przeszkody_poprzedniej = BRAK_PRZESZKODY;

	liczba_przeszkod = 0;

	L = 50;
	SL = 50;

	SP = 50;
	P = 50;

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM4_Init();
	MX_USART3_UART_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */

	servo_init(&S);

	jazda_init(&J);

	HAL_TIM_Base_Start_IT(&htim4); // timer serwa
	HAL_TIM_Base_Start_IT(&htim3); // timer do szybkosci obrotow serw
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

	licznik = 0;

	TIM1->CNT = 5000;
	TIM2->CNT = 5000;

	//J.pos_x_koncowa = 1500;  // dla testow
	//J.pos_y_koncowa = -500;  // dla testow

//	LK_PRZOD;
//	PK_PRZOD;

	HAL_UART_Receive_IT(&huart3, (uint8_t*) Received, ROZMIAR_RAMKI);

//	HAL_GPIO_WritePin(PWM_PK_GPIO_Port, PWM_PK_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(PWM_LK_GPIO_Port, PWM_LK_Pin, GPIO_PIN_SET);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		//	 pulse_count_1 = TIM2->CNT; // przepisanie wartosci z rejestru timera
		//	 pulse_count_2 = TIM1->CNT; // lewy
		//	  positions_1 = pulse_count_1/500;
		//	  positions_2 = pulse_count_2/500;

		if (flag_dane_czujniki_gotowe == 1) {

			flag_dane_czujniki_gotowe = 0;
			sprintf(Data2, "P d %d %d %d %d  \r\n                    ", L, SL,
					SP, P);
			HAL_UART_Transmit_IT(&huart3, (uint8_t*) Data2, ROZMIAR_RAMKI);
		}

		if (flag_kinematyka_prosta == 1) {

			kinematyka_prosta(&S);
			sprintf(Data2, "P k %d %d %d      ", S.X, S.Y, S.Z);
			Data2[18] = '\r';
			Data2[19] = '\n';

			int val = CRC8_DataArray(Data2, 15);

			sprintf(Data_CRC8, "%d   ", val);

			Data2[15] = Data_CRC8[0];
			Data2[16] = Data_CRC8[1];
			Data2[17] = Data_CRC8[2];

			HAL_UART_Transmit_IT(&huart3, (uint8_t*) Data2, ROZMIAR_RAMKI);
			flag_kinematyka_prosta = 0;
		}

		if (flag_kinematyka_odwrotna == 1) {

			kinematyka_odwrotna(&S);
			sprintf(Data2, "P c %d %d %d     ", S.serwo1_deg, S.serwo2_deg,
					-S.serwo3_deg);

			Data2[18] = '\r';
			Data2[19] = '\n';

			int val = CRC8_DataArray(Data2, 15);

			sprintf(Data_CRC8, "%d   ", val);

			Data2[15] = Data_CRC8[0];
			Data2[16] = Data_CRC8[1];
			Data2[17] = Data_CRC8[2];

			HAL_UART_Transmit_IT(&huart3, (uint8_t*) Data2, ROZMIAR_RAMKI);
			flag_kinematyka_odwrotna = 0;
		}




		if(flag_koniec_ruchu == 1
				&& (abs(J.pos_x_koncowa - J.pos_x_globalna) < TOLERANCJA_ODLEGLOSCI)
				&& (abs(J.pos_y_koncowa - J.pos_y_globalna) < TOLERANCJA_ODLEGLOSCI) )
		{
						flag_koniec_ruchu = 0;
						sprintf(Data2, "P j   \r\n                        ");
						HAL_UART_Transmit_IT(&huart3, (uint8_t*) Data2, ROZMIAR_RAMKI);
			}






		if (J.status_pracy == DZIALAJ) {
			J.status_pracy = CZELAKJ;  // blokujemy kolejne wykonywanie

			J.status_ruchu = STOP;

			J.impulsy_lewy = TIM1->CNT - 5000;
			TIM1->CNT -= J.impulsy_lewy;

			J.impulsy_prawy = TIM2->CNT - 5000;
			TIM2->CNT -= J.impulsy_prawy;

			dist = ((float) (J.impulsy_lewy / IMPULS_NA_OBROT)
					* DLUGOSC_JAZDY_JEDNEGO_OBROTU
					+ (float) (J.impulsy_prawy / IMPULS_NA_OBROT)
							* DLUGOSC_JAZDY_JEDNEGO_OBROTU) / 2.0;

			angle = ((float) (J.impulsy_prawy / IMPULS_NA_OBROT)
					* DLUGOSC_JAZDY_JEDNEGO_OBROTU
					- (float) (J.impulsy_lewy / IMPULS_NA_OBROT)
							* DLUGOSC_JAZDY_JEDNEGO_OBROTU) / 2.0;

			J.kat_obrotu_globalny += (180 * angle)
					/ (M_PI * PROMIEN_ROZSTAWU_KOL);

			if (J.kat_obrotu_globalny > 180)
				J.kat_obrotu_globalny = -360 + J.kat_obrotu_globalny;
			else if (J.kat_obrotu_globalny < -180)
				J.kat_obrotu_globalny = 360 + J.kat_obrotu_globalny;

			J.pos_x_globalna = J.pos_x_globalna
					+ dist * cos(J.kat_obrotu_globalny * (M_PI / 180));
			J.pos_y_globalna = J.pos_y_globalna
					+ dist * sin(J.kat_obrotu_globalny * (M_PI / 180));

			//	if (abs(J.pos_x_koncowa - J.pos_x_globalna) > TOLERANCJA_ODLEGLOSCI
			//			|| abs(J.pos_y_koncowa - J.pos_y_globalna)
			//					> TOLERANCJA_ODLEGLOSCI) {

			rodzaj_przeszkody = wykryto_przeszkode(L, SL, SP, P);

			if (abs(J.pos_x_koncowa - J.pos_x_globalna) < TOLERANCJA_ODLEGLOSCI
					&& abs(J.pos_y_koncowa - J.pos_y_globalna)
							< TOLERANCJA_ODLEGLOSCI)
				rodzaj_przeszkody = BRAK_PRZESZKODY;

			if (rodzaj_przeszkody_poprzedniej != rodzaj_przeszkody) {
				zakoncz_ruch();
				rodzaj_przeszkody_poprzedniej = rodzaj_przeszkody;
				liczba_przeszkod++;

				if (liczba_przeszkod > 30) {
					J.pos_x_koncowa = J.pos_x_globalna;
					J.pos_y_koncowa = J.pos_y_globalna;

				}
			}

			if (rodzaj_przeszkody == LEWO || rodzaj_przeszkody == L_90) {
				opoznienie_skretu_w_lewo--;
				if (opoznienie_skretu_w_lewo <= 0) {
					obrot_w_lewo();
					opoznienie_jazdy_robota = 50;
					opoznienie_skretu_w_prawo = 50;
					dist_omijania_przeszkody = 300;
				}
			} else if (rodzaj_przeszkody == PRAWO) {
				opoznienie_skretu_w_prawo--;
				if (opoznienie_skretu_w_prawo <= 0) {
					obrot_w_prawo();
					opoznienie_jazdy_robota = 50;
					opoznienie_skretu_w_lewo = 50;
					dist_omijania_przeszkody = 300;
				}
				//	} else if (rodzaj_przeszkody == L_90) {
				//		obrot_w_lewo();
				//		dist_omijania_przeszkody = 400;
			} else if (rodzaj_przeszkody == BRAK_PRZESZKODY) {

				opoznienie_jazdy_robota--;
				if (opoznienie_jazdy_robota <= 0) {
					opoznienie_jazdy_robota = 0;
					if (dist_omijania_przeszkody > 0) {
						dist_omijania_przeszkody -= dist;
						opoznienie_skretu_w_lewo = 50;
						opoznienie_skretu_w_prawo = 50;
						jazda_do_przodu();
					} else {
						jedz_do_celu(&J);
						// zakoncz_ruch();

					}
				} // koniec od opoznienie jazdy

			} // koniec od wykryto przeszkode
			  //}   zakoncz_ruch();



		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_Encoder_InitTypeDef sConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 10000;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 15;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 15;
	if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_Encoder_InitTypeDef sConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 10000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 15;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 15;
	if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 6399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 99;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void) {

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 31;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 19;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void) {

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
			SERVO_6_PWM_Pin | PK_2A_Pin | PK_1A_Pin | PWM_PK_Pin
					| SERVO_3_PWM_Pin | SERVO_1_PWM_Pin | SERVO_2_PWM_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED_Pin | LK_1A_Pin | PWM_LK_Pin | SERVO_4_PWM_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			HC_1_TRIG_Pin | LK_2A_Pin | HC_3_TRIG_Pin | HC_2_TRIG_Pin
					| HC_4_TRIG_Pin | SERVO_5_PWM_Pin | PWM_LK2_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SERVO_7_PWM_GPIO_Port, SERVO_7_PWM_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : SERVO_6_PWM_Pin PK_2A_Pin PK_1A_Pin PWM_PK_Pin
	 SERVO_3_PWM_Pin SERVO_1_PWM_Pin SERVO_2_PWM_Pin */
	GPIO_InitStruct.Pin = SERVO_6_PWM_Pin | PK_2A_Pin | PK_1A_Pin | PWM_PK_Pin
			| SERVO_3_PWM_Pin | SERVO_1_PWM_Pin | SERVO_2_PWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : LED_Pin LK_1A_Pin PWM_LK_Pin SERVO_4_PWM_Pin */
	GPIO_InitStruct.Pin = LED_Pin | LK_1A_Pin | PWM_LK_Pin | SERVO_4_PWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : HC_3_ECHO_Pin */
	GPIO_InitStruct.Pin = HC_3_ECHO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HC_3_ECHO_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : HC_1_ECHO_Pin HC_2_ECHO_Pin */
	GPIO_InitStruct.Pin = HC_1_ECHO_Pin | HC_2_ECHO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : HC_1_TRIG_Pin LK_2A_Pin HC_3_TRIG_Pin HC_2_TRIG_Pin
	 HC_4_TRIG_Pin SERVO_5_PWM_Pin PWM_LK2_Pin */
	GPIO_InitStruct.Pin = HC_1_TRIG_Pin | LK_2A_Pin | HC_3_TRIG_Pin
			| HC_2_TRIG_Pin | HC_4_TRIG_Pin | SERVO_5_PWM_Pin | PWM_LK2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : HC_4_ECHO_Pin */
	GPIO_InitStruct.Pin = HC_4_ECHO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HC_4_ECHO_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SERVO_7_PWM_Pin */
	GPIO_InitStruct.Pin = SERVO_7_PWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SERVO_7_PWM_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
