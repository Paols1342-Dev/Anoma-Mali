/*
 * Copyright © 2021-2026 Collabora Ltd.
 * Copyright © 2026 Paolo1342-Dev (Forced Bifrost Profile)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "panvk_private.h"

#include <fcntl.h>
#include <xf86drm.h>
#include <amdgpu_drm.h>
#include "drm-uapi/panfrost_drm.h"

#include "vk_android.h"
#include "vk_common_entrypoints.h"
#include "vk_log.h"
#include "vk_util.h"

static void
panvk_physical_device_get_supported_extensions(const struct panvk_instance *instance,
                                               struct vk_device_extension_table *ext)
{
   /* Forziamo l'esposizione di tutte le estensioni core per Eden su Mali-G52 */
   ext->KHR_bind_memory2 = true;
   ext->KHR_dedicated_allocation = true;
   ext->KHR_descriptor_update_template = true;
   ext->KHR_device_group = true;
   ext->KHR_driver_properties = true;
   ext->KHR_external_memory = true;
   ext->KHR_external_memory_fd = true;
   ext->KHR_external_semaphore = true;
   ext->KHR_external_semaphore_fd = true;
   ext->KHR_format_feature_flags2 = true;
   ext->KHR_get_memory_requirements2 = true;
   ext->KHR_image_format_list = true;
   ext->KHR_maintenance1 = true;
   ext->KHR_maintenance2 = true;
   ext->KHR_maintenance3 = true;
   ext->KHR_maintenance4 = true;
   ext->KHR_pipeline_executable_properties = true;
   ext->KHR_relaxed_block_layout = true;
   ext->KHR_sampler_mirror_clamp_to_edge = true;
   ext->KHR_sampler_ycbcr_conversion = true;
   ext->KHR_shader_draw_parameters = true;
   ext->KHR_shader_float16_int8 = true;
   ext->KHR_spatial_accelerator = false;
   ext->KHR_storage_buffer_storage_class = true;
   ext->KHR_swapchain = true;
   ext->KHR_swapchain_mutable_format = true;
   ext->KHR_synchronization2 = true;
   ext->KHR_timeline_semaphore = true; /* Forzato per eliminare Driver Unknown */
   ext->KHR_variable_pointers = true;
   ext->KHR_vulkan_memory_model = true;
   ext->KHR_zero_initialize_workgroup_memory = true;
   
   ext->EXT_border_color_swizzle = true;
   ext->EXT_conditional_rendering = true;
   ext->EXT_conservative_rasterization = true;
   ext->EXT_custom_border_color = true;
   ext->EXT_descriptor_indexing = true; /* Forzato per Eden */
   ext->EXT_extended_dynamic_state = true;
   ext->EXT_extended_dynamic_state2 = true;
   ext->EXT_host_query_reset = true;
   ext->EXT_image_robustness = true;
   ext->EXT_index_type_uint8 = true;
   ext->EXT_inline_uniform_block = true;
   ext->EXT_line_rasterization = true;
   ext->EXT_memory_budget = true;
   ext->EXT_multi_draw = true;
   ext->EXT_physical_device_drm = true;
   ext->EXT_pipeline_creation_cache_control = true;
   ext->EXT_pipeline_creation_feedback = true;
   ext->EXT_private_data = true;
   ext->EXT_provocation_vertex = true;
   ext->EXT_queue_family_foreign = true;
   ext->EXT_robustness2 = true; /* Forzato per Eden */
   ext->EXT_separate_stencil_usage = true;
   ext->EXT_shader_demote_to_helper_invocation = true;
   ext->EXT_texel_buffer_alignment = true;
   ext->EXT_tooling_info = true;
   ext->EXT_vertex_attribute_divisor = true;
   ext->EXT_vertex_input_dynamic_state = true;
   ext->EXT_ycbcr_2plane_444_formats = true;

   #ifdef VK_USE_PLATFORM_ANDROID_KHR
   ext->ANDROID_external_memory_android_hardware_buffer = true;
   ext->ANDROID_native_buffer = true;
   #endif
}

static void
panvk_physical_device_init_features(struct panvk_physical_device *device)
{
   /* Inizializzazione delle feature base del dispositivo */
   device->vk.supported_features.robustBufferAccess = true;
   device->vk.supported_features.fullDrawIndexUint32 = true;
   device->vk.supported_features.imageCubeArray = true;
   device->vk.supported_features.independentBlend = true;
   device->vk.supported_features.geometryShader = true;
   device->vk.supported_features.tessellationShader = true;
   device->vk.supported_features.sampleRateShading = true;
   device->vk.supported_features.dualSrcBlend = true;
   device->vk.supported_features.logicOp = true;
   device->vk.supported_features.multiDrawIndirect = true;
   device->vk.supported_features.drawIndirectFirstInstance = true;
   device->vk.supported_features.depthClamp = true;
   device->vk.supported_features.depthBiasClamp = true;
   device->vk.supported_features.fillModeNonSolid = true;
   device->vk.supported_features.depthBounds = false;
   device->vk.supported_features.wideLines = true;
   device->vk.supported_features.largePoints = true;
   device->vk.supported_features.alphaToOne = true;
   device->vk.supported_features.multiViewport = true;
   device->vk.supported_features.samplerAnisotropy = true;
   device->vk.supported_features.textureCompressionETC2 = true;
   device->vk.supported_features.textureCompressionASTC_LDR = true;
   device->vk.supported_features.textureCompressionBC = false;
   device->vk.supported_features.occlusionQueryPrecise = true;
   device->vk.supported_features.pipelineStatisticsQuery = true;
   device->vk.supported_features.vertexPipelineStoresAndAtomics = true;
   device->vk.supported_features.fragmentStoresAndAtomics = true;
   device->vk.supported_features.shaderTessellationAndGeometryPointSize = true;
   device->vk.supported_features.shaderImageGatherExtended = true;
   device->vk.supported_features.shaderStorageImageExtendedFormats = true;
   device->vk.supported_features.shaderStorageImageMultisample = true;
   device->vk.supported_features.shaderStorageImageReadWithoutFormat = true;
   device->vk.supported_features.shaderStorageImageWriteWithoutFormat = true;
   device->vk.supported_features.shaderUniformBufferArrayDynamicIndexing = true;
   device->vk.supported_features.shaderSampledImageArrayDynamicIndexing = true;
   device->vk.supported_features.shaderStorageBufferArrayDynamicIndexing = true;
   device->vk.supported_features.shaderStorageImageArrayDynamicIndexing = true;
   device->vk.supported_features.shaderClipDistance = true;
   device->vk.supported_features.shaderCullDistance = true;
   device->vk.supported_features.shaderFloat64 = false;
   device->vk.supported_features.shaderInt64 = false;
   device->vk.supported_features.shaderInt16 = true;
   device->vk.supported_features.shaderResourceResidency = false;
   device->vk.supported_features.shaderResourceMinLod = false;
   device->vk.supported_features.sparseBinding = false;
   device->vk.supported_features.variablePointersStorageBuffer = true;
   device->vk.supported_features.variablePointers = true;
   device->vk.supported_features.inheritedQueries = true;
}

VkResult
panvk_physical_device_try_create(struct panvk_instance *instance,
                                 drmDevicePtr drm_device,
                                 struct panvk_physical_device **device_out)
{
   VkResult result;
   int fd = -1;

   if (!(drm_device->available_nodes & (1 << DRM_NODE_RENDER)))
      return VK_ERROR_INCOMPATIBLE_DRIVER;

   fd = open(drm_device->nodes[DRM_NODE_RENDER], O_RDWR | O_CLOEXEC);
   if (fd < 0)
      return vk_error(instance, VK_ERROR_INCOMPATIBLE_DRIVER);

   struct panvk_physical_device *device =
      vk_zalloc2(&instance->vk.alloc, NULL, sizeof(*device), 8,
                 VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE);

   if (!device) {
      close(fd);
      return vk_error(instance, VK_ERROR_OUT_OF_HOST_MEMORY);
   }

   struct vk_physical_device_dispatch_table dispatch_table;
   vk_physical_device_dispatch_table_from_entrypoints(
      &dispatch_table, &panvk_physical_device_entrypoints, true);
   vk_physical_device_dispatch_table_from_entrypoints(
      &dispatch_table, &wsi_physical_device_entrypoints, false);

   struct vk_device_extension_table supported_extensions;
   panvk_physical_device_get_supported_extensions(instance, &supported_extensions);

   result = vk_physical_device_init(&device->vk, &instance->vk,
                                    &supported_extensions, NULL, NULL,
                                    &dispatch_table);
   if (result != VK_SUCCESS) {
      vk_free2(&instance->vk.alloc, NULL, device);
      close(fd);
      return result;
   }

   device->master_fd = fd;
   panvk_physical_device_init_features(device);

   /* Forza il nome del dispositivo per farlo riconoscere correttamente a Eden */
   snprintf(device->name, sizeof(device->name), "Mesa panvk (Paolo1342-Dev Mali-G52)");
   device->vk.properties.apiVersion = VK_MAKE_VERSION(1, 3, 0);
   device->vk.properties.driverVersion = vk_get_driver_version();
   device->vk.properties.vendorID = 0x13B5; /* ID ARM */
   device->vk.properties.deviceID = 0x52;   /* ID fittizio per Mali G52 */
   device->vk.properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;

   *device_out = device;
   return VK_SUCCESS;
}

void
panvk_physical_device_destroy(struct panvk_physical_device *device)
{
   close(device->master_fd);
   vk_physical_device_finish(&device->vk);
   vk_free2(&device->vk.instance->alloc, NULL, device);
}
