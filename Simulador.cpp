
#include "Simulador.h"


Simulador::Simulador()
{
	this->estacionamiento = Estacionamiento::obtenerEstacionamiento();
	this->cronometro =  Cronometro::obtenerCronometro();
	this->cantidadAutos = 0;
}


Simulador::~Simulador(){

	delete this->estacionamiento;
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
	  /*pid_t pConsola = fork();

	  cout<<"Primer proceso hijo creado "<<endl;

	  if( pConsola != 0)
	  {*/
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
						cout << "Proceso hijo creado " << getpid() << endl;

						double horasAleatoriasEstadia = this->getNumeroAleatorio();
						int horas = ceil(50*horasAleatoriasEstadia);
						Auto * automovil = new Auto(horas);
						//this->entrarAlEstacionamiento(automovil);
						automovil->run();
						//el while del auto
						//automovil->getTicket()->setPago(true);
						//this->salirDelEstacionamiento(automovil);
						delete(automovil);
						exit(0);
					} else {
						this->cantidadAutos++;
					}
			  }
			  sleep(1);

			  cout << "Cantidad de autos: " << this->cantidadAutos << endl;
		  }

		  cout << "Esperando que terminen todos los procesos..." << endl;
		  cout << "Cantidad de autos: " << this->cantidadAutos << endl;
		  while (this->cantidadAutos > 0) {
			  int estado;
			  w = wait(&estado);
			  cout << "Terminado proceso..." << w << endl;
			  this->cantidadAutos--;
			  cout << "Cantidad de autos: " << this->cantidadAutos << endl;

		  }


	/*}
	  else
	  {
		  //PIPE
	  	//consola comandos -v y -i.
	  }*/
}

bool Simulador::entrarAlEstacionamiento(Auto * automovil)
{
	if(Estacionamiento::obtenerEstacionamiento()->getEntradaAleatoria()->registrarEntradaAuto(automovil))
		automovil->setHaEntrado(true);

	return automovil->getHaEntrado();
}


bool Simulador::salirDelEstacionamiento(Auto * automovil)
{
	if(Estacionamiento::obtenerEstacionamiento()->getSalidaAleatoria()->registrarSalidaAuto(automovil))
		automovil->setHaEntrado(false);

	return automovil->getHaEntrado();

}


