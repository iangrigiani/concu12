
#include "Simulador.h"


Simulador::Simulador(int numeroEntrada)
{
	//this->estacionamiento = Estacionamiento::obtenerEstacionamiento();
	this->entrada = new Entrada(numeroEntrada);

	int estadoMemoria = this->memoria.crear ( (char*) "TP1.cpp",'R' );

	cout << "Hijo: duermo 5 segundos..." << endl;
	sleep ( 5 );

	cout<<"Antes del IF"<<endl;
	if ( estadoMemoria == SHM_OK ) {
		cout<<"Estoy en el IF"<<endl;
		this->administracion= (Administracion *)memoria.leer();

		//cout << "Hijo: leo el numero " << prueba1.entero << " y el numero " << prueba1.entero2 <<" de la memoria compartida" << endl;
	} else {
		cout<<"Estoy en el ELSE"<<endl;
		cout << "Hijo: error en memoria compartida: " << estadoMemoria << endl;
	}

	this->cronometro =  Cronometro::obtenerCronometro();
	this->cantidadAutos = 0;
}


Simulador::~Simulador(){

	//delete this->estacionamiento;
	delete this->cronometro;
	this->memoria.liberar();

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
			} else {
				// Entrada tiene que chequear si hay lugar disponible y lockear una ubicacion
				this->cantidadAutos++;
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


