#include "AsyncFATFs.h"
#include "ff15/source/diskio.h"
#include "ff15/source/ff.h"
#include "libblocksharedqueue/blk_shared_queue.h"
#include "FiberPool/FiberPool.h"
#include "libFSsharedqueue/fs_shared_queue.h"
#include <stdbool.h>
#include <stdint.h>

#define SD 0 /* Map SD card to physical drive 0 */
#define RequestPool_Size 128

#define Coroutine_STACKSIZE 0x40000

#define Client_CH 0
#define Server_CH 1

blk_queue_handle_t *blk_queue_handle;

struct sddf_fs_queue *FATfs_command_queue;
struct sddf_fs_queue *FATfs_completion_queue;

blk_req_queue_t *request;
blk_resp_queue_t *response;

void* Coroutine_STACK_ONE;
void* Coroutine_STACK_TWO;
void* Coroutine_STACK_THREE;
void* Coroutine_STACK_FOUR;

typedef enum {
    FREE,
    INUSE
} space_status;

typedef struct FS_request{
    /* Client side cmd info */
    void (*Request)(void);
    uint64_t args[9];
    uint64_t request_id;
    /* Client side response info */
    uint64_t data[2];
    int32_t status;
    /* FiberPool metadata */
    co_handle handle;
    /* self metadata */
    space_status stat;
} FSRequest;

static FSRequest RequestPool[MAX_COROUTINE_NUM];

void Fill_Client_Response(union sddf_fs_message* message, const FSRequest* Finished_Request) {
    message->completion.status = Finished_Request->args[Status_bit];
    message->completion.data[0] = Finished_Request->args[First_data_bit];
    message->completion.data[1] = Finished_Request->args[Second_data_bit];
    return;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    DRESULT res;
	int result;
	switch (pdrv) {
	case SD :
        blk_enqueue_req(blk_queue_handle, READ_BLOCKS, (uintptr_t)buff, sector, count,Get_Cohandle());
        Fiber_block();
        break;
    default:
        res = RES_PARERR;
	}
    return res;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    DRESULT res;
	int result;
	switch (pdrv) {
	case SD :
        blk_enqueue_req(blk_queue_handle, WRITE_BLOCKS, (uintptr_t)buff, sector, count,Get_Cohandle());
        Fiber_block();
        break;
    default:
        res = RES_PARERR;
	}
    return res;
}

void init(void) {
    // Init the block device queue
    blk_queue_init(blk_queue_handle, request, response, 0, 
    1024, 1024);

}

// mimic microkit_channel
void notified(int ch) {
    switch (ch) {
    case Client_CH: {
        union sddf_fs_message* message = NULL;
        while (sddf_fs_queue_pop(FATfs_command_queue, message)) {
            
        }
        break;
    }
    case Server_CH: {
        blk_response_status_t *status;
        uintptr_t *addr;
        uint16_t *count;
        uint16_t *success_count;
        uint32_t *id;
        while (!blk_req_queue_empty(blk_queue_handle)) {
            blk_dequeue_resp(blk_queue_handle, status, addr, count, success_count, id);
            FiberPool_SetArgs(RequestPool[*id].handle, (void* )(*status));
            Fiber_wake(RequestPool[*id].handle);
        }
        break;
    }
    default:
        break;
    }
    Fiber_yield();
    /** 
       If the code below get executed, then all the working coroutines are either blocked or finished.
       So the code below would send the result back to client through SDDF and do the cleanup for finished 
       coroutines. After that, the main coroutine coroutine would block wait on new requests or server sending
       responses.
    **/
    int32_t i;
    union sddf_fs_message message;
    for (i = 1; i < MAX_COROUTINE_NUM; i++) {
        if (RequestPool[i].handle == INVALID_COHANDLE && RequestPool[i].stat == INUSE) {
            message.completion.request_id = RequestPool[i].request_id;
            Fill_Client_Response(&message, &(RequestPool[i]));
            sddf_fs_queue_push(FATfs_completion_queue, message);
        }
    }
}