
#include "SimuladorSalida.h"

SimuladorSalida::SimuladorSalida(int numeroSalida,int cantidadPosiciones) : Simulador (numeroSalida,cantidadPosiciones)
{

}


void SimuladorSalida::simular(Pipe pipeSalida, Pipe pipePpal)
{
	stringstream mensajeLog;
	bool seguir = true;
	char recibido[BUFFSIZE];

	while (seguir)
	{
		int bytes = pipeSalida.leer(recibido, BUFFSIZE);
		recibido[bytes] = '\0';

		stringstream retorno;

		switch (recibido[0])
		{
			case 'f': {
				char * token = strtok(recibido, "|");
				token = strtok(NULL, "|");
				cout<<"La salida "<<this->getNumeroSalida()<<" recibio "<<token<<endl;
				seguir = false;
				break;
			}
			case 's': {

				char * token = strtok(recibido, "|");
				token = strtok(NULL, "|");
				int numeroPosicion = atoi(token);

				cout<<"Numero posicion recibido por salida "<<getNumeroSalida()<< " es "<<numeroPosicion<<endl;

				this->decrementarCantidadAutosEstacionamiento();

				this->liberarLugar(numeroPosicion);

				//Hay que avisarle al proceso servidor de mensajes (ppal) que agregue la
				//posicion que dejo el auto a la lista de libres
				stringstream param;

				param<<"d|";
				param<<numeroPosicion;
				param<<"?";

				pipePpal.escribir((char*)param.str().c_str(),param.str().length());

				cout<<"Soy la salida "<<getNumeroSalida()<<" y le escribo al principal"<<endl;

				break;
			}
		}

	}

	cout << "Soy salida " << this->getNumeroSalida()<< " y numero proceso "<<getpid()<<" y muero ahora" << endl;
	pipeSalida.cerrar();

}

int SimuladorSalida::getNumeroSalida()
{
	return this->numero;
}


void SimuladorSalida::liberarLugar(int numeroPosicion)
{
	MemoriaCompartida<Posicion> memoria;
	Posicion posicion;

	//Bloqueo la posicion
	semaforos[numeroPosicion].p();

	memoria = this->vectorMemoriaPosiciones[numeroPosicion];
	posicion = (Posicion) memoria.leer();
	posicion.setEstadoOcupado(false);

	memoria.escribir(posicion);

	this->vectorMemoriaPosiciones[numeroPosicion] = memoria;

	//Desbloqueo la posicion
	semaforos[numeroPosicion].v();

	stringstream mensajeLog;

	mensajeLog << "Memoria Compartida : soy la salida "<< this->getNumeroSalida()<<" y modifico la posicion " << numeroPosicion <<" poniendola como libre en el vector de posiciones.";

	Log::getInstance()->loguear(mensajeLog.str());
}


void SimuladorSalida::decrementarCantidadAutosEstacionamiento()
{

	// Tomo el semaforo
	this->smfAdministracion.p();

	Administracion admin = (Administracion)this->administracion.leer();

	admin.decrementarCantidadAutos();

	this->administracion.escribir(admin);

	// Libero el semaforo
	this->smfAdministracion.v();

	stringstream mensajeLog;

	mensajeLog <<"Se registra la salida de un auto al estacionamiento habiendo en total: "<<admin.getCantidadDeAutos();

	Log::getInstance()->loguear(mensajeLog.str());


}
