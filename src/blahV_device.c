
#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/core/blahV_utils.h"
#include "blahV/core/blahV_globals.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

VkBool32 VKAPI_CALL blvDebugReportCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData) {
    
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        BLV_LOG(BLV_LOG_ERROR, " ");
    }
    else {
        BLV_LOG(BLV_LOG_WARNING, " ");    
    }

    fprintf(stderr, "%s\n\n", pCallbackData->pMessage);

    return VK_FALSE;
}

VkDebugUtilsMessengerEXT blvRegisterDebugCallback(VkInstance instance) {
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
    vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (!vkCreateDebugUtilsMessengerEXT) {
        BLV_SET_ERROR(BLV_VULKAN_FUNCTION_LOAD_ERROR, "Failed to load vkCreateDebugUtilsMessengerEXT");
        return 0;
    }

    VkDebugUtilsMessengerEXT callback = {0};

    VkDebugUtilsMessengerCreateInfoEXT callback_info = {0};
    callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    callback_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    callback_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    callback_info.pfnUserCallback = blvDebugReportCallback;
    
    if (vkCreateDebugUtilsMessengerEXT(instance, &callback_info, NULL, &callback) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DEBUG_UTILS_ERROR, "Failed to create Debug Utils Messenger");
        return 0;
    }

    return callback;
}

BLV_Result blvDeviceInit(blvContext *context) {
    
    if (blvDeviceInstanceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    BLV_LOG(BLV_LOG_DEBUG, "Created Vulkan Instance\n");

    // Debug Messenger enabled?
    if (blv_error_validation_layers_enable) {
        context->device.debug_callback = blvRegisterDebugCallback(context->device.instance);
        if (!context->device.debug_callback) {
            BLV_SET_ERROR(BLV_VULKAN_DEBUG_UTILS_ERROR, "Failed to create Debug Utils Messenger");
            return BLV_ERROR;
        }
    }

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
    if (layer_count == 0) {
        BLV_SET_ERROR(BLV_VULKAN_INSTANCE_ERROR, "Layer Count is 0");
        return BLV_ERROR;
    }
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
            if (strcmp(wanted_layers[i], layer_properties[j].layerName) == 0) {
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

    const char* instance_extensions[BLV_MAX_INSTANCE_EXTENSIONS];
    uint32_t instance_extensions_index = 0;
    
    // glfw extensions
    for (; instance_extensions_index < glfw_extension_count; instance_extensions_index++) {
        instance_extensions[instance_extensions_index] = glfw_extensions[instance_extensions_index];
    }
    
    // Add debug extensions
    if (blv_error_validation_layers_enable) {
        instance_extensions[instance_extensions_index++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        instance_extensions[instance_extensions_index++] = VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME;
    }

    // Validation Features
    VkValidationFeatureEnableEXT enable_validation_features[] = {
        VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
        VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
    };

    VkValidationFeaturesEXT validation_features = {0};
    validation_features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validation_features.enabledValidationFeatureCount = BLV_ARRAY_COUNT(enable_validation_features);
    validation_features.pEnabledValidationFeatures = enable_validation_features;

    // Get System Api version 
    blvVulkanApiVersion api_version = blvGetVulkanApiVersion();
    BLV_LOG(BLV_LOG_DEBUG, "Api Vresion: %d.%d.%d\n", api_version.major, api_version.minor, api_version.patch);

    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "blv application";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName = "blv engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = VK_MAKE_API_VERSION(0, api_version.major,
                                              api_version.minor, 0); // we dont care about the patch bro

    VkInstanceCreateInfo instance_ceate_info = {0};
    instance_ceate_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_ceate_info.pApplicationInfo = &app_info;
    instance_ceate_info.enabledExtensionCount = instance_extensions_index;
    instance_ceate_info.ppEnabledExtensionNames = instance_extensions;

    // Validation Layers enabled? 
    if (blv_error_validation_layers_enable) {
        instance_ceate_info.pNext = &validation_features;
        instance_ceate_info.enabledLayerCount = BLV_ARRAY_COUNT(wanted_layers);
        instance_ceate_info.ppEnabledLayerNames = wanted_layers;
    } 
    else {
        instance_ceate_info.pNext = NULL;
        instance_ceate_info.enabledLayerCount = 0;
        instance_ceate_info.ppEnabledLayerNames = NULL;
    }

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
    uint32_t num_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(context->device.physical_device, &num_queue_families, NULL);
    VkQueueFamilyProperties queue_families[num_queue_families];
    vkGetPhysicalDeviceQueueFamilyProperties(context->device.physical_device, &num_queue_families, queue_families);   

    // find graphics queue
    uint32_t graphics_queue_index = 0;
    for (uint32_t i = 0; i < num_queue_families; i++) {
        VkQueueFamilyProperties queue_family = queue_families[i];
        if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics_queue_index = i;
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
    const char* device_extensions[BLV_MAX_DEVICE_EXTENSIONS];
    uint32_t device_extensions_index = 0;
    device_extensions[device_extensions_index++] = "VK_KHR_swapchain";
    
    // Check for dynamic rendering
    bool supports_dynamic_rendering = blvDevicePhysicalDeviceIsExtensionSupported(context, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
    blvVulkanApiVersion api_version = blvGetVulkanApiVersion();
    bool api_version_1_3_or_more = (api_version.major > 1) || (api_version.minor >= 3);
    if (api_version_1_3_or_more || supports_dynamic_rendering) {
        BLV_LOG(BLV_LOG_DEBUG, "Your driver does support dynamic rendering\n");
    }
    else if (api_version.minor == 2) {
        if (supports_dynamic_rendering) {
            device_extensions[device_extensions_index++] = VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME;
        }
        else {
            BLV_SET_ERROR(BLV_VULKAN_MISSING_DEVICE_EXTENSIONS, "Extension: %s not found",VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
            return BLV_ERROR;
        }
    } 
    else {
        BLV_SET_ERROR(BLV_VULKAN_MISSING_DEVICE_EXTENSIONS, "Extension: %s not found",VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
        return BLV_ERROR;
    }

    // Enable dynamic rendering 
    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature = {0};
    dynamic_rendering_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    dynamic_rendering_feature.pNext = NULL;
    dynamic_rendering_feature.dynamicRendering = VK_TRUE;

    VkDeviceCreateInfo device_create_info = {0};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = &dynamic_rendering_feature;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &enabled_features;
    device_create_info.enabledExtensionCount = device_extensions_index;
    device_create_info.ppEnabledExtensionNames = device_extensions;

    if (vkCreateDevice(context->device.physical_device, &device_create_info, NULL, &context->device.logical_device) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DEVICE_ERROR, "Failed to create logical device");
        return BLV_ERROR;
    }

    // Save graphics Queue
    context->graphics_queue.family_index = graphics_queue_index;
    vkGetDeviceQueue(context->device.logical_device, graphics_queue_index, 0, &context->graphics_queue.queue);

    return BLV_OK;
}

void blvDeviceDeinit(blvContext* context) {
    vkDestroyDevice(context->device.logical_device, NULL);

    if (context->device.debug_callback) {
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
        vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context->device.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (!vkDestroyDebugUtilsMessengerEXT) {
            BLV_SET_ERROR(BLV_VULKAN_FUNCTION_LOAD_ERROR, "Failed to load vkDestroyDebugUtilsMessengerEXT");
        }

        vkDestroyDebugUtilsMessengerEXT(context->device.instance, context->device.debug_callback, NULL);
    }

    vkDestroyInstance(context->device.instance, NULL);
} 

bool blvDevicePhysicalDeviceIsExtensionSupported(blvContext* context, const char *name) {
    uint32_t num_extensions = 0;
    vkEnumerateDeviceExtensionProperties(context->device.physical_device, NULL, &num_extensions, NULL);
    // TODO: idk it error tho
    if (num_extensions == 0) {
        BLV_SET_ERROR(BLV_VULKAN_PHYSICAL_DEVICE_ERROR, "Found 0 extensions");
    }
    VkExtensionProperties extensions[num_extensions];
    vkEnumerateDeviceExtensionProperties(context->device.physical_device, NULL, &num_extensions, extensions);

    for (uint32_t i = 0; i < num_extensions; i++) {
        if (strcmp(name, extensions[i].extensionName) == 0) return true;
    }

    return false;
}



