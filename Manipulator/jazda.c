/*
 * jazda.c
 *
 *  Created on: 25.05.2019
 *      Author: Mariusz
 */

#include "jazda.h"

void jazda_init(JAZDA* Datastruct) {

	Datastruct->pos_x_globalna = 0;
	Datastruct->pos_y_globalna = 0;
	Datastruct->kat_obrotu_globalny = 0;

	Datastruct->pos_x_koncowa = 0;
	Datastruct->pos_y_koncowa = 0;
	Datastruct->kat_obrotu_koncowy = 0;

	Datastruct->PWM_LK = WARTOSC_PWM_LEWE_KOLO;
	Datastruct->PWM_PK = WARTOSC_PWM_PRAWE_KOLO;

	Datastruct->odleglosc_do_celu = 0;

	Datastruct->status_pracy = CZELAKJ;
	Datastruct->status_ruchu = STOP;

}

void licz_kat_obrotu(JAZDA* Datastruct) { // raczej do poprawy bo to sprawdza od pozycji 0.0
	float tmp;
	float pos_x;
	float pos_y;

	pos_y = Datastruct->pos_y_koncowa - Datastruct->pos_y_globalna;
	pos_x = Datastruct->pos_x_koncowa - Datastruct->pos_x_globalna;

	if (pos_y == 0 && pos_x > 0)
		tmp = 0;
	else if (pos_x == 0 && pos_y > 0)
		tmp = 90;
	else if (pos_y == 0 && pos_x < 0)
		tmp = 180;
	else if (pos_x == 0 && pos_y < 0)
		tmp = -90;
	else if (pos_x == 0 && pos_y == 0)
		tmp = 0;
	else {
		tmp = pos_y / pos_x;
		if (tmp < 0)
			tmp = -tmp;
		tmp = atan(tmp) * (180 / M_PI);
		if (pos_x < 0 && pos_y > 0)
			tmp = 180 - tmp;
		else if (pos_x < 0 && pos_y < 0)
			tmp = -180 + tmp;
		else if (pos_x > 0 && pos_y < 0)
			tmp = -tmp;
	}

	Datastruct->kat_obrotu_koncowy = round(tmp); // wartosc kata jaki ma byc na koniec ruchu

}

void licz_odleglosc_do_celu(JAZDA* D) {

	D->odleglosc_do_celu = round(
			sqrt(
					pow((D->pos_x_koncowa - D->pos_x_globalna), 2)
							+ pow((D->pos_y_koncowa - D->pos_y_globalna), 2)));

}

void obrot_w_prawo() {
	LK_PRZOD
	;
	PK_TYL
	;
}

void obrot_w_lewo() {
	LK_TYL
	;
	PK_PRZOD
	;
}

void jazda_do_przodu() {
	LK_PRZOD
	;
	PK_PRZOD
	;
}

void zakoncz_ruch() {
	LK_STOP
	;
	PK_STOP
	;
}

void jedz_do_celu(JAZDA* J) {

	licz_kat_obrotu(J); // obliczamy kat obrotu koncowego wzgledem aktualnego polozenia

	roznica_kata_obrotu = J->kat_obrotu_globalny - J->kat_obrotu_koncowy;
	if (roznica_kata_obrotu > 180)
		roznica_kata_obrotu = -360.0 + roznica_kata_obrotu;
	else if (roznica_kata_obrotu < -180)
		roznica_kata_obrotu = 360.0 + roznica_kata_obrotu;

	// roznice wylicza juz prawid³owo

	if (abs(J->pos_x_koncowa - J->pos_x_globalna) > TOLERANCJA_ODLEGLOSCI
			|| abs(J->pos_y_koncowa - J->pos_y_globalna) > TOLERANCJA_ODLEGLOSCI) {
		if (abs(roznica_kata_obrotu) > TOLERANCJA_KATA) {
			if (roznica_kata_obrotu > 0)
				obrot_w_prawo();
			else
				obrot_w_lewo();
			opoznienie_jazdy = 50;
		} else {
			opoznienie_jazdy--;
			if (opoznienie_jazdy <= 0) { // powoduje zabezpieczenie po obrocie przed naglym zmianem kierunku o brotu
				opoznienie_jazdy = 0;
				jazda_do_przodu();
				if (abs(roznica_kata_obrotu) >= MALA_TOLERANCJA_TOLERANCJA_KATA) {
					if (roznica_kata_obrotu > 0) {
						J->PWM_PK = WARTOSC_PWM_PRAWE_KOLO;
						if (roznica_kata_obrotu >= 2) {
							J->PWM_LK++;
							if (J->PWM_LK > 38)
								J->PWM_LK = 38;
//
						} else if (roznica_kata_obrotu >= 1.5) {
							J->PWM_LK = WARTOSC_PWM_LEWE_KOLO + 2;

						} else
							J->PWM_LK = WARTOSC_PWM_LEWE_KOLO + 1;

					} else if (roznica_kata_obrotu < 0) {
						J->PWM_LK = WARTOSC_PWM_LEWE_KOLO;
						if (roznica_kata_obrotu <= -2) {
							J->PWM_PK++;
							if (J->PWM_PK > 38)
								J->PWM_PK = 38;

						} else if (roznica_kata_obrotu <= -1.5) {
							J->PWM_PK = WARTOSC_PWM_PRAWE_KOLO + 2;

						} else
							J->PWM_PK = WARTOSC_PWM_PRAWE_KOLO + 1;

					} else {
						J->PWM_LK = WARTOSC_PWM_LEWE_KOLO;
						J->PWM_PK = WARTOSC_PWM_PRAWE_KOLO;
					}
				}
			} else { // konied od opoznienia jazdy - jest ono w celu zabezpieczenia przed nagla zmiana jazdy
				zakoncz_ruch();
			}

		} // koniec od jazda do przodu

	} else
		zakoncz_ruch();
	flag_jedziemy = 1;

}

int wykryto_przeszkode(int L, int SL, int SP, int P) {

	int licznik = 0;
	int rodzaj_ruchu = BRAK_PRZESZKODY;

	if (L < 25)
		licznik += 1;
	if (SL < 25)
		licznik += 2;
	if (SP < 25)
		licznik += 4;
	if (P < 25)
		licznik += 8;

	switch (licznik) {
	case 1:
		rodzaj_ruchu = PRAWO;
		break;

	case 2:
		rodzaj_ruchu = PRAWO;
		break;

	case 3:
		rodzaj_ruchu = PRAWO;
		break;

	case 4:
		rodzaj_ruchu = LEWO;
		break;

	case 5:
		rodzaj_ruchu = PRAWO;
		break;

	case 6:
		if (L < 30)
			rodzaj_ruchu = PRAWO;   // poprawki zwiazane z przeszkoda a srodku
		else
			rodzaj_ruchu = LEWO;
		break;

	case 7:
		rodzaj_ruchu = PRAWO;
		break;

	case 8:
		rodzaj_ruchu = LEWO;
		break;

	case 9:
		rodzaj_ruchu = LEWO; // L_90;
		break;

	case 10:
		rodzaj_ruchu = LEWO;
		break;

	case 11:
		rodzaj_ruchu = LEWO; //L_90;
		break;

	case 12:
		rodzaj_ruchu = LEWO;
		break;

	case 13:
		rodzaj_ruchu = LEWO; // L_90;
		break;

	case 14:
		rodzaj_ruchu = LEWO;
		break;

	case 15:
		rodzaj_ruchu = LEWO; //L_90;
		break;

	}  // koniec od switch

	return rodzaj_ruchu;

}

