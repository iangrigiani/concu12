#include "Simulador.h"


int main() {

	Simulador * simulador = new Simulador();

	simulador->getCronometro()->setTiempoASimular(30);

	simulador->simular();

	delete simulador;

	return 0;
}
