
#ifndef ADMINISTRADORGENERAL_H_
#define ADMINISTRADORGENERAL_H_

#include "Estacionamiento.h"
#include "Lugares.h"

class AdministradorGeneral {

private:

		pid_t pConsola;
		vector<Estacionamiento> estacionamientos;
		vector<Lugares> lugares;

		void inicializaciones(int cantidadEstacionamientos,int cantidadDeLugares);

		void crearArchivosTemporales(int cantidadLugares,int numeroEstacionamiento);

		void liberarRecursos(int cantidadEstacionamientos,int cantidadLugares);

		void liberarMemoriaCompartidaEstacionamiento(int numeroEstacionamiento,int cantidadLugares);

		void eliminarArchivosTemporales(int numeroEstacionamiento, int cantidadLugares);

public:

	AdministradorGeneral();

	void run(int cantidadDeLugares, float costoHora, int tiempoEjecucion,int cantidadEstacionamientos);

	virtual ~AdministradorGeneral();
};

#endif /* ADMINISTRADORGENERAL_H_ */
