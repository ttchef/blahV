
#ifndef BLAHV_UTILS_H
#define BLAHV_UTILS_H

#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"

#include <stdint.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define BLV_ARRAY_COUNT(a) (sizeof((a)) / sizeof((a)[0]))
#define BLV_NOT_ZERO(val) (((val) != 0) ? true : false)
#define BLV_IS_ZERO(val) (((val)) == 0 ? true : false)


#define BLV_CHECK(val) \
    do { \
        if ((val) != BLV_OK) { \
            BLV_SET_ERROR((val), "General Error"); \
        } \
    } while(0)

#define BLV_CHECK_RETURN(val) \
    do { \
        if ((val) != BLV_OK) { \
            return (val); \
        } \
    } while(0)


typedef struct {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} blvVulkanApiVersion;

static inline blvVulkanApiVersion blvGetVulkanApiVersion() {
    uint32_t instance_version = 0;
    if (vkEnumerateInstanceVersion(&instance_version) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_ERROR, "Failed to get API version");
        return (blvVulkanApiVersion){0};
    }

    blvVulkanApiVersion result = {0};
    result.major = VK_API_VERSION_MAJOR(instance_version);
    result.minor = VK_API_VERSION_MINOR(instance_version);
    result.patch = VK_API_VERSION_PATCH(instance_version);

    return result;
}

static inline uint32_t blvFindMemoryType(blvContext* context, uint32_t memory_type_bits, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties device_memory_properties;
    vkGetPhysicalDeviceMemoryProperties(context->device.physical_device, &device_memory_properties);

    for (int32_t i = 0; i < device_memory_properties.memoryTypeCount; i++) {
        // is required memory type allowed?
        if ((memory_type_bits & (1 << i)) != 0) {
            // are the properties satisfied?
            if ((device_memory_properties.memoryTypes[i].propertyFlags & properties) == properties) return i;
        }
    }

    BLV_LOG(BLV_LOG_WARNING, "Couldnt find fitting Memory Type!\n");
    return UINT32_MAX;
}

#endif

