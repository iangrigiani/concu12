#ifndef SALIDA_H_
#define SALIDA_H_

#include "Historial.h"

class Salida {

private:
		int numeroDeSalida;


public:

	Salida(int numeroSalida)
	{
		this->numeroDeSalida = numeroSalida;

	}

	int getNumeroDeSalida();

	bool registrarSalidaAuto(Auto * automovil);

	virtual ~Salida() {}
};

#endif /* SALIDA_H_ */
