#ifndef AUTO_H_
#define AUTO_H_

//#include "Estacionamiento.h"
#include "Ticket.h"

class Auto
{
	private:

			Ticket * ticket;
			bool haEntrado;


	public:
			Auto();

			~Auto();

			void setTicket(Ticket * ticket);

			Ticket * getTicket();

			bool entrarAlEstacionamiento();

			bool salirDelEstacionamiento();

			void setHaEntrado(bool haEntrado);

			bool getHaEntrado();



};



#endif /* AUTO_H_ */
