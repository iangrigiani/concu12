#include "Simulador.h"
#include <sys/wait.h>
#include "MemoriaCompartida.h"
#include "Administracion.h"
#include <vector>
#include <string.h>
#include <stdio.h>
#include <sstream>

using namespace std;
int main() {

	Simulador * simulador;
	pid_t pConsola = fork();
	pid_t pEntrada1;
	pid_t pEntrada2;
	pid_t pEntrada3;

	int cantPosiciones = 50;


	//cout << "Creada la consola en el pid " << pConsola << endl;
	vector< MemoriaCompartida<Posicion> > vectorMemoria;


	MemoriaCompartida<Posicion> memoria;
	if( pConsola != 0)
	{

		int i;
		FILE * tmpConcu;
		int estadoMemoria = SHM_OK;
		for (i=0;i<cantPosiciones;i++){
			stringstream nombreArchivo;

			// Creo archivo temporal
			nombreArchivo << "/tmp/tmpConcuuuu";
			nombreArchivo << i;
			cout << "nombre archivo: " << nombreArchivo.str() << endl;

			tmpConcu = fopen(nombreArchivo.str().c_str(),"w");
			fclose(tmpConcu);

			// Creo la memoria asociada al archivo temporal
			estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );
			vectorMemoria.push_back(memoria);

			nombreArchivo.flush();

			// TODO: Agregar validacion por si no crea bien la memoria
		}

//		Administracion administracion;
//		administracion.setCostoHora(25);

//		if ( estadoMemoria == SHM_OK ) {

			// escribe un dato para el hijo
		Posicion posicion;
		for (i=0;i<cantPosiciones;i++){
			cout << "Padre: escribo el numero " << i << endl;
			posicion.setNumero(i);
			posicion.setEstadoOcupado(true);
			memoria = vectorMemoria[i];
			memoria.escribir(posicion);

		}

		for (i=0;i<cantPosiciones;i++){
			memoria = vectorMemoria[i];
			Posicion posi = memoria.leer();
			cout << "nmr: " << posi.getNumero() << " estado " << posi.getEstadoOcupado() << endl;
		}

			// libera la memoria
			//TODO tiene que ir donde muera la consola
			//memoria.liberar();
			/*
		}
		else
		{
			cout << "Padre: error en memoria compartida: " << estadoMemoria << endl;
		}
*/
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
		sleep ( 20 );

		int i;

		int estadoMemoria = SHM_OK;
		for (i=0;i<cantPosiciones;i++){

			Posicion posicion;
			stringstream nombreArchivo;
			// Creo archivo temporal

			nombreArchivo << "/tmp/tmpConcuuuu";
			nombreArchivo << i;
			cout << "Hijo: nombre archivo: " << nombreArchivo.str() << endl;


			// Creo la memoria asociada al archivo temporal
			estadoMemoria = memoria.crear ( (char*)nombreArchivo.str().c_str(),'R' );

			posicion = (Posicion)memoria.leer();

			cout << "valor posicion " << posicion.getNumero() << " estado: " << posicion.getEstadoOcupado() << endl;
			// TODO: Agregar validacion por si no crea bien la memoria
			memoria.liberar () ;
		}




//		cout<<"Antes del IF"<<endl;
//		if ( estadoMemoria2 == SHM_OK ) {
//			cout<<"Estoy en el IF"<<endl;
//			Administracion * administracion2 = (Administracion *)memoria2.leer();
//
//			cout << "Hijo: leo el numero " << (administracion2)->getCostoHora() << endl;
//		} else {
//			cout<<"Estoy en el ELSE"<<endl;
//			cout << "Hijo: error en memoria compartida: " << estadoMemoria2 << endl;
//		}



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

	int i;
	for (i=0;i<cantPosiciones;i++){
		memoria = vectorMemoria[i];
		memoria.liberar();
	}

	cout << "Terminado proceso..." << pConsola << endl;
	cout << "FIN SIMULACION" << endl;

	return 0;
}
