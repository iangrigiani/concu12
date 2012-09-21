#ifndef AUTO_H_
#define AUTO_H_

#include "Ticket.h"

class Auto
{
	private:

			Ticket * ticket;
			bool haEntrado;
			int horasEstadia;


	public:
			Auto(int horasEstadia);

			~Auto();

			void setTicket(Ticket * ticket);

			Ticket * getTicket();

			void setHaEntrado(bool haEntrado);

			bool getHaEntrado();

			int getHorasEstadia();

};



#endif /* AUTO_H_ */
