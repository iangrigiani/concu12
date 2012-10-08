
#include "Utils.h"

//Random usado para indice de vectores que van de 0 a n-1
int calcularRandom( int nroMaximo ) {
	int resultado = rand() % nroMaximo;
	return resultado;
}

int calcularRandom( int nroMinimo, int nroMaximo ) {
	int resultado = nroMinimo + rand() % ( nroMaximo - nroMinimo + 1 );
	return resultado;
}

std::string intToString( int numero ){
   std::stringstream ss;
   ss << numero;
   return ss.str();
}

long long getMicrosegundos(){
	struct timeval tv;
	gettimeofday( &tv , NULL );
	return ( long long ) tv.tv_sec * 1000000 + tv.tv_usec;
}

int getMilisegundos(){
	struct timeval tv;
	gettimeofday( &tv , NULL );
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

std::string completarNumero( int numero ){
	std::stringstream ss;
	ss << numero;
	std::string s = ss.str();
	if ( s.length() == 1 ){
		s = "0" + s;
	}
	return s;
}

std::string getFecha(){
	time_t t = time( 0 );
	struct tm* now = localtime( &t );
	std::stringstream ss;
	ss 	<< (now->tm_year + 1900)
		<< completarNumero( now->tm_mon + 1 )
		<< completarNumero( now->tm_mday );
	return ss.str();
}

std::string getHora(){
	time_t t = time( 0 );
	struct tm* now = localtime( &t );
	std::stringstream ss;
	ss 	<< completarNumero( now->tm_hour )
		<< completarNumero( now->tm_min )
		<< completarNumero( now->tm_sec );
	return ss.str();
}


std::string getStringParametros( Parametros* parametros ){
	return intToString( parametros->lugaresDisponibles ) + " "
			+ intToString( parametros->costoHora ) + " "
			+ intToString( parametros->tiempoSimulacion );
}



