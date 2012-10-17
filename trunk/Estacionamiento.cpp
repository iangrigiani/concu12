#include "Estacionamiento.h"



Estacionamiento::Estacionamiento() {
}


Estacionamiento::~Estacionamiento()
{
}


void Estacionamiento::crearArchivosTemporales(int cantidadLugares)
{

	int i;
	FILE * tmpFile;
	stringstream mensaje;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;

		// Creo archivo temporal posiciones
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;
		//cout << "nombre archivo: " << nombreArchivo.str() << endl;

		tmpFile = fopen(nombreArchivo.str().c_str(),"w");

		mensaje.str("");
		mensaje << "Creo el archivo temporal: " << nombreArchivo.str();
		Log::getInstance()->loguear(mensaje.str());
		fclose(tmpFile);


		nombreArchivo.flush();

	}

	// Creo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;

	tmpFile = fopen(archivoAdministracion.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

	// Creo archivos temporales para semaforos
	stringstream archivoSmfAdministracion;
	archivoSmfAdministracion << ARCHIVO_SEMAFORO_ADMINISTRACION;

	tmpFile = fopen(archivoSmfAdministracion.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoSmfAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

	stringstream archivoSmfPosiciones;
	archivoSmfPosiciones << ARCHIVO_SEMAFORO_POSICIONES;

	tmpFile = fopen(archivoSmfPosiciones.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoSmfPosiciones.str();
	Log::getInstance()->loguear(mensaje.str());
}

void Estacionamiento::eliminarArchivosTemporales(int cantidadLugares)
{
	int i;

	stringstream mensaje;
	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;

		// Destruyo archivo temporal posiciones
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;

		remove(nombreArchivo.str().c_str());
		mensaje.str("");
		mensaje << "Destruyo el archivo temporal: " << nombreArchivo.str();
		Log::getInstance()->loguear(mensaje.str());

		nombreArchivo.flush();

	}

	// Destruyo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;

	remove(archivoAdministracion.str().c_str());
	mensaje.str("");
	mensaje << "Destruyo el archivo temporal: " << archivoAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

	// Destruyo archivos temporales de semaforos
	stringstream archivoSmfAdministracion;
	archivoSmfAdministracion << ARCHIVO_SEMAFORO_ADMINISTRACION;

	remove(archivoSmfAdministracion.str().c_str());
	mensaje.str("");
	mensaje << "Destruyo el archivo temporal: " << archivoSmfAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

	stringstream archivoSmfPosiciones;
	archivoSmfPosiciones << ARCHIVO_SEMAFORO_POSICIONES;

	remove(archivoSmfPosiciones.str().c_str());
	mensaje.str("");
	mensaje << "Destruyo el archivo temporal: " << archivoSmfPosiciones.str();
	Log::getInstance()->loguear(mensaje.str());
}





bool Estacionamiento::crearMemoriaCompartidaPosiciones(int cantidadLugares)
{

	int i;
	Posicion posicion;
	MemoriaCompartida<Posicion> memoria;
	bool retorno = true;

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
			// Si hubo algun problema con la creacion de la memoria, se libera todo lo creado y se retorna false
			mensajeLog << "Error al inicializar el vector de memoria compartida en la posicion " << i << " en el estacionamiento";
			Log::getInstance()->loguear(mensajeLog.str());

			liberarMemoriaCompartidaPosiciones(i);
			retorno = false;
			break;
		}

		nombreArchivo.flush();
		mensajeLog.flush();
	}

	return retorno;
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

bool Estacionamiento::crearMemoriaCompartidaAdministracion(float costoHora)
{

	Administracion administracion;
	stringstream nombreArchivo;
	stringstream mensajeLog;

	bool pudoCrear = false;

	administracion.setCostoHora(costoHora);

	// Creo archivo temporal
	nombreArchivo << ARCHIVO_ADMINISTRACION;

	// Creo la memoria asociada al archivo temporal
	int estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	if(estadoMemoria == SHM_OK)
	{
		mensajeLog << "Memoria Compartida : escribo el costo hora del estacionamiento costo hora = " << administracion.getCostoHora();
		Log::getInstance()->loguear(mensajeLog.str());

		pudoCrear = true;
		this->administracion.escribir(administracion);
	}
	else
	{
		mensajeLog<<"Error al inicializar la administracion de memoria compartida en el estacionamiento";
		Log::getInstance()->loguear(mensajeLog.str());
	}

	return pudoCrear;
}




void Estacionamiento::run(int cantidadDeLugares, float costoHora, int tiempoEjecucion)
{
	bool creado = crearIniciales(cantidadDeLugares, costoHora);
	if ( creado ){

		// Genero un vector dinamico y lo inicializo para evitar los warning de compilador y de valgrind
		char recibido[BUFFSIZE];
		int q;

		for (q=0; q<BUFFSIZE; q++) {
			recibido[q] = 0;
		}

		//Inicializa el tiempo en el cronometro y arranca el tiempo a correr en la simulacion
		Cronometro::obtenerCronometro()->setTiempoASimular(tiempoEjecucion);
		Cronometro::obtenerCronometro()->iniciarTiempo();

		cout << "Inicio de la simulacion..." << endl;
		this->pConsola = fork();


		if( this->pConsola != 0)
		{
			Log::getInstance()->loguear("Soy el proceso principal");


			//Proceso padre
			this->pEntrada1 = fork();

			if( this->pEntrada1 == 0)
			{
				//Proceso entrada 1
				Log::getInstance()->loguear("Soy el proceso entrada 1");

				correrSimuladorEntrada(1,this->pipeEntrada1,cantidadDeLugares);
			}
			else
			{

				this->pEntrada2 = fork();

				if( this->pEntrada2 == 0)
				{
					//Proceso entrada 2
					Log::getInstance()->loguear("Soy el proceso entrada 2");

					correrSimuladorEntrada(2,this->pipeEntrada2,cantidadDeLugares);
				}
				else
				{

					this->pEntrada3 = fork();

					if( this->pEntrada3 == 0)
					{
						//Proceso entrada 3
						Log::getInstance()->loguear("Soy el proceso entrada 3");

						correrSimuladorEntrada(3,this->pipeEntrada3,cantidadDeLugares);
					}
					else
					{
						this->pSalida1 = fork();

						if (this->pSalida1 == 0)
						{
							//Proceso salida 1
							Log::getInstance()->loguear("Soy el proceso salida 1");

							correrSimuladorSalida(1,this->pipeSalida1,cantidadDeLugares);
						}
						else
						{
							this->pSalida2 = fork();

							if(this->pSalida2 == 0)
							{
								//Proceso salida 2
								Log::getInstance()->loguear("Soy el proceso salida 2");

								correrSimuladorSalida(2,this->pipeSalida2,cantidadDeLugares);

							}
							else
							{
								// Proceso principal (funciona como servidor de mensajes)
								Log::getInstance()->loguear("Soy el proceso principal funcionando como servidor de mensajes.");

								bool salir = false;
								int cantidadTotalEntradas = 0;

								while (!salir)
								{
									int bytes = this->pipePpal.leer((char*)recibido, BUFFSIZE);

									// Me aseguro que solo se procesen los bytes recibidos y no la posible basura existente en el array
									string stringRecibido(recibido);
									stringRecibido = stringRecibido.substr(0, bytes);

									string token;
									stringstream stream(stringRecibido);
									while( getline(stream, token, '?') ) {

										char * recibidoParcial = (char*)token.c_str();

										stringstream mensaje;
										mensaje << "ProcPrincipal - acabo de recibir: " << recibidoParcial;
										Log::getInstance()->loguear(mensaje.str());

										stringstream retorno;

										switch (recibidoParcial[0])
										{
											case 'a': {
												Log::getInstance()->loguear("Ingreso una 'a' por consola");

												int cantidad = this->obtenerCantidadActualDeAutos();
												retorno << "La cantidad actual de autos es : "<< cantidad;
												this->pipeConsola.escribir((char*)retorno.str().c_str(),retorno.str().length());
												break;
											}
											case 'm': {
												Log::getInstance()->loguear("Ingreso una 'm' por consola");

												float monto = this->obtenerMontoRecaudado();
												retorno << "El monto recaudado hasta el momento es : "<<monto;
												this->pipeConsola.escribir((char*)retorno.str().c_str(),retorno.str().length());
												break;
											}
											case 'q': {
												Log::getInstance()->loguear("Ingreso una 'q' por consola");

												salir = true;
												this->pipeConsola.cerrar();
												break;
											}
											case 'p': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");

												mensaje.str("");
												mensaje << "ProcPrincipal - Saco del vector de posiciones libres la posicion: " << token;
												Log::getInstance()->loguear(mensaje.str());

												int pos = atoi(token);
												this->quitarPosicionLibre(pos);
												break;
											}
											case 'd': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");

												mensaje.str("");
												mensaje << "ProcPrincipal - Agrego al  vector de posiciones libres la posicion: " << token;
												Log::getInstance()->loguear(mensaje.str());

												int pos = atoi(token);
												this->agregarPosicionLibre(pos);
												break;
											}
											case 's': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");
												int pos = atoi(token);
												int numeroSalida = this->getSalidaAleatoria();

												mensaje.str("");
												mensaje << "ProcPrincipal - Auto sale del estacionamiento por la salida " << numeroSalida << " y deja la posicion " << pos;
												Log::getInstance()->loguear(mensaje.str());

												retorno << "s|" << pos;
												if(numeroSalida == 1){
													this->pipeSalida1.escribir((char*)retorno.str().c_str(),retorno.str().length());
												} else {
													this->pipeSalida2.escribir((char*)retorno.str().c_str(),retorno.str().length());
												}
												break;
											}
											case 'f': {
													char * token = strtok(recibidoParcial, "|");
													token = strtok(NULL, "|");
													int numeroEntrada = atoi(token);

													cantidadTotalEntradas+=numeroEntrada;

													mensaje.str("");
													mensaje << "ProcPrincipal - La suma del numero de entradas total es " << cantidadTotalEntradas;
													Log::getInstance()->loguear(mensaje.str());

													if(cantidadTotalEntradas == CANTIDAD_TOTAL_ENTRADAS)
													{
														retorno << "f";
														this->pipeSalida1.escribir((char*)retorno.str().c_str(),retorno.str().length());
														this->pipeSalida2.escribir((char*)retorno.str().c_str(),retorno.str().length());

														cout << "Fin de la simulacion, ingrese 'q' para terminar el programa" << endl;
													}
													break;
											}
											default: {
												if ( recibidoParcial[0] > '0' && recibidoParcial[0] < '4') {
													int nroPosicion = this->getPosicionAleatoria();

													mensaje.str("");
													mensaje << "ProcPrincipal - Entrada " << recibidoParcial[0] << " pide una posicion, le envio: " << nroPosicion;
													Log::getInstance()->loguear(mensaje.str());

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
													cout << "Comando invalido! Ingrese 'a' para ver la cantidad de autos estacionados, ";
													cout << "'m' para conocer el monto recaudado ";
													cout << " o 'q' para finalizar el programa" << endl;
												}
												break;
											}
										}
									}
								}

								finalizarProcesos();

								liberarRecursos(cantidadDeLugares);

								finalizarPipes();
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
				cout << "Ingrese un comando: ( 'a' para averiguar la cantidad de autos estacionados o 'm' el monto recaudado )" << endl;
				cin.getline(recibido, BUFFSIZE);

				stringstream ssRecibido;
				char comando = tolower(recibido[0]);
				ssRecibido << comando;

				// Valido las opciones de entrada
				if (comando == 'a' || comando == 'm' || comando == 'q' ) {

					ssRecibido << "?";
					// Envio solo 2 bytes
					this->pipePpal.escribir((char*)ssRecibido.str().c_str(),2);

					if (comando != 'q') {

						char recibir[BUFFSIZE];
						int bytes = this->pipeConsola.leer(recibir, BUFFSIZE);
						recibir[bytes] = '\0';

						cout << recibir << endl;

					} else {
						salir = true;
					}
				} else {
					cout << "Comando invalido." << endl;
				}

			}

			this->pipeConsola.cerrar();
			this->pipePpal.cerrar();

			mensajeLog.str("");
			mensajeLog << "Consola - Termina el proceso: " << getpid() << " (consola)";
			Log::getInstance()->loguear(mensajeLog.str());
		}

		Cronometro::destruir();

	} else {
		cout << "Se ha producido un error. Revise el archivo de log para mas informacion" << endl;
	}
}



bool Estacionamiento::crearIniciales(int cantidadDeLugares, int costoHora){
	crearArchivosTemporales(cantidadDeLugares);

	Semaforo smfAdmin(ARCHIVO_SEMAFORO_ADMINISTRACION,1,'a');
	this->smfAdministracion = smfAdmin;

	stringstream mensajeLog;
	mensajeLog << "Estacionamiento - Creo semaforo id: " << this->smfAdministracion.getId() << " (admin)";
	Log::getInstance()->loguear(mensajeLog.str());

	bool pudoCrear = false;
	if ( crearMemoriaCompartidaAdministracion(costoHora) ){

		if ( crearMemoriaCompartidaPosiciones(cantidadDeLugares) ) {

			crearVectorPosicionesLibres(cantidadDeLugares);
			pudoCrear = true;
		} else {
			liberarMemoriaCompartidaAdministracion();

			// Elimino el semaforo de la administracion
			mensajeLog.str("");
			mensajeLog << "ProcPrincipal - Libero semaforo id: " << this->smfAdministracion.getId() << " (admin)";
			Log::getInstance()->loguear(mensajeLog.str());
			this->smfAdministracion.eliminar();
		}
	}

	return pudoCrear;
}


void Estacionamiento::correrSimuladorEntrada(int numeroEntrada,Pipe pipeEntrada, int cantidadPosiciones)
{
	SimuladorEntrada simuladorEntrada(numeroEntrada,cantidadPosiciones);

	simuladorEntrada.simular(pipeEntrada, this->pipePpal);

	pipeEntrada.cerrar();
}


void Estacionamiento::correrSimuladorSalida(int numeroSalida,Pipe pipeSalida, int cantidadPosiciones)
{
	SimuladorSalida simuladorSalida(numeroSalida,cantidadPosiciones);

	simuladorSalida.simular(pipeSalida, this->pipePpal);

	pipeSalida.cerrar();
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
	stringstream mensajeLog;

	int posicionBuscada = busquedaBinariaVectorLibres(inicio,fin,numeroPosicion);

	mensajeLog << "ProcPrincipal - Indice buscado =  " << posicionBuscada;
	Log::getInstance()->loguear(mensajeLog.str());

	this->vectorMemoriaPosicionesLibres.erase (this->vectorMemoriaPosicionesLibres.begin()+posicionBuscada);

	int a;

	mensajeLog.str("");
	mensajeLog << "ProcPrincipal - Contenido del array de posiciones libres: ";

	for(a=0;a<tam-1;a++){
		mensajeLog << this->vectorMemoriaPosicionesLibres[a] << " ";
	}
	Log::getInstance()->loguear(mensajeLog.str());
}


int Estacionamiento::busquedaBinariaVectorLibres(int inicio,int fin,int buscado)
{
	if (inicio > fin) {
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

	mensajeLog << "ProcPrincipal - Agrego posicion " << numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	this->vectorMemoriaPosicionesLibres.push_back(numeroPosicion);

	sort(this->vectorMemoriaPosicionesLibres.begin(), this->vectorMemoriaPosicionesLibres.end());

	int a;
	int tam = this->vectorMemoriaPosicionesLibres.size();

	mensajeLog.str("");
	mensajeLog << "ProcPrincipal - Contenido del array de posiciones libres: ";

	for(a=0;a<tam;a++){
		mensajeLog << this->vectorMemoriaPosicionesLibres[a] << " ";
	}
	Log::getInstance()->loguear(mensajeLog.str());

}


int Estacionamiento::obtenerCantidadActualDeAutos()
{
	stringstream mensajeLog;

	// Tomo el semaforo para lectura
	this->smfAdministracion.p();

	mensajeLog << "ProcPrincipal - Tome semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	Administracion administracion = (Administracion)this->administracion.leer();

	mensajeLog.str("");
	mensajeLog << "ProcPrincipal - Libero semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();

	return administracion.getCantidadDeAutos();
}

float Estacionamiento::obtenerMontoRecaudado()
{
	stringstream mensajeLog;
	// Tomo el semaforo para lectura
	this->smfAdministracion.p();

	mensajeLog << "ProcPrincipal - Tome semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	//cout << "PPAL: Acabo de tomar el semaforo de admin " << endl;
	Administracion administracion = (Administracion)this->administracion.leer();

	mensajeLog.str("");
	mensajeLog << "ProcPrincipal - Libero semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();
	//cout << "PPAL: Libero el semaforo de admin " << endl;
	return administracion.getImporteRecaudado();
}

void Estacionamiento::liberarRecursos(int cantidadLugares){

	// Libero memoria compartida de posiciones
	liberarMemoriaCompartidaPosiciones(cantidadLugares);

	// Libero memoria compartida de administracion
	liberarMemoriaCompartidaAdministracion();

	//Se eliminan los archivos temporales creados
	eliminarArchivosTemporales(cantidadLugares);

	// Elimino el semaforo de la administracion
	stringstream mensajeLog;
	mensajeLog << "ProcPrincipal - Libero semaforo id: " << this->smfAdministracion.getId() << " (admin)";
	Log::getInstance()->loguear(mensajeLog.str());
	this->smfAdministracion.eliminar();
}

void Estacionamiento::liberarMemoriaCompartidaPosiciones(int cantidadLugares)
{
	int i;

	//Se libera la memoria compartida correspondiente a las posiciones en el estacionamiento.
	for (i=0;i<cantidadLugares;i++)
	{
		MemoriaCompartida<Posicion> memoriaPosicion;
		memoriaPosicion = this->vectorMemoriaPosiciones[i];
		memoriaPosicion.liberar();
	}
}

void Estacionamiento::liberarMemoriaCompartidaAdministracion(){
	//Se libera la memoria compartida correspondiente a la administracion.
	this->administracion.liberar();
}

void Estacionamiento::finalizarProcesos(){
	stringstream mensajeFinalizacion;
	int estado;

	waitpid(this->pEntrada1, &estado, 0);
	waitpid(this->pEntrada2, &estado, 0);
	waitpid(this->pEntrada3, &estado, 0);
	waitpid(this->pSalida1, &estado,0);
	waitpid(this->pSalida2, &estado,0);
	waitpid(this->pConsola, &estado, 0);

	mensajeFinalizacion << "ProcPrincipal - Terminado proceso: " << pConsola << " (consola)";
	Log::getInstance()->loguear(mensajeFinalizacion.str());

	mensajeFinalizacion.str("");
	mensajeFinalizacion << "ProcPrincipal - FIN SIMULACION";
	Log::getInstance()->loguear(mensajeFinalizacion.str());
}

void Estacionamiento::finalizarPipes(){
	this->pipeEntrada1.cerrar();
	this->pipeEntrada2.cerrar();
	this->pipeEntrada3.cerrar();

	this->pipeSalida1.cerrar();
	this->pipeSalida2.cerrar();

	this->pipePpal.cerrar();
}


