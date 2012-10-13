#ifndef SIMULADOR_H_
#define SIMULADOR_H_




#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include "Cronometro.h"
#include "MemoriaCompartida.h"
#include "Administracion.h"
#include "Auto.h"
#include "Pipe.h"
#include "Constantes.h"
#include "Utils.h"
#include "Log.h"



class Simulador {

protected:

	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	MemoriaCompartida<Administracion> administracion;

//	Pipe pipePpal;
	int numero;

	void inicializarMemoriaCompartidaVectorPosiciones(int cantidadPosiciones);
	void inicializarMemoriaCompartidaAdministracion();

	void decrementarCantidadAutosEstacionamiento();

	int getCantidadAutosEstacionamiento();


public:

	Simulador(int numero,int cantidadPosiciones);
	virtual ~Simulador();

//	void setPipePrincipal(Pipe pipePrincipal);
//	Pipe getPipePrincipal();
};

#endif /* SIMULADOR_H_ */
