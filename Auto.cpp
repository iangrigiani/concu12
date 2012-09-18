#include "Auto.h"


Auto::Auto()
{
	this->haEntrado = false;

}


Auto::~Auto()
{
	delete this->ticket;

}


void Auto::setTicket(Ticket * ticket)
{
	this->ticket = ticket;
}

Ticket * Auto::getTicket()
{
	return this->ticket;
}


void Auto::setHaEntrado(bool haEntrado)
{
	this->haEntrado = haEntrado;
}


bool Auto::getHaEntrado()
{
	return this->haEntrado;
}

bool Auto::entrarAlEstacionamiento()
{
	//if(Estacionamiento::obtenerEstacionamiento()->getEntradaAleatoria()->registrarEntradaAuto(this))
		//this->setHaEntrado(true);

	return this->getHaEntrado();
}


bool Auto::salirDelEstacionamiento()
{
	//if(Estacionamiento::obtenerEstacionamiento()->getSalidaAleatoria()->registrarSalidaAuto(this))
		//this->setHaEntrado(false);

	return this->getHaEntrado();

}
