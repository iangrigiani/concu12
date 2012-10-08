#include "Estacionamiento.h"
#define BUFFSIZE		100


Estacionamiento::Estacionamiento() {


}


Estacionamiento::~Estacionamiento()
{

}


void Estacionamiento::crearArchivosTemporales(int cantidadLugares)
{

	int i;
	FILE * tmpPosicion;
	FILE * tmpLibres;
	FILE * tmpAdministracion;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;
		stringstream nombreArchivoLibres;

		// Creo archivo temporal posiciones
		nombreArchivo << "/tmp/tmpPosicion";
		nombreArchivo << i;
		//cout << "nombre archivo: " << nombreArchivo.str() << endl;


		// Creo archivo temporal posiciones libres
		nombreArchivoLibres << "/tmp/tmpPosicionLibre";
		nombreArchivoLibres << i;
		//cout << "nombre archivo libres: " << nombreArchivoLibres.str() << endl;


		tmpPosicion = fopen(nombreArchivo.str().c_str(),"w");
		fclose(tmpPosicion);


		tmpLibres = fopen(nombreArchivoLibres.str().c_str(),"w");
		fclose(tmpLibres);

		nombreArchivo.flush();

		nombreArchivoLibres.flush();
	}

	// Creo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << "/tmp/tmpAdministracion";
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
		nombreArchivo << "/tmp/tmpPosicion";
		nombreArchivo << i;
		//cout << "destruccion archivo: " << nombreArchivo.str() << endl;


		// Destruyo archivo temporal posiciones libres
		nombreArchivoLibres << "/tmp/tmpPosicionLibre";
		nombreArchivoLibres << i;
		//cout << "destruccion archivo libres: " << nombreArchivoLibres.str() << endl;


		remove(nombreArchivo.str().c_str());

		remove(nombreArchivoLibres.str().c_str());


		nombreArchivo.flush();

		nombreArchivoLibres.flush();
	}

	// Destruyo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << "/tmp/tmpAdministracion";
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

		// Creo archivo temporal
		nombreArchivo << "/tmp/tmpPosicion";
		nombreArchivo << i;
		//cout << "nombre archivo creando memoria posiciones: " << nombreArchivo.str() << endl;

		// Creo la memoria asociada al archivo temporal
		estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		//cout << "Padre: escribo el numero " << i << "en el vector de posiciones"<<endl;

		posicion.setNumero(i);
		posicion.setEstadoOcupado(true);

		memoria.escribir(posicion);

		this->vectorMemoriaPosiciones.push_back(memoria);

		nombreArchivo.flush();
	}
}

void Estacionamiento::crearMemoriaCompartidaPosicionesLibres(int cantidadLugares)
{

	int i;
	int estadoMemoria = SHM_OK;
	Posicion posicion;
	MemoriaCompartida<Posicion> memoria;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;

		// Creo archivo temporal
		nombreArchivo << "/tmp/tmpPosicionLibre";
		nombreArchivo << i;
		//cout << "nombre archivo creando memoria posiciones: " << nombreArchivo.str() << endl;

		// Creo la memoria asociada al archivo temporal
		estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		//cout << "Padre: escribo el numero " << i <<" en el vector de posiciones libres"<<endl;

		posicion.setNumero(i);
		posicion.setEstadoOcupado(true);

		memoria.escribir(posicion);

		this->vectorMemoriaPosicionesLibres.push_back(memoria);

		nombreArchivo.flush();
	}


}

void Estacionamiento::crearMemoriaCompartidaAdministracion(int costoHora)
{

	Administracion administracion;
	stringstream nombreArchivo;
	int estadoMemoria = SHM_OK;

	administracion.setCostoHora(costoHora);

	// Creo archivo temporal
	nombreArchivo << "/tmp/tmpAdministracion";
	//cout << "nombre archivo adminstracion: " << nombreArchivo.str() << endl;

	// Creo la memoria asociada al archivo temporal
	estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	//cout << "Padre: escribo el costo hora de la administracion" << administracion.getCostoHora()<<endl;

	this->administracion.escribir(administracion);

}




void Estacionamiento::run(int cantidadDeLugares, int costoHora, int tiempoEjecucion)
{
	// Pipe para la comunicacion de todas las entradas y la consola con el proceso principal. Todos escriben
	// ahi y el proceso principal solo lee
	Pipe pipePpal;

	// Pipe para responder las consultas de la consola
	Pipe pipeConsola;

	pid_t pConsola = fork();

	pid_t pEntrada1;
	pid_t pEntrada2;
	pid_t pEntrada3;

//	pid_t pSalida1;
//	pid_t pSalida2;

	if( pConsola != 0)
	{

		this->crearArchivosTemporales(cantidadDeLugares);

		this->crearMemoriaCompartidaPosiciones(cantidadDeLugares);
		this->crearMemoriaCompartidaPosicionesLibres(cantidadDeLugares);
		this->crearMemoriaCompartidaAdministracion(costoHora);

		Pipe pipeEntrada1;

		//Proceso padre
		pEntrada1 = fork();

		if( pEntrada1 != 0)
		{
			//Proceso entrada 1
			correrSimulador(1, tiempoEjecucion);
		}
		else
		{
			Pipe pipeEntrada2;
			pEntrada2 = fork();

			if( pEntrada2 != 0)
			{
				//Proceso entrada 2
				correrSimulador(2, tiempoEjecucion);
			}
			else
			{
				Pipe pipeEntrada3;
				pEntrada3 = fork();

				if( pEntrada3 != 0)
				{
					//Proceso entrada 3
					correrSimulador(3, tiempoEjecucion);
				}
				else {
					// Proceso principal (funciona como servidor de mensajes)

					bool salir = false;
					char recibido[BUFFSIZE];
					while (!salir)
					{
						int bytes = pipePpal.leer(recibido, BUFFSIZE);
						recibido[bytes] = '\0';

						cout << "ProcPrincipal: acabo de recibir: " << recibido << endl;

						stringstream retorno;

						switch (recibido[0])
						{
							case 'a': {
								cout << "Ingreso una a" << endl;
								pipeConsola.escribir((char*)retorno.str().c_str(),BUFFSIZE);
								break;
							}
							case 'm': {
								cout << "Ingreso una m" << endl;
								pipeConsola.escribir((char*)retorno.str().c_str(),BUFFSIZE);
								break;
							}
							case 'q': {
								cout << "Ingreso un q" << endl;
								salir = true;
								break;
							}
							case '1': {
								cout << "Entrada 1 pide una posicion" << endl;
								int nroEntrada;
								//nroEntrada = getEntradaAleatoria();
								retorno << nroEntrada;
								pipeEntrada1.escribir((char*)retorno.str().c_str(),BUFFSIZE);
								break;
							}
							case '2': {
								cout << "Entrada 1 pide una posicion" << endl;
								int nroEntrada;
								//nroEntrada = getEntradaAleatoria();

								retorno << nroEntrada;
								pipeEntrada2.escribir((char*)retorno.str().c_str(),BUFFSIZE);
								break;
							}
							case '3': {
								cout << "Entrada 1 pide una posicion" << endl;
								int nroEntrada;
								//nroEntrada = getEntradaAleatoria();

								retorno << nroEntrada;
								pipeEntrada3.escribir((char*)retorno.str().c_str(),BUFFSIZE);
								break;
							}
							default: {
								cout << "Comando invalido" << endl;
								break;
							}
						}
					}

					int estado;
					cout << endl << "Acordate de ingresar Q por teclado pq sino esto no muere mas!! " << endl;
					waitpid(pConsola, &estado, 0);
					waitpid(pEntrada1, &estado, 0);
					waitpid(pEntrada2, &estado, 0);
					waitpid(pEntrada3, &estado, 0);

					int i;

					for (i=0;i<cantidadDeLugares;i++)
					{
						MemoriaCompartida<Posicion> memoriaPosicion;
						memoriaPosicion = this->vectorMemoriaPosiciones[i];
						memoriaPosicion.liberar();

						MemoriaCompartida<Posicion> memoriaLibres;
						memoriaLibres = this->vectorMemoriaPosicionesLibres[i];
						memoriaLibres.liberar();
					}

					this->eliminarArchivosTemporales(cantidadDeLugares);

					cout << "Terminado proceso..." << pConsola << endl;
					cout << "FIN SIMULACION" << endl;
				}
			}

		}

	}
	else
	{
		// Proceso consola
		char entradaConsola[BUFFSIZE];
		entradaConsola[0] = '\0';

		// Para terminar el programa, el usuario debe escribir 'Q' o 'q'
		while (tolower(entradaConsola[0]) != 'q') {
			cout << "ProcConsola: Me quedo esperando que ingreses algo por consola..." << endl;
			cin.getline(entradaConsola, BUFFSIZE);

			// Aca hay que agregar la validacion para que solo pueda ingresar una letra para hacer consultas
			// o para salir
			pipePpal.escribir(entradaConsola,BUFFSIZE);

			if (entradaConsola[0] != 'Q') {
				char recibir[BUFFSIZE];
				int bytes = pipeConsola.leer(recibir, BUFFSIZE);
				recibir[bytes] = '\0';

				cout << "ProcConsola: acabo de recibir como rta: " << recibir << endl;

				cout << "ProcConsola: En la variable de consola hay: " << entradaConsola << endl;
			}
		}

		exit(0);
	}
}

void Estacionamiento::correrSimulador(int numeroEntrada, int tiempoEjecucion)
{
	Simulador * simulador;

	simulador = new Simulador(numeroEntrada);

	simulador->getCronometro()->setTiempoASimular(tiempoEjecucion);

	simulador->simular();

	delete simulador;
}

Entrada * Estacionamiento::getEntradaAleatoria()
{
	/*int cantidadEntradas = this->entradas->size();
	int numeroElegido;
	int indiceActual = 0;
	Entrada * entradaElegida;

	srand((unsigned)time(0));
	numeroElegido = (rand()%cantidadEntradas);

	list<Entrada*>::iterator it = this->entradas->begin();

	 while ( it != this->entradas->end())
	 {
		 if(indiceActual == numeroElegido)
		 {
			 entradaElegida = *it;
			 break;
		 }

		 ++it;
		 indiceActual++;
	 }

	 return entradaElegida;*/
	return NULL;
}



Salida * Estacionamiento::getSalidaAleatoria()
{
	/*int cantidadSalidas = this->salidas->size();
	int numeroElegido;
	int indiceActual = 0;
	Salida * salidaElegida;

	srand((unsigned)time(0));
	numeroElegido = (rand()%cantidadSalidas);

	list<Salida*>::iterator it = this->salidas->begin();

	 while ( it != this->salidas->end())
	 {
		 if(indiceActual == numeroElegido)
		 {
			 salidaElegida = *it;
			 break;
		 }

		 ++it;
		 indiceActual++;
	 }

	 return salidaElegida;*/
	return NULL;

}
