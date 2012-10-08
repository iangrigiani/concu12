
#include "Simulador.h"


Simulador::Simulador(int numeroEntrada)
{
	this->entrada = new Entrada(numeroEntrada);
	this->cronometro =  Cronometro::obtenerCronometro();
	this->cantidadAutos = 0;

}


Simulador::~Simulador(){

	delete this->cronometro;

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

					double horasAleatoriasEstadia = this->getNumeroAleatorio();
					int horas = ceil(50*horasAleatoriasEstadia);
					Auto * automovil = new Auto(horas);
					automovil->setNumeroPosicion(numeroPosicion);
					//this->entrarAlEstacionamiento(automovil);

					cout << "Entrada " << this->entrada->getNumeroDeEntrada() << " Soy el auto pid: " << getpid() << " y entro en la posicion: " << numeroPosicion << endl;
					//el while del auto
					automovil->run();
					//this->salirDelEstacionamiento(automovil);

					stringstream mensaje;
					mensaje << "h|";
					mensaje << intToString(horas);
					this->pipePpal.escribir((char*)mensaje.str().c_str(),BUFFSIZE);

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

				while (!cicloCompleto) {

					//Agregar semaforo para sincronizar el acceso
					//Pido una ubicacion libre al servidor
					this->pipePpal.escribir((char*)nroEntrada.str().c_str(),BUFFSIZE);

					char recibir[BUFFSIZE];
					recibir[0] = '\0';

					//Espero la respuesta y la guardo en la variable recibir
					this->pipeEntrada.leer(recibir, BUFFSIZE);

					int numeroPosicion = atoi(recibir);
					//Si devuelve un nro negativo, entonces no hay lugar y el auto no puede ingresar
					if (numeroPosicion < 0) {
						pipeAuto.escribir(recibir,BUFFSIZE);
						cicloCompleto = true;
					} else {
						//Aca intento tomar escribir en esa posicion de memoria para ocuparla, si no puedo, busco otra
						bool pudeOcuparPosicion = true;
						if (pudeOcuparPosicion) {
							pipeAuto.escribir(recibir,BUFFSIZE);

							//Tengo que avisarle al proceso principal que saque del vector de posiciones libres
							//la posicion que me asigno recien (le paso el nro por el pipe: s|nroPosicion )
							//la 's' es para avisarle que saque (convencion)
							this->cantidadAutos++;
							cicloCompleto = true;
						}

					}



					/*
					MemoriaCompartida<Posicion> memoriaLibre = this->vectorMemoriaPosicionesLibres[numero];


					Posicion posicionLibre =(Posicion)memoriaLibre.leer();

					MemoriaCompartida<Posicion> memoriaPosicion = this->vectorMemoriaPosiciones[posicionLibre.getNumero()];
					Posicion posicion = (Posicion)memoriaPosicion.leer();

					//Si esta ocupado tiene que buscar otro libre sino cambiarle el estado
					if(!posicion.getEstadoOcupado())
					{
						posicion.setEstadoOcupado(true);
						memoriaPosicion.escribir(posicion);
					}
					*/
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

bool Simulador::entrarAlEstacionamiento(Auto * automovil)
{
	/*if(Estacionamiento::obtenerEstacionamiento()->getEntradaAleatoria()->registrarEntradaAuto(automovil))
		automovil->setHaEntrado(true);
	 */
	return automovil->getHaEntrado();
}


bool Simulador::salirDelEstacionamiento(Auto * automovil)
{
	/*if(Estacionamiento::obtenerEstacionamiento()->getSalidaAleatoria()->registrarSalidaAuto(automovil))
		automovil->setHaEntrado(false);
	*/
	return automovil->getHaEntrado();

}


