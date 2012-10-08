#include "Estacionamiento.h"
#include "Log.h"
#include "Estructuras.h"

using namespace std;



void informarParametrosInvalidos(enum ParametrosInvalidos error) {
        switch (error) {
                case CANTIDAD_INVALIDA: {
                        Log::getInstance()->loguear("La cantidad de parametros es invalida.");
                        break;
                }
                case LUGARES_DISPONIBLES_INVALIDO: {
                        Log::getInstance()->loguear("La cantidad de lugares disponibles ingresados es invalida. Debe ser mayor a 0.");
                        break;
                }
                case COSTO_HORA_INVALIDO: {
                        Log::getInstance()->loguear("El costo por hora ingresado es invalido. Debe ser mayor a 0.");
                        break;
                }

                case TIEMPO_INVALIDO: {
                        Log::getInstance()->loguear("La duracion de la simulación ingresada es invalida. Debe ser mayor a 0.");
                        break;
                }
        }
}




int validarParametros(int argc, char* argv[], Parametros* parametros){

        int retorno = 0;

        if (argc != 4) {
                cerr << "Parametros invalidos" << endl;
                cerr << "Uso: " << argv[0] << " [Lugares Disponibles] [Costo Hora] [Tiempo]" << endl;
                informarParametrosInvalidos(CANTIDAD_INVALIDA);
                retorno = -1;
        }
        else {
                int i = 1;

                int c = atoi(argv[i++]);
                if (c > 0) parametros->lugaresDisponibles = c;
                else {
                        informarParametrosInvalidos(LUGARES_DISPONIBLES_INVALIDO);
                        retorno = -1;
                }

                c = atoi(argv[i++]);
                if (c > 0) parametros->costoHora= c;
                else {
                        informarParametrosInvalidos(COSTO_HORA_INVALIDO);
                        retorno = -1;
                }

                c = atoi(argv[i++]);
                if (c > 0) parametros->tiempoSimulacion = c;
                else {
                        informarParametrosInvalidos(TIEMPO_INVALIDO);
                        retorno = -1;
                }
        }

        return retorno;
}



int main(int argc, char *argv[])
{
	 int retorno;

	// Se loguea siempre
	Log::getInstance()->setModoDebug( true );

	Parametros parametros;
	if ( ( retorno = validarParametros( argc, argv, &parametros ) ) == 0 )
	{
			Log::getInstance()->loguear( string ( argv[ 0 ] ) + " " + getStringParametros( &parametros ) );

			Estacionamiento estacionamiento;

			estacionamiento.run(parametros.lugaresDisponibles,parametros.costoHora,parametros.tiempoSimulacion);

	}

	Log::destruir();

	return retorno;
}






