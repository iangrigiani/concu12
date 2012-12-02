#include "AdministradorGeneral.h"

AdministradorGeneral::AdministradorGeneral() {
	this->totalSumatoriaEstacionamientos = 0;
}

AdministradorGeneral::~AdministradorGeneral() {

}

void AdministradorGeneral::run(int cantidadDeLugares, float costoHora, int tiempoEjecucion,int cantidadEstacionamientos)
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

	cout << "Inicio de la simulacion..." << endl;


	//Crea las memorias compartidas asociadas a cada uno de los estacionamientos y los
	//vectores de posiciones libres
	this->inicializaciones(cantidadEstacionamientos,cantidadDeLugares);

	Cola<mensaje> colaMensajes( (char*)"valgrind.out",'a');

	// Creo otra cola para las consultas, cada estacionamiento va a tener un proceso escuchando esta cola
	//Cola<mensaje> colaConsultas( (char*)"valgrind.out",'a');

	// cantidadEstacionamientos+1 es el valor que va a filtrar el proceso ppal para recibir mensajes
	int msjPpal = cantidadEstacionamientos+1;

	this->pConsola = fork();

	if( this->pConsola != 0)
	{


		Log::getInstance()->loguear("Soy el proceso principal - el Administrador General");
		for(int i=1;i<=cantidadEstacionamientos;i++)
		{
			this->totalSumatoriaEstacionamientos+=i;
			pid_t pEstacionamiento = fork();

			//TODO ver si tenemos una lista de pipes, uno por cada estacionamiento que se
			//crea para comunicarse con el administrador general

			if(pEstacionamiento == 0)
			{
				Estacionamiento estacionamiento(i, msjPpal);
				estacionamiento.run(cantidadDeLugares,costoHora,tiempoEjecucion);
				Cronometro::destruir();
				return;
			}
		}
		//Soy el proceso administrador general

		// Variable para llevar el control del fin de la simulacion
		int sumatoriaEstacionamientosCerrados = 0;
		bool salir = false;

		while (!salir)
		{
			mensaje datoLeido;
			mensaje datoEnviado;
			colaMensajes.leer(msjPpal, &datoLeido);


			datoEnviado.from = msjPpal;
			datoEnviado.mtype = datoLeido.from;

			stringstream mensaje;
			mensaje << "ProcEstacionamiento - acabo de recibir: " << datoLeido.msj;
			Log::getInstance()->loguear(mensaje.str());

			stringstream retorno;

			switch (datoLeido.msj[0])
			{

				//TODO HAY QUE MANEJAR EL SERVIDOR DE MENSAJES DE CONSOLA EN EL PRINCIPAL DE TODOS

				case 'a': {
					Log::getInstance()->loguear("Ingreso una 'a' por consola");

					// Aca tiene que mandar el msj a la cola de consultas
					// colaConsultas.escribir(..);
					//int cantidad = this->obtenerCantidadActualDeAutos();
					retorno << "La cantidad actual de autos es : ";

					strcpy(datoEnviado.msj, retorno.str().c_str());
					colaMensajes.escribir(datoEnviado);
					break;
				}
				case 'm': {
					Log::getInstance()->loguear("Ingreso una 'm' por consola");

					// Aca tiene que mandar el msj a la cola de consultas
					// colaConsultas.escribir(..);
					//float monto = this->obtenerMontoRecaudado();
					retorno << "El monto recaudado hasta el momento es : ";

					strcpy(datoEnviado.msj, retorno.str().c_str());
					colaMensajes.escribir(datoEnviado);

					break;
				}
				case 'q': {
					Log::getInstance()->loguear("Ingreso una 'q' por consola");

					salir = true;

					break;
				}
				case 'p': {
					char * token = strtok(datoLeido.msj, "|");
					token = strtok(NULL, "|");

					mensaje.str("");
					mensaje << "ProcPrincipal - Saco del vector de posiciones libres la posicion: " << token;
					Log::getInstance()->loguear(mensaje.str());

					int pos = atoi(token);

					// Elimino la posicion libre del estacionamiento correspondiente
					this->lugares[datoLeido.from-1].quitarPosicionLibre(pos);

					break;
				}
				case 'd': {
					char * token = strtok(datoLeido.msj, "|");
					token = strtok(NULL, "|");

					mensaje.str("");
					mensaje << "ProcPrincipal - Agrego al  vector de posiciones libres la posicion: " << token;
					Log::getInstance()->loguear(mensaje.str());

					int pos = atoi(token);

					// Agrego la posicion libre al estacionamiento correspondiente
					this->lugares[datoLeido.from-1].agregarPosicionLibre(pos);
					//this->agregarPosicionLibre(pos);

					break;
				}
				case 'f': {

					sumatoriaEstacionamientosCerrados+=datoLeido.from;

					mensaje.str("");
					mensaje << "ProcPrincipal - La suma del numero de estacionamientos cerrados es " << sumatoriaEstacionamientosCerrados;
					Log::getInstance()->loguear(mensaje.str());

					if(sumatoriaEstacionamientosCerrados == this->totalSumatoriaEstacionamientos)
					{
						cout << "Fin de la simulacion, ingrese 'q' para terminar el programa" << endl;
					}

					break;
				}
				case 'r': {

					int nroPosicion = this->lugares[datoLeido.from-1].getPosicionAleatoria();

					retorno << nroPosicion;
					strcpy(datoEnviado.msj,retorno.str().c_str());


					mensaje.str("");
					mensaje << "ProcPrincipal - Estacionamiento " << datoLeido.from << " pide una posicion, le envio: " << nroPosicion;
					Log::getInstance()->loguear(mensaje.str());

					colaMensajes.escribir(datoEnviado);

					break;
				}
				default: {

					cout << "Comando invalido! Ingrese 'a' para ver la cantidad de autos estacionados, ";
					cout << "'m' para conocer el monto recaudado ";
					cout << " o 'q' para finalizar el programa" << endl;
					break;
				}
			}
		}


		//TODO finalizar procesos, debe esperar a que finalicen los procesos de los estacionamientos
		int estado;
		for(int i=0;i<cantidadEstacionamientos;i++)
		{
			cout << "Liberado el estacionamiento... " << i << endl;
			wait(&estado);
		}
		waitpid(this->pConsola, &estado,0);

		liberarRecursos(cantidadEstacionamientos,cantidadDeLugares);
		colaMensajes.destruir();
	}
	else
	{

		// cantidadEstacionamientos+2 es el mtype para la consola
		int msjConsola = cantidadEstacionamientos+2;

		// Proceso consola
		//TODO CAMBIAR EL INGRESO DE POSIBLES COMANDOS EN EL PROCESO CONSOLA
		stringstream mensajeLog;
		mensajeLog << "Soy el proceso consola esperando el ingreso por pantalla.";
		Log::getInstance()->loguear(mensajeLog.str());


		Cola<mensaje> cola( (char*)"valgrind.out",'a');
		mensaje datoEnviar;
		mensaje datoLeer;

		datoEnviar.mtype = msjPpal;
		datoEnviar.from = msjConsola;

		bool salir = false;
		// Para terminar el programa, el usuario debe escribir 'Q' o 'q'
		while (!salir) {


			cout << "Ingrese un comando: ( 'a' para averiguar la cantidad de autos estacionados o 'm' el monto recaudado )" << endl;
			cin.getline(recibido, BUFFSIZE);

			char * primerParametro = strtok(recibido, " ");


			stringstream ssRecibido;
			char letra = tolower(primerParametro[0]);
			ssRecibido << letra << "|";

			// Valido las opciones de entrada
			if (letra == 'a' || letra == 'm' || letra == 'q' ) {

				char * segundoParametro = strtok(NULL, " ");
				int nroEstacionamiento = 0;
				if (segundoParametro!=NULL){
					nroEstacionamiento = atoi((char*)segundoParametro);
					ssRecibido << nroEstacionamiento;
				}

				ssRecibido << "?";
				strcpy(datoEnviar.msj,ssRecibido.str().c_str());

				cola.escribir(datoEnviar);

				if (letra != 'q') {

					// agregar validacion de nro de estacionamiento
					cola.leer(msjConsola,&datoLeer);

					cout << "me contesto: " << datoLeer.msj << endl;

					//cout << recibir << endl;

				} else {
					salir = true;
				}
			} else {
				cout << "Comando invalido." << endl;
			}

		}

		mensajeLog.str("");
		mensajeLog << "Consola - Termina el proceso: " << getpid() << " (consola)";
		Log::getInstance()->loguear(mensajeLog.str());

	}

	Cronometro::destruir();
}


void AdministradorGeneral::inicializaciones(int cantidadEstacionamientos,int cantidadDeLugares)
{

	for(int i = 1; i<=cantidadEstacionamientos; i++)
	{
		Lugares lugar(i);

		crearArchivosTemporales(cantidadDeLugares,i);

		lugar.crearMemoriaCompartidaPosiciones(cantidadDeLugares,i);

		lugar.crearVectorPosicionesLibres(cantidadDeLugares);

		this->lugares.push_back(lugar);
	}
}


void AdministradorGeneral::crearArchivosTemporales(int cantidadLugares, int numeroEstacionamiento)
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
		nombreArchivo<<ESTACIONAMIENTO;
		nombreArchivo<<numeroEstacionamiento;

		tmpFile = fopen(nombreArchivo.str().c_str(),"w");

		mensaje.str("");
		mensaje << "Creo el archivo temporal: " << nombreArchivo.str();
		Log::getInstance()->loguear(mensaje.str());
		fclose(tmpFile);

		nombreArchivo.flush();

	}

	stringstream archivoSmfPosiciones;
	archivoSmfPosiciones << ARCHIVO_SEMAFORO_POSICIONES;
	archivoSmfPosiciones<<ESTACIONAMIENTO;
	archivoSmfPosiciones<<numeroEstacionamiento;

	tmpFile = fopen(archivoSmfPosiciones.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoSmfPosiciones.str();
	Log::getInstance()->loguear(mensaje.str());

}

void AdministradorGeneral::liberarMemoriaCompartidaEstacionamiento(int numeroEstacionamiento,int cantidadLugares)
{
	Lugares lugar = this->lugares[numeroEstacionamiento-1];
	lugar.liberarMemoriaCompartidaPosiciones(cantidadLugares);

}

void AdministradorGeneral::eliminarArchivosTemporales(int numeroEstacionamiento,int cantidadLugares)
{
	stringstream mensaje;
	int i;
	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;

		// Destruyo archivo temporal posiciones
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;
		nombreArchivo<<ESTACIONAMIENTO;
		nombreArchivo<<numeroEstacionamiento;

		remove(nombreArchivo.str().c_str());
		mensaje.str("");
		mensaje << "Destruyo el archivo temporal: " << nombreArchivo.str();
		Log::getInstance()->loguear(mensaje.str());

		nombreArchivo.flush();
	}

	stringstream archivoSmfPosiciones;
	archivoSmfPosiciones << ARCHIVO_SEMAFORO_POSICIONES;
	archivoSmfPosiciones<<ESTACIONAMIENTO;
	archivoSmfPosiciones<<numeroEstacionamiento;

	remove(archivoSmfPosiciones.str().c_str());
	mensaje.str("");
	mensaje << "Destruyo el archivo temporal: " << archivoSmfPosiciones.str();
	Log::getInstance()->loguear(mensaje.str());

}


void AdministradorGeneral::liberarRecursos(int cantidadEstacionamientos,int cantidadLugares)
{
	int i;

	for(i = 1; i<=cantidadEstacionamientos;i++)
	{
		// Libero memoria compartida de posiciones
		liberarMemoriaCompartidaEstacionamiento(i,cantidadLugares);

		//Se eliminan los archivos temporales creados
		eliminarArchivosTemporales(i, cantidadLugares);
	}

}

