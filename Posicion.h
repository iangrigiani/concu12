#ifndef POSICION_H_
#define POSICION_H_

class Posicion {

private:
		bool estadoOcupado;
		int numero;

public:
	Posicion(int numero);
	virtual ~Posicion();
};

#endif /* POSICION_H_ */
