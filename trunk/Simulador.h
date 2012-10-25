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
#include "Semaforo.h"



class Simulador {


private:

	// Crea las memorias compartidas para las posiciones del estacionamiento
	void inicializarMemoriaCompartidaVectorPosiciones(int cantidadPosiciones);

	// Crea la memoria compartida para la administracion
	void inicializarMemoriaCompartidaAdministracion();

protected:

	// Vector donde guardo las memorias compartidas
	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	MemoriaCompartida<Administracion> administracion;

	// Semaforo para la administracion
	Semaforo smfAdministracion;

	//Vector de semaforos para bloquear cada una de las posiciones
    vector<Semaforo> semaforos;

    int numero;

    // Devuelve la cantidad de autos almacenados en la memoria compartida de la administracion
	int getCantidadAutosEstacionamiento();


public:

	Simulador(int numero,int cantidadPosiciones);
	virtual ~Simulador();
};

#endif /* SIMULADOR_H_ */
