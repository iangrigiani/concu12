#include "Administracion.h"




Administracion::Administracion()
{
	this->historial = Historial::obtenerHistorial();
	this->importeRecaudado = 0;
}


Administracion* Administracion::administracion = NULL;
Administracion* Administracion::obtenerAdministracion(){
	if(administracion==NULL)
		administracion=new Administracion();
	return administracion;
}


float Administracion::getCostoHora()
{
	return this->costoHora;

}

void Administracion::setCostoHora(float costo)
{
	this->costoHora = costo;

}


Administracion::~Administracion()
{
	delete this->historial;

}

Historial * Administracion::getHistorial()
{
	return this->historial;
}

float Administracion::getImporteRecaudado(){
	return this->importeRecaudado;
}

void Administracion::actualizarImporteRecaudado(int cantidadHoras){
	this->importeRecaudado += (this->costoHora * cantidadHoras);
}



