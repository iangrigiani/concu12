#ifndef MENSAJE_H_
#define MENSAJE_H_

#define	PETICION	1
#define	RESPUESTA	2
#define TEXTO_SIZE	255


typedef struct mensaje {
	long mtype;
	int from;
	char msj[TEXTO_SIZE];
} mensaje;

#endif /* MENSAJE_H_ */
