#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE 256
uint8_t g_ram[RAM_SIZE];

bool vram_escribir(uint8_t fila, uint8_t col, char c) {
    if (fila >= 4 || col >= 4) return false;
    uint8_t dir = 0xF0 + (fila * 4) + col; /* Dirección VRAM */
    g_ram[dir] = (uint8_t)c;
    return true;
}

void vram_scroll_arriba(void) {
    for (int f = 0; f < 3; f++) {
        for (int c = 0; c < 4; c++) {
            g_ram[0xF0 + (f * 4) + c] = g_ram[0xF0 + ((f + 1) * 4) + c];
        }
    }
    for (int c = 0; c < 4; c++) {
        g_ram[0xF0 + (3 * 4) + c] = 0x20;
    }
}

void monitor_imprimir(void) {
    printf("+---+---+---+---+\n");
    for (int f = 0; f < 4; f++) {
        printf("|");
        for (int c = 0; c < 4; c++) {
            uint8_t val = g_ram[0xF0 + (f * 4) + c];
            printf(" %c |", val ? val : ' ');
        }
        printf("\n+---+---+---+---+\n");
    }
}

int main(void) {
    for (int i = 0; i < RAM_SIZE; i++) g_ram[i] = 0x20;

    printf("=== Experimento 3: VRAM (Matriz 4x4) ===\n");
    vram_escribir(0, 0, 'H');
    vram_escribir(0, 1, 'O');
    vram_escribir(0, 2, 'L');
    vram_escribir(0, 3, 'A');
    monitor_imprimir();

    printf("\nAplicando Scroll Arriba:\n");
    vram_scroll_arriba();
    monitor_imprimir();

    return 0;
}
