#ifndef ANOMA_MALI_EXTENSIONS_H
#define ANOMA_MALI_EXTENSIONS_H

#include <stdbool.h>
#include <string.h>

#define MALI_G52_NATIVE_EXT_COUNT 131

// Data structure to track native extensions and custom overrides
struct anoma_mali_extension_table {
    // Boolean array for the 131 native Mali-G52 extensions
    bool native_extensions[MALI_G52_NATIVE_EXT_COUNT];

    // Separate boolean flags for forced custom extensions
    bool custom_EXT_descriptor_indexing;
    bool custom_EXT_extended_dynamic_state;
};

// Initialization function: zeros out everything and sets defaults
static inline void anoma_mali_init_extension_table(struct anoma_mali_extension_table *table) {
    if (!table) return;

    // Set all 131 native extensions to false
    memset(table->native_extensions, 0, sizeof(table->native_extensions));

    // Set custom extensions to false by default
    table->custom_EXT_descriptor_indexing = false;
    table->custom_EXT_extended_dynamic_state = false;
}

#endif // ANOMA_MALI_EXTENSIONS_H

