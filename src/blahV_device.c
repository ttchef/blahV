
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include "blahV/blahV_utils.h"

#include <GLFW/glfw3.h>
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

    // Get Layers
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    VkLayerProperties layer_properties[layer_count];
    vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);

    // Layers we want
    const char* wanted_layers[] = {
        "VK_LAYER_KHRONOS_validation",
    };

    // Do We have the layer
    for (uint32_t i = 0; i < BLV_ARRAY_COUNT(wanted_layers); i++) {
        bool found = false;
        for (uint32_t j = 0; j < layer_count; j++) {
            if (memcmp(wanted_layers[i], layer_properties[j].layerName, strlen(layer_properties[j].layerName)) == 0) {
                found = true;
                BLV_LOG(BLV_LOG_DEBUG, "Found Instance Layer: %s\n", wanted_layers[i]);
                break;
            }
        }
        if (!found) {
            BLV_SET_ERROR(BLV_VULKAN_MISSING_INSTANCE_LAYER, "The Instance Layer: %s wasnt found", wanted_layers[i]);
            return BLV_ERROR;
        }
    }

    // Extensions
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions = NULL;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    const char* additional_instance_extensions[] = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
    };

    uint32_t instance_extension_count = glfw_extension_count + BLV_ARRAY_COUNT(additional_instance_extensions);
    const char* instance_extensions[instance_extension_count];
    
    // glfw extensions
    for (uint32_t i = 0; i < glfw_extension_count; i++) {
        instance_extensions[i] = glfw_extensions[i];
    }

    // other extensions
    for (uint32_t i = 0; i < BLV_ARRAY_COUNT(additional_instance_extensions); i++) {
        instance_extensions[glfw_extension_count + i] = additional_instance_extensions[i];
    }

    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "blv application";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName = "blv engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = VK_API_VERSION_1_2; // TODO change later

    VkInstanceCreateInfo instance_ceate_info = {0};
    instance_ceate_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_ceate_info.pApplicationInfo = &app_info;
    instance_ceate_info.enabledLayerCount = BLV_ARRAY_COUNT(wanted_layers);
    instance_ceate_info.ppEnabledLayerNames = wanted_layers;
    instance_ceate_info.enabledExtensionCount = BLV_ARRAY_COUNT(instance_extensions);
    instance_ceate_info.ppEnabledExtensionNames = instance_extensions;

    if (vkCreateInstance(&instance_ceate_info, NULL, &context->device.instance) != VK_SUCCESS) {
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
    
    // Features
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

