#ifndef HISTORIAL_H_
#define HISTORIAL_H_

#include <list>
#include "Auto.h"

class Historial {

	private:

		list<Auto*> * historialDeAutos;
		int cantidadDeLugaresDisponibles;
		static Historial* historial;
		Historial();


	public:

		static Historial* obtenerHistorial();

		virtual ~Historial();

		void agregarNuevoAutoAlHistorial(Auto * automovil);

		void quitarAutoDelHistorial(Auto * automovil);

		list<Auto*> * getHistorialDeAutos();

		int getCantidadDeAutosEnEstacionamiento();

		int getCantidadDeLugaresDisponibles();

		void setCantidadLugaresDispobibles(int lugaresDisponibles);


};

#endif /* HISTORIAL_H_ */
