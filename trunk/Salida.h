#ifndef SALIDA_H_
#define SALIDA_H_

#include "Historial.h"

class Salida {


public:

	Salida() {}
	bool registrarSalidaAuto(Auto * automovil);
	virtual ~Salida() {}
};

#endif /* SALIDA_H_ */
