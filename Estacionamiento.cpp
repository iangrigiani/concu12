/*
 * Estacionamiento.cpp
 *
 *  Created on: 11/09/2012
 *      Author: catu
 */

#include "Estacionamiento.h"

Estacionamiento::Estacionamiento() {
	// TODO Auto-generated constructor stub

}

Estacionamiento::~Estacionamiento() {
	// TODO Auto-generated destructor stub
}

//Como el estacionamiento es uno solo//
//PATRON SINGLETON//
static Estacionamiento Estacionamiento::obtenerEstacionamiento(){
	if(estacionamiento==null)
		estacionamiento=new Estacionamiento();
	return estacionamiento;
}

private Estacionamiento::Estacionamiento(){
	//this.administracionDelEstacionamiento=Administracion.obtenerAdministracion();
	//TODO: new de entradas y salidas
}


Administracion Estacionamiento::obtenerAdministracion(){
	return this->administracionDelEstacionamiento;
}
