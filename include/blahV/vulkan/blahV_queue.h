
#ifndef BLAHV_QUEUE_H
#define BLAHV_QUEUE_H

#include <vulkan/vulkan.h>

typedef struct {
    VkQueue queue;
    uint32_t family_index;
} blvQueue;

#endif
