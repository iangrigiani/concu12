
#include "SimuladorEntrada.h"


SimuladorEntrada::SimuladorEntrada(int numeroEntrada,int cantidadPosiciones) : Simulador (numeroEntrada,cantidadPosiciones)
{
	this->cantidadAutos = 0;

}


double SimuladorEntrada::getNumeroAleatorio()
{
	return rand() / double(RAND_MAX);
}



int SimuladorEntrada::getNumeroEntrada()
{
	return this->numero;
}


void SimuladorEntrada::simular(Pipe pipeEntrada, Pipe pipePpal){

	pid_t w;


	// Genero un vector dinamico y lo inicializo para evitar los warning de compilador y de valgrind
	char recibir[BUFFSIZE];
	int q;

	for (q=0; q<BUFFSIZE; q++) {
		recibir[q] = 0;
	}

	while (!Cronometro::obtenerCronometro()->llegoAlFinal())
	{
		int semilla = time(0) * this->getNumeroEntrada();
		srand((unsigned)semilla);
		double numeroAlearorio = this->getNumeroAleatorio();

		double flujoDeAutos=Cronometro::obtenerCronometro()->getFlujoDeAutos();
		//cout << "Entrada " << this->getNumeroEntrada()  << " numero elegido: " << numeroAlearorio << " flujo de autos: " << flujoDeAutos << endl;
		if ( numeroAlearorio < flujoDeAutos )
		{
			Pipe pipeAuto;
			pid_t pAuto = fork();

			if( pAuto ==  0)
			{
				cout << "Entrada " << this->getNumeroEntrada() << " Proceso hijo creado " << getpid() << endl;

				// Frena el proceso hasta que la entrada le diga si puede entrar o no
				int bytes = pipeAuto.leer(recibir, BUFFSIZE);
				int numeroPosicion = atoi(recibir);

				// Si el numero de posicion es > 0 entonces hay lugar y el auto puede entrar
				if (numeroPosicion > -1)
				{

					cout<<"Numero Posicion asignada al auto que entra  "<<numeroPosicion<<" en la entrada "<<this->getNumeroEntrada()<<endl;

					double horasAleatoriasEstadia = this->getNumeroAleatorio();
					int horas = ceil(50*horasAleatoriasEstadia);
					Auto * automovil = new Auto(horas);
					automovil->setNumeroPosicion(numeroPosicion);


					cout << "Entrada " << this->getNumeroEntrada() << " Soy el auto pid: " << getpid() << " y entro en la posicion: " << numeroPosicion << endl;

					//El auto duerme el tiempo que permanece en el estacionamiento
					automovil->run();

					this->incrementarMontoRecaudado(horas);

					//this->cantidadAutos--;

					//Pipe al principal para que le avise que tiene que sacar el auto
					//del estacionamiento y agregar la posicion nuevamente en la lista de posiciones libres
					stringstream salida;

					salida<<"s|";
					salida<<automovil->getNumeroPosicion();
					salida<<"?";
					cout<<"La posicion que dejo es "<<automovil->getNumeroPosicion()<<" y la salida es "<<salida.str()<<endl;

					pipePpal.escribir((char*)salida.str().c_str(),salida.str().length());

					delete(automovil);
				} else {
					cout << "Entrada " << this->getNumeroEntrada() << " Soy el auto pid: " << getpid() << " y no me dejaron entrar porque no hay lugar!" << endl;
				}
				pipeAuto.cerrar();
				return;
			}
			else
			{
				//Enviar un mensaje al padre para pedir una posicion libre
				//el padre le devuelve la posicion libre del vector posiciones
				//si es que existe o -1 en otro caso

				// El ciclo se va a completar cuando el proceso obtenga una posicion que este desocupada o
				// cuando el proceso padre le diga que no hay mas ubicaciones libres
				bool cicloCompleto = false;

				stringstream nroEntrada;
				nroEntrada << this->getNumeroEntrada();
				nroEntrada << "?";

				while (!cicloCompleto)
				{

					//Agregar semaforo para sincronizar el acceso
					//Pido una ubicacion libre al servidor
					pipePpal.escribir((char*)nroEntrada.str().c_str(),nroEntrada.str().length());


					//Espero la respuesta y la guardo en la variable recibir
					pipeEntrada.leer(recibir, BUFFSIZE);

					int numeroPosicion = 0;
					numeroPosicion = atoi(recibir);

					cout<<"Soy la entrada "<<this->getNumeroEntrada()<<" y recibi la posicion "<<numeroPosicion<<endl;

					//Si devuelve un nro negativo, entonces no hay lugar y el auto no puede ingresar
					if (numeroPosicion < 0)
					{
						pipeAuto.escribir(recibir,strlen(recibir));
						cicloCompleto = true;
					}
					else
					{

						//Aca intento tomar escribir en esa posicion de memoria para ocuparla, si no puedo, busco otra
						bool pudeOcuparPosicion = this->modificarPosicionCompartida(numeroPosicion);

						if (pudeOcuparPosicion)
						{
							cout<<"Soy la entrada "<<this->getNumeroEntrada()<<"y pude ocupar la posicion "<<numeroPosicion<<endl;

							pipeAuto.escribir(recibir,strlen(recibir));

							//Tengo que avisarle al proceso principal que saque del vector de posiciones libres
							//la posicion que me asigno recien (le paso el nro por el pipe: p|nroPosicion )
							//la 'p' es para avisarle que saque (convencion)

							stringstream param;

							param<<"p|";
							param<<numeroPosicion;
							param<<"?";

							cout<<"Soy la entrada "<<this->getNumeroEntrada()<<" y le envio al principal "<<param.str()<<endl;

							pipePpal.escribir((char*)param.str().c_str(),param.str().length());


							this->incrementarCantidadDeAutosEstacionamiento();
							this->cantidadAutos++;


							cicloCompleto = true;
						} else {
							cout << "Entrada " << this->getNumeroEntrada() << " no puedo ocupar posicion: " << numeroPosicion << endl;
						}

					}

				}


				pipeAuto.cerrar();

			}
		}
		sleep(1);

		cout << "Entrada " << this->getNumeroEntrada() << " Cantidad de autos: " << this->cantidadAutos << endl;
	}


	cout << "Entrada " << this->getNumeroEntrada() << " Fin simulacion, esperando que terminen todos los procesos..." << endl;
	cout << "Entrada " << this->getNumeroEntrada() << " Cantidad de autos: " << this->cantidadAutos << endl;
	int estado;
	while (this->cantidadAutos > 0)
	{
		w = wait(&estado);
		cout << "Entrada " << this->getNumeroEntrada() << " Terminado proceso..." << w << endl;

		this->cantidadAutos--;

		cout << "Entrada " << this->getNumeroEntrada() << " Cantidad de autos: " << this->cantidadAutos << ". Esperando que salgan los demas..." << endl;

	}


	//Envia un pipe al principal para que le diga que tiene que finalizar el ciclo a la salida.
	stringstream numeroEntrada;
	numeroEntrada<<"f|";
	numeroEntrada<<this->getNumeroEntrada();
	numeroEntrada<<"?";


	cout << "Soy entrada " << this->getNumeroEntrada() << " y escribo: " << numeroEntrada.str() << endl;
	pipePpal.escribir((char*)numeroEntrada.str().c_str(),numeroEntrada.str().length());

	cout << "Soy entrada " << this->getNumeroEntrada() << " y muero ahora" << endl;

	pipeEntrada.cerrar();

}



bool SimuladorEntrada::modificarPosicionCompartida(int numeroPosicion)
{

	MemoriaCompartida<Posicion> memoria;
	Posicion posicion;

	//Bloqueo la posicion
	semaforos[numeroPosicion].p();

	memoria = this->vectorMemoriaPosiciones[numeroPosicion];
	posicion = (Posicion)memoria.leer();

	//Si la posicion ya no fue ocupada
	if(!posicion.getEstadoOcupado())
	{
		posicion.setEstadoOcupado(true);
		memoria.escribir(posicion);
		this->vectorMemoriaPosiciones[numeroPosicion] = memoria;

	}

	//Desbloqueo la posicion
	semaforos[numeroPosicion].v();

	if(posicion.getEstadoOcupado())
	{

		stringstream mensajeLog;

		mensajeLog << "Memoria Compartida : soy la entrada "<< this->getNumeroEntrada()<<" y modifico la posicion " << numeroPosicion <<" poniendola como ocupada en el vector de posiciones.";

		Log::getInstance()->loguear(mensajeLog.str());
	}

	//Si devuelve true es que pudo ocupar la posicion, sino no pudo y devuelve false.
	return posicion.getEstadoOcupado();

}

void SimuladorEntrada::incrementarCantidadDeAutosEstacionamiento()
{
	// Tomo el semaforo
	this->smfAdministracion.p();

	Administracion admin = (Administracion)this->administracion.leer();

	admin.incrementarCantidadAutos();

	this->administracion.escribir(admin);

	// Libero el semaforo
	this->smfAdministracion.v();

	stringstream mensajeLog;

	mensajeLog <<"Se registra la entrada de un auto al estacionamiento habiendo en total: "<<admin.getCantidadDeAutos();

	Log::getInstance()->loguear(mensajeLog.str());


}


void SimuladorEntrada::incrementarMontoRecaudado(int horas)
{

	// Tomo el semaforo
	this->smfAdministracion.p();

	Administracion admin = (Administracion)this->administracion.leer();

	admin.actualizarImporteRecaudado(horas);

	this->administracion.escribir(admin);

	stringstream mensajeLog;

	mensajeLog <<"El importe total registrado hasta el momento es : "<<admin.getImporteRecaudado();

	// Libero el semaforo
	this->smfAdministracion.v();

	Log::getInstance()->loguear(mensajeLog.str());

}




