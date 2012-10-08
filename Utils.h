#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include "Estructuras.h"

int calcularRandom( int nroMaximo );

int calcularRandom( int nroMinimo, int nroMaximo );

std::string intToString( int number );

long long getMicrosegundos();

int getMilisegundos();

std::string getFecha();

std::string getHora();

std::string getStringParametros( Parametros* parametros );



#endif /* UTILS_H_ */
