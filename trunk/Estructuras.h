
#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_


struct Parametros {
        int lugaresDisponibles;
        float costoHora;
        int tiempoSimulacion;
        int cantidadEstacionamientos;
};



enum ParametrosInvalidos {

        LUGARES_DISPONIBLES_INVALIDO = 0,
        COSTO_HORA_INVALIDO = 1,
        TIEMPO_INVALIDO = 2,
        CANTIDAD_INVALIDA = 3,
        CANTIDAD_ESTACIONAMIENTOS = 4

};


#endif /* ESTRUCTURAS_H_ */
