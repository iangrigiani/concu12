#include "Estacionamiento.h"
#define BUFFSIZE		100
#define	ARCHIVO_FIFO_LECTURA	"/tmp/archivo_fifo_lectura"
#define ARCHIVO_FIFO_ESCRITURA	"/tmp/archivo_fifo_escritura"


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
	pid_t pConsola = fork();
	Simulador * simulador;
	pid_t pEntrada1;
	pid_t pEntrada2;
	pid_t pEntrada3;
	pid_t pSalida1;
	pid_t pSalida2;

	if( pConsola != 0)
	{

		this->crearArchivosTemporales(cantidadDeLugares);

		this->crearMemoriaCompartidaPosiciones(cantidadDeLugares);
		this->crearMemoriaCompartidaPosicionesLibres(cantidadDeLugares);
		this->crearMemoriaCompartidaAdministracion(costoHora);

		stringstream posicionElegida;

		char buffer [ BUFFSIZE ];
		Fifo canalLectura (ARCHIVO_FIFO_ESCRITURA);
		Fifo canalEscritura (ARCHIVO_FIFO_LECTURA);

		int bytesLeidos = canalLectura.leer ( buffer,BUFFSIZE );
		buffer[bytesLeidos] = '\0';
		cout << "Lector: lei el dato [" << buffer << "] del fifo" << endl;

		canalLectura.cerrar ();

		//Busco una posicion libre aleatoria
		int tamanioVector = this->vectorMemoriaPosicionesLibres.size();
		srand((unsigned)time(0));
		int numeroElegido = (rand()%tamanioVector);

		posicionElegida<<numeroElegido;

		char *posicion = (char*) posicionElegida.str().c_str();

		canalEscritura.escribir ( posicion, strlen(posicion) );

		cout << "Escritor: escribo el dato [" << posicion << "] en el fifo" << endl;

		canalEscritura.cerrar();

		canalEscritura.eliminar ();

		canalLectura.eliminar();


		/*bytesLeidos = canal.leer ( buffer,BUFFSIZE );
		buffer[bytesLeidos] = '\0';
		cout << "Lector: lei el dato [" << buffer << "] del fifo" << endl;
		 */

		//Proceso padre
		/*pEntrada1 = fork();

		if( pEntrada1 != 0)
		{
			//Proceso entrada 1
			simulador = new Simulador(1);

			simulador->getCronometro()->setTiempoASimular(tiempoEjecucion);

			simulador->simular();

			delete simulador;

		}
		else
		{
			pEntrada2 = fork();

			if( pEntrada2 != 0)
			{
				//Proceso entrada2

				simulador = new Simulador(2);

				simulador->getCronometro()->setTiempoASimular(tiempoEjecucion);

				simulador->simular();

				delete simulador;

			}
			else
			{
				pEntrada3 = fork();

				if( pEntrada3 != 0)
				{
					//Proceso entrada3

					simulador = new Simulador(3);

					simulador->getCronometro()->setTiempoASimular(tiempoEjecucion);

					simulador->simular();

					delete simulador;

				}
				else {}
			}

		}
*/
	}
	else
	{
		//Enviar fifo para recibir una posicion libre

		char buffer [ BUFFSIZE ];
		char *posicion = (char *) "Quiero una posicion\0";
		Fifo canalEscritura ( ARCHIVO_FIFO_ESCRITURA );
		Fifo canalLectura (ARCHIVO_FIFO_LECTURA);

		canalEscritura.escribir ( posicion, strlen(posicion) );
		cout << "Escritor: escribo el dato [" << posicion << "] en el fifo" << endl;

		canalEscritura.cerrar ();

		//Espero a que responda
		sleep(10);

		int bytesLeidos = canalLectura.leer ( buffer,BUFFSIZE );
		buffer[bytesLeidos] = '\0';
		cout << "Lector: lei el dato [" << buffer << "] del fifo" << endl;

		canalLectura.cerrar();

		//Proceso consola

		string texto;
		cin >> texto;

		cout << "Ingresaste por consola: " << texto << endl;


	}

	int estado;
	cout << endl << "Acordate de ingresar algo por teclado pq sino esto no muere mas!! " << endl;
	waitpid(pConsola, &estado, 0);
	//waitpid(pEntrada1, &estado, 0);
	//waitpid(pEntrada2, &estado, 0);
	//waitpid(pEntrada3, &estado, 0);

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
