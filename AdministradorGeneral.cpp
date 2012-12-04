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

	stringstream mensajeLog;


	//Inicializa el tiempo en el cronometro y arranca el tiempo a correr en la simulacion
	Cronometro::obtenerCronometro()->setTiempoASimular(tiempoEjecucion);
	Cronometro::obtenerCronometro()->iniciarTiempo();

	cout << "Inicio de la simulacion..." << endl;

	//Crea las memorias compartidas asociadas a cada uno de los estacionamientos y los
	//vectores de posiciones libres
	this->inicializaciones(cantidadEstacionamientos,cantidadDeLugares);

	Cola<mensaje> colaMensajes( (char*)ARCHIVO_COLA_MENSAJES,'a');

	// Creo otra cola para las consultas, cada estacionamiento va a tener un proceso escuchando esta cola
	Cola<mensaje> colaConsultas( (char*)ARCHIVO_COLA_MENSAJES_CONSULTAS,'a');

	// cantidadEstacionamientos+1 es el valor que va a filtrar el proceso ppal para recibir mensajes
	int msjPpal = cantidadEstacionamientos+1;

	this->pConsola = fork();

	if( this->pConsola != 0)
	{

		// Guardo los pids de los estacionamientos
		pid_t pEstacionamientos[cantidadEstacionamientos];

		Log::getInstance()->loguear("Soy el proceso principal - el Administrador General");
		for(int i=1;i<=cantidadEstacionamientos;i++)
		{
			this->totalSumatoriaEstacionamientos+=i;
			pid_t pEstacionamiento = fork();
			pEstacionamientos[i-1] = pEstacionamiento;

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

			mensajeLog.str("");
			mensajeLog << "ProcPrincipal - acabo de recibir: " << datoLeido.msj;
			Log::getInstance()->loguear(mensajeLog.str());

			stringstream retorno;

			switch (datoLeido.msj[0])
			{

				case 'a': {
					Log::getInstance()->loguear("Ingreso una 'a' por consola");

					int fromOriginal = datoLeido.from;

					char * token = strtok(datoLeido.msj, "|");
					token = strtok(NULL, "|");

					int nroEstacionamiento = atoi(token);
					datoEnviado.mtype = nroEstacionamiento;

					stringstream consulta;
					consulta << "a";
					strcpy(datoEnviado.msj,(char*)consulta.str().c_str());

					// Aca tiene que mandar el msj a la cola de consultas
					colaConsultas.escribir(datoEnviado);

					colaConsultas.leer(msjPpal,&datoLeido);

					datoEnviado.mtype = fromOriginal;
					datoEnviado.from = msjPpal;

					retorno << "La cantidad actual de autos en el estacionamiento " << nroEstacionamiento << " es : " << datoLeido.msj;

					strcpy(datoEnviado.msj, retorno.str().c_str());
					colaMensajes.escribir(datoEnviado);
					break;
				}
				case 'm': {
					Log::getInstance()->loguear("Ingreso una 'm' por consola");

					int fromOriginal = datoLeido.from;

					char * token = strtok(datoLeido.msj, "|");
					token = strtok(NULL, "|");

					int nroEstacionamiento = atoi(token);
					datoEnviado.mtype = nroEstacionamiento;

					stringstream consulta;
					consulta << "m";
					strcpy(datoEnviado.msj,(char*)consulta.str().c_str());

					// Aca tiene que mandar el msj a la cola de consultas
					colaConsultas.escribir(datoEnviado);

					colaConsultas.leer(msjPpal,&datoLeido);

					datoEnviado.mtype = fromOriginal;
					datoEnviado.from = msjPpal;

					retorno << "El monto recaudado hasta el momento en el estacionamiento " << nroEstacionamiento << " es : " << datoLeido.msj;

					strcpy(datoEnviado.msj, retorno.str().c_str());
					colaMensajes.escribir(datoEnviado);
					break;
				}
				case 'q': {
					Log::getInstance()->loguear("Ingreso una 'q' por consola");

					retorno << "q";

					for(int i=1;i<=cantidadEstacionamientos;i++)
					{
						datoEnviado.mtype = i;
						strcpy(datoEnviado.msj,(char*)retorno.str().c_str());

						colaConsultas.escribir(datoEnviado);
					}

					salir = true;

					break;
				}
				case 'p': {
					char * token = strtok(datoLeido.msj, "|");
					token = strtok(NULL, "|");

					mensajeLog.str("");
					mensajeLog << "ProcPrincipal - Saco del vector de posiciones libres la posicion: " << token;
					Log::getInstance()->loguear(mensajeLog.str());

					int pos = atoi(token);

					// Elimino la posicion libre del estacionamiento correspondiente
					this->lugares[datoLeido.from-1].quitarPosicionLibre(pos);

					break;
				}
				case 'd': {
					char * token = strtok(datoLeido.msj, "|");
					token = strtok(NULL, "|");

					mensajeLog.str("");
					mensajeLog << "ProcPrincipal - Agrego al  vector de posiciones libres la posicion: " << token;
					Log::getInstance()->loguear(mensajeLog.str());

					int pos = atoi(token);

					// Agrego la posicion libre al estacionamiento correspondiente
					this->lugares[datoLeido.from-1].agregarPosicionLibre(pos);
					//this->agregarPosicionLibre(pos);

					break;
				}
				case 'f': {

					sumatoriaEstacionamientosCerrados+=datoLeido.from;

					mensajeLog.str("");
					mensajeLog << "ProcPrincipal - La suma del numero de estacionamientos cerrados es " << sumatoriaEstacionamientosCerrados;
					Log::getInstance()->loguear(mensajeLog.str());

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


					mensajeLog.str("");
					mensajeLog << "ProcPrincipal - Estacionamiento " << datoLeido.from << " pide una posicion, le envio: " << nroPosicion;
					Log::getInstance()->loguear(mensajeLog.str());

					colaMensajes.escribir(datoEnviado);

					break;
				}
				default: {

					cout << "Comando invalido! Ingrese 'a' seguido del nro de estacionamiento para ver la cantidad de autos estacionados, ";
					cout << "'m' seguido del nro de estacionamiento para conocer el monto recaudado ";
					cout << " o 'q' para finalizar el programa" << endl;
					break;
				}
			}
		}

		int estado;
		for(int i=0;i<cantidadEstacionamientos;i++)
		{
			mensajeLog.str("");
			mensajeLog << "Liberado el estacionamiento... " << i+1;
			Log::getInstance()->loguear(mensajeLog.str());

			waitpid(pEstacionamientos[i],&estado,0);
		}
		waitpid(this->pConsola, &estado,0);

		colaMensajes.destruir();
		colaConsultas.destruir();
		liberarRecursos(cantidadEstacionamientos,cantidadDeLugares);
	}
	else
	{

		// cantidadEstacionamientos+2 es el mtype para la consola
		int msjConsola = cantidadEstacionamientos+2;

		// Proceso consola
		stringstream mensajeLog;
		mensajeLog << "Soy el proceso consola esperando el ingreso por pantalla.";
		Log::getInstance()->loguear(mensajeLog.str());

		mensaje datoEnviar;
		mensaje datoLeer;

		datoEnviar.mtype = msjPpal;
		datoEnviar.from = msjConsola;

		bool salir = false;
		// Para terminar el programa, el usuario debe escribir 'Q' o 'q'
		while (!salir) {


			cout << "Ingrese un comando: ( 'a' seguido del nro de estacionamiento para averiguar la cantidad de autos estacionados ";
			cout << "o 'm' seguido del nro de estacionamiento para averiguar el monto recaudado )" << endl;
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

				if (nroEstacionamiento <= cantidadEstacionamientos) {

					strcpy(datoEnviar.msj,ssRecibido.str().c_str());

					colaMensajes.escribir(datoEnviar);

					if (letra != 'q') {
						colaMensajes.leer(msjConsola,&datoLeer);

						cout << datoLeer.msj << endl;
					} else {
						salir = true;
					}
				} else {
					cout << "Numero de estacionamiento invalido, debe ser menor o igual a " << cantidadEstacionamientos << endl;
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

	// Cola de mensajes
	stringstream archivoColaMsjs;
	archivoColaMsjs << ARCHIVO_COLA_MENSAJES;

	tmpFile = fopen(archivoColaMsjs.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoColaMsjs.str();
	Log::getInstance()->loguear(mensaje.str());


	stringstream archivoColaMsjsConsultas;
	archivoColaMsjsConsultas << ARCHIVO_COLA_MENSAJES_CONSULTAS;

	tmpFile = fopen(archivoColaMsjsConsultas.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoColaMsjsConsultas.str();
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


	// Cola de mensajes
	stringstream archivoColaMsjs;
	archivoColaMsjs << ARCHIVO_COLA_MENSAJES;

	remove(archivoColaMsjs.str().c_str());
	mensaje.str("");
	mensaje << "Destruyo el archivo temporal: " << archivoColaMsjs.str();
	Log::getInstance()->loguear(mensaje.str());


	stringstream archivoColaMsjsConsultas;
	archivoColaMsjsConsultas << ARCHIVO_COLA_MENSAJES_CONSULTAS;

	remove(archivoColaMsjsConsultas.str().c_str());
	mensaje.str("");
	mensaje << "Destruyo el archivo temporal: " << archivoColaMsjsConsultas.str();
	Log::getInstance()->loguear(mensaje.str());

}


void AdministradorGeneral::liberarRecursos(int cantidadEstacionamientos,int cantidadLugares)
{
	int i;

	for(i = 1; i<=cantidadEstacionamientos;i++)
	{
		//Se eliminan los archivos temporales creados
		eliminarArchivosTemporales(i, cantidadLugares);

		// Libero memoria compartida de posiciones
		liberarMemoriaCompartidaEstacionamiento(i,cantidadLugares);
	}

}

