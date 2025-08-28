
#ifndef BLAHV_UTILS_H
#define BLAHV_UTILS_H

#include "blahV/blahV_log.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define BLV_ARRAY_COUNT(a) (sizeof((a)) / sizeof((a)[0]))

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

#endif

