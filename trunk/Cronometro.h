
#ifndef CRONOMETRO_H_
#define CRONOMETRO_H_

#include <stdio.h>

class Cronometro {

private:

	int tiempoASimular;
	//static Date horaInicial;
	static Cronometro * cronometro;
	Cronometro();

public:

	virtual ~Cronometro();

	static Cronometro * obtenerCronometro();

	void iniciarTiempo();

	double getFlujoDeAutos();

	bool llegoAlFinal();

	void pausaAleatoria(long milisegundos);

	int getTiempoASimular();

	void setTiempoASimular(int tiempo);

};

#endif /* CRONOMETRO_H_ */
