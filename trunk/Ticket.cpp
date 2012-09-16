#include "Ticket.h"


Ticket::Ticket()
{
	this->fechaEntrada = new Fecha();
	this->pago = false;

}


Ticket::~Ticket()
{
	delete this->fechaEntrada;

}


void Ticket::setPago(bool pago)
{
	this->pago=pago;

}


bool Ticket::getPago()
{
	return this->pago;
}


Fecha * Ticket::getFechaEntrada()
{
	return this->fechaEntrada;
}



