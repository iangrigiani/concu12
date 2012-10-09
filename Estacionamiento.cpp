#include "Estacionamiento.h"



Estacionamiento::Estacionamiento() {
}


Estacionamiento::~Estacionamiento()
{

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
	//cout << "nombre archivo adminstracion: " << archivoAdministracion.str() << endl;
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
		//cout << "destruccion archivo: " << nombreArchivo.str() << endl;

		remove(nombreArchivo.str().c_str());

		remove(nombreArchivoLibres.str().c_str());


		nombreArchivo.flush();

	}

	// Destruyo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;
	//cout << "destruccion archivo adminstracion: " << archivoAdministracion.str() << endl;
	remove(archivoAdministracion.str().c_str());
}



void Estacionamiento::crearMemoriaCompartidaPosiciones(int cantidadLugares)
{

	int i;
	int estadoMemoria = SHM_OK;
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
		estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		mensajeLog << "Memoria Compartida : escribo el numero " << i <<" en el vector de posiciones.";

		Log::getInstance()->loguear(mensajeLog.str());

		posicion.setNumero(i);

		memoria.escribir(posicion);

		this->vectorMemoriaPosiciones.push_back(memoria);

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

void Estacionamiento::crearMemoriaCompartidaAdministracion(int costoHora)
{

	Administracion administracion;
	stringstream nombreArchivo;
	stringstream mensajeLog;
	int estadoMemoria = SHM_OK;

	administracion.setCostoHora(costoHora);


	// Creo archivo temporal
	nombreArchivo << ARCHIVO_ADMINISTRACION;
	//cout << "nombre archivo adminstracion: " << nombreArchivo.str() << endl;

	// Creo la memoria asociada al archivo temporal
	estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );


	mensajeLog << "Memoria Compartida : escribo el costo hora del estacionamiento costo hora = " << administracion.getCostoHora();

	Log::getInstance()->loguear(mensajeLog.str());

	this->administracion.escribir(administracion);

}




void Estacionamiento::run(int cantidadDeLugares, int costoHora, int tiempoEjecucion)
{
		// Pipe para la comunicacion de todas las entradas y la consola con el proceso principal. Todos escriben
		// ahi y el proceso principal solo lee

		// Pipe para responder las consultas de la consola

		pid_t pConsola = fork();

		pid_t pEntrada1;
		pid_t pEntrada2;
		pid_t pEntrada3;

		//	pid_t pSalida1;
		//	pid_t pSalida2;

		if( pConsola != 0)
		{

			stringstream mensajeLog;

			mensajeLog << "Soy el proceso principal";

			Log::getInstance()->loguear(mensajeLog.str());

			this->crearArchivosTemporales(cantidadDeLugares);

			this->crearMemoriaCompartidaPosiciones(cantidadDeLugares);

			this->crearMemoriaCompartidaAdministracion(costoHora);

			this->crearVectorPosicionesLibres(cantidadDeLugares);


			//Proceso padre
			pEntrada1 = fork();

			if( pEntrada1 == 0)
			{
				//Proceso entrada 1
				stringstream mensajeLog;

				mensajeLog << "Soy el proceso entrada 1";

				Log::getInstance()->loguear(mensajeLog.str());

				correrSimulador(1, tiempoEjecucion, this->pipeEntrada1,cantidadDeLugares);
			}
			else
			{

				pEntrada2 = fork();

				if( pEntrada2 == 0)
				{
					//Proceso entrada 2
					stringstream mensajeLog;
					mensajeLog << "Soy el proceso entrada 2";
					Log::getInstance()->loguear(mensajeLog.str());

					correrSimulador(2, tiempoEjecucion, this->pipeEntrada2,cantidadDeLugares);
				}
				else
				{

					pEntrada3 = fork();

					if( pEntrada3 == 0)
					{
						//Proceso entrada 3
						stringstream mensajeLog;
						mensajeLog << "Soy el proceso entrada 3";
						Log::getInstance()->loguear(mensajeLog.str());

						correrSimulador(3, tiempoEjecucion, this->pipeEntrada3,cantidadDeLugares);
					}
					else {
						// Proceso principal (funciona como servidor de mensajes)

						stringstream mensajeLog;
						mensajeLog << "Soy el proceso principal funcionando como servidor de mensajes.";
						Log::getInstance()->loguear(mensajeLog.str());

						bool salir = false;
						char recibido[BUFFSIZE];
						while (!salir)
						{
							int bytes = this->pipePpal.leer(recibido, BUFFSIZE);
							recibido[bytes] = '\0';

							cout << "ProcPrincipal: acabo de recibir: " << recibido << endl;

							stringstream retorno;

							switch (recibido[0])
							{
								case 'a': {
									cout << "Ingreso una a" << endl;
									int cantidad = this->obtenerCantidadActualDeAutos();
									retorno << "La cantidad actual de autos es : "<< cantidad;
									this->pipeConsola.escribir((char*)retorno.str().c_str(),BUFFSIZE);
									break;
								}
								case 'm': {
									cout << "Ingreso una m" << endl;
									float monto = this->obtenerMontoRecaudado();
									retorno << "El monto recaudado hasta el momento es : "<<monto;
									this->pipeConsola.escribir((char*)retorno.str().c_str(),BUFFSIZE);
									break;
								}
								case 'q': {
									cout << "Ingreso un q" << endl;
									salir = true;
									this->pipeConsola.cerrar();
									break;
								}
								case 's': {
									char * token = strtok(recibido, "|");
									token = strtok(NULL, "|");
									cout << "Saco del vector de posiciones libres la posicion: " << token << endl;
									int pos = atoi(token);
									this->quitarPosicionLibre(pos);
									break;
								}
								default: {
									if ( recibido[0] > '0' && recibido[0] < '4') {
										int nroPosicion;
										nroPosicion = this->getPosicionAleatoria();
										cout << "Entrada " << recibido[0] << " pide una posicion, le envio: " << nroPosicion << endl;
										retorno << nroPosicion;
										if (recibido[0] == '1'){
											this->pipeEntrada1.escribir((char*)retorno.str().c_str(),BUFFSIZE);
										} else {
											if (recibido[0] == '2') {
												this->pipeEntrada2.escribir((char*)retorno.str().c_str(),BUFFSIZE);
											} else {
												this->pipeEntrada3.escribir((char*)retorno.str().c_str(),BUFFSIZE);
											}
										}
									} else {
										cout << "Comando invalido" << endl;
									}
									break;
								}
							}
						}

						int estado;
						cout << endl << "Acordate de ingresar Q por teclado pq sino esto no muere mas!! " << endl;
						cout << "Pconsola es...: " << pConsola << endl;
						waitpid(pConsola, &estado, 0);
						waitpid(pEntrada1, &estado, 0);
						waitpid(pEntrada2, &estado, 0);
						waitpid(pEntrada3, &estado, 0);

						this->liberarMemoriaCompartida(cantidadDeLugares);

						this->pipeEntrada1.cerrar();
						this->pipeEntrada2.cerrar();
						this->pipeEntrada3.cerrar();

						pipePpal.cerrar();
						cout << "Terminado proceso..." << pConsola << endl;
						cout << "FIN SIMULACION" << endl;
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

			char entradaConsola[BUFFSIZE];

			bool salir = false;
			// Para terminar el programa, el usuario debe escribir 'Q' o 'q'
			while (!salir) {
				entradaConsola[0] = '\0';
				cout << "ProcConsola: Me quedo esperando que ingreses algo por consola..." << endl;
				cin.getline(entradaConsola, BUFFSIZE);

				// Aca hay que agregar la validacion para que solo pueda ingresar una letra para hacer consultas
				// o para salir
				this->pipePpal.escribir(entradaConsola,BUFFSIZE);

				if (entradaConsola[0] != 'q' && entradaConsola[0] != 'Q') {

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
			return;
		}
}




void Estacionamiento::correrSimulador(int numeroEntrada, int tiempoEjecucion, Pipe pipeEntrada, int cantidadPosiciones)
{
	Simulador * simulador;

	simulador = new Simulador(numeroEntrada,cantidadPosiciones);

	simulador->getCronometro()->setTiempoASimular(tiempoEjecucion);
	simulador->setPipePrincipal(this->pipePpal);
	simulador->setPipeEntrada(pipeEntrada);
	simulador->simular();

	pipeEntrada.cerrar();
	delete simulador;
}


int Estacionamiento::getPosicionAleatoria()
{
	int cantidadPosiciones = this->vectorMemoriaPosicionesLibres.size();

	//Si la lista de posiciones libres esta vacia, entonces debe retornar un numero
	//negativo de manera que no deje entrar a ningun auto.
	if(cantidadPosiciones == 0)
		return -1;

	int numeroElegido;
	int indiceActual = 0;
	int posicionElegida = 0;

	srand((unsigned)time(0));
	numeroElegido = calcularRandom(cantidadPosiciones);

	vector<int>::iterator it = this->vectorMemoriaPosicionesLibres.begin();

	 while ( it != this->vectorMemoriaPosicionesLibres.end())
	 {
		 if(indiceActual == numeroElegido)
		 {
			 posicionElegida = this->vectorMemoriaPosicionesLibres[indiceActual];
			 break;
		 }

		 ++it;
		 indiceActual++;
	 }

	 return posicionElegida;

}

void Estacionamiento::quitarPosicionLibre(int numeroPosicion)
{
	int inicio = 0;
	int fin = this->vectorMemoriaPosicionesLibres.size()-1;

	int posicionBuscado = busquedaBinariaVectorLibres(inicio,fin,numeroPosicion);

	//cout<<"La posicion buscada y a eliminar es "<<posicionBuscado<<endl;

	this->vectorMemoriaPosicionesLibres.erase (this->vectorMemoriaPosicionesLibres.begin()+posicionBuscado);

	/*int tamanio = this->vectorMemoriaPosicionesLibres.size();

	for(int i=0; i<tamanio;i++)
	{
		int pos = this->vectorMemoriaPosicionesLibres[i];
		cout<<"El vector tiene la pos "<< pos <<endl;

	}*/


}


int Estacionamiento::busquedaBinariaVectorLibres(int inicio,int fin,int buscado)
{
	if (inicio > fin)
		return -1;
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
	this->vectorMemoriaPosicionesLibres.push_back(numeroPosicion);

}


int Estacionamiento::obtenerCantidadActualDeAutos()
{
	Administracion administracion = (Administracion)this->administracion.leer();
	return administracion.getCantidadDeAutos();
}

float Estacionamiento::obtenerMontoRecaudado()
{
	Administracion administracion = (Administracion)this->administracion.leer();
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


