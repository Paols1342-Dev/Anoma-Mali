#ifndef ANOMA_MALI_EXTENSIONS_H
#define ANOMA_MALI_EXTENSIONS_H

#include <stdbool.h>
#include <string.h>

#define MALI_G52_NATIVE_EXT_COUNT 131

// Struttura dati per tracciare estensioni native e forzature custom
struct anoma_mali_extension_table {
    // Array booleano per le 131 estensioni native della Mali-G52
    bool native_extensions[MALI_G52_NATIVE_EXT_COUNT];

    // Flag booleani separati per le estensioni custom forzate
    bool custom_EXT_descriptor_indexing;
    bool custom_EXT_extended_dynamic_state;
};

// Funzione di inizializzazione: azzera tutto e imposta i valori a false
static inline void anoma_mali_init_extension_table(struct anoma_mali_extension_table *table) {
    if (!table) return;

    // Imposta a false tutte le 131 estensioni native
    memset(table->native_extensions, 0, sizeof(table->native_extensions));

    // Imposta a false le estensioni custom
    table->custom_EXT_descriptor_indexing = false;
    table->custom_EXT_extended_dynamic_state = false;
}

#endif // ANOMA_MALI_EXTENSIONS_H
