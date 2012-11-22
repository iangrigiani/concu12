#ifndef LUGARES_H_
#define LUGARES_H_

#include <vector>
#include "MemoriaCompartida.h"
#include "Posicion.h"
#include "Utils.h"
#include "Log.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>


class Lugares {

private:

	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	vector<int> vectorMemoriaPosicionesLibres;

public:

	Lugares();

	// Crea las posiciones de memoria compartida segun la cantidad de lugares pasada por parametro
	// En caso de haber algun error libera la memoria creada y devuelve false
	bool crearMemoriaCompartidaPosiciones(int cantidadLugares,int numeroEstacionamiento);

	// Crea en el stack el vector donde almaceno las memorias creadas
	void crearVectorPosicionesLibres(int cantidadLugares);

	// Elimina del vector de posiciones libres la posicion pasada por parametro
	void quitarPosicionLibre(int numeroPosicion);

	// Agrega de manera ordenada al vector de posiciones libres la posicion pasada por parametro
	void agregarPosicionLibre(int numeroPosicion);

	// Realiza una busqueda binaria recursiva de la posicion buscada. Devuelve el indice de la posicion o -1 si no la encuentra
	int busquedaBinariaVectorLibres(int inicio,int fin,int buscado);

	// Devuelve una posicion aleatoria del vector de posiciones libres. En caso de no haber ninguna devuelve -1
	int getPosicionAleatoria();


	virtual ~Lugares();
};

#endif /* LUGARES_H_ */
