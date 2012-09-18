
#include "Simulador.h"


Simulador::Simulador()
{
	this->estacionamiento = Estacionamiento::obtenerEstacionamiento();

}


Simulador::~Simulador(){

}

void Simulador::simular(){
	/*
	Cronometro.obtenerCronometro().iniciarTiempo();
	while (!Cronometro.llegoAlFinal()){
		if(Math.random()<Cronometro.getFlujoDePersonas()){
			new Thread( Simulador.generarPersona()).start();
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	*/
}


