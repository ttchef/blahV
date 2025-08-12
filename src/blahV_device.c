
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include "blahV/blahV_utils.h"

#include <vulkan/vulkan_core.h>

BLV_Result blvDeviceInit(blvContext *context) {
    
    if (blvDeviceInstanceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    BLV_LOG(BLV_LOG_DEBUG, "Created Vulkan Instance\n");

    if (blvDevicePhysicalDeviceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    BLV_LOG(BLV_LOG_DEBUG, "Created Vulkan Physical Device\n");

    if (blvDeviceLogicalDeviceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    BLV_LOG(BLV_LOG_DEBUG, "Created Vulkan Logical Device\n");

    return BLV_OK;
}

BLV_Result blvDeviceInstanceInit(blvContext *context) {

    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "blv application";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "blv engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2; // TODO change later

    VkInstanceCreateInfo instanceCreateInfo = {0};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&instanceCreateInfo, NULL, &context->device.instance) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_INSTANCE_ERROR, "Couldnt create vulkan instance");
        return BLV_ERROR;
    }

    return BLV_OK;
}

BLV_Result blvDevicePhysicalDeviceInit(blvContext *context) {
    
    uint32_t num_devices;
    vkEnumeratePhysicalDevices(context->device.instance, &num_devices, NULL);
    if (num_devices == 0) {
        BLV_SET_ERROR(BLV_VULKAN_PHYSICAL_DEVICE_ERROR, "Failed to find fitting GPU");
        context->device.physical_device = 0;
        return BLV_ERROR;
    }

    VkPhysicalDevice physical_devices[num_devices];
    vkEnumeratePhysicalDevices(context->device.instance, &num_devices, physical_devices);
    
    BLV_LOG(BLV_LOG_DEBUG, "Found %d GPUs\n", num_devices);

    for (uint32_t i = 0; i < num_devices; i++) {
        VkPhysicalDeviceProperties device_properties = {0};
        vkGetPhysicalDeviceProperties(physical_devices[i], &device_properties);
        BLV_LOG(BLV_LOG_DEBUG, "Found GPU: %s\n", device_properties.deviceName);
    }

    VkPhysicalDeviceProperties selected_device_properties = {0};
    vkGetPhysicalDeviceProperties(physical_devices[0], &selected_device_properties);
    context->device.physical_device = physical_devices[0];
    context->device.physical_device_properties = selected_device_properties;

    BLV_LOG(BLV_LOG_DEBUG, "Will select %s GPU\n", selected_device_properties.deviceName);

    return BLV_OK;
}

BLV_Result blvDeviceLogicalDeviceInit(blvContext *context) {

    // Get queue families
    uint32_t num_queque_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(context->device.physical_device, &num_queque_families, NULL);
    VkQueueFamilyProperties queue_families[num_queque_families];
    vkGetPhysicalDeviceQueueFamilyProperties(context->device.physical_device, &num_queque_families, NULL);   

    // find graphics queue
    uint32_t graphics_queque_index = 0;
    for (uint32_t i = 0; i < num_queque_families; i++) {
        VkQueueFamilyProperties queue_family = queue_families[i];
        if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics_queque_index = i;
            break;
        }
    }

    // Queue Create Info
    float priorities[] = { 1.0f };
    VkDeviceQueueCreateInfo queue_create_info = {0};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = priorities;
    
    // Featues
    VkPhysicalDeviceFeatures enabled_features = {0};

    // Device Extensions
    const char* device_extensions[] = {
        "VK_KHR_swapchain",
    };
    

    VkDeviceCreateInfo device_create_info = {0};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &enabled_features;
    device_create_info.enabledExtensionCount = BLV_ARRAY_COUNT(device_extensions);
    device_create_info.ppEnabledExtensionNames = device_extensions;

    if (vkCreateDevice(context->device.physical_device, &device_create_info, NULL, &context->device.logical_device) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DEVICE_ERROR, "Failed to create logical device");
        return BLV_ERROR;
    }

    return BLV_OK;
}

