
#include "Log.h"

Log* Log::instance = NULL;
const long long Log::microsegundosInicial = getMicrosegundos();
const string Log::nombreArchivo = string( PATH_LOG ) + getFecha() + getHora() + string( EXTENSION_LOG );

Log::Log()
 : modoDebug ( false ),
   logFile ( (char*) nombreArchivo.c_str() )
{

	cout.setf( ios::fixed );
	cout.precision( PRECISION_SEGUNDOS );

}



Log* Log::getInstance()
{
	if ( instance == NULL )
		instance = new Log();
	return instance;
}

void Log::destruir ()
{
	if ( instance != NULL ) {
		//Log::getInstance()->loguear("Log: destruido.");
		delete ( instance );
		instance = NULL;
	}
}

void Log::setModoDebug(bool modoDebug)
{
	this->modoDebug = modoDebug;
}

void Log::loguear(string mensaje)
{
	if ( this->modoDebug )
	{
		if (logFile.tomarLock() == 0)
		{
			long long microsegundos = getMicrosegundos() - microsegundosInicial;
			float segundos = (float) microsegundos / 1000000;
			stringstream s;
			s.setf( ios::fixed );
			s.precision( PRECISION_SEGUNDOS );
			s << "Proceso: "<< getpid() << " " << segundos << "s: " << mensaje << endl;


			if (logFile.escribir(s.str().c_str(), strlen(s.str().c_str())) == -1) {
				cerr << "Ha ocurrido un error al escibir el mensaje" << endl;
				cerr << "\"" << s << "\"" << endl;
			}

			logFile.liberarLock();
		}
		else {
			cerr << "Proceso: " << getpid() << ": Error al abrir el archivo: " << nombreArchivo << ". Mensaje: " << mensaje << endl;
		}
	}
}

void Log::imprimir( string mensaje )
{
	long long microsegundos = getMicrosegundos() - microsegundosInicial;
	float segundos = ( float ) microsegundos / 1000000;

	cout << segundos <<  "s: " << mensaje << endl;
}

