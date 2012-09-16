#include "Salida.h"


bool Salida::registrarSalidaAuto(Auto * automovil)
{
	bool pudoSalir = false;
	//ACA se deberia verificar si el auto pagó el ticket
	if(automovil->getTicket()->getPago())
	{
		int cantidadLugares = Historial::obtenerHistorial()->getCantidadDeLugaresDisponibles();
		Historial::obtenerHistorial()->quitarAutoDelHistorial(automovil);
		cantidadLugares++;
		Historial::obtenerHistorial()->setCantidadLugaresDispobibles(cantidadLugares);
		pudoSalir = true;
	}
	return pudoSalir;
}
