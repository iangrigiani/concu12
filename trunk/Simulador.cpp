
#include "Simulador.h"

Simulador::Simulador(int numero, int cantidadPosiciones)
{
	//Asocian a la misma memoria compartida que crea el principal
	inicializarMemoriaCompartidaVectorPosiciones(cantidadPosiciones);
	inicializarMemoriaCompartidaAdministracion();

	this->numero = numero;

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

		// Creo archivo temporal
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;

		// Creo la memoria asociada al archivo temporal
		int estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		if ( estadoMemoria == SHM_OK )
			this->vectorMemoriaPosiciones.push_back(memoria);

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


//void Simulador::setPipePrincipal(Pipe pipePrincipal){
//	this->pipePpal = pipePrincipal;
//}
//
//Pipe Simulador::getPipePrincipal()
//{
//	return this->pipePpal;
//}


void Simulador::decrementarCantidadAutosEstacionamiento()
{
	Administracion admin = (Administracion)this->administracion.leer();

	int cantAutos = admin.getCantidadDeAutos();
	admin.decrementarCantidadAutos();

	this->administracion.escribir(admin);

	Administracion admin2 = (Administracion)this->administracion.leer();
	cout<<"Al decrementar en la salida "<<this->numero<<"antes "<<cantAutos<<" se leen ahora "<<admin2.getCantidadDeAutos()<<endl;

}

int Simulador::getCantidadAutosEstacionamiento()
{
	Administracion admin = (Administracion)this->administracion.leer();

	return admin.getCantidadDeAutos();

}


