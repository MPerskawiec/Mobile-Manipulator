/*
 * zyroskop&akcelerometr.h
 *
 *  Created on: 09.04.2019
 *      Author: Mariusz
 */

#ifndef ZYROSKOP_AKCELEROMETR_H_
#define ZYROSKOP_AKCELEROMETR_H_

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "main.h"
#include "Math.h"


#define CS_ACC_ON   HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_RESET)
#define CS_ACC_OFF  HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_SET)

#define CS_GYRO_ON   HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET)
#define CS_GYRO_OFF  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET)


typedef struct  {

	/* Public */


	int16_t Gyroscope_X;     /*!< Gyroscope value X axis */
	int16_t Gyroscope_Y;     /*!< Gyroscope value Y axis */
	int16_t Gyroscope_Z;     /*!< Gyroscope value Z axis */

	float Gyroscope_X_DGS;     /*!< Gyroscope value X axis */
	float Gyroscope_Y_DGS;     /*!< Gyroscope value Y axis */
	float Gyroscope_Z_DGS;     /*!< Gyroscope value Z axis */

	float Gyroscope_X_DGS_PAST;     /*!< Gyroscope value X axis */
	float Gyroscope_Y_DGS_PAST;     /*!< Gyroscope value Y axis */
	float Gyroscope_Z_DGS_PAST;     /*!< Gyroscope value Z axis */

	double Gyroscope_roll;
	double Gyroscope_pitch;
	double Gyroscope_yaw;

	int16_t Accelerometer_X; /*!< Accelerometer value X axis */
	int16_t Accelerometer_Y; /*!< Accelerometer value Y axis */
	int16_t Accelerometer_Z; /*!< Accelerometer value Z axis */

	float Accelerometer_X_g; /*!< Accelerometer value X axis */
	float Accelerometer_Y_g; /*!< Accelerometer value Y axis */
	float Accelerometer_Z_g; /*!< Accelerometer value Z axis */

	double Accelerometer_roll;
	double Accelerometer_pitch;
	//SPI_HandleTypeDef* SPIx;
} ACC_GYRO;



void GYROSCOPE_Init(SPI_HandleTypeDef* SPIx );
void ACCELEROMETER_Init(SPI_HandleTypeDef* SPIx );

void Read_Accelerometer(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct);
void Read_Accelerometer_RPY(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct);
void Read_Gyroscope(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct);
void Read_Gyroscope_RPY(SPI_HandleTypeDef* SPIx, ACC_GYRO* Datastruct);















#endif /* ZYROSKOP_AKCELEROMETR_H_ */
