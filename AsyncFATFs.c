#include "AsyncFATFs.h"
#include "ff15/source/diskio.h"
#include "libblocksharedringbuffer/blk_shared_queue.h"
#include "FiberPool/FiberPool.h"
#include <stdbool.h>
#include <stdint.h>

#define SD 0 /* Map SD card to physical drive 0 */
#define RequestPool_Size 128

#define Coroutine_STACKSIZE 0x40000

#define Client_CH 0
#define Server_CH 1

blk_queue_handle_t *ring_handle;

struct sddf_fs_queue *FATfs_command_queue;
struct sddf_fs_queue *FATfs_completion_queue;

blk_req_queue_t *request;
blk_resp_queue_t *response;

void* Coroutine_STACK_ONE;
void* Coroutine_STACK_TWO;
void* Coroutine_STACK_THREE;
void* Coroutine_STACK_FOUR;

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    DRESULT res;
	int result;
	switch (pdrv) {
	case SD :
        blk_enqueue_req(ring_handle, READ_BLOCKS, (uintptr_t)buff, sector, count,Get_Cohandle());
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
        blk_enqueue_req(ring_handle, WRITE_BLOCKS, (uintptr_t)buff, sector, count,Get_Cohandle());
        Fiber_block();
        break;
    default:
        res = RES_PARERR;
	}
    return res;
}

/*
FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);				
FRESULT f_close (FIL* fp);											
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	
FRESULT f_lseek (FIL* fp, FSIZE_t ofs);								
FRESULT f_truncate (FIL* fp);										
FRESULT f_sync (FIL* fp);
FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);					
*/

void f_open_async() {
    struct f_open_s* args = Fiber_GetArgs();
    args->RET = f_open(args->fp, args->path, args->mode);
    Fiber_kill();
}

void f_close_async() {
    struct f_close_s* args = Fiber_GetArgs();
    args->RET = f_close(args->fp);
    Fiber_kill();
}

void f_read_async() {
    struct f_read_s* args = Fiber_GetArgs();
    args->RET = f_read(args->fp, args->buff, args->btr, args->br);
    Fiber_kill();
}

void f_write_async() {
    struct f_write_s* args = Fiber_GetArgs();
    args->RET = f_write(args->fp, args->buff, args->btw, args->bw);
    Fiber_kill();
}

void f_lseek_async () {
    struct f_lseek_s* args = Fiber_GetArgs();
    args->RET = f_lseek(args->fp, args->ofs);
    Fiber_kill();
}

void f_truncate_async () {
    struct f_truncate_s* args = Fiber_GetArgs();
    args->RET = f_truncate(args->fp);
    Fiber_kill();
}

// Wrapper function for f_sync
void f_sync_async() {
    struct f_sync_s* args = Fiber_GetArgs(); // Get the arguments
    args->RET = f_sync(args->fp); // Call f_sync and store the result
    Fiber_kill(); // Kill the fiber/coroutine
}

void f_opendir_async() {
    struct f_opendir_s* args = Fiber_GetArgs();
    args->RET = f_opendir(args->dp, args->path);
    Fiber_kill();
}

void f_closedir_async() {
    struct f_closedir_s* args = Fiber_GetArgs();
    args->RET = f_closedir(args->dp);
    Fiber_kill();
}

void f_readdir_async() {
    struct f_readdir_s* args = Fiber_GetArgs();
    args->RET = f_readdir(args->dp, args->fno);
    Fiber_kill();
}

void f_findfirst_async() {
    struct f_findfirst_s* args = Fiber_GetArgs();
    args->RET = f_findfirst(args->dp, args->fno, args->path, args->pattern);
    Fiber_kill();
}

void f_findnext_async() {
    struct f_findnext_s* args = Fiber_GetArgs();
    args->RET = f_findnext(args->dp, args->fno);
    Fiber_kill();
}

void f_mkdir_async() {
    struct f_mkdir_s* args = Fiber_GetArgs();
    args->RET = f_mkdir(args->path);
    Fiber_kill();
}

void f_unlink_async() {
    struct f_unlink_s* args = Fiber_GetArgs();
    args->RET = f_unlink(args->path);
    Fiber_kill();
}

void f_rename_async() {
    struct f_rename_s* args = Fiber_GetArgs();
    args->RET = f_rename(args->path_old, args->path_new);
    Fiber_kill();
}

void f_stat_async() {
    struct f_stat_s* args = Fiber_GetArgs();
    args->RET = f_stat(args->path, args->fno);
    Fiber_kill();
}

void f_chmod_async() {
    struct f_chmod_s* args = Fiber_GetArgs();
    args->RET = f_chmod(args->path, args->attr, args->mask);
    Fiber_kill();
}

void f_utime_async() {
    struct f_utime_s* args = Fiber_GetArgs();
    args->RET = f_utime(args->path, args->fno);
    Fiber_kill();
}

void f_chdir_async() {
    struct f_chdir_s* args = Fiber_GetArgs();
    args->RET = f_chdir(args->path);
    Fiber_kill();
}

void f_chdrive_async() {
    struct f_chdrive_s* args = Fiber_GetArgs();
    args->RET = f_chdrive(args->path);
    Fiber_kill();
}

void f_getcwd_async() {
    struct f_getcwd_s* args = Fiber_GetArgs();
    args->RET = f_getcwd(args->buff, args->len);
    Fiber_kill();
}

void f_getfree_async() {
    struct f_getfree_s* args = Fiber_GetArgs();
    args->RET = f_getfree(args->path, args->nclst, args->fatfs);
    Fiber_kill();
}

typedef struct FS_request{
    void (*Request)(void);
    void* data;
} FSRequest;
static FSRequest requestPool[RequestPool_Size];
static uint16_t Ringhead = 0, Ringtail = 0;

ASYNCFRESULT Enqueue_request(void (*Request)(void), void* data) {
    if (((Ringtail + 1) % RequestPool_Size) == Ringhead) {
        return ASYNCFR_TOO_MANY_REQUESTS;
    }
    requestPool[Ringtail].Request = Request;
    requestPool[Ringtail].data = data;
    
    Ringtail = (Ringtail + 1) % RequestPool_Size;
    return ASYNCFR_REQUEST_IN_POOL;
}

int32_t Event_Loop() {
    while (true) {
        
        Fiber_yield();
    }
}

void init(void) {
    // Init the block device queue
    blk_queue_init(ring_handle, request, response, 0, 
    1024, 1024);

}

// mimic microkit_channel
void notified(int ch) {
    switch (ch) {
    case SERIAL_RX_CH:
        active_events |= mp_event_source_serial;
        break;
    case TIMER_CH:
        active_events |= mp_event_source_timer;
        break;
    case VMM_CH:
        /* We have gotten a message from the VMM, which means the framebuffer is ready. */
        active_events |= mp_event_source_framebuffer;
        break;
    case NFS_CH:
        active_events |= mp_event_source_nfs;
        break;
    default:
        printf("MP|ERROR: unexpected notification received from channel: 0x%lx\n", ch);
    }
    if (active_events & mp_blocking_events) {
        co_switch(t_mp);
    }
}