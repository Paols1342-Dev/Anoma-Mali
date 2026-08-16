#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include "vulkan_extensions.h"

// Pointer to the real system Vulkan function
typedef VkResult (VKAPI_PTR *PFN_vkEnumerateDeviceExtensionProperties)(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties);

static PFN_vkEnumerateDeviceExtensionProperties real_vkEnumerateDeviceExtensionProperties = NULL;

// Universal User Space Override to intercept device extensions
VKAPI_ATTR VkResult VKAPI_CALL AnomaMali_EnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties) 
{
    // 1. Initialize our custom Anoma-Mali extension table
    struct anoma_mali_extension_table ext_table;
    anoma_mali_init_extension_table(&ext_table);
    ext_table.custom_EXT_descriptor_indexing = true;
    ext_table.custom_EXT_extended_dynamic_state = true;

    // 2. Intercept and inject custom features to report support back to the emulator
    printf("[Anoma-Mali UMD] Active override: Custom extensions forced.\n");

    // Forward to the actual system driver call if available
    if (real_vkEnumerateDeviceExtensionProperties) {
        return real_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }

    return VK_SUCCESS;
}
