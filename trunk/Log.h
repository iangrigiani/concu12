
#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <string>


#include "LockFile.h"
#include "Utils.h"
#include "Constantes.h"

using namespace std;

class Log {

private:

	static Log* instance;
	static const long long microsegundosInicial;
	static const string nombreArchivo;
	bool modoDebug;
	LockFile logFile;
	Log();

public:

	static Log* getInstance ();
	static void destruir ();
	void setModoDebug( bool modoDebug );
	void loguear( string mensaje );
	void imprimir( string mensaje );

};

#endif /* LOG_H_ */
