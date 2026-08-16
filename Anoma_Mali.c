#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include "vulkan_extensions.h"

// Global array synchronized with header macros
static const char* custom_extension_names[ANOMA_MALI_CUSTOM_EXT_COUNT] = {
    ANOMA_EXT_DESCRIPTOR_INDEXING,
    ANOMA_EXT_EXTENDED_DYNAMIC_STATE,
    ANOMA_KHR_SHADER_FLOAT16_INT8,
    ANOMA_EXT_SHADER_VIEWPORT_INDEX_LAYER
};

typedef VkResult (VKAPI_PTR *PFN_vkEnumerateDeviceExtensionProperties)(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties);

static PFN_vkEnumerateDeviceExtensionProperties real_vkEnumerateDeviceExtensionProperties = NULL;

// Extension enumeration override
VKAPI_ATTR VkResult VKAPI_CALL AnomaMali_EnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties) 
{
    struct anoma_mali_extension_table ext_table;
    anoma_mali_init_extension_table(&ext_table);

    uint32_t native_count = 0;
    VkResult result = VK_SUCCESS;

    if (real_vkEnumerateDeviceExtensionProperties) {
        result = real_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, &native_count, NULL);
    } else {
        native_count = MALI_G52_NATIVE_EXT_COUNT;
    }

    if (pProperties == NULL) {
        if (pPropertyCount) {
            *pPropertyCount = native_count + ANOMA_MALI_CUSTOM_EXT_COUNT;
        }
        return result;
    }

    if (real_vkEnumerateDeviceExtensionProperties) {
        real_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
    }

    uint32_t base_index = native_count;
    for (uint32_t i = 0; i < ANOMA_MALI_CUSTOM_EXT_COUNT; i++) {
        if (base_index + i < *pPropertyCount) {
            memset(&pProperties[base_index + i], 0, sizeof(VkExtensionProperties));
            strncpy(pProperties[base_index + i].extensionName, custom_extension_names[i], VK_MAX_EXTENSION_NAME_SIZE - 1);
            pProperties[base_index + i].specVersion = 1;
        }
    }

    return VK_SUCCESS;
}

// Layer function pointer resolver (Device level)
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL AnomaMali_GetDeviceProcAddr(
    VkDevice device,
    const char* pName) 
{
    if (pName && strcmp(pName, "vkEnumerateDeviceExtensionProperties") == 0) {
        return (PFN_vkVoidFunction)AnomaMali_EnumerateDeviceExtensionProperties;
    }
    return NULL;
}

// Layer function pointer resolver (Instance level)
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL AnomaMali_GetInstanceProcAddr(
    VkInstance instance,
    const char* pName) 
{
    if (pName && strcmp(pName, "vkEnumerateDeviceExtensionProperties") == 0) {
        return (PFN_vkVoidFunction)AnomaMali_EnumerateDeviceExtensionProperties;
    }
    if (pName && strcmp(pName, "vkGetDeviceProcAddr") == 0) {
        return (PFN_vkVoidFunction)AnomaMali_GetDeviceProcAddr;
    }
    return NULL;
}


