
#ifndef TICKET_H_
#define TICKET_H_

#include "Fecha.h"

class Ticket
{

	private:
			Fecha * fechaEntrada;
			bool pago;


	public:
			Ticket();
			~Ticket();
			void setPago(bool pago);
			bool getPago();
			Fecha * getFechaEntrada();
};




#endif /* TICKET_H_ */
