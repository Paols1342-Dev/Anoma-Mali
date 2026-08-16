#ifndef ANOMA_MALI_EXTENSIONS_H
#define ANOMA_MALI_EXTENSIONS_H

#include <stdbool.h>
#include <string.h>
#include <vulkan/vulkan.h>

#define MALI_G52_NATIVE_EXT_COUNT 131
#define ANOMA_MALI_CUSTOM_EXT_COUNT 4
#define ANOMA_MALI_TOTAL_EXT_COUNT (MALI_G52_NATIVE_EXT_COUNT + ANOMA_MALI_CUSTOM_EXT_COUNT)

// Exact macro definitions for injected extensions
#define ANOMA_EXT_DESCRIPTOR_INDEXING "VK_EXT_descriptor_indexing"
#define ANOMA_EXT_EXTENDED_DYNAMIC_STATE "VK_EXT_extended_dynamic_state"
#define ANOMA_KHR_SHADER_FLOAT16_INT8 "VK_KHR_shader_float16_int8"
#define ANOMA_EXT_SHADER_VIEWPORT_INDEX_LAYER "VK_EXT_shader_viewport_index_layer"

// Data structure to track extension states
struct anoma_mali_extension_table {
    bool native_extensions[MALI_G52_NATIVE_EXT_COUNT];
    bool custom_EXT_descriptor_indexing;
    bool custom_EXT_extended_dynamic_state;
    bool custom_KHR_shader_float16_int8;
    bool custom_EXT_shader_viewport_index_layer;
};

// Clean initialization function
static inline void anoma_mali_init_extension_table(struct anoma_mali_extension_table *table) {
    if (!table) return;

    memset(table->native_extensions, 0, sizeof(table->native_extensions));

    table->custom_EXT_descriptor_indexing = true;
    table->custom_EXT_extended_dynamic_state = true;
    table->custom_KHR_shader_float16_int8 = true;
    table->custom_EXT_shader_viewport_index_layer = true;
}

#endif // ANOMA_MALI_EXTENSIONS_H



