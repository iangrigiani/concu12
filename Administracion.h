#ifndef ADMINISTRACION_H_
#define ADMINISTRACION_H_

#include "Historial.h"

class Administracion
{


	private:

			Historial * historial;
			Administracion();
			static Administracion * administracion;
			float costoHora;
			float importeRecaudado;



	public:
			~Administracion();
			Historial * getHistorial();
			static Administracion* obtenerAdministracion();
			float getCostoHora();
			void setCostoHora(float costo);

			float getImporteRecaudado();
			void actualizarImporteRecaudado(int cantidadHoras);



};

#endif /* ADMINISTRACION_H_ */
