#include "Simulador.h"


int main() {

	Simulador * simulador = new Simulador();

	simulador->simular();

	delete simulador;

	return 0;
}
