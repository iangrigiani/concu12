#include "Auto.h"


Auto::Auto(){

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

