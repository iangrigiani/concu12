#ifndef AUTO_H_
#define AUTO_H_

#include "Ticket.h"

class Auto
{
	private:

			Ticket * ticket;


	public:
			Auto();
			~Auto();
			void setTicket(Ticket * ticket);
			Ticket * getTicket();


};



#endif /* AUTO_H_ */
