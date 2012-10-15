#include "Estacionamiento.h"



Estacionamiento::Estacionamiento() {

	Semaforo smfAdmin(ARCHIVO_SEMAFORO_ADMINISTRACION,1,'a');

	this->smfAdministracion = smfAdmin;
}


Estacionamiento::~Estacionamiento()
{
	this->smfAdministracion.eliminar();
}


void Estacionamiento::crearArchivosTemporales(int cantidadLugares)
{

	int i;
	FILE * tmpPosicion;
	FILE * tmpAdministracion;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;

		// Creo archivo temporal posiciones
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;
		//cout << "nombre archivo: " << nombreArchivo.str() << endl;

		tmpPosicion = fopen(nombreArchivo.str().c_str(),"w");
		fclose(tmpPosicion);


		nombreArchivo.flush();

	}

	// Creo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;


	tmpAdministracion = fopen(archivoAdministracion.str().c_str(),"w");
	fclose(tmpAdministracion);

}

void Estacionamiento::eliminarArchivosTemporales(int cantidadLugares)
{
	int i;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;
		stringstream nombreArchivoLibres;

		// Destruyo archivo temporal posiciones
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;


		remove(nombreArchivo.str().c_str());

		remove(nombreArchivoLibres.str().c_str());


		nombreArchivo.flush();

	}

	// Destruyo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;

	remove(archivoAdministracion.str().c_str());
}



void Estacionamiento::crearMemoriaCompartidaPosiciones(int cantidadLugares)
{

	int i;
	Posicion posicion;
	MemoriaCompartida<Posicion> memoria;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;
		stringstream mensajeLog;

		// Creo archivo temporal
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;

		// Creo la memoria asociada al archivo temporal
		int estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		if(estadoMemoria ==  SHM_OK)
		{
			mensajeLog << "Memoria Compartida : escribo el numero " << i <<" en el vector de posiciones.";

			Log::getInstance()->loguear(mensajeLog.str());

			posicion.setNumero(i);

			memoria.escribir(posicion);

			this->vectorMemoriaPosiciones.push_back(memoria);
		}
		else
		{
			mensajeLog<<"Error al inicializar el vector de memoria compartida en la posicion "<<i<<" en el estacionamiento";

			Log::getInstance()->loguear(mensajeLog.str());

		}

		nombreArchivo.flush();
		mensajeLog.flush();
	}
}

void Estacionamiento::crearVectorPosicionesLibres(int cantidadLugares)
{

	int i;

	for (i=0;i<cantidadLugares;i++)
	{

		stringstream mensajeLog;

		mensajeLog << "Escribo el numero " << i <<" en el vector de posiciones libres.";

		Log::getInstance()->loguear(mensajeLog.str());

		this->vectorMemoriaPosicionesLibres.push_back(i);

		mensajeLog.flush();
	}

}

void Estacionamiento::crearMemoriaCompartidaAdministracion(float costoHora)
{

	Administracion administracion;
	stringstream nombreArchivo;
	stringstream mensajeLog;

	administracion.setCostoHora(costoHora);


	// Creo archivo temporal
	nombreArchivo << ARCHIVO_ADMINISTRACION;

	// Creo la memoria asociada al archivo temporal
	int estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	if(estadoMemoria == SHM_OK)
	{
		mensajeLog << "Memoria Compartida : escribo el costo hora del estacionamiento costo hora = " << administracion.getCostoHora();

		Log::getInstance()->loguear(mensajeLog.str());

		this->administracion.escribir(administracion);
	}
	else
	{
		mensajeLog<<"Error al inicializar la administracion de memoria compartida en el estacionamiento";
		Log::getInstance()->loguear(mensajeLog.str());


	}



}




void Estacionamiento::run(int cantidadDeLugares, float costoHora, int tiempoEjecucion)
{

	// Genero un vector dinamico y lo inicializo para evitar los warning de compilador y de valgrind
	char recibido[BUFFSIZE];
	int q;

	for (q=0; q<BUFFSIZE; q++) {
		recibido[q] = 0;
	}


	//Inicializa el tiempo en el cronometro y arranca el tiempo a correr en la simulacion
	Cronometro::obtenerCronometro()->setTiempoASimular(tiempoEjecucion);
	Cronometro::obtenerCronometro()->iniciarTiempo();

	this->pConsola = fork();


	if( this->pConsola != 0)
	{

		stringstream mensajeLog;

		mensajeLog << "Soy el proceso principal";

		Log::getInstance()->loguear(mensajeLog.str());

		this->crearIniciales(cantidadDeLugares, costoHora);


		//Proceso padre
		this->pEntrada1 = fork();

		if( this->pEntrada1 == 0)
		{
			//Proceso entrada 1
			stringstream mensajeLog;
			mensajeLog << "Soy el proceso entrada 1";
			Log::getInstance()->loguear(mensajeLog.str());

			correrSimuladorEntrada(1,this->pipeEntrada1,cantidadDeLugares);
		}
		else
		{

			this->pEntrada2 = fork();

			if( this->pEntrada2 == 0)
			{
				//Proceso entrada 2
				stringstream mensajeLog;
				mensajeLog << "Soy el proceso entrada 2";
				Log::getInstance()->loguear(mensajeLog.str());

				correrSimuladorEntrada(2,this->pipeEntrada2,cantidadDeLugares);
			}
			else
			{

				this->pEntrada3 = fork();

				if( this->pEntrada3 == 0)
				{
					//Proceso entrada 3
					stringstream mensajeLog;
					mensajeLog << "Soy el proceso entrada 3";
					Log::getInstance()->loguear(mensajeLog.str());

					correrSimuladorEntrada(3,this->pipeEntrada3,cantidadDeLugares);
				}
				else
				{
					this->pSalida1 = fork();

					if (this->pSalida1 == 0)
					{
						//Proceso salida 1
						stringstream mensajeLog;
						mensajeLog<<"Soy el proceso salida 1";
						Log::getInstance()->loguear(mensajeLog.str());

						correrSimuladorSalida(1,this->pipeSalida1,cantidadDeLugares);
					}
					else
					{
						this->pSalida2 = fork();

						if(this->pSalida2 == 0)
						{
							//Proceso salida 2
							stringstream mensajeLog;
							mensajeLog<<"Soy el proceso salida 2";
							Log::getInstance()->loguear(mensajeLog.str());

							correrSimuladorSalida(2,this->pipeSalida2,cantidadDeLugares);

						}
						else
						{
							// Proceso principal (funciona como servidor de mensajes)
							stringstream mensajeLog;
							mensajeLog << "Soy el proceso principal funcionando como servidor de mensajes.";
							Log::getInstance()->loguear(mensajeLog.str());

							bool salir = false;

							int cantidadTotalEntradas = 0;

							while (!salir)
							{
								//cout << "PPAL: ARRANCO EL WHILE CON ESTA MIERDA EN EL RECIBIDO: " << recibido << endl;
								int bytes = this->pipePpal.leer((char*)recibido, BUFFSIZE);

								string stringRecibido(recibido);
								stringRecibido = stringRecibido.substr(0, bytes);

								//cout << "ProcPrincipal: acabo de recibir " << bytes << " bytes: " << str << endl;
								string token;
								stringstream stream(stringRecibido);
								while( getline(stream, token, '?') ) {

									char * recibidoParcial = (char*)token.c_str();

									//cout << "ProcPrincipal: acabo de recibir: " << word << endl;

									stringstream retorno;

									switch (recibidoParcial[0])
									{
										case 'a': {
											cout << "Ingreso una a" << endl;
											int cantidad = this->obtenerCantidadActualDeAutos();
											retorno << "La cantidad actual de autos es : "<< cantidad;
											this->pipeConsola.escribir((char*)retorno.str().c_str(),retorno.str().length());
											break;
										}
										case 'm': {
											cout << "Ingreso una m" << endl;
											float monto = this->obtenerMontoRecaudado();
											retorno << "El monto recaudado hasta el momento es : "<<monto;
											this->pipeConsola.escribir((char*)retorno.str().c_str(),retorno.str().length());
											break;
										}
										case 'q': {
											cout << "Ingreso un q" << endl;
											salir = true;
											this->pipeConsola.cerrar();
											break;
										}
										case 'p': {
											char * token = strtok(recibidoParcial, "|");
											token = strtok(NULL, "|");
											cout << "Saco del vector de posiciones libres la posicion: " << token << endl;
											int pos = atoi(token);
											this->quitarPosicionLibre(pos);
											break;
										}
										case 'd': {
											char * token = strtok(recibidoParcial, "|");
											token = strtok(NULL, "|");
											cout << "Agrego al  vector de posiciones libres la posicion: " << token << endl;
											int pos = atoi(token);
											this->agregarPosicionLibre(pos);
											break;
										}
										case 's': {
											char * token = strtok(recibidoParcial, "|");
											token = strtok(NULL, "|");
											int pos = atoi(token);
											int numeroSalida = this->getSalidaAleatoria();
											cout << "Auto sale del estacionamiento por la salida "<<numeroSalida<<" y deja la posicion " << pos << endl;
											retorno<<"s|";
											retorno<<pos;
											if(numeroSalida == 1){
												this->pipeSalida1.escribir((char*)retorno.str().c_str(),retorno.str().length());
											}
											else
											{
												this->pipeSalida2.escribir((char*)retorno.str().c_str(),retorno.str().length());

											}
											break;
										}
										case 'f': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");
												int numeroEntrada = atoi(token);

												cantidadTotalEntradas+=numeroEntrada;

												cout<<"La cantidad total es "<<cantidadTotalEntradas<<endl;

												if(cantidadTotalEntradas == CANTIDAD_TOTAL_ENTRADAS)
												{
													retorno<<"f|";
													retorno<<"NO ESPERES MAS";
													this->pipeSalida1.escribir((char*)retorno.str().c_str(),retorno.str().length());
													this->pipeSalida2.escribir((char*)retorno.str().c_str(),retorno.str().length());

												}
												break;
										}
										default: {
											if ( recibidoParcial[0] > '0' && recibidoParcial[0] < '4') {
												int nroPosicion;
												nroPosicion = this->getPosicionAleatoria();
												cout << "Entrada " << recibidoParcial[0] << " pide una posicion, le envio: " << nroPosicion << endl;
												retorno << nroPosicion;
												if (recibidoParcial[0] == '1'){
													this->pipeEntrada1.escribir((char*)retorno.str().c_str(),retorno.str().length());
												} else {
													if (recibidoParcial[0] == '2') {
														this->pipeEntrada2.escribir((char*)retorno.str().c_str(),retorno.str().length());
													} else {
														this->pipeEntrada3.escribir((char*)retorno.str().c_str(),retorno.str().length());
													}
												}
											} else {
												cout << "Comando invalido" << endl;
											}
											break;
										}
									}
								}
							}

							cout << endl << "Acordate de ingresar Q por teclado pq sino esto no muere mas!! " << endl;
							cout << "Pconsola es...: " << pConsola << endl;

							finalizarProcesos();

							this->liberarMemoriaCompartida(cantidadDeLugares);

							finalizarPipes();

							cout << "Terminado proceso..." << pConsola << endl;
							cout << "FIN SIMULACION" << endl;

							stringstream mensajeFinalizacion;
							mensajeFinalizacion<<"FIN SIMULACION";
							Log::getInstance()->loguear(mensajeFinalizacion.str());
						}
					}
				}
			}
		}
	}
	else
	{
		// Proceso consola

		stringstream mensajeLog;
		mensajeLog << "Soy el proceso consola esperando el ingreso por pantalla.";
		Log::getInstance()->loguear(mensajeLog.str());

		bool salir = false;
		// Para terminar el programa, el usuario debe escribir 'Q' o 'q'
		while (!salir) {
			cout << "ProcConsola: Me quedo esperando que ingreses algo por consola..." << endl;
			cin.getline(recibido, 2);

			stringstream ssRecibido;
			ssRecibido << recibido;
			ssRecibido << "?";

			// Aca hay que agregar la validacion para que solo pueda ingresar una letra para hacer consultas
			// o para salir
			// Envio solo 2 bytes
			this->pipePpal.escribir((char*)ssRecibido.str().c_str(),2);

			if (recibido[0] != 'q' && recibido[0] != 'Q') {

				char recibir[BUFFSIZE];
				int bytes = this->pipeConsola.leer(recibir, BUFFSIZE);
				recibir[bytes] = '\0';

				cout << "ProcConsola: acabo de recibir como rta: " << recibir << endl;

			} else {
				salir = true;
			}
		}

		this->pipeConsola.cerrar();
		this->pipePpal.cerrar();
		cout << "Me rajo antes que muera... soy: " << getpid() << endl;
	}

	Cronometro::destruir();
}



void Estacionamiento::crearIniciales(int cantidadDeLugares, int costoHora){
	this->crearArchivosTemporales(cantidadDeLugares);

	this->crearMemoriaCompartidaPosiciones(cantidadDeLugares);

	this->crearMemoriaCompartidaAdministracion(costoHora);

	this->crearVectorPosicionesLibres(cantidadDeLugares);
}


void Estacionamiento::correrSimuladorEntrada(int numeroEntrada,Pipe pipeEntrada, int cantidadPosiciones)
{
	SimuladorEntrada * simuladorEntrada = new SimuladorEntrada(numeroEntrada,cantidadPosiciones);

	simuladorEntrada->simular(pipeEntrada, this->pipePpal);

	pipeEntrada.cerrar();
	delete simuladorEntrada;
}


void Estacionamiento::correrSimuladorSalida(int numeroSalida,Pipe pipeSalida, int cantidadPosiciones)
{
	SimuladorSalida * simuladorSalida = new SimuladorSalida(numeroSalida,cantidadPosiciones);

	simuladorSalida->simular(pipeSalida, this->pipePpal);

	pipeSalida.cerrar();
	delete simuladorSalida;


}


int Estacionamiento::getPosicionAleatoria()
{
	int cantidadPosiciones = this->vectorMemoriaPosicionesLibres.size();

	//Si la lista de posiciones libres esta vacia, entonces debe retornar un numero
	//negativo de manera que no deje entrar a ningun auto.
	if(cantidadPosiciones == 0)
		return -1;

	int numeroElegido;

	srand((unsigned)time(0));
	numeroElegido = calcularRandom(cantidadPosiciones);

	return this->vectorMemoriaPosicionesLibres[numeroElegido];

}

int Estacionamiento::getSalidaAleatoria()
{
	return calcularRandom(1,2);
}

void Estacionamiento::quitarPosicionLibre(int numeroPosicion)
{
	int inicio = 0;
	int tam = this->vectorMemoriaPosicionesLibres.size();
	int fin = tam-1;

	int posicionBuscada = busquedaBinariaVectorLibres(inicio,fin,numeroPosicion);

	stringstream mensajeLog;
	mensajeLog << "Posicion buscada =  " << posicionBuscada << endl;
	Log::getInstance()->loguear(mensajeLog.str());

	this->vectorMemoriaPosicionesLibres.erase (this->vectorMemoriaPosicionesLibres.begin()+posicionBuscada);

	int a;
	mensajeLog << "Contenido array: ";

	for(a=0;a<tam-1;a++){
		mensajeLog << this->vectorMemoriaPosicionesLibres[a] << "-";
	}
	mensajeLog << endl;
	Log::getInstance()->loguear(mensajeLog.str());
}


int Estacionamiento::busquedaBinariaVectorLibres(int inicio,int fin,int buscado)
{
	if (inicio > fin) {
		stringstream mensajeLog;
		mensajeLog << "Buscado: " << buscado << " Inicio =  " << inicio << " Fin " << fin  <<endl;
		Log::getInstance()->loguear(mensajeLog.str());

		return -1;
	}
	else {
		  int pos = (inicio + fin) / 2;
		  if (buscado < this->vectorMemoriaPosicionesLibres[pos])
			 return(this->busquedaBinariaVectorLibres(inicio, pos - 1, buscado));
		  else if (buscado > this->vectorMemoriaPosicionesLibres[pos])
			 return(this->busquedaBinariaVectorLibres(pos + 1, fin, buscado));
		  else
			 return(pos);
	   	   }
}



void Estacionamiento::agregarPosicionLibre(int numeroPosicion)
{

	stringstream mensajeLog;

	this->vectorMemoriaPosicionesLibres.push_back(numeroPosicion);

	sort(this->vectorMemoriaPosicionesLibres.begin(), this->vectorMemoriaPosicionesLibres.end());

	int a;
	int tam = this->vectorMemoriaPosicionesLibres.size();
	mensajeLog << "Contenido array: ";

	for(a=0;a<tam;a++){
		mensajeLog << this->vectorMemoriaPosicionesLibres[a] << "-";
	}
	mensajeLog << endl;
	Log::getInstance()->loguear(mensajeLog.str());

}


int Estacionamiento::obtenerCantidadActualDeAutos()
{
	// Tomo el semaforo para lectura
	this->smfAdministracion.p();

	Administracion administracion = (Administracion)this->administracion.leer();

	// Libero el semaforo
	this->smfAdministracion.v();

	return administracion.getCantidadDeAutos();
}

float Estacionamiento::obtenerMontoRecaudado()
{

	// Tomo el semaforo para lectura
	this->smfAdministracion.p();

	cout << "PPAL: Acabo de tomar el semaforo de admin " << endl;
	Administracion administracion = (Administracion)this->administracion.leer();

	// Libero el semaforo
	this->smfAdministracion.v();
	cout << "PPAL: Libero el semaforo de admin " << endl;
	return administracion.getImporteRecaudado();
}


void Estacionamiento::liberarMemoriaCompartida(int cantidadLugares)
{
	int i;

	//Se libera la memoria compartida correspondiente a las posiciones en el estacionamiento.
	for (i=0;i<cantidadLugares;i++)
	{
		MemoriaCompartida<Posicion> memoriaPosicion;
		memoriaPosicion = this->vectorMemoriaPosiciones[i];
		memoriaPosicion.liberar();
	}

	//Se libera la memoria compartida correspondiente a la administracion.
	this->administracion.liberar();

	//Se eliminan los archivos temporales creados
	this->eliminarArchivosTemporales(cantidadLugares);

}

void Estacionamiento::finalizarProcesos(){
	int estado;
	waitpid(this->pConsola, &estado, 0);
	waitpid(this->pEntrada1, &estado, 0);
	waitpid(this->pEntrada2, &estado, 0);
	waitpid(this->pEntrada3, &estado, 0);
	waitpid(this->pSalida1, &estado,0);
	waitpid(this->pSalida2, &estado,0);
}

void Estacionamiento::finalizarPipes(){
	this->pipeEntrada1.cerrar();
	this->pipeEntrada2.cerrar();
	this->pipeEntrada3.cerrar();

	this->pipeSalida1.cerrar();
	this->pipeSalida2.cerrar();

	this->pipePpal.cerrar();
}


