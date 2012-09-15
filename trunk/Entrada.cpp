#include "Entrada.h"

Entrada::Entrada() {
	// TODO Auto-generated constructor stub

}

Entrada::~Entrada() {
	// TODO Auto-generated destructor stub
}

/**
	 * Implementacion del metodo definido en la clase Lector
	 * Registra la entrada de una persona al edificio
	 * Devuelve true si la persona pudo acceder al edificio.
	 * @param persona
	 * @return boolean
	 */

//TODO: implementacion de metodo abstracto en barrera
/*	public synchronized   boolean registrarEntradaDeUnaPersona(Persona persona){

		Date entrada = new Date();
		Registro registro = new Registro(entrada);

		if(!(persona.haEntradoAlEdificio())){
			registro.setHaAccedido(true);
			if(!this.getHistorial().getHistorialDePersonas().containsKey(persona)){
				this.getHistorial().agregarNuevaPersonaAlHistorial(persona,registro);
				}
			else{
				this.getHistorial().agregarNuevoRegistroAlHistorial(persona, registro);
				}
		}
		else{
			RegistroIndebido acceso= new RegistroIndebido(persona,entrada);
			this.getHistorial().agregarNuevoAccesoIndebido(acceso);
			}

		return (registro.haAccedido());
	}
*/
