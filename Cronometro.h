
#ifndef CRONOMETRO_H_
#define CRONOMETRO_H_

#include <stdio.h>
#include "Fecha.h"

class Cronometro {

private:

	int tiempoASimular;
	Fecha * horaInicial;
	static Cronometro * cronometro;
	Cronometro();

public:

	virtual ~Cronometro();

	static Cronometro * obtenerCronometro();

	void iniciarTiempo();

	double getFlujoDeAutos();

	bool llegoAlFinal();

	int getTiempoASimular();

	void setTiempoASimular(int tiempo);

	static void destruir ();

};

#endif /* CRONOMETRO_H_ */
