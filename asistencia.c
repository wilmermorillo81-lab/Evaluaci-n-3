#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiciones constantes
#define MAX_ESTUDIANTES 50
#define NOMBRE_ARCHIVO "registro_asistencia.txt"

// Estructura para almacenar la información del estudiante
typedef struct {
    char nombre[50];
    char cedula[20];
    int dias_asistidos;
    int dias_totales_clase; // Cuántos días se ha tomado asistencia desde que se registró
} Estudiante;

// Variables globales
Estudiante listaEstudiantes[MAX_ESTUDIANTES];
int cantidadEstudiantes = 0;

// --- Prototipos de funciones ---
void registrarEstudiante();
void marcarAsistencia();
void mostrarReporte();
void guardarDatos();
void cargarDatos();
int calcularTotalAsistenciasRecursiva(int index); // Requerimiento 5
void limpiarBuffer();

// --- Función Principal ---
int main() {
    int opcion;

    // Intentamos cargar datos previos al iniciar
    cargarDatos();

    do {
        printf("\n--- SISTEMA DE REGISTRO DE ASISTENCIA ---\n");
        printf("1. Registrar Estudiante\n");
        printf("2. Marcar Asistencia del Día\n");
        printf("3. Mostrar Reporte y Porcentajes\n");
        printf("4. Guardar Cambios\n");
        printf("5. Calcular Total de Asistencias\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        limpiarBuffer(); // Consumir el salto de línea pendiente

        switch (opcion) {
            case 1:
                registrarEstudiante();
                break;
            case 2:
                marcarAsistencia();
                break;
            case 3:
                mostrarReporte();
                break;
            case 4:
                guardarDatos();
                break;
            case 5:
                printf("\n--- Total Global de Asistencias ---\n");
                // Llamada a la función recursiva comenzando desde el índice 0
                int total = calcularTotalAsistenciasRecursiva(0);
                printf("El numero total acumulado de asistencias de todos los alumnos es: %d\n", total);
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);

    return 0;
}

// --- Implementación de Funciones ---

// 1. Registrar hasta 50 estudiantes
void registrarEstudiante() {
    if (cantidadEstudiantes >= MAX_ESTUDIANTES) {
        printf("Error: Se ha alcanzado el limite de %d estudiantes.\n", MAX_ESTUDIANTES);
        return;
    }

    Estudiante nuevo;
    
    printf("\nIngrese Nombre del estudiante: ");
    // Leemos la línea completa incluyendo espacios
    scanf("%[^\n]", nuevo.nombre);
    limpiarBuffer();

    printf("Ingrese Cedula del estudiante: ");
    scanf("%[^\n]", nuevo.cedula);
    limpiarBuffer();

    // Inicializamos contadores
    nuevo.dias_asistidos = 0;
    nuevo.dias_totales_clase = 0;

    listaEstudiantes[cantidadEstudiantes] = nuevo;
    cantidadEstudiantes++;
    
    printf("Estudiante registrado exitosamente.\n");
}

// 2. Marcar asistencia diaria
void marcarAsistencia() {
    if (cantidadEstudiantes == 0) {
        printf("\nNo hay estudiantes registrados para tomar asistencia.\n");
        return;
    }

    printf("\n--- Tomando Asistencia del Dia ---\n");
    printf("Responda con 1 para PRESENTE o 0 para AUSENTE.\n");

    for (int i = 0; i < cantidadEstudiantes; i++) {
        int presencia = -1;
        while (presencia != 0 && presencia != 1) {
            printf("Estudiante: %s (C.I. %s) esta presente? [1/0]: ", 
                   listaEstudiantes[i].nombre, listaEstudiantes[i].cedula);
            scanf("%d", &presencia);
            limpiarBuffer();
        }

        // Aumentamos el contador de días evaluados para este alumno
        listaEstudiantes[i].dias_totales_clase++;
        
        // Si vino (1), sumamos asistencia
        if (presencia == 1) {
            listaEstudiantes[i].dias_asistidos++;
        }
    }
    printf("Asistencia guardada temporalmente (recuerde usar la opcion Guardar).\n");
}

// 3. Mostrar porcentaje de asistencia
void mostrarReporte() {
    if (cantidadEstudiantes == 0) {
        printf("\nNo hay datos para mostrar.\n");
        return;
    }

    printf("\n%-30s | %-15s | %-10s | %-10s | %-10s\n", "Nombre", "Cedula", "Asist.", "Clases", "Porcentaje");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < cantidadEstudiantes; i++) {
        float porcentaje = 0.0;
        if (listaEstudiantes[i].dias_totales_clase > 0) {
            porcentaje = ((float)listaEstudiantes[i].dias_asistidos / listaEstudiantes[i].dias_totales_clase) * 100;
        }

        printf("%-30s | %-15s | %-10d | %-10d | %.2f%%\n", 
               listaEstudiantes[i].nombre, 
               listaEstudiantes[i].cedula, 
               listaEstudiantes[i].dias_asistidos,
               listaEstudiantes[i].dias_totales_clase,
               porcentaje);
    }
}

// 4. Guardar y Cargar (Archivos de texto)
void guardarDatos() {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }

    // Primero guardamos cuántos estudiantes hay
    fprintf(archivo, "%d\n", cantidadEstudiantes);

    // Guardamos los datos de cada uno
    for (int i = 0; i < cantidadEstudiantes; i++) {
        // Formato: Nombre|Cedula|Asistencias|TotalDias
        // Usamos un delimitador (|) para facilitar la lectura si hay espacios
        fprintf(archivo, "%s|%s|%d|%d\n", 
                listaEstudiantes[i].nombre, 
                listaEstudiantes[i].cedula, 
                listaEstudiantes[i].dias_asistidos, 
                listaEstudiantes[i].dias_totales_clase);
    }

    fclose(archivo);
    printf("Datos guardados correctamente en '%s'.\n", NOMBRE_ARCHIVO);
}

void cargarDatos() {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "r");
    if (archivo == NULL) {
        // Si no existe el archivo, simplemente no cargamos nada (primera ejecución)
        return;
    }

    if (fscanf(archivo, "%d\n", &cantidadEstudiantes) != 1) {
        cantidadEstudiantes = 0;
        fclose(archivo);
        return;
    }

    for (int i = 0; i < cantidadEstudiantes; i++) {
        // Leemos con el formato específico usando scanset [^|] para leer hasta la barra
        fscanf(archivo, "%[^|]|%[^|]|%d|%d\n", 
               listaEstudiantes[i].nombre, 
               listaEstudiantes[i].cedula, 
               &listaEstudiantes[i].dias_asistidos, 
               &listaEstudiantes[i].dias_totales_clase);
    }

    fclose(archivo);
    printf("Se han cargado %d estudiantes del archivo.\n", cantidadEstudiantes);
}

// 5. Función Recursiva para calcular total de asistencias
// Suma las asistencias del estudiante actual + la llamada recursiva del siguiente
int calcularTotalAsistenciasRecursiva(int index) {
    // Caso base: si llegamos al final de la lista, retornamos 0
    if (index >= cantidadEstudiantes) {
        return 0;
    }
    
    // Paso recursivo: Asistencias del actual + Asistencias del resto
    return listaEstudiantes[index].dias_asistidos + calcularTotalAsistenciasRecursiva(index + 1);
}

// Utilidad para limpiar el buffer del teclado
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}