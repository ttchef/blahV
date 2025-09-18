
#include <stdio.h>
#include <threads.h>

#include "blahV/core/blahV_log.h"

_Thread_local blvErrorInfo blv_error_last = {0};
bool blv_error_log_enable = true;
bool blv_error_validation_layers_enable = true;
blvPfnErrorCallback blv_error_callback = blvErrorDefaultCallback;
BLV_Log_Level blv_error_log_level = BLV_LOG_WARNING;

void blvErrorDefaultCallback(const blvErrorInfo* error) {
    printf("Error in %s:%d (%s): %s - %s\n",
        error->file_name,
        error->line_number,
        error->function_name,
        blvErrorString(error->error_code),
        error->message
    );
}

const char* blvErrorString(BLV_Result result) {
    switch (result) {
        case BLV_UNDEFINED:                         return "Undefinied";
        case BLV_OK:                                return "Success";
        case BLV_ERROR:                             return "General Error";
        case BLV_GLFW_ERROR:                        return "Glfw Error";
        case BLV_INVALID_LOG_LEVEL:                 return "Invalid Log Level";
        case BLV_ALLOC_FAIL:                        return "Allocation Fail";
        case BLV_FILE_ERROR:                        return "File Error";
        case BLV_VULKAN_INSTANCE_ERROR:             return "Vulkan Instance Error";
        case BLV_VULKAN_PHYSICAL_DEVICE_ERROR:      return "Vulkan Physical Device Error";
        case BLV_VULKAN_DEVICE_ERROR:               return "Vulkan Device Error";
        case BLV_VULKAN_MISSING_INSTANCE_LAYER:     return "Vulkan Missing Instance Layer";
        case BLV_VULKAN_DEBUG_UTILS_ERROR:          return "Vulkan Debug Utils Messenger Error";
        case BLV_VULKAN_FUNCTION_LOAD_ERROR:        return "Vulkan Failed to load function Error";
        case BLV_VULKAN_SURFACE_ERROR:              return "Vulkan Suface Error";
        case BLV_VULKAN_SWAPCHAIN_ERROR:            return "Vulkan Swapchain Error";
        case BLV_VULKAN_MISSING_DEVICE_EXTENSIONS:  return "Vulkan Missing Device Extension";
        case BLV_VULKAN_PIPELINE_ERROR:             return "Vulkan Pipeline Error";
        case BLV_VULKAN_COMMAND_POOL_ERROR:         return "Vulkan Command Pool Error";
        case BLV_VULKAN_COMMAND_BUFFER_ERROR:       return "Vulkan Command Buffer Error";
        case BLV_VULKAN_QUEUE_SUBMIT_ERROR:         return "Vulkan Queue Submit Error";
        case BLV_VULKAN_BUFFER_ERROR:               return "Vulkan Buffer Error";
        case BLV_VULKAN_MEMORY_ERROR:               return "Vulkan Memory Error";
        case BLV_VULKAN_DESCRIPTOR_SET_ERROR:       return "Vulkan Descriptor Set Error";
        case BLV_CAMERA_ERROR:                      return "Camera Error";
        case BLV_INVALID_FUNCTION_INPUT:            return "Invalid Function Input";
        case BLV_VULKAN_IMAGE_ERROR:                return "Vulkan Image Error";
        case BLV_IMAGE_ERROR:                       return "Image Error";
        default:                                    return "Unkown Error";
    }
}

const char* blvErrorLogLevelString(BLV_Log_Level level) {
    switch (level) {
        case BLV_LOG_DEBUG:     return "BLV_DEBUG";
        case BLV_LOG_WARNING:   return "BLV_WARNING";
        case BLV_LOG_ERROR:     return "BLV_ERROR";
        default:                return "BLV_NOT_SPECIFIED_LOG_LEVEL";
    }
}

void blvErrorEnableLog(bool enable) {
    blv_error_log_enable = enable;
}

void blvErrorEnableValidationLayers(bool enable) {
    blv_error_validation_layers_enable = enable;
}

void blvErrorSetCallback(blvPfnErrorCallback callback) {
    blv_error_callback = callback;
}

const blvErrorInfo* blvErrorGetLast() {
    return &blv_error_last;
}

void blvErrorClearLast() {
    blv_error_last.error_code = BLV_UNDEFINED;
    blv_error_last.file_name = 0;
    blv_error_last.function_name = 0;
    blv_error_last.line_number = 0;
}

void blvErrorSetLogLevel(BLV_Log_Level level) {
    if (level < 0) {
        BLV_SET_ERROR(BLV_INVALID_LOG_LEVEL, "Log Level is Invalid");
        return;
    }
    blv_error_log_level = level;
}

