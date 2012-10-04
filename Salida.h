#ifndef SALIDA_H_
#define SALIDA_H_

#include "Administracion.h"
#include "Auto.h"

class Salida {

private:
		int numeroDeSalida;


public:

	Salida(int numeroSalida)
	{
		this->numeroDeSalida = numeroSalida;

	}

	int getNumeroDeSalida();

	bool registrarSalidaAuto(Auto * automovil,Administracion * administracion);

	virtual ~Salida() {}
};

#endif /* SALIDA_H_ */
