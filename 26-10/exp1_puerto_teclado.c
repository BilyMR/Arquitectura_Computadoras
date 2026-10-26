#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t address_bus;
    uint8_t data_bus;
    bool io_read;
    bool mem_read;
} SystemBuses;

typedef struct {
    uint8_t reg_status;
    uint8_t reg_data;
} TecladoHardware;

void tick_controlador_teclado(SystemBuses *bus, TecladoHardware *kb) {
    if (bus->io_read) {
        if (bus->address_bus == 0x01) {
            bus->data_bus = kb->reg_status;
        } else if (bus->address_bus == 0x02) {
            bus->data_bus = kb->reg_data;
            kb->reg_status = 0; /* Handshake automático (limpia el estatus) */
        }
    }
}

uint8_t cpu_instruccion_in(uint8_t puerto, SystemBuses *g_bus, TecladoHardware *g_teclado) {
    g_bus->address_bus = puerto;
    g_bus->io_read = true;
    g_bus->mem_read = false;
    tick_controlador_teclado(g_bus, g_teclado);
    uint8_t dato = g_bus->data_bus;
    g_bus->io_read = false;
    return dato;
}

int main(void) {
    SystemBuses g_bus = {0, 0, false, false};
    TecladoHardware g_teclado = {1, 'K'}; /* Status 1 (lista), Dato 'K' */

    printf("=== Experimento 1: E/S por Puerto ===\n");
    printf("Leyendo Status (0x01): %u\n", cpu_instruccion_in(0x01, &g_bus, &g_teclado));
    printf("Leyendo Data (0x02):   %c\n", cpu_instruccion_in(0x02, &g_bus, &g_teclado));
    printf("Leyendo Status (0x01) tras handshake: %u\n", cpu_instruccion_in(0x01, &g_bus, &g_teclado));

    return 0;
}
