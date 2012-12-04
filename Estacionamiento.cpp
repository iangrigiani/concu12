#include "Estacionamiento.h"



Estacionamiento::Estacionamiento(int numero, int nroPpal) {
	this->numeroEstacionamiento = numero;
	this->numeroPpal = nroPpal;

}


Estacionamiento::~Estacionamiento()
{
}


void Estacionamiento::crearArchivoTemporalAdministracion()
{

	FILE * tmpFile;
	stringstream mensaje;

	// Creo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;
	archivoAdministracion<<ESTACIONAMIENTO;
	archivoAdministracion<<this->numeroEstacionamiento;

	tmpFile = fopen(archivoAdministracion.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Creo el archivo temporal: " << archivoAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

	// Creo archivos temporales para semaforos
	stringstream archivoSmfAdministracion;
	archivoSmfAdministracion << ARCHIVO_SEMAFORO_ADMINISTRACION;
	archivoSmfAdministracion << ESTACIONAMIENTO;
	archivoSmfAdministracion << this->numeroEstacionamiento;

	tmpFile = fopen(archivoSmfAdministracion.str().c_str(),"w");
	fclose(tmpFile);

	mensaje.str("");
	mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Creo el archivo temporal: " << archivoSmfAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

}

void Estacionamiento::eliminarArchivoTemporalAdministracion()
{

	stringstream mensaje;

	// Destruyo archivo temporal administracion
	stringstream archivoAdministracion;
	archivoAdministracion << ARCHIVO_ADMINISTRACION;
	archivoAdministracion<<ESTACIONAMIENTO;
	archivoAdministracion<<this->numeroEstacionamiento;

	remove(archivoAdministracion.str().c_str());
	mensaje.str("");
	mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Destruyo el archivo temporal: " << archivoAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

	// Destruyo archivos temporales de semaforos
	stringstream archivoSmfAdministracion;
	archivoSmfAdministracion << ARCHIVO_SEMAFORO_ADMINISTRACION;
	archivoSmfAdministracion << ESTACIONAMIENTO;
	archivoSmfAdministracion << this->numeroEstacionamiento;

	remove(archivoSmfAdministracion.str().c_str());
	mensaje.str("");
	mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Destruyo el archivo temporal: " << archivoSmfAdministracion.str();
	Log::getInstance()->loguear(mensaje.str());

}


bool Estacionamiento::crearMemoriaCompartidaAdministracion(float costoHora)
{

	Administracion administracion;
	stringstream nombreArchivo;
	stringstream mensajeLog;

	bool pudoCrear = false;

	administracion.setCostoHora(costoHora);

	// Creo archivo temporal
	nombreArchivo << ARCHIVO_ADMINISTRACION;
	nombreArchivo << ESTACIONAMIENTO;
	nombreArchivo << this->numeroEstacionamiento;

	// Creo la memoria asociada al archivo temporal
	int estadoMemoria = this->administracion.crear ( (char*)nombreArchivo.str().c_str(),'R' );

	if(estadoMemoria == SHM_OK)
	{
		mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Memoria Compartida : escribo el costo hora del estacionamiento costo hora = " << administracion.getCostoHora();
		Log::getInstance()->loguear(mensajeLog.str());

		pudoCrear = true;
		this->administracion.escribir(administracion);
	}
	else
	{
		mensajeLog<<"Estacionamiento " << this->numeroEstacionamiento << " - Error al inicializar la administracion de memoria compartida en el estacionamiento";
		Log::getInstance()->loguear(mensajeLog.str());
	}

	return pudoCrear;
}




void Estacionamiento::run(int cantidadDeLugares, float costoHora, int tiempoEjecucion)
{
	//Ver que hacer con las iniciales
	bool creado = crearIniciales(costoHora);

	stringstream msjLog;
	if ( creado ){

		// Genero un vector dinamico y lo inicializo para evitar los warning de compilador y de valgrind
		char recibido[BUFFSIZE];
		int q;

		for (q=0; q<BUFFSIZE; q++) {
			recibido[q] = 0;
		}

		//Proceso consola que va a atender consultas
		this->pConsola = fork();

		if( this->pConsola != 0)
		{

			this->pEntrada1 = fork();

			if( this->pEntrada1 == 0)
			{
				//Proceso entrada 1
				msjLog.str("");
				msjLog << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el proceso entrada 1";
				Log::getInstance()->loguear(msjLog.str());

				correrSimuladorEntrada(1,this->pipeEntrada1,cantidadDeLugares);
			}
			else
			{
				this->pEntrada2 = fork();

				if( this->pEntrada2 == 0)
				{
					//Proceso entrada 2
					msjLog.str("");
					msjLog << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el proceso entrada 2";
					Log::getInstance()->loguear(msjLog.str());

					correrSimuladorEntrada(2,this->pipeEntrada2,cantidadDeLugares);
				}
				else
				{

					this->pEntrada3 = fork();

					if( this->pEntrada3 == 0)
					{
						//Proceso entrada 3
						msjLog.str("");
						msjLog << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el proceso entrada 3";
						Log::getInstance()->loguear(msjLog.str());

						correrSimuladorEntrada(3,this->pipeEntrada3,cantidadDeLugares);
					}
					else
					{
						this->pSalida1 = fork();

						if (this->pSalida1 == 0)
						{
							//Proceso salida 1
							msjLog.str("");
							msjLog << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el proceso salida 1";
							Log::getInstance()->loguear(msjLog.str());

							correrSimuladorSalida(1,this->pipeSalida1,cantidadDeLugares);
						}
						else
						{
							this->pSalida2 = fork();

							if(this->pSalida2 == 0)
							{
								//Proceso salida 2
								msjLog.str("");
								msjLog << "Estacionamiento " << this->numeroEstacionamiento << " - Soy el proceso salida 2";
								Log::getInstance()->loguear(msjLog.str());

								correrSimuladorSalida(2,this->pipeSalida2,cantidadDeLugares);

							}
							else
							{

								Cola<mensaje> colaMensajes( (char*)ARCHIVO_COLA_MENSAJES,'a');

								mensaje datoRecibir;
								mensaje datoEnviar;

								datoEnviar.from = this->numeroEstacionamiento;
								datoEnviar.mtype = this->numeroPpal;

								// Proceso estacionamiento principal (funciona como servidor de mensajes)

								bool salir = false;
								int cantidadTotalEntradas = 0;

								while (!salir)
								{
									int bytes = this->pipePpal.leer((char*)recibido, BUFFSIZE);

									// Me aseguro que solo se procesen los bytes recibidos y no la posible basura existente en el array
									string stringRecibido(recibido);
									stringRecibido = stringRecibido.substr(0, bytes);

									string token;
									stringstream stream(stringRecibido);
									while( getline(stream, token, '?') ) {

										char * recibidoParcial = (char*)token.c_str();

										stringstream mensaje;
										mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - acabo de recibir: " << recibidoParcial;
										Log::getInstance()->loguear(mensaje.str());

										stringstream retorno;

										switch (recibidoParcial[0])
										{
											case 'p': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");

												mensaje.str("");
												mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Saco del vector de posiciones libres la posicion: " << token;
												Log::getInstance()->loguear(mensaje.str());

												int pos = atoi(token);

												retorno << "p|" << pos;
												strcpy(datoEnviar.msj,retorno.str().c_str());

												colaMensajes.escribir(datoEnviar);

												break;
											}
											case 'd': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");

												mensaje.str("");
												mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Agrego al  vector de posiciones libres la posicion: " << token;
												Log::getInstance()->loguear(mensaje.str());

												int pos = atoi(token);

												retorno << "d|" << pos;
												strcpy(datoEnviar.msj,retorno.str().c_str());

												colaMensajes.escribir(datoEnviar);

												break;
											}
											case 's': {
												char * token = strtok(recibidoParcial, "|");
												token = strtok(NULL, "|");
												int pos = atoi(token);
												int numeroSalida = this->getSalidaAleatoria();

												mensaje.str("");
												mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Auto sale del estacionamiento por la salida " << numeroSalida << " y deja la posicion " << pos;
												Log::getInstance()->loguear(mensaje.str());

												retorno << "s|" << pos;
												if(numeroSalida == 1){
													this->pipeSalida1.escribir((char*)retorno.str().c_str(),retorno.str().length());
												} else {
													this->pipeSalida2.escribir((char*)retorno.str().c_str(),retorno.str().length());
												}
												break;
											}
											case 'f': {
													char * token = strtok(recibidoParcial, "|");
													token = strtok(NULL, "|");
													int numeroEntrada = atoi(token);

													cantidadTotalEntradas+=numeroEntrada;

													mensaje.str("");
													mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - La suma del numero de entradas total es " << cantidadTotalEntradas;
													Log::getInstance()->loguear(mensaje.str());

													if(cantidadTotalEntradas == CANTIDAD_TOTAL_ENTRADAS)
													{
														retorno << "f";
														this->pipeSalida1.escribir((char*)retorno.str().c_str(),retorno.str().length());
														this->pipeSalida2.escribir((char*)retorno.str().c_str(),retorno.str().length());

														strcpy(datoEnviar.msj, retorno.str().c_str());
														colaMensajes.escribir(datoEnviar);

														salir = true;
													}
													break;
											}
											default: {
												if ( recibidoParcial[0] > '0' && recibidoParcial[0] < '4') {

													// Le paso 'r' de "random"
													strcpy(datoEnviar.msj, "r");

													colaMensajes.escribir(datoEnviar);
													colaMensajes.leer(this->numeroEstacionamiento, &datoRecibir);

													int nroPosicion = atoi(datoRecibir.msj);

													mensaje.str("");
													mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - Entrada " << recibidoParcial[0] << " pide una posicion, le envio: " << nroPosicion;
													Log::getInstance()->loguear(mensaje.str());

													retorno << nroPosicion;
													if (recibidoParcial[0] == '1'){
														this->pipeEntrada1.escribir((char*)retorno.str().c_str(),retorno.str().length());
													} else {
														if (recibidoParcial[0] == '2') {
															this->pipeEntrada2.escribir((char*)retorno.str().c_str(),retorno.str().length());
														} else {
															this->pipeEntrada3.escribir((char*)retorno.str().c_str(),retorno.str().length());
														}
													}
												}
												break;
											}
										}
									}
								}

								finalizarProcesos();

								liberarRecursos(cantidadDeLugares);

								finalizarPipes();
							}
						}
					}
				}
			}
		} else {
			// Proceso consulta
			bool salir = false;

			// Creo una cola para las consultas, dedicada a la comunicacion con la administracion general
			Cola<mensaje> colaConsultas( (char*)ARCHIVO_COLA_MENSAJES_CONSULTAS,'a');

			while (!salir)
			{
				mensaje datoLeido;
				mensaje datoEnviado;
				colaConsultas.leer(this->numeroEstacionamiento, &datoLeido);


				datoEnviado.from = this->numeroEstacionamiento;
				datoEnviado.mtype = datoLeido.from;

				stringstream mensaje;
				mensaje << "Estacionamiento " << this->numeroEstacionamiento << " - acabo de recibir: " << datoLeido.msj;
				Log::getInstance()->loguear(mensaje.str());

				stringstream retorno;

				switch (datoLeido.msj[0])
				{

					case 'a': {
						int cantidad = this->obtenerCantidadActualDeAutos();

						retorno << cantidad;
						strcpy(datoEnviado.msj,(char*)retorno.str().c_str());

						// Aca tiene que mandar la rta a la cola de consultas
						colaConsultas.escribir(datoEnviado);

						break;
					}
					case 'm': {

						float monto = this->obtenerMontoRecaudado();

						retorno << monto;

						strcpy(datoEnviado.msj, retorno.str().c_str());
						colaConsultas.escribir(datoEnviado);

						break;
					}
					case 'q': {
						salir = true;
						break;
					}
					default: break;
				}
			}
		}
	} else {

		cout << "Se ha producido un error. Revise el archivo de log para mas informacion" << endl;
	}
}



bool Estacionamiento::crearIniciales(int costoHora){

	crearArchivoTemporalAdministracion();

	stringstream archivoSmfAdministracion;
	archivoSmfAdministracion << ARCHIVO_SEMAFORO_ADMINISTRACION;
	archivoSmfAdministracion << ESTACIONAMIENTO;
	archivoSmfAdministracion << this->numeroEstacionamiento;

	Semaforo smfAdmin(archivoSmfAdministracion.str().c_str(),1,'a');
	this->smfAdministracion = smfAdmin;

	stringstream mensajeLog;
	mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Creo semaforo id: " << this->smfAdministracion.getId() << " (admin)";
	Log::getInstance()->loguear(mensajeLog.str());

	bool pudoCrear = false;
	if ( crearMemoriaCompartidaAdministracion(costoHora) )
		pudoCrear = true;
	else {
		liberarMemoriaCompartidaAdministracion();

		// Elimino el semaforo de la administracion
		mensajeLog.str("");
		mensajeLog << "Estacionamiento " << this->numeroEstacionamiento << " - Libero semaforo id: " << this->smfAdministracion.getId() << " (admin)";
		Log::getInstance()->loguear(mensajeLog.str());
		this->smfAdministracion.eliminar();
	}

	return pudoCrear;
}


void Estacionamiento::correrSimuladorEntrada(int numeroEntrada,Pipe pipeEntrada, int cantidadPosiciones)
{
	SimuladorEntrada simuladorEntrada(this->numeroEstacionamiento,numeroEntrada,cantidadPosiciones);

	simuladorEntrada.simular(pipeEntrada, this->pipePpal, this->numeroEstacionamiento);

	pipeEntrada.cerrar();
}


void Estacionamiento::correrSimuladorSalida(int numeroSalida,Pipe pipeSalida, int cantidadPosiciones)
{
	SimuladorSalida simuladorSalida(this->numeroEstacionamiento,numeroSalida,cantidadPosiciones);

	simuladorSalida.simular(pipeSalida, this->pipePpal);

	pipeSalida.cerrar();
}


int Estacionamiento::getSalidaAleatoria()
{
	return calcularRandom(1,2);
}




int Estacionamiento::obtenerCantidadActualDeAutos()
{
	stringstream mensajeLog;

	// Tomo el semaforo para lectura
	this->smfAdministracion.p();

	mensajeLog << "Estacionamiento "<<this->numeroEstacionamiento<< " - Tome semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	Administracion administracion = (Administracion)this->administracion.leer();

	mensajeLog.str("");
	mensajeLog << "Estacionamiento " <<this->numeroEstacionamiento <<" - Libero semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();

	return administracion.getCantidadDeAutos();
}

float Estacionamiento::obtenerMontoRecaudado()
{
	stringstream mensajeLog;
	// Tomo el semaforo para lectura
	this->smfAdministracion.p();

	mensajeLog << "Estacionamiento "<<this->numeroEstacionamiento<<" - Tome semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	//cout << "PPAL: Acabo de tomar el semaforo de admin " << endl;
	Administracion administracion = (Administracion)this->administracion.leer();

	mensajeLog.str("");
	mensajeLog << "Estacionamiento "<<this->numeroEstacionamiento<<" - Libero semaforo administracion";
	Log::getInstance()->loguear(mensajeLog.str());

	// Libero el semaforo
	this->smfAdministracion.v();
	//cout << "PPAL: Libero el semaforo de admin " << endl;
	return administracion.getImporteRecaudado();
}

void Estacionamiento::liberarRecursos(int cantidadLugares){

	// Libero memoria compartida de administracion
	liberarMemoriaCompartidaAdministracion();

	//Se eliminan los archivos temporales creados
	eliminarArchivoTemporalAdministracion();

	// Elimino el semaforo de la administracion
	stringstream mensajeLog;
	mensajeLog << "Estacionamiento " <<this->numeroEstacionamiento<<" - Libero semaforo id: " << this->smfAdministracion.getId() << " (admin)";
	Log::getInstance()->loguear(mensajeLog.str());
	this->smfAdministracion.eliminar();
}


void Estacionamiento::liberarMemoriaCompartidaAdministracion(){
	//Se libera la memoria compartida correspondiente a la administracion.
	this->administracion.liberar();
}

void Estacionamiento::finalizarProcesos(){
	stringstream mensajeFinalizacion;
	int estado;

	waitpid(this->pEntrada1, &estado, 0);
	waitpid(this->pEntrada2, &estado, 0);
	waitpid(this->pEntrada3, &estado, 0);
	waitpid(this->pSalida1, &estado,0);
	waitpid(this->pSalida2, &estado,0);
	waitpid(this->pConsola, &estado, 0);

	mensajeFinalizacion << "Estacionamiento " << this->numeroEstacionamiento << " - Terminado proceso: " << pConsola << " (consola)";
	Log::getInstance()->loguear(mensajeFinalizacion.str());

	mensajeFinalizacion.str("");
	mensajeFinalizacion << "Estacionamiento " << this->numeroEstacionamiento << " - FIN SIMULACION";
	Log::getInstance()->loguear(mensajeFinalizacion.str());
}

void Estacionamiento::finalizarPipes(){
	this->pipeEntrada1.cerrar();
	this->pipeEntrada2.cerrar();
	this->pipeEntrada3.cerrar();

	this->pipeSalida1.cerrar();
	this->pipeSalida2.cerrar();

	this->pipePpal.cerrar();
}


