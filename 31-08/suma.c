#include <stdio.h>

int main() {
    int numeros[] = {5, 12, 8, 23, 4};
    
    int longitud = sizeof(numeros) / sizeof(numeros[0]);
    
    int suma = 0; 
    for (int i = 0; i < longitud; i++) {
        suma += numeros[i];
    }

    printf("La suma total es: %d\n", suma);

    return 0;
}