
#include "Simulador.h"

Simulador::Simulador(int numero, int cantidadPosiciones)
{
	//Asocian a la misma memoria compartida que crea el principal
	inicializarMemoriaCompartidaVectorPosiciones(cantidadPosiciones);
	inicializarMemoriaCompartidaAdministracion();

	this->numero = numero;


	Semaforo smfAdmin(ARCHIVO_SEMAFORO_ADMINISTRACION,1,'a');
	this->smfAdministracion = smfAdmin;

}

Simulador::~Simulador()
{
	 // Elimino los semaforos
	  vector<Semaforo>::iterator itSemaforos = this->semaforos.begin();
		while(itSemaforos != this->semaforos.end()){
				itSemaforos->eliminar();
				itSemaforos++;
		}

		//Habria que eliminar administracion o como ya lo esta haciendo estacionamiento
		// no hace falta??? Porque es el mismo semaforo

}

void Simulador::inicializarMemoriaCompartidaVectorPosiciones(int cantidadPosiciones)
{
	int i;
	Posicion posicion;
	MemoriaCompartida<Posicion> memoria;

	for (i=0;i<cantidadPosiciones;i++)
	{
		stringstream nombreArchivo;
		stringstream mensajeLog;

		// Creo archivo temporal
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;

		// Creo la memoria asociada al archivo temporal
		int estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		if ( estadoMemoria == SHM_OK )
		{
			this->vectorMemoriaPosiciones.push_back(memoria);

			//Creo un semaforo por cada posicion inicializada
			Semaforo semaforo((char*)nombreArchivo.str().c_str(), 1, (char) i);
			this->semaforos.push_back(semaforo);
		}
		else
		{
			mensajeLog<<"Error al inicializar el vector de memoria compartida en la posicion "<<i<<" en el simulador";
			Log::getInstance()->loguear(mensajeLog.str());
			mensajeLog.flush();

		}

		nombreArchivo.flush();

	}

}

void Simulador::inicializarMemoriaCompartidaAdministracion()
{
	stringstream nombreArchivo;
	stringstream mensajeLog;

	// Creo archivo temporal
	nombreArchivo << ARCHIVO_ADMINISTRACION;

	// Hago el atach a  la memoria asociada al archivo temporal
	int estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	if(estadoMemoria != SHM_OK)
	{
		mensajeLog<<"Error al inicializar la administracion de memoria compartida en el simulador";
		Log::getInstance()->loguear(mensajeLog.str());
	}

}




int Simulador::getCantidadAutosEstacionamiento()
{
	// Tomo el semaforo
	this->smfAdministracion.p();

	Administracion admin = (Administracion)this->administracion.leer();

	// Libero el semaforo
	this->smfAdministracion.v();

	return admin.getCantidadDeAutos();

}


