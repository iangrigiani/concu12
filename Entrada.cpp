#include "Entrada.h"


/*Este método registra la entra de un auto en el estacionamiento asignandole al mismo un Ticket,
esto debe generar un proceso por cada auto que entra*/
/*Devuelve true si el auto pudo acceder al estacionamiento*/

bool Entrada::registrarEntradaAuto(Auto * automovil, Administracion * administracion)
{
	bool pudoAcceder = false;
	//ACA se deberia verificar si el estacionamiento esta o no lleno.
	//Si el estacionamiento no esta lleno se le permite el ingreso del auto.
	/*int cantidadLugares = administracion->getCantidadLugaresDisponibles();
	if(cantidadLugares > 0 && !automovil->getHaEntrado())
	{
		Ticket * ticket = new Ticket();
		automovil->setTicket(ticket);
		administracion->incrementarCantidadAutos();
		cantidadLugares--;
		administracion->decrementarCantidadLugares();
		pudoAcceder = true;
	}

	return pudoAcceder;*/
	return false;
}


int Entrada::getNumeroDeEntrada()
{
	return this->numeroDeEntrada;
}

