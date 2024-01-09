/*
 * kinematyka.h
 *
 *  Created on: 11.05.2019
 *      Author: Mariusz
 */

#ifndef KINEMATYKA_H_
#define KINEMATYKA_H_

#include "main.h"
#include "math.h"


#define S_1_VALUE_START 141
#define S_2_VALUE_START 134
#define S_3_VALUE_START 137
#define S_4_VALUE_START 122
#define S_5_VALUE_START 150



#define VAL_MINUS_90 68.0   // wartosc serwa1 dla -90 stopni
#define VAL_PLUS_90 224.0   // wartosc serwa1 dla +90 stopni
#define VARTOSC_STOPNIA (VAL_PLUS_90 - VAL_MINUS_90)/180.0  // wartosc w przeliczeniu na 1 stopien

#define D1_LENGTH 20
#define D2_LENGTH 6
#define D3_LENGTH 18

#define TOLERANCJA 0.5


typedef struct  {

	volatile int serwo1; // aktualna pozycja serw
	volatile int serwo2;
	volatile int serwo3;
	volatile int serwo4;
	volatile int serwo5;

	volatile int speed;

	int serwo1_val;  // zadana pozycja serw do osiagniecia
	volatile int serwo2_val;
	volatile int serwo3_val;
	volatile int serwo4_val;
	volatile int serwo5_val;


	int serwo1_deg; // odczytana wartosc w kontach
    int serwo2_deg;
	int serwo3_deg;
	int serwo4_deg;
	int serwo5_deg;

	int X;  // zmienne do kinematyki prostej
	int Y;
	int Z;

	int x_odwrotna; // zmienne do kinematyki odwrotnej
	int y_odwrotna;
	int z_odwrotna;


	int P[3][2];  // do testowania w kinematyce odwrotnej
	//int angle_1;
	//int angle_2;
	//int angle_3;

} SERVO;


void servo_init(SERVO* Datastruct);
void kinematyka_prosta(SERVO* Datastruct);
void kinematyka_odwrotna(SERVO* Datastruct);



#endif /* KINEMATYKA_H_ */
