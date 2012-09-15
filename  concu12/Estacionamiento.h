/*
 * Estacionamiento.h
 *
 *  Created on: 11/09/2012
 *      Author: catu
 */

#ifndef ESTACIONAMIENTO_H_
#define ESTACIONAMIENTO_H_

#include "Administracion.h";

class Estacionamiento {
private:
	//TODO: ver declaracion de listas
	List <Barrera> entradas;
	List <Barrera> salidas;
	Administracion administracionDelEstacionamiento;
	static Estacionamiento estacionamiento;

public:
	Estacionamiento();
	virtual ~Estacionamiento();

	static Estacionamiento obtenerEstacionamiento();

	Administracion obtenerAdministracion();

};

#endif /* ESTACIONAMIENTO_H_ */
