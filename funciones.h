#ifndef FUNCIONES_H
#define FUNCIONES_H

struct Goleador {
    char nombre[30];
    int goles;
};

struct Partido {
    char equipoLocal[30];
    char equipoVisitante[30];
    int golesLocal;
    int golesVisitante;
    struct Goleador goleadores[1];
    int numGoleadores;
    char mvp[30];
};

struct Equipo {
    char nombre[30];
    int partidosJugados;
    int ganados;
    int empatados;
    int perdidos;
    int golesFavor;
    int golesContra;
    int puntos;
};

void leerCadena(char *cadena, int tam);
int menu();
void guardarPartidos(struct Partido partidos[], int numPartidos);
int cargarPartidos(struct Partido partidos[], int *numPartidos);
void registrarPartido(struct Partido partidos[], int *numPartidos);
void mostrarPartidos(struct Partido partidos[], int numPartidos);
void mostrarGoleadores(struct Partido partidos[], int numPartidos);
void mostrarMVP(struct Partido partidos[], int numPartidos);
int encontrarEquipo(struct Equipo equipos[], int numEquipos, char nombre[]);
void actualizarEquipo(struct Equipo *equipo, int golesFavor, int golesContra, int resultado);
void ordenarEquipos(struct Equipo equipos[], int numEquipos);
void calcularPosiciones(struct Partido partidos[], int numPartidos, struct Equipo equipos[], int *numEquipos);
void mostrarPosiciones(struct Equipo equipos[], int numEquipos);

#endif
