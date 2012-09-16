
#ifndef ENTRADA_H_
#define ENTRADA_H_

#include "Historial.h"


class Entrada {

public:
	Entrada(){}
	bool registrarEntradaAuto(Auto * automovil);
	virtual ~Entrada() { }
};

#endif /* ENTRADA_H_ */
