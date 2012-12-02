
#include "Simulador.h"

Simulador::Simulador(int numero, int cantidadPosiciones,int numeroEstacionamiento)
{
	this->numero = numero;
	this->numeroEstacionamiento = numeroEstacionamiento;

	//Asocian a la misma memoria compartida que crea el principal
	inicializarMemoriaCompartidaVectorPosiciones(cantidadPosiciones);
	inicializarMemoriaCompartidaAdministracion();

	stringstream archivoSmfAdministracion;

	archivoSmfAdministracion << ARCHIVO_SEMAFORO_ADMINISTRACION;
	archivoSmfAdministracion << ESTACIONAMIENTO;
	archivoSmfAdministracion << this->numeroEstacionamiento;


	Semaforo smfAdmin(archivoSmfAdministracion.str().c_str(),1,'a');
	this->smfAdministracion = smfAdmin;

	stringstream mensajeLog;
	mensajeLog << "Simulador - Creo semaforo id: " << this->smfAdministracion.getId() << " (admin)";
	Log::getInstance()->loguear(mensajeLog.str());
}

Simulador::~Simulador()
{
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
		stringstream msj;

		// Creo archivo temporal
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;
		nombreArchivo<<ESTACIONAMIENTO;
		nombreArchivo<<this->numeroEstacionamiento;

		msj << "este es el archivo: " << nombreArchivo.str();
		Log::getInstance()->loguear(msj.str());

		// Creo la memoria asociada al archivo temporal
		int estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		if ( estadoMemoria == SHM_OK )
		{
			this->vectorMemoriaPosiciones.push_back(memoria);

			//Creo un semaforo por cada posicion inicializada
			Semaforo semaforo((char*)nombreArchivo.str().c_str(), 1, (char) i);
			this->semaforos.push_back(semaforo);

			mensajeLog << "Creo semaforo id: " << semaforo.getId();
			Log::getInstance()->loguear(mensajeLog.str());
			mensajeLog.flush();
		}
		else
		{
			mensajeLog << "Simulador - Error al inicializar el vector de memoria compartida en la posicion " << i << " en el simulador";
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
	nombreArchivo<<ESTACIONAMIENTO;
	nombreArchivo<<this->numeroEstacionamiento;

	// Hago el atach a  la memoria asociada al archivo temporal
	int estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	if(estadoMemoria != SHM_OK)
	{
		mensajeLog << "Error al inicializar la administracion de memoria compartida en el simulador";
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


