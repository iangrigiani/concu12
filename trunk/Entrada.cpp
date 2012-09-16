#include "Entrada.h"


/*Este método registra la entra de un auto en el estacionamiento asignandole al mismo un Ticket,
esto debe generar un proceso por cada auto que entra*/
/*Devuelve true si el auto pudo acceder al estacionamiento*/

bool Entrada::registrarEntradaAuto(Auto * automovil)
{
	bool pudoAcceder = false;
	//ACA se deberia verificar si el estacionamiento esta o no lleno.
	//Si el estacionamiento no esta lleno se le permite el ingreso del auto.
	int cantidadLugares = Historial::obtenerHistorial()->getCantidadDeLugaresDisponibles();

	if(cantidadLugares > 0)
	{
		Ticket * ticket = new Ticket();
		automovil->setTicket(ticket);
		Historial::obtenerHistorial()->agregarNuevoAutoAlHistorial(automovil);
		cantidadLugares--;
		Historial::obtenerHistorial()->setCantidadLugaresDispobibles(cantidadLugares);
		pudoAcceder = true;
	}

	return pudoAcceder;
}

