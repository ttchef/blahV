
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvDeviceInit(blvContext *context) {
    
    if (blvDeviceInstanceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    if (blvDevicePhysicalDeviceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    if (blvDeviceLogicalDeviceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

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

    return BLV_OK;
}

