#include "Administracion.h"




Administracion::Administracion()
{
	this->historial = Historial::obtenerHistorial();

}


Administracion* Administracion::administracion = NULL;
Administracion* Administracion::obtenerAdministracion(){
	if(administracion==NULL)
		administracion=new Administracion();
	return administracion;
}


Administracion::~Administracion()
{
	delete this->historial;

}

Historial * Administracion::getHistorial()
{
	return this->historial;
}




