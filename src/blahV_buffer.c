
#include "blahV/vulkan/blahV_buffer.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_utils.h"
#include "blahV/vulkan/blahV_vkMemory.h"
#include <vulkan/vulkan_core.h>

// Size in bytes
BLV_Result blvBufferInit(blvContext *context, VkDeviceSize size, VkBufferUsageFlags usage,
                           VkMemoryPropertyFlags properties, blvBuffer* buffer) {
    VkBufferCreateInfo buffer_info = {0};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(context->device.logical_device, &buffer_info, NULL, &buffer->buffer) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_BUFFER_ERROR, "Failed to create Buffer");
        return BLV_ERROR;
    }

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(context->device.logical_device, buffer->buffer, &memory_requirements);

    VkMemoryAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = memory_requirements.size;
    alloc_info.memoryTypeIndex = blvFindMemoryType(context, memory_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(context->device.logical_device, &alloc_info, NULL, &buffer->memory) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_MEMORY_ERROR, "Failed to allocate buffer memory");
        return BLV_ERROR;
    }

    vkBindBufferMemory(context->device.logical_device, buffer->buffer, buffer->memory, 0);
    return BLV_OK;
}

BLV_Result blvBufferVertexCreate(blvContext *context, blvBuffer* buffer, void* vertices, uint32_t vertices_count) {

    // TODO fix size thing
    VkDeviceSize buffer_size = sizeof(((uint32_t*)vertices)[0]) * vertices_count;
    blvBufferInit(context, buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer);
    blvMemoryUploadDataToBuffer(context, buffer, vertices, vertices_count);

    return BLV_OK;
}

BLV_Result blvBufferIndexCreate(blvContext *context, blvBuffer *buffer, void *indices, uint32_t indices_count) {

    // TODO fix size thing
    VkDeviceSize buffer_size = sizeof(((uint32_t*)indices)[0]) * indices_count;
    blvBufferInit(context, buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer);
    blvMemoryUploadDataToBuffer(context, buffer, indices, indices_count);

    return BLV_OK;
}

void blvBufferDeinit(blvContext *context, blvBuffer* buffer) {
    vkDestroyBuffer(context->device.logical_device, buffer->buffer, NULL);
    vkFreeMemory(context->device.logical_device, buffer->memory, NULL);
}

