
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

void Simulador::simular(){

	pid_t w;

	this->cronometro->iniciarTiempo();
	while (!this->cronometro->llegoAlFinal())
	{
		srand((unsigned)time(NULL));
		double numeroAlearorio = this->getNumeroAleatorio();

		cout << "numero elegido " << numeroAlearorio << endl;
		double flujoDeAutos=this->cronometro->getFlujoDeAutos();
		cout << "flujo de autos " << flujoDeAutos << endl;
		if ( numeroAlearorio < flujoDeAutos )
		{
			pid_t pAuto = fork();

			if( pAuto ==  0)
			{

				// Frenar el proceso hasta que la entrada le diga si puede entrar o no
				// PIPE o algo simil, no sleep

				cout << "Proceso hijo creado " << getpid() << endl;

				double horasAleatoriasEstadia = this->getNumeroAleatorio();
				int horas = ceil(50*horasAleatoriasEstadia);
				Auto * automovil = new Auto(horas);
				//this->entrarAlEstacionamiento(automovil);

				// Si la entrada le dio permiso, entra, sino muere el proceso
				// if (puedeEntrar){
				//el while del auto
				automovil->run();
				// }

				//automovil->getTicket()->setPago(true);
				//this->salirDelEstacionamiento(automovil);

				delete(automovil);
				return;
			}
			else
			{
				//Enviar un mensaje al padre para pedir una posicion libre
				//el padre le devuelve la posicion libre del vector posiciones
				//si es que existe u false en otro caso
				/*
				if(tamanioVector > 0)
				{
					/*int numeroElegido;
					srand((unsigned)time(0));
					numeroElegido = (rand()%tamanioVector);

					//Agregar semaforo para sincronizar el acceso

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
				}
				else
				{

					//Enviar mensaje al auto diciendo que no hay lugar y que se tiene que ir
				}*/
			}
		}
		sleep(1);

		cout << "Cantidad de autos: " << this->cantidadAutos << endl;
	}


	cout << "Esperando que terminen todos los procesos..." << endl;
	cout << "Cantidad de autos: " << this->cantidadAutos << endl;
	int estado;
	while (this->cantidadAutos > 0) {
		w = wait(&estado);
		cout << "Terminado proceso..." << w << endl;
		this->cantidadAutos--;
		cout << "Cantidad de autos: " << this->cantidadAutos << ". Esperando que salgan los demas..." << endl;

	}

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


