#include "Fecha.h"

Fecha::Fecha() {

	// current date/time based on current system
	time_t now = time(0);

	tm *ltm = localtime(&now);

	this->anio = 1900 + ltm->tm_year;
	this->mes = 1 + ltm->tm_mon;
	this->dia = ltm->tm_mday;
	this->hora = ltm->tm_hour;
	this->minutos = ltm->tm_min;
	this->segundos = ltm->tm_sec;
	this->diaSemana = ltm->tm_wday;

}

Fecha::~Fecha() {

}



void Fecha::mostrarFechaActual(){

	cout << "Hora " << this->hora<<":"<<this->minutos<<":"<<this->segundos<< " anio " <<this->anio<<endl;


}
