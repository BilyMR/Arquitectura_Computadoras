#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE 256
#define COLA_BASE_DIR 0x70
#define COLA_CAPACIDAD 8
#define DIR_KB_HEAD 0x78
#define DIR_KB_TAIL 0x79
#define DIR_KB_COUNT 0x7A

uint8_t g_ram[RAM_SIZE];

bool cola_teclado_encolar(uint8_t ascii) {
    uint8_t tail = g_ram[DIR_KB_TAIL];
    uint8_t count = g_ram[DIR_KB_COUNT];
    if (count >= COLA_CAPACIDAD) return false; /* BUFFER OVERFLOW */
    
    g_ram[COLA_BASE_DIR + tail] = ascii;
    g_ram[DIR_KB_TAIL] = (tail + 1) % COLA_CAPACIDAD;
    g_ram[DIR_KB_COUNT] = count + 1;
    return true;
}

bool cola_teclado_desencolar(uint8_t *salida_ascii) {
    uint8_t head = g_ram[DIR_KB_HEAD];
    uint8_t count = g_ram[DIR_KB_COUNT];
    if (count == 0) return false; /* COLA VACÍA */
    
    *salida_ascii = g_ram[COLA_BASE_DIR + head];
    g_ram[DIR_KB_HEAD] = (head + 1) % COLA_CAPACIDAD;
    g_ram[DIR_KB_COUNT] = count - 1;
    return true;
}

int main(void) {
    for (int i = 0; i < RAM_SIZE; i++) g_ram[i] = 0;

    printf("=== Experimento 2: Cola Circular FIFO ===\n");
    printf("Encolando: A, B, C\n");
    cola_teclado_encolar('A');
    cola_teclado_encolar('B');
    cola_teclado_encolar('C');

    uint8_t valor;
    while (cola_teclado_desencolar(&valor)) {
        printf("Desencolado: %c\n", valor);
    }

    return 0;
}
