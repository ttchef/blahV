
#ifndef BLAHV_LOG_H
#define BLAHV_LOG_H

#include <stdbool.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>

typedef enum {
    BLV_UNDEFINED = 1,
    BLV_OK = 0,
    BLV_ERROR = -1,
    BLV_GLFW_ERROR = -2,
} BLV_Result;

typedef struct {
    BLV_Result error_code;
    const char* function_name;
    const char* file_name;
    uint32_t line_number;
    char message[256];
} blvErrorInfo;

typedef void (*blv_pfn_error_callback)(const blvErrorInfo* error);

extern blvErrorInfo blv_error_last;
extern bool blv_error_log_enable;
extern blv_pfn_error_callback blv_error_callback;

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

void blv_error_default_callback(const blvErrorInfo* error);
void blv_error_set_callback(blv_pfn_error_callback callback);
void blv_error_enable_log(bool enable);
const blvErrorInfo* blv_error_get_last();
void blv_error_clear_last();
const char* blv_error_string(BLV_Result result);

#endif // BLAHV_LOG_H
