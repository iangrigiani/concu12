#include "AdministradorGeneral.h"

AdministradorGeneral::AdministradorGeneral() {

}

AdministradorGeneral::~AdministradorGeneral() {

}

void AdministradorGeneral::run(int cantidadDeLugares, float costoHora, int tiempoEjecucion,int cantidadEstacionamientos)
{
	//Inicializa el tiempo en el cronometro y arranca el tiempo a correr en la simulacion
	Cronometro::obtenerCronometro()->setTiempoASimular(tiempoEjecucion);
	Cronometro::obtenerCronometro()->iniciarTiempo();

	cout << "Inicio de la simulacion..." << endl;

	//Crea las memorias compartidas asociadas a cada uno de los estacionamientos y los
	//vectores de posiciones libres
	this->inicializaciones(cantidadEstacionamientos,cantidadDeLugares);

	this->pConsola = fork();

	if( this->pConsola != 0)
	{
		Log::getInstance()->loguear("Soy el proceso principal");

		for(int i=0;i<cantidadEstacionamientos;i++)
		{
			pid_t pEstacionamiento = fork();

			if(pEstacionamiento == 0)
			{
				Estacionamiento estacionamiento;
				//TODO llamar al run del estacionamiento
			}
		}

		//Soy el proceso administrador general
		//TODA LA LOGICA DEL ADMINISTRADOR GENERAL, servidor de mensajes

	}
	else
	{
		// Proceso consola
		//TODO CAMBIAR EL INGRESO DE POSIBLES COMANDOS EN EL PROCESO CONSOLA
		/*
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
		*/
	}

	Cronometro::destruir();

}


void AdministradorGeneral::inicializaciones(int cantidadEstacionamientos,int cantidadDeLugares)
{

	for(int i = 0; i<cantidadEstacionamientos; i++)
	{
		Lugares lugar;

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
		nombreArchivo<<"Estacionamiento";
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
	archivoSmfPosiciones<<"Estacionamiento";
	archivoSmfPosiciones<<numeroEstacionamiento;

	tmpFile = fopen(archivoSmfPosiciones.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Creo el archivo temporal: " << archivoSmfPosiciones.str();
	Log::getInstance()->loguear(mensaje.str());

}

