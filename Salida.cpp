#include "Salida.h"


bool Salida::registrarSalidaAuto(Auto * automovil, Administracion * administracion)
{
	bool pudoSalir = false;
	//ACA se deberia verificar si el auto pagó el ticket
	if(automovil->getTicket()->getPago() && automovil->getHaEntrado())
	{
		administracion->incrementarCantidadLugares();
		pudoSalir = true;
	}
	return pudoSalir;
}


int Salida::getNumeroDeSalida()
{
	return this->numeroDeSalida;
}
