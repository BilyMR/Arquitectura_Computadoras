#include <iostream>
#include <fstream>
#include <cstring>   // memcpy, memmove, memset
#include <cstdlib>   // rand, srand
#include <ctime>     // time

const int TAMANO_ORIGINAL   = 1000;
const int TAMANO_TRANSFER   = 100;
const int INDICE_INICIO     = 500;   // indice inicial del bloque a transferir
const int INDICE_FIN        = 599;   // indice final del bloque a transferir
const char* ARCHIVO_DATOS   = "datos_arreglo.bin";
// ----------------------------------------------------------------------------
void generarArchivoDatos(const char* nombreArchivo, int tamano) {
    std::ofstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error: no se pudo crear el archivo de datos.\n";
        exit(1);
    }

    srand(static_cast<unsigned int>(time(nullptr)));
    int* datosTemp = new int[tamano];
    for (int i = 0; i < tamano; i++) {
        datosTemp[i] = (rand() % 1000) + 1; // valores entre 1 y 1000
    }

    archivo.write(reinterpret_cast<char*>(datosTemp), tamano * sizeof(int));
    archivo.close();
    delete[] datosTemp;
}

// ----------------------------------------------------------------------------
// Lee el arreglo de enteros desde el archivo (memoria secundaria) hacia el
// arreglo en memoria principal (RAM).
// ----------------------------------------------------------------------------
bool leerArregloDesdeArchivo(const char* nombreArchivo, int* arreglo, int tamano) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error: no se pudo abrir el archivo de datos.\n";
        return false;
    }
    archivo.read(reinterpret_cast<char*>(arreglo), tamano * sizeof(int));
    archivo.close();
    return true;
}

// ----------------------------------------------------------------------------
// Imprime los primeros y ultimos 'n' elementos de un arreglo.
// ----------------------------------------------------------------------------
void imprimirPrimerosYUltimos(const int* arreglo, int tamano, int n) {
    std::cout << "Primeros " << n << " elementos: ";
    for (int i = 0; i < n; i++) {
        std::cout << arreglo[i] << (i < n - 1 ? ", " : "\n");
    }

    std::cout << "Ultimos " << n << " elementos: ";
    for (int i = tamano - n; i < tamano; i++) {
        std::cout << arreglo[i] << (i < tamano - 1 ? ", " : "\n");
    }
}

// ----------------------------------------------------------------------------
// Imprime todos los elementos de un arreglo (usado para el arreglo de 100).
// ----------------------------------------------------------------------------
void imprimirArregloCompleto(const int* arreglo, int tamano, int elementosPorLinea = 10) {
    for (int i = 0; i < tamano; i++) {
        std::cout << arreglo[i] << "\t";
        if ((i + 1) % elementosPorLinea == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

// ----------------------------------------------------------------------------
// Verifica que un rango del arreglo original contenga unicamente ceros.
// ----------------------------------------------------------------------------
bool validarRangoEnCeros(const int* arreglo, int indiceInicio, int indiceFin) {
    for (int i = indiceInicio; i <= indiceFin; i++) {
        if (arreglo[i] != 0) {
            return false;
        }
    }
    return true;
}

int main() {
    // ---- 1. Generar / leer el arreglo desde memoria secundaria ----
    generarArchivoDatos(ARCHIVO_DATOS, TAMANO_ORIGINAL);

    int arregloOriginal[TAMANO_ORIGINAL];
    if (!leerArregloDesdeArchivo(ARCHIVO_DATOS, arregloOriginal, TAMANO_ORIGINAL)) {
        return 1;
    }

    std::cout << "========================================================\n";
    std::cout << " ARREGLO ORIGINAL (1000 elementos) - ANTES DEL TRASLADO\n";
    std::cout << "========================================================\n";
    imprimirPrimerosYUltimos(arregloOriginal, TAMANO_ORIGINAL, 5);

    // ---- 2. Declarar el segundo arreglo y transferir el bloque 500-599 ----
    int arregloTransferido[TAMANO_TRANSFER];

    // memcpy: los rangos de origen y destino no se solapan, es seguro usarlo
    memcpy(arregloTransferido,
           &arregloOriginal[INDICE_INICIO],
           TAMANO_TRANSFER * sizeof(int));

    std::cout << "\n========================================================\n";
    std::cout << " ARREGLO SEGUNDO (100 elementos transferidos, indices "
              << INDICE_INICIO << "-" << INDICE_FIN << ")\n";
    std::cout << "========================================================\n";
    imprimirArregloCompleto(arregloTransferido, TAMANO_TRANSFER);

    // ---- 3. Limpiar (poner en cero) el bloque trasladado en el arreglo original ----
    // memmove tambien seria valido aqui; se usa memset porque el objetivo es
    // simplemente poner en cero el rango, no mover datos.
    memset(&arregloOriginal[INDICE_INICIO], 0, TAMANO_TRANSFER * sizeof(int));

    // ---- 4. Validar que el rango 500-599 del arreglo original quedo en cero ----
    bool rangoLimpio = validarRangoEnCeros(arregloOriginal, INDICE_INICIO, INDICE_FIN);

    std::cout << "\n========================================================\n";
    std::cout << " VALIDACION DEL ARREGLO ORIGINAL - DESPUES DEL TRASLADO\n";
    std::cout << "========================================================\n";
    std::cout << "Indices " << INDICE_INICIO << " a " << INDICE_FIN
              << " en cero: " << (rangoLimpio ? "SI, correcto" : "NO, error") << "\n";

    // Muestra una pequena ventana alrededor del rango para verificar visualmente
    std::cout << "\nMuestra del rango afectado (indices "
              << INDICE_INICIO << " a " << INDICE_INICIO + 9 << "): ";
    for (int i = INDICE_INICIO; i < INDICE_INICIO + 10; i++) {
        std::cout << arregloOriginal[i] << " ";
    }
    std::cout << "\n";

    // Los primeros y ultimos 5 elementos no deberian haber cambiado
    std::cout << "\nPrimeros y ultimos elementos del arreglo original tras la operacion:\n";
    imprimirPrimerosYUltimos(arregloOriginal, TAMANO_ORIGINAL, 5);

    return 0;
}