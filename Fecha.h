#ifndef FECHA_H_
#define FECHA_H_

#include <iostream>
#include <ctime>

using namespace std;

class Fecha {

private:

	  int segundos;
	  int minutos;
	  int hora;
	  int dia;
	  int mes;
	  int anio;
	  int diaSemana;

public:

	Fecha();
	virtual ~Fecha();
	void mostrarFechaActual();
	long getTime();

};

#endif /* FECHA_H_ */
