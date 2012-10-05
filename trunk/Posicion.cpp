#include "Posicion.h"

Posicion::Posicion() {
	this->estadoOcupado = false;
	this->numero = 0;
}

Posicion::~Posicion() {}


bool Posicion::getEstadoOcupado(){
	return this->estadoOcupado;
}


void Posicion::setEstadoOcupado(bool estado){
	this->estadoOcupado = estado;
}

int Posicion::getNumero(){
	return this->numero;
}

void Posicion::setNumero(int numero){
	this->numero = numero;
}

