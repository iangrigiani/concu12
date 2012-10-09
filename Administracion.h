#ifndef ADMINISTRACION_H_
#define ADMINISTRACION_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "Posicion.h"


using namespace std;


class Administracion
{


	private:

			float costoHora;
			float importeRecaudado;
			int cantidadDeAutos;




	public:
			Administracion();

			~Administracion(){};


			float getCostoHora();

			void setCostoHora(float costo);

			void incrementarCantidadAutos();

			int getCantidadDeAutos();

			float getImporteRecaudado();

			void actualizarImporteRecaudado(int cantidadHoras);

			void decrementarCantidadAutos();



};

#endif /* ADMINISTRACION_H_ */
