#include "ff15/source/ff.h"
#include "ff15/source/diskio.h"
#include <stdbool.h>
#include <stdint.h>
#include "libblocksharedqueue/blk_shared_queue.h"
#include "FiberPool/FiberPool.h"

#define SD 0 /* Map SD card to physical drive 0 */

extern blk_queue_handle_t *blk_queue_handle;

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	default:
		return RES_OK;
	}
	return STA_NOINIT;
}

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	default:

		return RES_OK;
	}
	return STA_NOINIT;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff) {
    DRESULT res;
    switch (pdrv) {
    default:
        if (cmd == GET_SECTOR_SIZE) {
            WORD *size = buff;
            *size = 512;
            res = RES_OK;
        }
	}
    return res;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    DRESULT res;
	switch (pdrv) {
	default: {
        blk_enqueue_req(blk_queue_handle, READ_BLOCKS, (uintptr_t)buff, sector, count,Get_Cohandle());
        Fiber_block();
        res = (DRESULT)(uintptr_t)Fiber_GetArgs();
        break;
    }
	}
    return res;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    DRESULT res;
	switch (pdrv) {
	default:
        blk_enqueue_req(blk_queue_handle, WRITE_BLOCKS, (uintptr_t)buff, sector, count,Get_Cohandle());
        Fiber_block();
        res = (DRESULT)(uintptr_t)Fiber_GetArgs();
        break;
	}
    return res;
}