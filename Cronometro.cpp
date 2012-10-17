
#include "Cronometro.h"


Cronometro::Cronometro() {
	this->tiempoASimular = 0;
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

	 Fecha fechaInicial;
	 this->horaInicial = fechaInicial;
  }

/**
 * Metodo que calcula el flujo de autos que entran al estacionamiento de acuerdo a la hora.
 * @return double
 */
double Cronometro::getFlujoDeAutos(){

	Fecha fechaActual;

	double x = ((fechaActual.getTime())-(this->horaInicial.getTime()))/ 1000.0;

	return (90000.0-(x-300)*(x-300))/90000.0;
}

/**
 * Metodo que determina si la simulacion llego a su final.
 * @return boolean
 */
bool Cronometro::llegoAlFinal() {

	bool retorno = false;

	Fecha fechaActual;
	double x = ((fechaActual.getTime())-(this->horaInicial.getTime()))/ 1000.0;

	if(x>this->tiempoASimular)
		retorno = true;

	return retorno;
}

void Cronometro::destruir ()
{
	if ( cronometro != NULL ) {
		delete ( cronometro );
		cronometro = NULL;
	}
}
