/*
 * Cronometro.h
 *
 *  Created on: 11/09/2012
 *      Author: catu
 */

#ifndef CRONOMETRO_H_
#define CRONOMETRO_H_

class Cronometro {

private:

	int tiempoASimular;
	//static Date horaInicial;
	static Cronometro cronometro;

	Cronometro();

public:

	virtual ~Cronometro();

	static Cronometro obtenerCronometro();

	void iniciarTiempo();

	static double getFlujoDeAutos();

	static bool llegoAlFinal();

	static void pausaAleatoria(long milisegundos);

	int getTiempoASimular();

	void setTiempoASimular(int tiempo);

};

#endif /* CRONOMETRO_H_ */
