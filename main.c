#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main() {
    struct Partido partidos[100];
    struct Equipo equipos[50];
    int numPartidos = 0;
    int numEquipos = 0;
    int opcion;

    // Cargar partidos guardados
    cargarPartidos(partidos, &numPartidos);

    do {
        opcion = menu();

        switch(opcion) {
            case 1:
                registrarPartido(partidos, &numPartidos);
                break;
            case 2:
                mostrarPartidos(partidos, numPartidos);
                break;
            case 3:
                calcularPosiciones(partidos, numPartidos, equipos, &numEquipos);
                mostrarPosiciones(equipos, numEquipos);
                break;
            case 4:
                mostrarGoleadores(partidos, numPartidos);
                break;
            case 5:
                mostrarMVP(partidos, numPartidos);
                break;
            case 6:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
        }

    } while(opcion != 6);

    return 0;
}
