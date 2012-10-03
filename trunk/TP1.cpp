#include "Simulador.h"
#include <sys/wait.h>


int main() {



	pid_t pConsola = fork();
	cout << "Creada la consola en el pid " << pConsola << endl;

	if( pConsola != 0)
	{

		Simulador * simulador = new Simulador();

		simulador->getCronometro()->setTiempoASimular(30);

		simulador->simular();

		delete simulador;
	}
	else
	{

		string texto;
		cin >> texto;

		cout << "Ingresaste por consola: " << texto << endl;
		return 0;
		//PIPE
		//consola comandos -v y -i.
	}

	int estado;
	cout << endl << "Acordate de ingresar algo por teclado pq sino esto no muere mas!! " << endl;
	waitpid(pConsola, &estado, 0);
	cout << "Terminado proceso..." << pConsola << endl;
	cout << "FIN SIMULACION" << endl;

	return 0;
}
