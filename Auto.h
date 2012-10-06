#ifndef AUTO_H_
#define AUTO_H_

#include <iostream>
using namespace std;

class Auto
{
	private:

			bool haEntrado;
			int horasEstadia;
			int numeroPosicion;


	public:
			Auto(int horasEstadia);

			~Auto();

			void setHaEntrado(bool haEntrado);

			bool getHaEntrado();

			int getHorasEstadia();


			int getNumeroPosicion();

			void setNumeroPosicion(int numero);

			void run();

};



#endif /* AUTO_H_ */
