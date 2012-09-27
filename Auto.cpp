#include "Auto.h"


Auto::Auto(int horasEstadia)
{
	this->haEntrado = false;
	this->horasEstadia = horasEstadia;
	this->ticket = NULL;

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

int Auto::getHorasEstadia(){
	return this->horasEstadia;
}


void Auto::run(){

	cout << "Soy " << getpid() << " y voy a estar estacionado " << this->horasEstadia << " horas" << endl;
	sleep(this->horasEstadia);
}
