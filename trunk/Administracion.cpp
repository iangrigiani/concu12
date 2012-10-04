#include "Administracion.h"




Administracion::Administracion()
{
	this->importeRecaudado = 0;
}


float Administracion::getCostoHora()
{
	return this->costoHora;

}

void Administracion::setCostoHora(float costo)
{
	this->costoHora = costo;

}


float Administracion::getImporteRecaudado(){
	return this->importeRecaudado;
}

void Administracion::actualizarImporteRecaudado(int cantidadHoras){
	this->importeRecaudado += (this->costoHora * cantidadHoras);
}

void Administracion::incrementarCantidadAutos()
{
	this->cantidadDeAutos++;
}

void Administracion::decrementarCantidadAutos()
{
	this->cantidadDeAutos++;
}


void Administracion::incrementarCantidadLugares()
{
	this->cantidadDeLugaresDisponibles++;
}


void Administracion::decrementarCantidadLugares()
{
	this->cantidadDeLugaresDisponibles--;
}



int Administracion::getCantidadDeAutos()
{
	return this->cantidadDeAutos;
}


int Administracion::getCantidadLugaresDisponibles()
{
	return this->cantidadDeLugaresDisponibles;
}





