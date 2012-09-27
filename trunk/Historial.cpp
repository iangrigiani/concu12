
#include "Historial.h"

Historial::Historial() {

	//this->historialDeAutos = new list<Auto*>();

}

Historial::~Historial() {

	//TODO tener en cuenta de liberar la lista de autos

	//delete [] this->historialDeAutos;
}


Historial * Historial::historial = NULL;
Historial* Historial::obtenerHistorial()
{
	if(historial==NULL)
			historial=new Historial();
		return historial;


}


void Historial::agregarNuevoAutoAlHistorial(Auto * automovil)
{
	this->historialDeAutos->push_back(automovil);

}

void Historial::quitarAutoDelHistorial(Auto * automovil)
{
	this->historialDeAutos->remove(automovil);

}

list<Auto*> * Historial::getHistorialDeAutos()
{
	return this->historialDeAutos;
}


int Historial::getCantidadDeAutosEnEstacionamiento()
{
	return this->historialDeAutos->size();
}


int Historial::getCantidadDeLugaresDisponibles(){

	return this->cantidadDeLugaresDisponibles;
}

void Historial::setCantidadLugaresDispobibles(int lugaresDisponibles)
{
	this->cantidadDeLugaresDisponibles = lugaresDisponibles;
}






