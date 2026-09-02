#include <stdio.h>

int main() {
    int numeros[] = {5, 12, 8, 23, 4};
    int longitud = sizeof(numeros) / sizeof(numeros[0]);
    int suma = 0;

    // Suma utilizando ensamblador en línea
    __asm__ (
        "movl $0, %%eax\n\t"        // Inicializa el acumulador (EAX) en 0
        "movl $0, %%ecx\n\t"        // Inicializa el índice del bucle (ECX) en 0
        "1:\n\t"                    // Etiqueta local para el inicio del bucle
        "cmpl %2, %%ecx\n\t"        // Compara el índice (ECX) con la longitud (%2)
        "jge 2f\n\t"                // Si el índice >= longitud, salta al final (etiqueta 2f)
        "addl (%1, %%ecx, 4), %%eax\n\t" // Suma numeros[ECX] a EAX (desplazamiento de 4 bytes)
        "incl %%ecx\n\t"            // Incrementa el índice (ECX++)
        "jmp 1b\n\t"                // Salta al inicio del bucle (etiqueta 1b)
        "2:\n\t"                    // Etiqueta de salida
        "movl %%eax, %0"            // Mueve el resultado final de EAX a la variable suma (%0)
        : "=r"(suma)                // Salida: %0 es la variable 'suma' (escritura)
        : "r" (numeros), "r" (longitud) // Entradas: %1 es el array, %2 es la longitud
        : "%eax", "%ecx", "cc"      // Registros modificados (clobbered) y registro de estado
    );

    printf("La suma total es: %d\n", suma);
    return 0;
}
