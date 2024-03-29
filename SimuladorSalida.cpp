
#include "SimuladorSalida.h"

SimuladorSalida::SimuladorSalida(int numeroEstacionamiento,int numeroSalida,int cantidadPosiciones) : Simulador (numeroSalida,cantidadPosiciones,numeroEstacionamiento)
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

		// Me aseguro que solo se procesen los bytes recibidos y no la posible basura existente en el array
		string stringRecibido(recibido);
		stringRecibido = stringRecibido.substr(0, bytes);

		string token;
		stringstream stream(stringRecibido);
		while( getline(stream, token, '?') ) {

			char * recibidoParcial = (char*)token.c_str();
			stringstream retorno;

			switch (recibidoParcial[0])
			{
				case 'f': {
					char * token = strtok(recibidoParcial, "|");

					//cout << "Salida - " << this->getNumeroSalida() << " recibio " << token << endl;
					mensajeLog.str("");
					mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Recibio " << token;
					Log::getInstance()->loguear(mensajeLog.str());

					seguir = false;
					break;
				}
				case 's': {

					char * token = strtok(recibidoParcial, "|");
					token = strtok(NULL, "|");
					int numeroPosicion = atoi(token);

					//cout<<"Numero posicion recibido por salida "<<getNumeroSalida()<< " es "<<numeroPosicion<<endl;
					mensajeLog.str("");
					mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Numero posicion recibido por salida " << getNumeroSalida() << " es " << numeroPosicion;
					Log::getInstance()->loguear(mensajeLog.str());

					this->decrementarCantidadAutosEstacionamiento();

					this->liberarLugar(numeroPosicion);

					//Hay que avisarle al proceso servidor de mensajes (ppal) que agregue la
					//posicion que dejo el auto a la lista de libres
					stringstream param;

					param<<"d|";
					param<<numeroPosicion;
					param<<"?";

					pipePpal.escribir((char*)param.str().c_str(),param.str().length());

					//cout << "Salida - " << getNumeroSalida() << " y le escribo al principal" << endl;
					mensajeLog.str("");
					mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << getNumeroSalida() << " - Le escribo al principal para que libere la posicion " << numeroPosicion;
					Log::getInstance()->loguear(mensajeLog.str());

					break;
				}
			}
		}
	}

	liberarSemaforos();
	//cout << "Salida - " << this->getNumeroSalida() << " y numero proceso " << getpid() << " y muero ahora" << endl;
	mensajeLog.str("");
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Numero proceso " << getpid() << " y muero ahora";
	Log::getInstance()->loguear(mensajeLog.str());

	pipeSalida.cerrar();

}

int SimuladorSalida::getNumeroSalida()
{
	return this->numero;
}


void SimuladorSalida::liberarLugar(int numeroPosicion)
{
	stringstream mensajeLog;
	MemoriaCompartida<Posicion> memoria;
	Posicion posicion;

	//Bloqueo la posicion
	semaforos[numeroPosicion].p();
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Tome el semaforo de posicion " << numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	memoria = this->vectorMemoriaPosiciones[numeroPosicion];
	posicion = (Posicion) memoria.leer();
	posicion.setEstadoOcupado(false);

	memoria.escribir(posicion);

	this->vectorMemoriaPosiciones[numeroPosicion] = memoria;

	mensajeLog.str("");
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Libero el semaforo de posicion " << numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	//Desbloqueo la posicion
	semaforos[numeroPosicion].v();

	mensajeLog.str("");
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Modifico la posicion " << numeroPosicion <<" poniendola como ocupada en el vector de posiciones.";
	Log::getInstance()->loguear(mensajeLog.str());
}


void SimuladorSalida::decrementarCantidadAutosEstacionamiento()
{
	stringstream mensajeLog;
	// Tomo el semaforo
	this->smfAdministracion.p();

	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Tome semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	Administracion admin = (Administracion)this->administracion.leer();

	admin.decrementarCantidadAutos();

	this->administracion.escribir(admin);

	mensajeLog.str("");
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Libero semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();

	mensajeLog.str("");
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Se registra la salida de un auto al estacionamiento habiendo en total: " << admin.getCantidadDeAutos();
	Log::getInstance()->loguear(mensajeLog.str());
}

void SimuladorSalida::liberarSemaforos(){
	 // Elimino los semaforos
	vector<Semaforo>::iterator itSemaforos = this->semaforos.begin();
	while(itSemaforos != this->semaforos.end()){

		stringstream mensajeLog;

		mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Salida " << this->getNumeroSalida() << " - Libero semaforo id: " << itSemaforos->getId();
		Log::getInstance()->loguear(mensajeLog.str());

		itSemaforos->eliminar();

		itSemaforos++;
	}
}


