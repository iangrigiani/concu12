/*
 * Cronometro.cpp
 *
 *  Created on: 11/09/2012
 *      Author: catu
 */

#include "Cronometro.h"

//TODO: declarar variables stactic

Cronometro::Cronometro() {
	// TODO Auto-generated constructor stub

}

Cronometro::~Cronometro() {
	// TODO Auto-generated destructor stub
}

int Cronometro::getTiempoASimular(){
	return this->tiempoASimular;
}

void Cronometro::setTiempoASimular(int tiempo){
	this->tiempoASimular = tiempo;
}


//Como el cronometro es uno solo//
//PATRON SINGLETON//
static Cronometro Cronometro::obtenerCronometro(){
	if(cronometro == NULL){
		cronometro= new Cronometro();
	}
	return cronometro;
}

/**
 * Metodo que inicia el tiempo del cronometro cuando comienza la simulacion.
 */
 public void Cronometro::iniciarTiempo(){

//		  horaInicial = new Date();
  }

/**
 * Metodo que calcula el flujo de autos que entran al estacionamiento de acuerdo a la hora.
 * @return double
 */
static double Cronometro::getFlujoDeAutos(){
//	double x= ((new Date()).getTime()- (Cronometro.horaInicial).getTime())/1000.0;
//  return (90000.0-(x-300)*(x-300))/90000.0;
	return 0; //TODO: sacar!
}

/**
 * Metodo que determina si la simulacion llego a su final.
 * @return boolean
 */
static bool Cronometro::llegoAlFinal() {

//	double x= ((new Date()).getTime()- (Cronometro.horaInicial).getTime())/1000.0;
//	if(x>Cronometro.tiempoASimular) return true;
	return false;
}

/**
 * Metodo que genera una pausa aleatoria en el proceso actual.
 * @param milisegundos
 * @throws InterruptedException
 */
static void Cronometro::pausaAleatoria(long milisegundos) {
	//Thread.sleep((long)(milisegundos*Math.random()));
}



