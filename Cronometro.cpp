
#include "Cronometro.h"


Cronometro::Cronometro() {

}

Cronometro::~Cronometro() {

}

int Cronometro::getTiempoASimular(){
	return this->tiempoASimular;
}

void Cronometro::setTiempoASimular(int tiempo){
	this->tiempoASimular = tiempo;
}


Cronometro * Cronometro::cronometro = NULL;
Cronometro * Cronometro::obtenerCronometro(){

	if(cronometro == NULL)
		cronometro= new Cronometro();

	return cronometro;
}

/**
 * Metodo que inicia el tiempo del cronometro cuando comienza la simulacion.
 */
 void Cronometro::iniciarTiempo(){

	 this->horaInicial = new Fecha();
  }

/**
 * Metodo que calcula el flujo de autos que entran al estacionamiento de acuerdo a la hora.
 * @return double
 */
double Cronometro::getFlujoDeAutos(){
//	double x= ((new Date()).getTime()- (Cronometro.horaInicial).getTime())/1000.0;
//  return (90000.0-(x-300)*(x-300))/90000.0;
	return 0;
}

/**
 * Metodo que determina si la simulacion llego a su final.
 * @return boolean
 */
bool Cronometro::llegoAlFinal() {

//	double x= ((new Date()).getTime()- (Cronometro.horaInicial).getTime())/1000.0;
//	if(x>Cronometro.tiempoASimular) return true;
	return false;
}

/**
 * Metodo que genera una pausa aleatoria en el proceso actual.
 * @param milisegundos
 */
void Cronometro::pausaAleatoria(long milisegundos) {
	//Thread.sleep((long)(milisegundos*Math.random()));
}



