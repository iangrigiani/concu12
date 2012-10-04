#include "Simulador.h"
#include <sys/wait.h>
#include "MemoriaCompartida.h"
#include "Administracion.h"


int main() {

	Simulador * simulador;
	pid_t pConsola = fork();
	pid_t pEntrada1;
	pid_t pEntrada2;
	pid_t pEntrada3;


	//cout << "Creada la consola en el pid " << pConsola << endl;

	if( pConsola != 0)
	{
		MemoriaCompartida<Administracion> memoria;


		int estadoMemoria = memoria.crear ( (char*) "/bin/uname",'R' );

		Administracion administracion;
		administracion.setCostoHora(25);

		if ( estadoMemoria == SHM_OK ) {

			// escribe un dato para el hijo
			cout << "Padre: escribo el numero " << administracion.getCostoHora() << endl;
			memoria.escribir(&administracion);


			// libera la memoria
			//TODO tiene que ir donde muera la consola
			//memoria.liberar();
		}
		else
		{
			cout << "Padre: error en memoria compartida: " << estadoMemoria << endl;
		}

		/*
		pEntrada1 = fork();

		if( pEntrada1 != 0)
		{
			simulador = new Simulador(1);

			simulador->getCronometro()->setTiempoASimular(30);

			simulador->simular();

			delete simulador;

		} else
		{
			pEntrada2 = fork();

			if( pEntrada2 != 0)
			{
				simulador = new Simulador(2);

				simulador->getCronometro()->setTiempoASimular(30);

				simulador->simular();

				delete simulador;

			}
			else
			{
				pEntrada3 = fork();

				if( pEntrada3 != 0)
				{
					simulador = new Simulador(3);

					simulador->getCronometro()->setTiempoASimular(30);

					simulador->simular();

					delete simulador;

				}
				else {}
			}

		}*/
	}
	else
	{

		cout << "Hijo: duermo 5 segundos..." << endl;
		sleep ( 5 );

		MemoriaCompartida<Administracion> memoria2;
		int estadoMemoria2 = memoria2.crear ( (char*) "/bin/uname",'R' );

		cout<<"Antes del IF"<<endl;
		if ( estadoMemoria2 == SHM_OK ) {
			cout<<"Estoy en el IF"<<endl;
			Administracion * administracion2 = (Administracion *)memoria2.leer();

			cout << "Hijo: leo el numero " << (administracion2)->getCostoHora() << endl;
		} else {
			cout<<"Estoy en el ELSE"<<endl;
			cout << "Hijo: error en memoria compartida: " << estadoMemoria2 << endl;
		}



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
/*	waitpid(pEntrada1, &estado, 0);
	waitpid(pEntrada2, &estado, 0);
	waitpid(pEntrada3, &estado, 0);
*/
	cout << "Terminado proceso..." << pConsola << endl;
	cout << "FIN SIMULACION" << endl;

	return 0;
}
