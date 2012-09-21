#include "Auto.h"


Auto::Auto(int horasEstadia)
{
	this->haEntrado = false;
	this->horasEstadia = horasEstadia;

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
