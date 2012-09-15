/*
 * Salida.cpp
 *
 *  Created on: 11/09/2012
 *      Author: catu
 */

#include "Salida.h"

Salida::Salida() {
	// TODO Auto-generated constructor stub

}

Salida::~Salida() {
	// TODO Auto-generated destructor stub
}



//TODO: implementacion de metodo abstracto en barrera
/**
	 * Implementacion del metodo definido en la clase Lector
	 * Registra la salida de una persona del edificio
	 * Devuelve true si la persona ha entrado al edificio y no ha entrado a un piso
	 * @param persona
	 * @return boolean
	 */
	 /*public synchronized boolean registrarSalidaDeUnaPersona(Persona persona){

		Date salida = new Date();
		boolean haSalido=false;

		if(persona.haEntradoAlEdificio() && !(persona.haEntradoAUnPiso())){
			Iterator<Registro> itDeRegistro=this.getHistorial().getHistorialDePersonas().get(persona).iterator();
			Registro registro;
			while(itDeRegistro.hasNext()){
				registro=itDeRegistro.next();
				if(registro.haAccedido()){
					registro.setHaAccedido(false);
				}
			}
			haSalido=true;
		}else{
			RegistroIndebido salir = new RegistroIndebido(persona,salida);
			this.getHistorial().agregarNuevaSalidaIndebida(salir);
		}
		return(haSalido);
	}*/
