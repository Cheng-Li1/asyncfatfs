#include "ff15/source/ff.h"
#include "ff15/source/diskio.h"
#include "libblocksharedringbuffer/sddf_blk_shared_ringbuffer.h"
#include "FiberPool/FiberPool.h"
#include <stdint.h>

#define SD 0 /* Map SD card to physical drive 0 */

sddf_blk_ring_handle_t *ring_handle;

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    DRESULT res;
	int result;
	switch (pdrv) {
	case SD :
        sddf_blk_enqueue_cmd(ring_handle,SDDF_BLK_COMMAND_READ, (uintptr_t)buff, sector, count,Get_Cohandle());
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
        sddf_blk_enqueue_cmd(ring_handle,SDDF_BLK_COMMAND_WRITE, (uintptr_t)buff, sector, count,Get_Cohandle());
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
*/

struct f_open_s{
    FIL* fp;
    const TCHAR* path;
    BYTE mode;
    FRESULT RET;
};

void f_open_async() {
    struct f_open_s* args = Fiber_GetArgs();
    args->RET = f_open(args->fp, args->path, args->mode);
    Fiber_kill();
}