
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

#define DA_SET_ERROR(code, msg, ...) \
    do { \
        if (da_error_log_enable) { \
            da_last_error.error_code = code; \
            da_last_error.function_name = __func__; \
            da_last_error.file_name = __FILE__; \
            da_last_error.line_number = __LINE__; \
            snprintf(da_last_error.message, sizeof(da_last_error.message), msg, ##__VA_ARGS__); \
            if (da_error_callback) da_error_callback(&da_last_error); \
        } \
    } while(0)

void blv_error_default_callback(const blvErrorInfo* error);
void blv_error_set_callback(blv_pfn_error_callback callback);
void blv_error_enable_log(bool enable);
const blvErrorInfo* blv_error_get_last();
void blv_error_clear_last();
const char* blv_error_string(BLV_Result result);

#endif // BLAHV_LOG_H
