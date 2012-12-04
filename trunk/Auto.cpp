#include "Auto.h"


Auto::Auto(int horasEstadia, int numeroEstacionamiento)
{
	this->haEntrado = false;
	this->horasEstadia = horasEstadia;
	this->numeroPosicion = 0;
	this->numeroEstacionamiento = numeroEstacionamiento;
}


Auto::~Auto()
{

}

int Auto::getNumeroPosicion()
{
	return this->numeroPosicion;
}

void Auto::setNumeroPosicion(int numero)
{
	this->numeroPosicion = numero;
}

void Auto::setHaEntrado(bool haEntrado)
{
	this->haEntrado = haEntrado;
}


bool Auto::getHaEntrado()
{
	return this->haEntrado;
}

int Auto::getHorasEstadia(){
	return this->horasEstadia;
}


void Auto::run(){
	stringstream mensajeLog;

	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el auto " << getpid() << " y voy a estar estacionado " << this->horasEstadia << " horas en la posicion " << this->numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	cout << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el auto " << getpid() << " y voy a estar estacionado " << this->horasEstadia << " horas en la posicion " << this->numeroPosicion  << endl;
	sleep(this->horasEstadia);
}
