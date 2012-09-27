
#include "Cronometro.h"


Cronometro::Cronometro() {
	this->horaInicial = NULL;
	this->tiempoASimular = 0;
}

Cronometro::~Cronometro() {
	delete this->horaInicial;
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

	Fecha * fechaActual = new Fecha();

	double x = ((fechaActual->getTime())-(this->horaInicial->getTime()))/ 1000.0;

	delete fechaActual;

	return (90000.0-(x-300)*(x-300))/90000.0;
}

/**
 * Metodo que determina si la simulacion llego a su final.
 * @return boolean
 */
bool Cronometro::llegoAlFinal() {

	bool retorno = false;
	Fecha * fechaActual = new Fecha();
	double x = ((fechaActual->getTime())-(this->horaInicial->getTime()))/ 1000.0;

	delete fechaActual;
	if(x>this->tiempoASimular)
		retorno = true;


	return retorno;
}

/**
 * Metodo que genera una pausa aleatoria en el proceso actual.
 * @param milisegundos
 */
void Cronometro::pausaAleatoria(long milisegundos) {
	//Thread.sleep((long)(milisegundos*Math.random()));
}



