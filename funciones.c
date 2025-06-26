#include <stdio.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int tam) {
    fflush(stdin);
    fgets(cadena, tam, stdin);
    int len = strlen(cadena);
    if (cadena[len - 1] == '\n')
        cadena[len - 1] = '\0';
}

int menu() {
    int opc;
    printf("\n=== MENU ===\n");
    printf("1. Registrar partido\n");
    printf("2. Mostrar partidos\n");
    printf("3. Mostrar tabla de posiciones\n");
    printf("4. Mostrar goleadores\n");
    printf("5. Mostrar MVP por partido\n");
    printf("6. Salir\n");
    printf(">> ");
    scanf("%d", &opc);
    return opc;
}

void guardarPartidos(struct Partido partidos[], int numPartidos) {
    FILE *f = fopen("partidos.dat", "wb");
    if (f == NULL) {
        printf("Error al guardar los partidos.\n");
        return;
    }
    fwrite(&numPartidos, sizeof(int), 1, f);
    fwrite(partidos, sizeof(struct Partido), numPartidos, f);
    fclose(f);
}

int cargarPartidos(struct Partido partidos[], int *numPartidos) {
    FILE *f = fopen("partidos.dat", "rb");
    if (f == NULL) {
        *numPartidos = 0;
        return 0;
    }
    fread(numPartidos, sizeof(int), 1, f);
    fread(partidos, sizeof(struct Partido), *numPartidos, f);
    fclose(f);
    return 1;
}

void registrarPartido(struct Partido partidos[], int *numPartidos) {
    struct Partido nuevo;
    printf("Ingrese nombre del equipo local: ");
    leerCadena(nuevo.equipoLocal, 30);
    printf("Ingrese nombre del equipo visitante: ");
    leerCadena(nuevo.equipoVisitante, 30);

    printf("Ingrese goles del equipo local: ");
    scanf("%d", &nuevo.golesLocal);
    printf("Ingrese goles del equipo visitante: ");
    scanf("%d", &nuevo.golesVisitante);

    int totalGoles = nuevo.golesLocal + nuevo.golesVisitante;
    nuevo.numGoleadores = 1;
    getchar();

    for (int i = 0; i < nuevo.numGoleadores; i++) {
        printf("Nombre del goleador: ");
        leerCadena(nuevo.goleadores[i].nombre, 30);
        do {
            printf("Goles anotados (max %d): ", totalGoles);
            scanf("%d", &nuevo.goleadores[i].goles);
            if (nuevo.goleadores[i].goles > totalGoles) {
                printf("No puede anotar más goles que el total del partido. Intente nuevamente.\n");
            }
        } while (nuevo.goleadores[i].goles > totalGoles);
        getchar();
    }

    printf("Ingrese el nombre del MVP: ");
    leerCadena(nuevo.mvp, 30);

    partidos[*numPartidos] = nuevo;
    (*numPartidos)++;
    guardarPartidos(partidos, *numPartidos);
    printf("Partido registrado con exito.\n");
}

void mostrarPartidos(struct Partido partidos[], int numPartidos) {
    if (numPartidos == 0) {
        printf("No hay partidos registrados.\n");
        return;
    }
    for (int i = 0; i < numPartidos; i++) {
        printf("\nPartido #%d\n", i + 1);
        printf("%s %d - %d %s\n", partidos[i].equipoLocal, partidos[i].golesLocal,
               partidos[i].golesVisitante, partidos[i].equipoVisitante);
        printf("MVP: %s\n", partidos[i].mvp);
    }
}

void mostrarGoleadores(struct Partido partidos[], int numPartidos) {
    printf("\nGOLEADOR POR PARTIDO\n");
    for (int i = 0; i < numPartidos; i++) {
        printf("Partido %s vs %s\n", partidos[i].equipoLocal, partidos[i].equipoVisitante);
        printf("- %s: %d goles\n", partidos[i].goleadores[0].nombre, partidos[i].goleadores[0].goles);
    }
}

void mostrarMVP(struct Partido partidos[], int numPartidos) {
    printf("\nMVP DE CADA PARTIDO\n");
    for (int i = 0; i < numPartidos; i++) {
        printf("Partido %d: %s\n", i + 1, partidos[i].mvp);
    }
}

int encontrarEquipo(struct Equipo equipos[], int numEquipos, char nombre[]) {
    for (int i = 0; i < numEquipos; i++) {
        if (strcmp(equipos[i].nombre, nombre) == 0)
            return i;
    }
    return -1;
}

void actualizarEquipo(struct Equipo *equipo, int golesFavor, int golesContra, int resultado) {
    equipo->partidosJugados++;
    equipo->golesFavor += golesFavor;
    equipo->golesContra += golesContra;
    if (resultado == 1) {
        equipo->ganados++;
        equipo->puntos += 3;
    } else if (resultado == 0) {
        equipo->empatados++;
        equipo->puntos += 1;
    } else {
        equipo->perdidos++;
    }
}

void ordenarEquipos(struct Equipo equipos[], int numEquipos) {
    for (int i = 0; i < numEquipos - 1; i++) {
        for (int j = 0; j < numEquipos - i - 1; j++) {
            if (equipos[j].puntos < equipos[j + 1].puntos ||
               (equipos[j].puntos == equipos[j + 1].puntos &&
                equipos[j].golesFavor < equipos[j + 1].golesFavor)) {
                struct Equipo temp = equipos[j];
                equipos[j] = equipos[j + 1];
                equipos[j + 1] = temp;
            }
        }
    }
}

void calcularPosiciones(struct Partido partidos[], int numPartidos, struct Equipo equipos[], int *numEquipos) {
    *numEquipos = 0;
    for (int i = 0; i < numPartidos; i++) {
        int idxLocal = encontrarEquipo(equipos, *numEquipos, partidos[i].equipoLocal);
        int idxVisit = encontrarEquipo(equipos, *numEquipos, partidos[i].equipoVisitante);

        if (idxLocal == -1) {
            memset(&equipos[*numEquipos], 0, sizeof(struct Equipo));
            strcpy(equipos[*numEquipos].nombre, partidos[i].equipoLocal);
            idxLocal = (*numEquipos)++;
        }
        if (idxVisit == -1) {
            memset(&equipos[*numEquipos], 0, sizeof(struct Equipo));
            strcpy(equipos[*numEquipos].nombre, partidos[i].equipoVisitante);
            idxVisit = (*numEquipos)++;
        }

        int golesL = partidos[i].golesLocal;
        int golesV = partidos[i].golesVisitante;
        int resLocal = (golesL > golesV) ? 1 : (golesL == golesV ? 0 : -1);
        int resVisit = (golesV > golesL) ? 1 : (golesL == golesV ? 0 : -1);

        actualizarEquipo(&equipos[idxLocal], golesL, golesV, resLocal);
        actualizarEquipo(&equipos[idxVisit], golesV, golesL, resVisit);
    }
    ordenarEquipos(equipos, *numEquipos);
}

void mostrarPosiciones(struct Equipo equipos[], int numEquipos) {
    printf("\nTABLA DE POSICIONES\n");
    printf("%-20s PJ  PG  PE  PP  GF  GC  PTS\n", "Equipo");
    for (int i = 0; i < numEquipos; i++) {
        printf("%-20s %2d  %2d  %2d  %2d  %2d  %2d  %3d\n",
               equipos[i].nombre,
               equipos[i].partidosJugados,
               equipos[i].ganados,
               equipos[i].empatados,
               equipos[i].perdidos,
               equipos[i].golesFavor,
               equipos[i].golesContra,
               equipos[i].puntos);
    }
}
