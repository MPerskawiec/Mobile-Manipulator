/*
 * zyroskop&akcelerometr.c
 *
 *  Created on: 09.04.2019
 *      Author: Mariusz
 */

#include "zyroskop&akcelerometr.h"

void GYROSCOPE_Init(SPI_HandleTypeDef* SPIx) {

	uint8_t TxBuf[2];

	CS_GYRO_ON;
	TxBuf[0] = 0x20;  //
	TxBuf[1] = 0x2F;  // 95Hz, normal mode, Pomiar danych Z, X, Y
	while (HAL_SPI_Transmit(SPIx, TxBuf, 2, 50) != HAL_OK)
		;
	CS_GYRO_OFF;

	CS_GYRO_ON;
	TxBuf[0] = 0x23;
	TxBuf[1] = 0x11; // 500dps, 3-wire interface
	while (HAL_SPI_Transmit(SPIx, TxBuf, 2, 50) != HAL_OK)
		;
	CS_GYRO_OFF;

}

void ACCELEROMETER_Init(SPI_HandleTypeDef* SPIx) {

	uint8_t TxBuf[2];

	CS_ACC_ON;
	TxBuf[0] = 0x20;
	TxBuf[1] = 0x37; //
	while (HAL_SPI_Transmit(SPIx, TxBuf, 2, 50) != HAL_OK);
	CS_ACC_OFF;

	//ustawiamy tryb 3 przewodowy
	CS_ACC_ON;
	TxBuf[0] = 0x23;
	TxBuf[1] = 0x07; //
	while (HAL_SPI_Transmit(SPIx, TxBuf, 2, 50) != HAL_OK);
	CS_ACC_OFF;

}

void Read_Accelerometer(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct){

	uint8_t TxBuf[1];
	uint8_t Data[6];


	  CS_ACC_ON;
	  TxBuf[0] = 0x28 | 0x80;
	  while (HAL_SPI_Transmit(SPIx, TxBuf, 1, 50) != HAL_OK);
	  while(HAL_SPI_Receive(SPIx, Data, 6, 50)!=HAL_OK);
	  CS_ACC_OFF;


	  Datastruct->Accelerometer_X = ((Data[1] <<8) | Data[0] );
	  Datastruct->Accelerometer_Y = ((Data[3] <<8) | Data[2] );
	  Datastruct->Accelerometer_Z = ((Data[5] <<8) | Data[4] );

}

void Read_Accelerometer_RPY(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct){
	  Datastruct->Accelerometer_X_g =((float)  Datastruct->Accelerometer_X  - 700)/16384;
	  Datastruct->Accelerometer_Y_g = ((float) Datastruct->Accelerometer_Y - 1000)/16384;
	  Datastruct->Accelerometer_Z_g = ((float) Datastruct->Accelerometer_Z +200)/16384;

	  Datastruct->Accelerometer_roll = atan2(Datastruct->Accelerometer_Y_g, Datastruct->Accelerometer_Z_g) * (180/M_PI);
	  Datastruct->Accelerometer_pitch = atan2(Datastruct->Accelerometer_X_g, Datastruct->Accelerometer_Z_g) * (180/ M_PI);


}




void Read_Gyroscope(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct) {

	uint8_t TxBuf[1];
	uint8_t Data[6];

	CS_GYRO_ON;
	TxBuf[0] = 0x28 | 0xC0;    // czytanie i zwiekszanie adresow
	while (HAL_SPI_Transmit(SPIx, TxBuf, 1, 50) != HAL_OK);
	while (HAL_SPI_Receive(SPIx, Data, 6, 50) != HAL_OK);
	CS_GYRO_OFF;

	Datastruct->Gyroscope_X = ((Data[1] << 8) | Data[0]);
	Datastruct->Gyroscope_Y = ((Data[3] << 8) | Data[2]);
	Datastruct->Gyroscope_Z = ((Data[5] << 8) | Data[4]);

}

void Read_Gyroscope_RPY(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct) {

	Datastruct->Gyroscope_X_DGS_PAST = Datastruct->Gyroscope_X_DGS;
	Datastruct->Gyroscope_Y_DGS_PAST = Datastruct->Gyroscope_Y_DGS;
	Datastruct->Gyroscope_Z_DGS_PAST = Datastruct->Gyroscope_Z_DGS;

	// offset ustawie recznie
	// FS dla 500 DPI 17.5 mdps/deg
	// zamienic trzeba na 1/ 0.0175 = 57

	Datastruct->Gyroscope_X_DGS = (Datastruct->Gyroscope_X - 145) / 57.142857;
	Datastruct->Gyroscope_Y_DGS = (Datastruct->Gyroscope_Y - 0) / 57.142857;
	Datastruct->Gyroscope_Z_DGS = (Datastruct->Gyroscope_Z - 60) / 57.142857;

// Zapobieganiu ca³kowania wartocia skacz¹cym, gdy uk³ad jest w spoczynku

	if ((Datastruct->Gyroscope_X_DGS < 1 && Datastruct->Gyroscope_X_DGS > -1)
			&& (Datastruct->Gyroscope_X_DGS_PAST < 1
					&& Datastruct->Gyroscope_X_DGS_PAST > -1)) {
		Datastruct->Gyroscope_X_DGS = 0;
	}

	if ((Datastruct->Gyroscope_Y_DGS < 1 && Datastruct->Gyroscope_Y_DGS > -1)
			&& (Datastruct->Gyroscope_Y_DGS_PAST < 1
					&& Datastruct->Gyroscope_Y_DGS_PAST > -1)) {
		Datastruct->Gyroscope_Y_DGS = 0;
	}

	if ((Datastruct->Gyroscope_Z_DGS < 1 && Datastruct->Gyroscope_Z_DGS > -1)
			&& (Datastruct->Gyroscope_Z_DGS_PAST < 1
					&& Datastruct->Gyroscope_Z_DGS_PAST > -1)) {
		Datastruct->Gyroscope_Z_DGS = 0;
	}

	//  1/100    przerwanie wykonuje siê co 10ms, sekunda ma 1000 ms czyli 10/1000
	//  to jest czas ca³kowania
	Datastruct->Gyroscope_roll = Datastruct->Gyroscope_roll
			+ (0.5 * 0.01)
					* (Datastruct->Gyroscope_X_DGS
							+ Datastruct->Gyroscope_X_DGS_PAST);

	Datastruct->Gyroscope_pitch = Datastruct->Gyroscope_pitch
			+ (0.5 * 0.01)
					* (Datastruct->Gyroscope_Y_DGS
							+ Datastruct->Gyroscope_Y_DGS_PAST);

	Datastruct->Gyroscope_yaw = Datastruct->Gyroscope_yaw
			+ (0.5 * 0.01)
					* (Datastruct->Gyroscope_Z_DGS
							+ Datastruct->Gyroscope_Z_DGS_PAST);

	// zapobieganie > 360 stoni

	if (Datastruct->Gyroscope_roll >= 360 || Datastruct->Gyroscope_roll <= -360)
		Datastruct->Gyroscope_roll = 0;
	if (Datastruct->Gyroscope_pitch >= 360
			|| Datastruct->Gyroscope_pitch <= -360)
		Datastruct->Gyroscope_pitch = 0;
	if (Datastruct->Gyroscope_yaw >= 360 || Datastruct->Gyroscope_yaw <= -360)
		Datastruct->Gyroscope_yaw = 0;

}

