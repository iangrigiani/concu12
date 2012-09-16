#include "Estacionamiento.h"



Estacionamiento::Estacionamiento() {

	this->entradas = new list<Entrada*>();
	this->salidas  = new list<Salida*>();

	//TODO agregar las entradas y salidas correspondientes

	Entrada * entrada1 = new Entrada();
	this->entradas->push_back(entrada1);

	//Llamar a la administracion
	this->administracionDelEstacionamiento = Administracion::obtenerAdministracion();

}

Estacionamiento::~Estacionamiento()
{
	//TODO tener en cuenta de liberar las listas de entradas
	//y salidas

}

Estacionamiento * Estacionamiento::estacionamiento = NULL;
Estacionamiento* Estacionamiento::obtenerEstacionamiento(){
	if(estacionamiento==NULL)
		estacionamiento=new Estacionamiento();
	return estacionamiento;
}

Administracion* Estacionamiento::getAdministracion(){
	return this->administracionDelEstacionamiento;
}


void Estacionamiento::setCantidadDeLugaresDisponibles(int cantidadLugares)
{
	this->administracionDelEstacionamiento->getHistorial()->setCantidadLugaresDispobibles(cantidadLugares);
}
