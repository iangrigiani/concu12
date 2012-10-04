#include "Estacionamiento.h"



Estacionamiento::Estacionamiento() {

	this->entradas = new list<Entrada*>();
	this->salidas  = new list<Salida*>();

	Entrada * entrada1 = new Entrada(1);
	Entrada * entrada2 = new Entrada(2);
	Entrada * entrada3 = new Entrada(3);

	this->entradas->push_back(entrada1);
	this->entradas->push_back(entrada2);
	this->entradas->push_back(entrada3);

	Salida * salida1 = new Salida(1);
	Salida * salida2 = new Salida(2);

	this->salidas->push_back(salida1);
	this->salidas->push_back(salida2);

	//Llamar a la administracion
	//this->administracionDelEstacionamiento = Administracion::obtenerAdministracion();

}

Estacionamiento::~Estacionamiento()
{
	//TODO IMPORTANTE!!! tener en cuenta de liberar las listas de entradas
	//y salidas Y LA ADMINISTRACION
	//delete this->administracionDelEstacionamiento;

	list<Entrada*>::iterator it = this->entradas->begin();

	while ( it != this->entradas->end())
	{
		delete *it;
		++it;
	}

	list<Salida*>::iterator it2 = this->salidas->begin();

	while ( it2 != this->salidas->end())
	{
		delete *it2;
		++it2;
	}

	delete this->entradas;
	delete this->salidas;

}

Estacionamiento * Estacionamiento::estacionamiento = NULL;
Estacionamiento* Estacionamiento::obtenerEstacionamiento(){
	if(estacionamiento==NULL)
		estacionamiento=new Estacionamiento();
	return estacionamiento;
}

Administracion * Estacionamiento::getAdministracion(){
	return this->administracionDelEstacionamiento;
}


void Estacionamiento::setCantidadDeLugaresDisponibles(int cantidadLugares)
{
	//this->administracionDelEstacionamiento->getHistorial()->setCantidadLugaresDispobibles(cantidadLugares);
}


Entrada * Estacionamiento::getEntradaAleatoria()
{
	int cantidadEntradas = this->entradas->size();
	int numeroElegido;
	int indiceActual = 0;
	Entrada * entradaElegida;

	srand((unsigned)time(0));
	numeroElegido = (rand()%cantidadEntradas);

	list<Entrada*>::iterator it = this->entradas->begin();

	 while ( it != this->entradas->end())
	 {
		 if(indiceActual == numeroElegido)
		 {
			 entradaElegida = *it;
			 break;
		 }

		 ++it;
		 indiceActual++;
	 }

	 return entradaElegida;
}



Salida * Estacionamiento::getSalidaAleatoria()
{
	int cantidadSalidas = this->salidas->size();
	int numeroElegido;
	int indiceActual = 0;
	Salida * salidaElegida;

	srand((unsigned)time(0));
	numeroElegido = (rand()%cantidadSalidas);

	list<Salida*>::iterator it = this->salidas->begin();

	 while ( it != this->salidas->end())
	 {
		 if(indiceActual == numeroElegido)
		 {
			 salidaElegida = *it;
			 break;
		 }

		 ++it;
		 indiceActual++;
	 }

	 return salidaElegida;

}
