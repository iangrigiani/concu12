
#include "SimuladorSalida.h"

SimuladorSalida::SimuladorSalida(int numeroSalida,int cantidadPosiciones) : Simulador (numeroSalida,cantidadPosiciones)
{

}


void SimuladorSalida::setPipeSalida(Pipe pipeSalida)
{
	this->pipeSalida = pipeSalida;
}

Pipe SimuladorSalida::getPipeSalida()
{
	return this->pipeSalida;
}


void SimuladorSalida::simular()
{
	stringstream mensajeLog;
	bool seguir = true;
	char recibido[BUFFSIZE];

	while (seguir)
	{
		int bytes = this->pipeSalida.leer(recibido, BUFFSIZE);
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

				this->pipePpal.escribir((char*)param.str().c_str(),BUFFSIZE);

				cout<<"Soy la salida "<<getNumeroSalida()<<" y le escribo al principal"<<endl;

				break;
			}
		}

	}

	cout << "Soy salida " << this->getNumeroSalida()<< " y numero proceso "<<getpid()<<" y muero ahora" << endl;
	this->pipeSalida.cerrar();

}

int SimuladorSalida::getNumeroSalida()
{
	return this->numero;
}


void SimuladorSalida::liberarLugar(int numeroPosicion)
{
	//TODO Semaforos!!!

	MemoriaCompartida<Posicion> memoria;
	Posicion posicion;

	stringstream mensajeLog;

	mensajeLog << "Memoria Compartida : soy la salida "<< this->getNumeroSalida()<<" y modifico la posicion " << numeroPosicion <<" poniendola como libre en el vector de posiciones.";

	Log::getInstance()->loguear(mensajeLog.str());

	memoria = this->vectorMemoriaPosiciones[numeroPosicion];

	posicion.setNumero(numeroPosicion);
	posicion.setEstadoOcupado(true);
	memoria.escribir(posicion);
	this->vectorMemoriaPosiciones[numeroPosicion] = memoria;

}

