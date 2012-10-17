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

long Fecha::getTime()
{
	  /*1 hora ________ 60 minutos
	  1 minuto _______60 segundos
	  1 segundo _______ 1000 milisegundos*/

	 long miliSegundosHora = this->hora*60*60*1000;
	 long miliSegundosMinuto = this->minutos*60*1000;
	 long miliSegundosSegundo = this->segundos*1000;

	 long time = miliSegundosHora + miliSegundosMinuto + miliSegundosSegundo;

	 return time;
}
