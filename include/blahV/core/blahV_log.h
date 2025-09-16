
#ifndef BLAHV_LOG_H
#define BLAHV_LOG_H

#include "blahV/core/blahV_globals.h"

#include <stdbool.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <threads.h>

typedef enum {
    BLV_UNDEFINED = 1,
    BLV_OK = 0,
    BLV_ERROR = -1,
    BLV_GLFW_ERROR = -2,
    BLV_INVALID_LOG_LEVEL = -3,
    BLV_ALLOC_FAIL = -4,
    BLV_FILE_ERROR = -5,
    BLV_VULKAN_INSTANCE_ERROR = -6,
    BLV_VULKAN_PHYSICAL_DEVICE_ERROR = -7,
    BLV_VULKAN_DEVICE_ERROR = -8,
    BLV_VULKAN_MISSING_INSTANCE_LAYER = -9,
    BLV_VULKAN_DEBUG_UTILS_ERROR = -10,
    BLV_VULKAN_FUNCTION_LOAD_ERROR = -11,
    BLV_VULKAN_SURFACE_ERROR = -12,
    BLV_VULKAN_SWAPCHAIN_ERROR = -13,
    BLV_VULKAN_MISSING_DEVICE_EXTENSIONS = -14,
    BLV_VULKAN_PIPELINE_ERROR = -15,
    BLV_VULKAN_COMMAND_POOL_ERROR = -16,
    BLV_VULKAN_COMMAND_BUFFER_ERROR = -17,
    BLV_VULKAN_QUEUE_SUBMIT_ERROR = -18,
    BLV_VULKAN_BUFFER_ERROR = -19,
    BLV_VULKAN_MEMORY_ERROR = -20,
    BLV_VULKAN_DESCRIPTOR_SET_ERROR = -21,
    BLV_CAMERA_ERROR = -22,
    BLV_INVALID_FUNCTION_INPUT = -23,
} BLV_Result;

typedef enum {
    BLV_LOG_INFO = 0,
    BLV_LOG_DEBUG = 1,
    BLV_LOG_WARNING = 2,
    BLV_LOG_ERROR = 3,
} BLV_Log_Level;

typedef struct {
    BLV_Result error_code;
    const char* function_name;
    const char* file_name;
    uint32_t line_number;
    char message[256];
} blvErrorInfo;

typedef void (*blvPfnErrorCallback)(const blvErrorInfo* error);

_Thread_local extern blvErrorInfo blv_error_last;
extern bool blv_error_log_enable;
extern bool blv_error_validation_layers_enable;
extern blvPfnErrorCallback blv_error_callback;
extern BLV_Log_Level blv_error_log_level;

void blvErrorDefaultCallback(const blvErrorInfo* error);
BLVAPI void blvErrorSetCallback(blvPfnErrorCallback callback);
BLVAPI void blvErrorEnableLog(bool enable);
BLVAPI void blvErrorEnableValidationLayers(bool enable);
const blvErrorInfo* blvErrorGetLast();
void blvErrorClearLast();
const char* blvErrorString(BLV_Result result);
const char* blvErrorLogLevelString(BLV_Log_Level level);
BLVAPI void blvErrorSetLogLevel(BLV_Log_Level level);

#define BLV_SET_ERROR(code, msg, ...) \
    do { \
        if (blv_error_log_enable) { \
            blv_error_last.error_code = code; \
            blv_error_last.function_name = __func__; \
            blv_error_last.file_name = __FILE__; \
            blv_error_last.line_number = __LINE__; \
            snprintf(blv_error_last.message, sizeof(blv_error_last.message), msg, ##__VA_ARGS__); \
            if (blv_error_callback) blv_error_callback(&blv_error_last); \
        } \
    } while(0)


#define BLV_LOG(log_level, msg, ...) \
    do { \
        if (blv_error_log_enable && blv_error_log_level <= log_level) { \
            printf("[%s] ", blvErrorLogLevelString(log_level)); \
            printf(msg, ##__VA_ARGS__); \
        } \
    } while(0)

#endif // BLAHV_LOG_H

