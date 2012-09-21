
#include "Simulador.h"


Simulador::Simulador()
{
	this->estacionamiento = Estacionamiento::obtenerEstacionamiento();

}


Simulador::~Simulador(){

}

void Simulador::simular(){

	/*
	 * pid1 = fork();
	 *
	 * if( pid1 != 0)
	 * {
	 *
		 * 	/*
			Cronometro.obtenerCronometro().iniciarTiempo();
			while (!Cronometro.llegoAlFinal())
			{
				if(Math.random()<Cronometro.getFlujoDeAutos())
				{
					pid2 = fork();

					if( pid2 ==  0)
					{
						Auto * auto = new Auto(cantidad tiempo aleatorio);
						this->entrarALes...
						auto->run(); el while del auto
						auto->setPago()
						this->salir del estacionamiento
					}
					else
					{
						//ACA HAY QUE VER como eliminar
						aca vendria lo que haga el proceso principal
					}



				}

		 *
		 * }
		 }
	 * else
	 * {
	 * 		//PIPE
	 * 		consola comandos -v y -i.
	 *
	 * }

	 */
	printf("...simulando...");

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


