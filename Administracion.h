#ifndef ADMINISTRACION_H_
#define ADMINISTRACION_H_

#include "Historial.h"

class Administracion
{


	private:

			Historial * historial;
			Administracion();
			static Administracion * administracion;



	public:
			~Administracion();
			Historial * getHistorial();
			static Administracion* obtenerAdministracion();



};

#endif /* ADMINISTRACION_H_ */
