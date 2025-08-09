
#include <stdio.h>

#include "blahV/blahV_log.h"

blvErrorInfo blv_error_last = {0};
bool blv_error_log_enable = true;
blv_pfn_error_callback blv_error_callback = blv_error_default_callback;

void blv_default_error_callback(const blvErrorInfo* error) {
    printf("Error in %s:%d (%s): %s - %s\n",
        error->file_name,
        error->line_number,
        error->function_name,
        blv_error_string(error->error_code),
        error->message
    );
}

const char* blv_error_string(BLV_Result result) {
    switch (result) {
        case BLV_UNDEFINED:             return "Undefinied";
        case BLV_OK:                    return "Success";
        case BLV_ERROR:                 return "General Error";
        default:                        return "Unkown Error";
    }
}

void blv_error_enable_log(bool enable) {
    blv_error_log_enable = enable;
}

void blv_error_set_callback(blv_pfn_error_callback callback) {
    blv_error_callback = callback;
}

const blvErrorInfo* blv_error_get_last() {
    return &blv_error_last;
}

void blv_error_clear_last() {
    blv_error_last.error_code = BLV_UNDEFINED;
    blv_error_last.file_name = 0;
    blv_error_last.function_name = 0;
    blv_error_last.line_number = 0;
}

