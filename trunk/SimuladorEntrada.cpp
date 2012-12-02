
#include "SimuladorEntrada.h"


SimuladorEntrada::SimuladorEntrada(int numeroEstacionamiento,int numeroEntrada,int cantidadPosiciones) : Simulador (numeroEntrada,cantidadPosiciones,numeroEstacionamiento)
{
	this->cantidadAutosCreados = 0;

}


double SimuladorEntrada::getNumeroAleatorio()
{
	return rand() / double(RAND_MAX);
}



int SimuladorEntrada::getNumeroEntrada()
{
	return this->numero;
}


void SimuladorEntrada::simular(Pipe pipeEntrada, Pipe pipePpal, int nroEstacionamiento){

	pid_t w;

	stringstream mensajeLog;

	// Genero un vector dinamico y lo inicializo para evitar los warning de compilador y de valgrind
	char recibir[BUFFSIZE];
	int q;

	for (q=0; q<BUFFSIZE; q++) {
		recibir[q] = 0;
	}

	while (!Cronometro::obtenerCronometro()->llegoAlFinal())
	{
		// Multiplico la variable por el numero de entrada para que las 3 tengan distinta semilla
		int semilla = time(0) * this->getNumeroEntrada() * nroEstacionamiento;
		srand((unsigned)semilla);
		double numeroAlearorio = this->getNumeroAleatorio();

		double flujoDeAutos=Cronometro::obtenerCronometro()->getFlujoDeAutos();

		if ( numeroAlearorio < flujoDeAutos )
		{
			Pipe pipeAuto;

			pid_t pAuto = fork();

			// Aumento la variable que me indica la cantidad de proceso hijos creados
			this->cantidadAutosCreados++;

			if( pAuto ==  0)
			{
				//cout << "Entrada " << this->getNumeroEntrada() << " Proceso hijo creado " << getpid() << endl;
				mensajeLog.str("");
				mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Proceso hijo creado " << getpid();
				Log::getInstance()->loguear(mensajeLog.str());

				// Frena el proceso hasta que la entrada le diga si puede entrar o no
				int bytes = pipeAuto.leer(recibir, BUFFSIZE);

				string stringRecibido(recibir);
				stringRecibido = stringRecibido.substr(0, bytes);

				int numeroPosicion = atoi(stringRecibido.c_str());

				// Si el numero de posicion es > 0 entonces hay lugar y el auto puede entrar
				if (numeroPosicion > -1)
				{

					//cout<<"Numero Posicion asignada al auto que entra  "<<numeroPosicion<<" en la entrada "<<this->getNumeroEntrada()<<endl;
					mensajeLog.str("");
					mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Numero Posicion asignada al auto que entra: " << numeroPosicion;
					Log::getInstance()->loguear(mensajeLog.str());

					double horasAleatoriasEstadia = this->getNumeroAleatorio();
					int horas = ceil(50*horasAleatoriasEstadia);
					Auto automovil(horas);
					automovil.setNumeroPosicion(numeroPosicion);


					//cout << "Entrada " << this->getNumeroEntrada() << " Soy el auto pid: " << getpid() << " y entro en la posicion: " << numeroPosicion << endl;
					mensajeLog.str("");
					mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Soy el auto pid: " << getpid() << " y entro en la posicion: " << numeroPosicion;
					Log::getInstance()->loguear(mensajeLog.str());

					//El auto duerme el tiempo que permanece en el estacionamiento
					automovil.run();

					this->incrementarMontoRecaudado(horas);

					//Pipe al principal para que le avise que tiene que sacar el auto
					//del estacionamiento y agregar la posicion nuevamente en la lista de posiciones libres
					stringstream salida;

					salida<<"s|";
					salida<<automovil.getNumeroPosicion();
					salida<<"?";

					cout << "Soy el auto " << getpid() << " y la posicion que dejo es " << automovil.getNumeroPosicion() << endl;
					mensajeLog.str("");
					mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Soy el auto pid: " << getpid() << " La posicion que dejo es " << automovil.getNumeroPosicion();
					Log::getInstance()->loguear(mensajeLog.str());

					pipePpal.escribir((char*)salida.str().c_str(),salida.str().length());

				} else {
					//cout << "Entrada " << this->getNumeroEntrada() << " Soy el auto pid: " << getpid() << " y no me dejaron entrar porque no hay lugar!" << endl;
					mensajeLog.str("");
					mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Soy el auto pid: " << getpid() << " y no me dejaron entrar porque no hay lugar!";
					Log::getInstance()->loguear(mensajeLog.str());
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

					//Pido una ubicacion libre al servidor
					pipePpal.escribir((char*)nroEntrada.str().c_str(),nroEntrada.str().length());


					//Espero la respuesta y la guardo en la variable recibir
					int bytes = pipeEntrada.leer(recibir, BUFFSIZE);

					string stringRecibido(recibir);
					stringRecibido = stringRecibido.substr(0, bytes);

					int numeroPosicion = 0;
					numeroPosicion = atoi((char*)stringRecibido.c_str());

					//cout<<"Soy la entrada "<<this->getNumeroEntrada()<<" y recibi la posicion "<<numeroPosicion<<endl;
					mensajeLog.str("");
					mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Recibi la posicion " << numeroPosicion;
					Log::getInstance()->loguear(mensajeLog.str());

					//Si devuelve un nro negativo, entonces no hay lugar y el auto no puede ingresar
					if (numeroPosicion < 0)
					{
						pipeAuto.escribir((char*)stringRecibido.c_str(),bytes);
						cicloCompleto = true;
					}
					else
					{

						//Aca intento tomar escribir en esa posicion de memoria para ocuparla, si no puedo, busco otra
						bool pudeOcuparPosicion = this->modificarPosicionCompartida(numeroPosicion);

						if (pudeOcuparPosicion)
						{
							//cout<<"Soy la entrada "<<this->getNumeroEntrada()<<"y pude ocupar la posicion "<<numeroPosicion<<endl;
							mensajeLog.str("");
							mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Pude ocupar la posicion " << numeroPosicion;
							Log::getInstance()->loguear(mensajeLog.str());

							pipeAuto.escribir((char*)stringRecibido.c_str(),bytes);

							//Tengo que avisarle al proceso principal que saque del vector de posiciones libres
							//la posicion que me asigno recien (le paso el nro por el pipe: p|nroPosicion )
							//la 'p' es para avisarle que saque (convencion)

							stringstream param;

							param<<"p|";
							param<<numeroPosicion;
							param<<"?";

							//cout<<"Soy la entrada "<<this->getNumeroEntrada()<<" y le envio al principal "<<param.str()<<endl;
							mensajeLog.str("");
							mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Le envio al principal " << param.str();
							Log::getInstance()->loguear(mensajeLog.str());

							pipePpal.escribir((char*)param.str().c_str(),param.str().length());

							this->incrementarCantidadDeAutosEstacionamiento();

							cicloCompleto = true;
						} else {
							//cout << "Entrada " << this->getNumeroEntrada() << " no puedo ocupar posicion: " << numeroPosicion << endl;
							mensajeLog.str("");
							mensajeLog << "Entrada " << this->getNumeroEntrada() << " - no puedo ocupar posicion: " << numeroPosicion;
							Log::getInstance()->loguear(mensajeLog.str());
						}
					}
				}
				pipeAuto.cerrar();
			}
		}
		sleep(1);
		//cout << "Entrada " << this->getNumeroEntrada() << " Cantidad de autos: " << this->cantidadAutosCreados << endl;
		mensajeLog.str("");
		mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Cantidad de autos creados: " << this->cantidadAutosCreados;
		Log::getInstance()->loguear(mensajeLog.str());
	}


	//cout << "Entrada " << this->getNumeroEntrada() << " Fin simulacion, esperando que terminen todos los procesos..." << endl;
	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Fin simulacion, esperando que terminen todos los procesos...";
	Log::getInstance()->loguear(mensajeLog.str());

	//cout << "Entrada " << this->getNumeroEntrada() << " Cantidad de autos: " << this->cantidadAutosCreados << endl;
	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Cantidad de autos creados: " << this->cantidadAutosCreados;
	Log::getInstance()->loguear(mensajeLog.str());

	int estado;
	while (this->cantidadAutosCreados > 0)
	{
		w = wait(&estado);
		//cout << "Entrada " << this->getNumeroEntrada() << " Terminado proceso..." << w << endl;
		mensajeLog.str("");
		mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Terminado proceso..." << w;
		Log::getInstance()->loguear(mensajeLog.str());

		this->cantidadAutosCreados--;

		//cout << "Entrada " << this->getNumeroEntrada() << " Cantidad de autos: " << this->cantidadAutosCreados << ". Esperando que salgan los demas..." << endl;
		mensajeLog.str("");
		mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Cantidad de autos: " << this->cantidadAutosCreados << ". Esperando que salgan los demas...";
		Log::getInstance()->loguear(mensajeLog.str());
	}


	//Envia un pipe al principal para que le diga que tiene que finalizar el ciclo a la salida.
	stringstream numeroEntrada;
	numeroEntrada<<"f|";
	numeroEntrada<<this->getNumeroEntrada();
	numeroEntrada<<"?";

	//cout << "Entrada " << this->getNumeroEntrada() << " - Escribo: " << numeroEntrada.str() << endl;
	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Escribo: " << numeroEntrada.str();
	Log::getInstance()->loguear(mensajeLog.str());

	pipePpal.escribir((char*)numeroEntrada.str().c_str(),numeroEntrada.str().length());

	//cout << "Entrada " << this->getNumeroEntrada() << " - y muero ahora" << endl;
	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - y muero ahora";
	Log::getInstance()->loguear(mensajeLog.str());

	pipeEntrada.cerrar();

}



bool SimuladorEntrada::modificarPosicionCompartida(int numeroPosicion)
{
	stringstream mensajeLog;

	bool pude = false;
	MemoriaCompartida<Posicion> memoria;
	Posicion posicion;

	//Bloqueo la posicion
	semaforos[numeroPosicion].p();

	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Tome el semaforo de posicion " << numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	memoria = this->vectorMemoriaPosiciones[numeroPosicion];
	posicion = (Posicion)memoria.leer();

	//Si la posicion ya no fue ocupada
	if(!posicion.getEstadoOcupado())
	{
		posicion.setEstadoOcupado(true);
		memoria.escribir(posicion);
		this->vectorMemoriaPosiciones[numeroPosicion] = memoria;
		pude = true;
	}

	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Libero el semaforo de posicion " << numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	//Desbloqueo la posicion
	semaforos[numeroPosicion].v();

	if(pude)
	{
		mensajeLog.str("");
		mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Modifico la posicion " << numeroPosicion <<" poniendola como ocupada en el vector de posiciones.";
		Log::getInstance()->loguear(mensajeLog.str());
	}

	//Si devuelve true es que pudo ocupar la posicion, sino no pudo y devuelve false.
	return pude;

}

void SimuladorEntrada::incrementarCantidadDeAutosEstacionamiento()
{
	stringstream mensajeLog;

	// Tomo el semaforo
	this->smfAdministracion.p();

	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Tome el semaforo de administracion ";
	Log::getInstance()->loguear(mensajeLog.str());
	Administracion admin = (Administracion)this->administracion.leer();

	admin.incrementarCantidadAutos();

	this->administracion.escribir(admin);

	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Libero el semaforo de administracion ";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();

	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Se registra la entrada de un auto al estacionamiento habiendo en total: " << admin.getCantidadDeAutos();
	Log::getInstance()->loguear(mensajeLog.str());
}


void SimuladorEntrada::incrementarMontoRecaudado(int horas)
{
	stringstream mensajeLog;

	// Tomo el semaforo
	this->smfAdministracion.p();

	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Tome el semaforo de administracion ";
	Log::getInstance()->loguear(mensajeLog.str());

	Administracion admin = (Administracion)this->administracion.leer();

	admin.actualizarImporteRecaudado(horas);

	this->administracion.escribir(admin);

	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - Libero el semaforo de administracion ";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();

	mensajeLog.str("");
	mensajeLog << "Entrada " << this->getNumeroEntrada() << " - El importe total registrado hasta el momento es : " << admin.getImporteRecaudado();
	Log::getInstance()->loguear(mensajeLog.str());
}





