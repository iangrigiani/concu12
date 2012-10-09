
#include "Simulador.h"


Simulador::Simulador(int numeroEntrada,int cantidadPosiciones)
{
	this->entrada = new Entrada(numeroEntrada);
	this->cronometro =  Cronometro::obtenerCronometro();
	this->cantidadAutos = 0;

	//Asocian a la misma memoria compartida que crea el principal
	inicializarMemoriaCompartidaVectorPosiciones(cantidadPosiciones);
	inicializarMemoriaCompartidaAdministracion();

}


Simulador::~Simulador(){

	delete this->cronometro;

}

void Simulador::inicializarMemoriaCompartidaVectorPosiciones(int cantidadPosiciones)
{
	int i;
	int estadoMemoria = SHM_OK;
	Posicion posicion;
	MemoriaCompartida<Posicion> memoria;

	for (i=0;i<cantidadPosiciones;i++)
	{
		stringstream nombreArchivo;
		stringstream mensajeLog;

		// Creo archivo temporal
		nombreArchivo << ARCHIVO_POSICIONES;
		nombreArchivo << i;

		// Creo la memoria asociada al archivo temporal
		estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

		posicion = memoria.leer();
		//cout<<"Estoy en simulador y soy la entrada"<<this->entrada->getNumeroDeEntrada() <<" con posicion "<<posicion.getNumero()<<" el estado ocupado es "<<posicion.getEstadoOcupado()<<endl;


		this->vectorMemoriaPosiciones.push_back(memoria);

		nombreArchivo.flush();

	}

}

void Simulador::inicializarMemoriaCompartidaAdministracion()
{
	Administracion administracion;
	stringstream nombreArchivo;
	int estadoMemoria = SHM_OK;

	// Creo archivo temporal
	nombreArchivo << ARCHIVO_ADMINISTRACION;

	// Hago el atach a  la memoria asociada al archivo temporal
	estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	administracion = (Administracion)this->administracion.leer();

	//cout<<"Estoy en la entrada "<<this->entrada->getNumeroDeEntrada()<<" y leo que el costo hora es "<< administracion.getCostoHora()<<endl;
}


Cronometro * Simulador::getCronometro(){
	return this->cronometro;
}


double Simulador::getNumeroAleatorio()
{
	return rand() / double(RAND_MAX);
}

void Simulador::setPipePrincipal(Pipe pipePrincipal){
	this->pipePpal = pipePrincipal;
}

Pipe Simulador::getPipePrincipal(){
	return this->pipePpal;
}

void Simulador::setPipeEntrada(Pipe pipeEntrada){
	this->pipeEntrada = pipeEntrada;
}

Pipe Simulador::getPipeEntrada(){
	return this->pipeEntrada;
}

void Simulador::simular(){

	pid_t w;

	this->cronometro->iniciarTiempo();
	while (!this->cronometro->llegoAlFinal())
	{
		int semilla = time(0) * this->entrada->getNumeroDeEntrada();
		srand((unsigned)semilla);
		double numeroAlearorio = this->getNumeroAleatorio();

		double flujoDeAutos=this->cronometro->getFlujoDeAutos();
		cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " numero elegido: " << numeroAlearorio << " flujo de autos: " << flujoDeAutos << endl;
		if ( numeroAlearorio < flujoDeAutos )
		{
			Pipe pipeAuto;
			pid_t pAuto = fork();

			if( pAuto ==  0)
			{
				cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Proceso hijo creado " << getpid() << endl;
				char recibir[BUFFSIZE];
				recibir[0] = '\0';

				// Frena el proceso hasta que la entrada le diga si puede entrar o no
				int bytes = pipeAuto.leer(recibir, BUFFSIZE);
				int numeroPosicion = atoi(recibir);

				// Si el numero de posicion es > 0 entonces hay lugar y el auto puede entrar
				if (numeroPosicion > 0) {

					cout<<"Numero Posicion asignada al auto que entra  "<<numeroPosicion<<" en la entrada "<<this->entrada->getNumeroDeEntrada()<<endl;

					double horasAleatoriasEstadia = this->getNumeroAleatorio();
					int horas = ceil(50*horasAleatoriasEstadia);
					Auto * automovil = new Auto(horas);
					automovil->setNumeroPosicion(numeroPosicion);


					cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Soy el auto pid: " << getpid() << " y entro en la posicion: " << numeroPosicion << endl;
					//El auto duerme el tiempo que permanece en el estacionamiento
					automovil->run();

					this->incrementarMontoRecaudado(horas);

					//TODO hay que agregar el pipe al principal para que le avise que tiene que sacar el auto
					//y la salida sacarlo

					delete(automovil);
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
				nroEntrada << this->entrada->getNumeroDeEntrada();

				while (!cicloCompleto)
				{

					//Agregar semaforo para sincronizar el acceso
					//Pido una ubicacion libre al servidor
					this->pipePpal.escribir((char*)nroEntrada.str().c_str(),BUFFSIZE);

					char recibir[BUFFSIZE];
					recibir[0] = '\0';

					//Espero la respuesta y la guardo en la variable recibir
					this->pipeEntrada.leer(recibir, BUFFSIZE);

					int numeroPosicion = atoi(recibir);
					//Si devuelve un nro negativo, entonces no hay lugar y el auto no puede ingresar
					if (numeroPosicion < 0)
					{
						pipeAuto.escribir(recibir,BUFFSIZE);
						cicloCompleto = true;
					}
					else {

						//TODO semaforo
						//Aca intento tomar escribir en esa posicion de memoria para ocuparla, si no puedo, busco otra
						bool pudeOcuparPosicion = this->modificarPosicionCompartida(numeroPosicion);

						if (pudeOcuparPosicion)
						{
							stringstream param;
							pipeAuto.escribir(recibir,BUFFSIZE);

							this->incrementarCantidadDeAutos();

							//Tengo que avisarle al proceso principal que saque del vector de posiciones libres
							//la posicion que me asigno recien (le paso el nro por el pipe: s|nroPosicion )
							//la 's' es para avisarle que saque (convencion)

							param<<"s|";
							param<<numeroPosicion;

							this->pipePpal.escribir((char*)param.str().c_str(),BUFFSIZE);
							cicloCompleto = true;
						}

					}

				}

				pipeAuto.cerrar();

			}
		}
		sleep(1);

		cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Cantidad de autos: " << this->cantidadAutos << endl;
	}


	cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Fin simulacion, esperando que terminen todos los procesos..." << endl;
	cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Cantidad de autos: " << this->cantidadAutos << endl;
	int estado;
	while (this->cantidadAutos > 0) {
		w = wait(&estado);
		cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Terminado proceso..." << w << endl;
		this->cantidadAutos--;
		cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Cantidad de autos: " << this->cantidadAutos << ". Esperando que salgan los demas..." << endl;

	}

	cout << "Soy entrada " << this->entrada->getNumeroDeEntrada() << " y muero ahora" << endl;
	pipeEntrada.cerrar();

}



bool Simulador::modificarPosicionCompartida(int numeroPosicion)
{

	int estadoMemoria = SHM_OK;
	//TODO Semaforos!!!
	bool pudoOcuparPosicion = true;
	MemoriaCompartida<Posicion> memoria;
	Posicion posicion;

	stringstream mensajeLog;

	mensajeLog << "Memoria Compartida : soy la entrada "<< this->entrada->getNumeroDeEntrada()<<" y modifico la posicion " << numeroPosicion <<" poniendola como ocupada en el vector de posiciones.";

	Log::getInstance()->loguear(mensajeLog.str());

	memoria = this->vectorMemoriaPosiciones[numeroPosicion];

	posicion.setNumero(numeroPosicion);
	posicion.setEstadoOcupado(true);
	memoria.escribir(posicion);
	this->vectorMemoriaPosiciones[numeroPosicion] = memoria;

	return pudoOcuparPosicion;

}

void Simulador::incrementarCantidadDeAutos()
{
	Administracion admin = (Administracion)this->administracion.leer();

	//cout<<"Entrada : "<<this->entrada->getNumeroDeEntrada()<<" Hay "<<admin.getCantidadDeAutos()<<" en el estacionamiento"<<endl;

	admin.incrementarCantidadAutos();

	this->administracion.escribir(admin);

	stringstream mensajeLog;

	mensajeLog <<"Se registra la entrada de un auto al estacionamiento habiendo en total: "<<admin.getCantidadDeAutos();

	Log::getInstance()->loguear(mensajeLog.str());


	//ESTO ERA PARA SABER SI ESCRIBIA BIEN Y SI AL LEER SE OBTIENE EL DATO MODIFICADO
	//Administracion administracion2 = (Administracion)this->administracion.leer();
	//cout<<"Entrada : "<<this->entrada->getNumeroDeEntrada()<<"Y como ingresa un auto ahora hay "<<administracion2.getCantidadDeAutos()<<endl;


}


void Simulador::incrementarMontoRecaudado(int horas)
{
	Administracion admin = (Administracion)this->administracion.leer();
	admin.actualizarImporteRecaudado(horas);
	this->administracion.escribir(admin);

}

