#ifndef POSICION_H_
#define POSICION_H_

class Posicion {

private:
		bool estadoOcupado;
		int numero;

public:
	Posicion();
	virtual ~Posicion();

	bool getEstadoOcupado();
	void setEstadoOcupado(bool estado);
	int getNumero();
	void setNumero(int numero);
};

#endif /* POSICION_H_ */
