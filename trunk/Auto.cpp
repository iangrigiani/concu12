#include "Auto.h"


Auto::Auto(int horasEstadia)
{
	this->haEntrado = false;
	this->horasEstadia = horasEstadia;
	this->numeroPosicion = 0;
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

	cout << "Soy " << getpid() << " y voy a estar estacionado " << this->horasEstadia << " horas" << endl;
	sleep(this->horasEstadia);
}
