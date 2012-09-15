#include "Estacionamiento.h"



Estacionamiento::Estacionamiento() {
	// TODO Auto-generated constructor stub

}

Estacionamiento::~Estacionamiento() {
	// TODO Auto-generated destructor stub
}

Estacionamiento * Estacionamiento::estacionamiento = NULL;
Estacionamiento* Estacionamiento::obtenerEstacionamiento(){
	if(estacionamiento==NULL)
		estacionamiento=new Estacionamiento();
	return estacionamiento;
}

Administracion* Estacionamiento::obtenerAdministracion(){
	return this->administracionDelEstacionamiento;
}

