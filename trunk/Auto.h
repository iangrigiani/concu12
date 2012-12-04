#ifndef AUTO_H_
#define AUTO_H_

#include <iostream>
#include <sstream>
#include "Log.h"

using namespace std;

class Auto
{
	private:

			bool haEntrado;
			int horasEstadia;
			int numeroPosicion;
			int numeroEstacionamiento;


	public:
			Auto(int horasEstadia, int numeroEstacionamiento);

			~Auto();

			void setHaEntrado(bool haEntrado);

			bool getHaEntrado();

			int getHorasEstadia();

			int getNumeroPosicion();

			void setNumeroPosicion(int numero);

			// Mantiene al auto dormido la cantidad de horas necesarias para la simulacion
			void run();
};



#endif /* AUTO_H_ */
