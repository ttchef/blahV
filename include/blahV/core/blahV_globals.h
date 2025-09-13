
#ifndef BLAHV_GLOBALS_H
#define BLAHV_GLOBALS_H

#define BLV_MAX_INSTANCE_EXTENSIONS 16
#define BLV_MAX_DEVICE_EXTENSIONS 16

#if defined(_WIN32) || defined(_WIN64)
    #ifdef BLAHV_BUILD_SHARED
        #define BLVAPI __declspec(dllexport)
    #else 
        #define BLVAPI __declspec(dllimport)
    #endif
#else 
    #define BLVAPI __attribute((visibility("default")))
#endif

#endif // GLOABLS_H
