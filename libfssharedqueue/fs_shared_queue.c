#include <stdbool.h>
#include <stdint.h>

#include "fs_shared_queue.h"
#include "fence.h"

// This ringbuffer always waste one block. But I think it is necessary to distinct if the buffer is full or empty
bool sddf_fs_queue_push(struct sddf_fs_queue *queue, union sddf_fs_message message) {
    if (sddf_fs_queue_isFull(queue)) {
        return false;
    }
    queue->buffer[queue->write_index] = message;
    THREAD_MEMORY_RELEASE();
    queue->write_index = (queue->write_index + 1) % SDDF_FS_QUEUE_CAPACITY;
    return true;
}

bool sddf_fs_queue_pop(struct sddf_fs_queue *queue, union sddf_fs_message *message) {
    if (sddf_fs_queue_isEmpty(queue)) {
        return false;
    }
    *message = queue->buffer[queue->read_index];
    THREAD_MEMORY_RELEASE();
    queue->read_index = (queue->read_index + 1) % SDDF_FS_QUEUE_CAPACITY;
    return true;
}

bool sddf_fs_queue_isFull(struct sddf_fs_queue *queue) {
    if ((queue->write_index + 1) % SDDF_FS_QUEUE_CAPACITY == queue->read_index) {
        return true;
    }
    return false;
}

bool sddf_fs_queue_isEmpty(struct sddf_fs_queue *queue) {
    if (queue->write_index == queue->read_index) {
        return true;
    }
    return false;
}

// Only client init the queue!
void sddf_fs_init(struct sddf_fs_queue *queue) {
    queue->write_index = 0;
    queue->read_index = 0;
}