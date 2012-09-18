
#ifndef ENTRADA_H_
#define ENTRADA_H_

#include "Historial.h"

class Entrada {

private:
		int numeroDeEntrada;

public:

	Entrada(int numeroDeEntrada)
	{
		this->numeroDeEntrada = numeroDeEntrada;

	}

	int getNumeroDeEntrada();

	bool registrarEntradaAuto(Auto * automovil);

	virtual ~Entrada() { }
};

#endif /* ENTRADA_H_ */
