/*
 * kinematyka.c
 *
 *  Created on: 11.05.2019
 *      Author: Mariusz
 */

#include "kinematyka.h"


void servo_init(SERVO* Datastruct){

	Datastruct->serwo1 = S_1_VALUE_START;  //
	Datastruct->serwo2 = S_2_VALUE_START;
	Datastruct->serwo3 = S_3_VALUE_START;
	Datastruct->serwo4 = S_4_VALUE_START;
	Datastruct->serwo5 = S_5_VALUE_START;

	Datastruct->speed = 5;

	Datastruct->serwo1_deg = 0;  // punkt 00
	Datastruct->serwo2_deg = 0;
	Datastruct->serwo3_deg = 0;
	Datastruct->serwo4_deg = 0;
	Datastruct->serwo5_deg = 0;

	Datastruct->serwo1_val = S_1_VALUE_START;
	Datastruct->serwo2_val = S_2_VALUE_START;
	Datastruct->serwo3_val = S_3_VALUE_START;
	Datastruct->serwo4_val = S_4_VALUE_START;
	Datastruct->serwo5_val = S_5_VALUE_START;

	kinematyka_prosta(Datastruct);

}


void kinematyka_prosta(SERVO* D){
//	M_PI/180
	// DEG_3 musi byc przeciwna czyli -deg bo idzie w 2 stronê !

	D->X = round(cos(D->serwo1_deg * (M_PI/180))
		   * cos((D->serwo2_deg - D->serwo3_deg) * (M_PI/180)) * D3_LENGTH
		   + cos(D->serwo1_deg*(M_PI/180))
		   * cos(D->serwo2_deg * (M_PI/180)) * D2_LENGTH);

	D->Y =round( sin(D->serwo1_deg * (M_PI/180))
		   * cos((D->serwo2_deg - D->serwo3_deg) * (M_PI/180)) * D3_LENGTH
		   + sin(D->serwo1_deg *(M_PI/180) )
		   * cos(D->serwo2_deg *(M_PI/180)) * D2_LENGTH);

	D->Z =round( sin((D->serwo2_deg - D->serwo3_deg) * (M_PI/180)) * D3_LENGTH
		   + sin(D->serwo2_deg * (M_PI/180)) * D2_LENGTH + D1_LENGTH);
}


void kinematyka_odwrotna(SERVO* D){


	float dist;     // odleglosc punktu od poczatku ukladu wspolrzednych
	float angle_1;  // katy dla poszczegolnych serw
	float angle_2;
	float angle_3;
	float P[3][2];  // Przechowuje nowo wyliczane punkty
	float b[2]; 	// Pozycja pocz¹tkowa - korzenia
	float t[2];		// Pozycja docelowa
	float r[2];
	float l[2];		// zmienne do skalowania nowych punktow
	float roznica;
	float d[2];
	d[0] = D2_LENGTH;
	d[1] = D3_LENGTH;

	// Stosujemy zamiane przestrzeni 3D na przestrzen 2D

	dist = sqrt(pow(D->x_odwrotna,2) + pow(D->y_odwrotna,2));   // obliczamy dystans
	if(D->x_odwrotna < 0){
		dist = -dist;
	}


	// Wyznaczamy wartosc kata dla 1 serwomechanizmu
	if(D->x_odwrotna == 0){
		if(D->y_odwrotna == 0){
			angle_1 = 0;
		}
		else if(D->y_odwrotna > 0){
			angle_1 = 90;
		}
		else{
			angle_1 = -90;
		}
	} else {
		float arg_x = D->x_odwrotna;
		float arg_y = D->y_odwrotna;
		angle_1 = atan(arg_y/arg_x) * 180/M_PI; // kont obrotu w okó³ q1
	}

	// Traktujemy jako punkt poszukiwan pozycje startowa robota
	P[0][0] = 0;
	P[0][1] = D1_LENGTH;
	P[1][0] = D2_LENGTH;
	P[1][1] = D1_LENGTH;
	P[2][0] = D2_LENGTH + D3_LENGTH;
	P[2][1] = D1_LENGTH;


	// sprawdzamy czy cel jest osiagalny jezzeli tak to

	b[0] = P[0][0];  // ustawiamy b jako pozycje pocz¹tkow¹
	b[1] = P[0][1];

	t[0] = dist;     // przypisujemy pozycje docelowa
	t[1] = D->z_odwrotna;

	roznica = sqrt(pow((t[0]-P[2][0]),2) + pow((t[1] - P[2][1]),2));

	while(roznica > TOLERANCJA){  // do poki odleglosc jest wieksza niz tolerancja

		P[2][0] = t[0]; 		 // ustawiamy efektor koncowy jako nowy cel
		P[2][1] = t[1];

		for(int i = 1; i>=0; i--){

			r[i] = sqrt(pow((P[i+1][0] - P[i][0]),2)
					+ pow((P[i+1][1] - P[i][1]),2));    // odleg³osc miedzy punktami

			l[i] = d[i]/r[i];

			P[i][0] = (1-l[i])*P[i+1][0] + l[i]*P[i][0];
			P[i][1] = (1-l[i])*P[i+1][1] + l[i]*P[i][1];

		}


		// w druga strone szukamy
		P[0][0] = b[0];
		P[0][1] = b[1];

		for(int i = 0; i<= 1; i++){

			l[i] = d[i]/r[i];
			P[i+1][0] = (1-l[i])*P[i][0] + l[i]*P[i+1][0];
			P[i+1][1] = (1-l[i])*P[i][1] + l[i]*P[i+1][1];

		}

		roznica = sqrt(pow((t[0]-P[2][0]),2) + pow((t[1] - P[2][1]),2));

	} // end od while tolerancja


	// zle wyznaczam punkty  !!! trzeba na tangensy przerobic !

	 angle_2 = atan((P[1][1]-P[0][1])/(P[1][0]-P[0][0])) * 180/M_PI; // kont obrotu w okó³ q2


	 angle_3 = atan((P[2][1]-P[1][1])/(P[2][0]-P[1][0])) * 180/M_PI - angle_2; // kont obrotu w okó³ q3
	 if(P[2][0]<P[1][0]){
		angle_3 = 180 + angle_3;
	 }


	 D->serwo1_deg = round(angle_1);
	 D->serwo2_deg = round(angle_2);
	 D->serwo3_deg = round(-angle_3);

	 D->serwo1_val = round(S_1_VALUE_START + D->serwo1_deg*VARTOSC_STOPNIA);
	 D->serwo2_val = round(S_2_VALUE_START + D->serwo2_deg*VARTOSC_STOPNIA);
	 D->serwo3_val = round(S_3_VALUE_START + D->serwo3_deg*VARTOSC_STOPNIA); // to serwo dziala odwrotnie - to plus


	/*
		D->P[2][0] = round(P[2][0]);
		D->P[2][1] = round(P[2][1]);

		D->P[1][0] = round(P[1][0]);
		D->P[1][1] = round(P[1][1]);

		D->P[0][0] = round(P[0][0]);
		D->P[0][1] = round(P[0][1]);
	*/

}



