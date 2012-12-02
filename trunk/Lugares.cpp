#include "Lugares.h"

Lugares::Lugares(int nroEstacionamiento) {
	this->nroEstacionamiento = nroEstacionamiento;
}

Lugares::~Lugares() {
}



bool Lugares::crearMemoriaCompartidaPosiciones(int cantidadLugares,int numeroEstacionamiento)
{
	int i;
	Posicion posicion;
	MemoriaCompartida<Posicion> memoria;
	bool retorno = true;

	for (i=0;i<cantidadLugares;i++)
	{
		stringstream nombreArchivo;
		stringstream mensajeLog;
		stringstream msj;

		// Creo archivo temporal
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;
		nombreArchivo<<ESTACIONAMIENTO;
		nombreArchivo<<numeroEstacionamiento;

		msj << "Lugares: creo archivos: " << nombreArchivo.str();
		Log::getInstance()->loguear(msj.str());

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
			mensajeLog << "Error al inicializar el vector de memoria compartida en la posicion " << i << " en el estacionamiento" << numeroEstacionamiento;
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

void Lugares::crearVectorPosicionesLibres(int cantidadLugares)
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

void Lugares::quitarPosicionLibre(int numeroPosicion)
{
	int inicio = 0;
	int tam = this->vectorMemoriaPosicionesLibres.size();
	int fin = tam-1;
	stringstream mensajeLog;

	int posicionBuscada = busquedaBinariaVectorLibres(inicio,fin,numeroPosicion);


	mensajeLog << "Estacionamiento " << this->nroEstacionamiento << " - Indice buscado =  " << posicionBuscada;
	Log::getInstance()->loguear(mensajeLog.str());

	this->vectorMemoriaPosicionesLibres.erase (this->vectorMemoriaPosicionesLibres.begin()+posicionBuscada);

	int a;

	mensajeLog.str("");
	mensajeLog<<"Estacionamiento " << this->nroEstacionamiento << " - Contenido del array de posiciones libres: ";

	for(a=0;a<tam-1;a++){
		mensajeLog << this->vectorMemoriaPosicionesLibres[a] << " ";
	}
	Log::getInstance()->loguear(mensajeLog.str());

}


void Lugares::agregarPosicionLibre(int numeroPosicion)
{
	stringstream mensajeLog;

	mensajeLog << "ProcPrincipal - Agrego posicion " << numeroPosicion;
	Log::getInstance()->loguear(mensajeLog.str());

	this->vectorMemoriaPosicionesLibres.push_back(numeroPosicion);

	sort(this->vectorMemoriaPosicionesLibres.begin(), this->vectorMemoriaPosicionesLibres.end());

	int a;
	int tam = this->vectorMemoriaPosicionesLibres.size();

	mensajeLog.str("");
	mensajeLog << "Estacionamiento " << this->nroEstacionamiento << " - Contenido del array de posiciones libres: ";

	for(a=0;a<tam;a++){
		mensajeLog << this->vectorMemoriaPosicionesLibres[a] << " ";
	}
	Log::getInstance()->loguear(mensajeLog.str());

}


int Lugares::busquedaBinariaVectorLibres(int inicio,int fin,int buscado)
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


int Lugares::getPosicionAleatoria()
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

void Lugares::liberarMemoriaCompartidaPosiciones(int cantidadLugares)
{
	//Se libera la memoria compartida correspondiente a las posiciones en el estacionamiento.
	int i;
	for (i=0;i<cantidadLugares;i++)
	{
		MemoriaCompartida<Posicion> memoriaPosicion;
		memoriaPosicion = this->vectorMemoriaPosiciones[i];
		memoriaPosicion.liberar();
	}

}

