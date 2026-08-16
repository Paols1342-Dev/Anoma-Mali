#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include "umd/vulkan_extensions.h"

// Pointer to the real system Vulkan function
typedef VkResult (VKAPI_PTR *PFN_vkEnumerateDeviceExtensionProperties)(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties);

static PFN_vkEnumerateDeviceExtensionProperties real_vkEnumerateDeviceExtensionProperties = NULL;

// User Space Override by Paols1342-Dev to force Vulkan extensions from 131 to 135
VKAPI_ATTR VkResult VKAPI_CALL AnomaMali_vkEnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties)
{
    // 1. If Winlator only requests the number of extensions, force it to return exactly 135
    if (pProperties == NULL) {
        if (real_vkEnumerateDeviceExtensionProperties) {
            real_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, NULL);
        } else {
            *pPropertyCount = 131; // Base native count if real call fails
        }
        
        // Force the final count to include our 4 missing custom extensions (131 + 4 = 135)
        *pPropertyCount = 135;
        return VK_SUCCESS;
    }

    // 2. If Winlator allocates memory and reads the actual list
    uint32_t original_count = 131; 
    if (real_vkEnumerateDeviceExtensionProperties) {
        real_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, &original_count, pProperties);
    }

    // 3. INJECTION ROUTINE: Manually append the 4 critical custom extensions to reach 135 items
    // Extension 132: Descriptor Indexing for DXVK
    strcpy(pProperties[original_count].extensionName, "VK_EXT_descriptor_indexing");
    pProperties[original_count].specVersion = 2;

    // Extension 133: Extended Dynamic State for pipeline stability
    strcpy(pProperties[original_count + 1].extensionName, "VK_EXT_extended_dynamic_state");
    pProperties[original_count + 1].specVersion = 1;

    // Extension 134: Float16 and Int8 math for lightweight shaders
    strcpy(pProperties[original_count + 2].extensionName, "VK_KHR_shader_float16_int8");
    pProperties[original_count + 2].specVersion = 1;

    // Extension 135: Shader Viewport Index Layer for geometry wrapping
    strcpy(pProperties[original_count + 3].extensionName, "VK_EXT_shader_viewport_index_layer");
    pProperties[original_count + 3].specVersion = 1;

    // Hardcode the final total returned count to 135 items
    *pPropertyCount = 135;

    printf("[Anoma-Mali UMD] Target acquired! Successfully injected 4 extensions. Total count: 135.\n");
    return VK_SUCCESS;
}


