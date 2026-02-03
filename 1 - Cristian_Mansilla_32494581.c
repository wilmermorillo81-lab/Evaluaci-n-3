/*Cristian Mansilla 32.494.581*/
/* 02/02/2026 */
#include <stdio.h>

int main() {
    int opcion;
    struct Estudiante {
        char Nombre[50];
        char Cedula[30];
        char Carrera[30];
        int Asistencias;
    };

    printf("|----------------------------------------------------------------------|\n");
    printf("|--- Bienvenido al Sistema de Registro de Asistencia de Estudiantes ---|\n");
    printf("|----------------------------------------------------------------------|\n");

    printf("\n|--- Que desea hacer? ---|\n");
    printf("1. Registrar Estudiante\n");
    printf("2. Marcar Asistencia del Dia\n");
    printf("3. Mostrar Reporte y Porcentajes de Asistencia\n");
    printf("4. Guardar Cambios\n");
    printf("5. Calcular total de Asistencias\n");
    printf("6. Salir del Programa\n");
    printf("\nSeleccione una opcion (1-6): ");
    scanf("%d, &opcion");


return 0;
}