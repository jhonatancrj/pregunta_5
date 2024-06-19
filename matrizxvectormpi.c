#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 3 // Tamaño de la matriz y el vector

// Función para multiplicar una matriz NxN por un vector de dimensión N
void multiplicar_matriz_vector(int matriz[N][N], int vector[N], int resultado_local[N]) {
    for (int i = 0; i < N; i++) {
        resultado_local[i] = 0;
        for (int j = 0; j < N; j++) {
            resultado_local[i] += matriz[i][j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int procesador, cantidad;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &procesador);
    MPI_Comm_size(MPI_COMM_WORLD, &cantidad);

    if (cantidad != N) {
        printf("El número de procesos MPI debe coincidir con el tamaño N.\n");
        MPI_Finalize();
        return 1;
    }

    int matriz[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int vector[N] = {1, 2, 3};
    int resultado_local[N], resultado[N];

    // Multiplicar la submatriz local por el vector
    multiplicar_matriz_vector(matriz, vector, resultado_local);

    // Recopilar los resultados locales en el proceso raíz (proceso 0)
    MPI_Gather(resultado_local, N, MPI_INT, resultado, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Si es el proceso 0, imprimir el resultado final
    if (procesador == 0) {
        printf("Resultado de la multiplicación de la matriz por el vector:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", resultado[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}


