#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define N 1024

/* Función de medición de tiempo de alta precisión */
static double medir_tiempo_segundos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void algoritmo_loop_unrolling_base(float *A, float *B, float *C) {
    // Inicialización lenta con bucles anidados
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i * N + j] = 0.0f;
        }
    }
    
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            float reg_a = A[i * N + k];
            for (int j = 0; j < N; j += 4) {
                // Sin restricción de alias y cálculo repetitivo de índices
                C[i * N + j]     += reg_a * B[k * N + j];
                C[i * N + j + 1] += reg_a * B[k * N + j + 1];
                C[i * N + j + 2] += reg_a * B[k * N + j + 2];
                C[i * N + j + 3] += reg_a * B[k * N + j + 3];
            }
        }
    }
}

void algoritmo_loop_unrolling_refactorizado(const float * restrict A, const float * restrict B, float * restrict C) {
    memset(C, 0, N * N * sizeof(float));

    for (int i = 0; i < N; i++) {
        const float * restrict ptr_a = &A[i * N];
        float * restrict ptr_c = &C[i * N];
        
        for (int k = 0; k < N; k++) {
            register const float reg_a = ptr_a[k];
            const float * restrict ptr_b = &B[k * N];
            int j = 0;
            
            // Desenrollado seguro en bloques de 4
            for (; j <= N - 4; j += 4) {
                ptr_c[j]     += reg_a * ptr_b[j];
                ptr_c[j + 1] += reg_a * ptr_b[j + 1];
                ptr_c[j + 2] += reg_a * ptr_b[j + 2];
                ptr_c[j + 3] += reg_a * ptr_b[j + 3];
            }
            // Bucle residual por si N no fuera múltiplo exacto de 4
            for (; j < N; j++) {
                ptr_c[j] += reg_a * ptr_b[j];
            }
        }
    }
}

int main(void) {
    size_t total = (size_t)N * N, bytes = total * sizeof(float);
    double gflops = (2.0 * (double)N * N * N) / 1e9;
    
    float *A  = (float*)malloc(bytes);
    float *B  = (float*)malloc(bytes);
    float *C1 = (float*)malloc(bytes);
    float *C2 = (float*)malloc(bytes);

    if (!A || !B || !C1 || !C2) {
        fprintf(stderr, "Error de asignación de memoria.\n");
        return 1;
    }

    // Inicialización de matrices con valores determinísticos
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (float)((i + j) % 50) * 0.02f + 1.0f;
            B[i * N + j] = (float)((i * 2 + j) % 50) * 0.02f + 0.5f;
        }
    }

    double t0, t_base, t_refac;

    // Ejecución y medición Versión Base
    t0 = medir_tiempo_segundos(); 
    algoritmo_loop_unrolling_base(A, B, C1); 
    t_base = medir_tiempo_segundos() - t0;

    // Ejecución y medición Versión Refactorizada
    t0 = medir_tiempo_segundos(); 
    algoritmo_loop_unrolling_refactorizado(A, B, C2); 
    t_refac = medir_tiempo_segundos() - t0;

    // Reporte de resultados
    printf("\n=== COMPARATIVA: CÓDIGO BASE VS REFACTORIZADO (N=%d) ===\n", N);
    printf("1. Versión Base (Sin refactorizar)   : %7.4f s | %6.2f GFLOPS | Speedup: 1.00x\n", t_base, gflops/t_base);
    printf("2. Versión Refactorizada (Optimizado): %7.4f s | %6.2f GFLOPS | Speedup: %.2fx\n", t_refac, gflops/t_refac, t_base/t_refac);

    free(A); free(B); free(C1); free(C2);
    return 0;
}