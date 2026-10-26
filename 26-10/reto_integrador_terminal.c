#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE 256
#define COLA_BASE_DIR 0x70
#define COLA_CAPACIDAD 8
#define DIR_KB_HEAD 0x78
#define DIR_KB_TAIL 0x79
#define DIR_KB_COUNT 0x7A

typedef struct {
    uint8_t ram[RAM_SIZE];
    uint16_t cursor_vram;
} PCSystem;

PCSystem g_pc;

bool cola_teclado_encolar(uint8_t ascii) {
    uint8_t tail = g_pc.ram[DIR_KB_TAIL];
    uint8_t count = g_pc.ram[DIR_KB_COUNT];
    if (count >= COLA_CAPACIDAD) return false;
    
    g_pc.ram[COLA_BASE_DIR + tail] = ascii;
    g_pc.ram[DIR_KB_TAIL] = (tail + 1) % COLA_CAPACIDAD;
    g_pc.ram[DIR_KB_COUNT] = count + 1;
    return true;
}

bool cola_teclado_desencolar(uint8_t *salida_ascii) {
    uint8_t head = g_pc.ram[DIR_KB_HEAD];
    uint8_t count = g_pc.ram[DIR_KB_COUNT];
    if (count == 0) return false;
    
    *salida_ascii = g_pc.ram[COLA_BASE_DIR + head];
    g_pc.ram[DIR_KB_HEAD] = (head + 1) % COLA_CAPACIDAD;
    g_pc.ram[DIR_KB_COUNT] = count - 1;
    return true;
}

void monitor_actualizar_pantalla(void) {
    printf("--- Monitor VRAM Actualizado ---\n");
    for (int f = 0; f < 4; f++) {
        printf("| ");
        for (int c = 0; c < 4; c++) {
            uint8_t val = g_pc.ram[0xF0 + (f * 4) + c];
            printf("%c | ", (val >= 32 && val <= 126) ? val : ' ');
        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

bool terminal_procesar_siguiente_tecla(void) {
    uint8_t tecla;
    if (!cola_teclado_desencolar(&tecla)) return false;
    
    if (tecla == 0x08 || tecla == 0x7F) { /* BACKSPACE */
        if (g_pc.cursor_vram > 0xF0) {
            g_pc.cursor_vram--;
            g_pc.ram[g_pc.cursor_vram] = 0x20; /* Borra carácter */
        }
    } else if (tecla >= 32 && tecla <= 126) {
        if (g_pc.cursor_vram <= 0xFF) {
            g_pc.ram[g_pc.cursor_vram] = tecla;
            g_pc.cursor_vram++;
        }
    }
    monitor_actualizar_pantalla();
    return true;
}

int main(void) {
    for (int i = 0; i < RAM_SIZE; i++) g_pc.ram[i] = 0x20;
    g_pc.cursor_vram = 0xF0;

    printf("=== Reto Integrador: Terminal con Backspace ===\n");
    
    // Simulamos escribir "A", "B", "C", luego Backspace (0x08), y luego "X"
    cola_teclado_encolar('A');
    cola_teclado_encolar('B');
    cola_teclado_encolar('C');
    cola_teclado_encolar(0x08); /* Tecla Backspace */
    cola_teclado_encolar('X');

    while (terminal_procesar_siguiente_tecla()) {
        // Procesa la cola secuencialmente
    }

    return 0;
}
