#include <iostream>

int main() {
    int a = 5;
    int b = 3;
    int c = 0;

    // Ensamblador Inline en sintaxis Intel (MSVC / Clang / GCC con directive)
    __asm__ (
        ".intel_syntax noprefix\n\t"  // Cambia a sintaxis Intel (mov eax, [a])
        "mov eax, %[val_a]\n\t"        // Carga 'a' en eax
        "mov ebx, %[val_b]\n\t"        // Carga 'b' en ebx
        "add eax, ebx\n\t"             // Suma eax + ebx (resultado en eax)
        "mov %[val_c], eax\n\t"        // Guarda el resultado en 'c'
        ".att_syntax prefix\n\t"       // Restaura la sintaxis predeterminada (AT&T)
        : [val_c] "=r" (c)             // Salida: modifica 'c'
        : [val_a] "r" (a), [val_b] "r" (b) // Entradas: lee 'a' y 'b'
        : "eax", "ebx", "cc"           // Registros y flags modificados
    );

    std::cout << "El valor de c es: " << c << std::endl; // Imprime: 8

    return 0;
}