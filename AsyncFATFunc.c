#include "AsyncFATFs.h"
#include "ff15/source/ff.h"
#include "FiberPool/FiberPool.h"
#include <stdbool.h>
#include <string.h>
#include "libfssharedqueue/fs_shared_queue.h"

/*
This file define a bunch of wrapper functions of FATFs functions so thos functions can be run in the 
coroutine.
*/

void Function_Fill_Response(void* data, FRESULT result, uint64_t RETDATA, uint64_t RETDATA2) {
    uint64_t *args = (uint64_t *) data;
    args[Status_bit] = result;
    args[First_data_bit] = RETDATA;
    args[Second_data_bit] = RETDATA2;
    return;
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

void f_mount_async() {
    struct f_mount_s* args = Fiber_GetArgs();
    args->RET = f_mount(args->fs, args->path, args->opt);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_open_async() {
    struct f_open_s* args = Fiber_GetArgs();
    args->RET = f_open(args->fp, args->path, args->mode);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_close_async() {
    struct f_close_s* args = Fiber_GetArgs();
    args->RET = f_close(args->fp);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_read_async() {
    struct f_read_s* args = Fiber_GetArgs();
    args->RET = f_read(args->fp, args->buff, args->btr, args->br);
    Function_Fill_Response(args, args->RET, *(args->br), 0);
    Fiber_kill();
}

void f_write_async() {
    struct f_write_s* args = Fiber_GetArgs();
    args->RET = f_write(args->fp, args->buff, args->btw, args->bw);
    Function_Fill_Response(args, args->RET, *(args->bw), 0);
    Fiber_kill();
}

void f_lseek_async () {
    struct f_lseek_s* args = Fiber_GetArgs();
    args->RET = f_lseek(args->fp, args->ofs);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_pread_async() {
    struct f_pread_s* args = Fiber_GetArgs();
    args->RET = f_lseek(args->fp, args->ofs);
    if (args->RET != ASYNCFR_OK) {
        args->br = 0;
        Function_Fill_Response(args, args->RET, 0, 0);
        Fiber_kill();
    }
    args->RET = f_read(args->fp, args->buff, args->btr, args->br);
    Function_Fill_Response(args, args->RET, *(args->br), 0);
    Fiber_kill();
}

void f_pwrite_async() {
    struct f_pwrite_s* args = Fiber_GetArgs();
    args->RET = f_lseek(args->fp, args->ofs);
    if (args->RET != ASYNCFR_OK) {
        args->bw = 0;
        Function_Fill_Response(args, args->RET, 0, 0);
        Fiber_kill();
    }
    args->RET = f_write(args->fp, args->buff, args->btw, args->bw);
    Function_Fill_Response(args, args->RET, *(args->bw), 0);
    Fiber_kill();
}

void f_truncate_async () {
    struct f_truncate_s* args = Fiber_GetArgs();
    args->RET = f_truncate(args->fp);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

// Wrapper function for f_sync
void f_sync_async() {
    struct f_sync_s* args = Fiber_GetArgs(); // Get the arguments
    args->RET = f_sync(args->fp); // Call f_sync and store the result
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill(); // Kill the fiber/coroutine
}

void f_opendir_async() {
    struct f_opendir_s* args = Fiber_GetArgs();
    args->RET = f_opendir(args->dp, args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_closedir_async() {
    struct f_closedir_s* args = Fiber_GetArgs();
    args->RET = f_closedir(args->dp);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_readdir_async() {
    struct f_readdir_s* args = Fiber_GetArgs();
    args->RET = f_readdir(args->dp, args->fno);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}
/*
void f_findfirst_async() {
    struct f_findfirst_s* args = Fiber_GetArgs();
    args->RET = f_findfirst(args->dp, args->fno, args->path, args->pattern);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}*/

/*
void f_findnext_async() {
    struct f_findnext_s* args = Fiber_GetArgs();
    args->RET = f_findnext(args->dp, args->fno);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}*/

void f_mkdir_async() {
    struct f_mkdir_s* args = Fiber_GetArgs();
    args->RET = f_mkdir(args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_unlink_async() {
    struct f_unlink_s* args = Fiber_GetArgs();
    args->RET = f_unlink(args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_rename_async() {
    struct f_rename_s* args = Fiber_GetArgs();
    args->RET = f_rename(args->path_old, args->path_new);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_stat_async() {
    struct f_stat_s* args = Fiber_GetArgs();
    args->RET = f_stat(args->path, args->fno);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

/*
void f_chmod_async() {
    struct f_chmod_s* args = Fiber_GetArgs();
    args->RET = f_chmod(args->path, args->attr, args->mask);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}*/

/*
void f_utime_async() {
    struct f_utime_s* args = Fiber_GetArgs();
    args->RET = f_utime(args->path, args->fno);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}
*/
/*
void f_chdir_async() {
    struct f_chdir_s* args = Fiber_GetArgs();
    args->RET = f_chdir(args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}*/

/*
void f_chdrive_async() {
    struct f_chdrive_s* args = Fiber_GetArgs();
    args->RET = f_chdrive(args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}
*/

/*
void f_getcwd_async() {
    struct f_getcwd_s* args = Fiber_GetArgs();
    args->RET = f_getcwd(args->buff, args->len);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}
*/
void f_getfree_async() {
    struct f_getfree_s* args = Fiber_GetArgs();
    args->RET = f_getfree(args->path, args->nclst, args->fatfs);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

# define MAX_FATFS 1
# define MAX_OPENED_FILENUM 128
# define MAX_OPENED_DIRNUM 128

static FATFS Fatfs[MAX_FATFS];

static bool File_FreeList[MAX_OPENED_FILENUM];
static FIL Files[MAX_OPENED_FILENUM];

static bool Dirs_FreeList[MAX_OPENED_DIRNUM];
static DIR Dirs[MAX_OPENED_DIRNUM];

uint32_t Find_FreeFile() {
    uint32_t i;
    for (i = 0; i < MAX_OPENED_FILENUM; i++) {
        if (File_FreeList[i] == 0) {
            return i;
        }
    }
    return i;
}

uint32_t Find_FreeDir() {
    uint32_t i;
    for (i = 0; i < MAX_OPENED_DIRNUM; i++) {
        if (Dirs_FreeList[i] == 0) {
            return i;
        }
    }
    return i;
}

// Change here later to support more than one FAT volumes
void fat_mount() {
    uint64_t *args = Fiber_GetArgs();
    FRESULT RET = f_mount(&(Fatfs[0]), "", 1);
    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_open() {
    uint64_t *args = Fiber_GetArgs();
    uint32_t fd = Find_FreeFile();
    if (fd == MAX_OPENED_FILENUM) {
        Function_Fill_Response(args, FR_TOO_MANY_OPEN_FILES, 0, 0);
        Fiber_kill();
    }
    FIL* file = &(Files[fd]);

    FRESULT RET = f_open(file, (void *)args[0], args[1]);
    
    // Error handling
    if (RET != FR_OK) {
        Function_Fill_Response(args, RET, 0, 0);
        Fiber_kill();
    }
    
    // Set the position to 1 to indicate this file structure is in use
    File_FreeList[fd] = 1;

    Function_Fill_Response(args, RET, fd, 0);
    Fiber_kill();
}

void fat_pwrite() {
    uint64_t *args = Fiber_GetArgs();
    uint64_t fd = args[0];
    void* data = (void *) args[1];
    uint64_t btw = args[2];
    uint64_t offset = args[3];
    
    // Maybe add validation check of file descriptor here
    FIL* file = &(Files[fd]);
    FRESULT RET = FR_TIMEOUT;

    RET = f_lseek(file, offset);

    if (RET != FR_OK) {
        Function_Fill_Response(args, RET, 0, 0);
        Fiber_kill();
    }
    
    uint32_t bw = 0;

    RET = f_write(file, data, btw, &bw);

    Function_Fill_Response(args, RET, bw, 0);
    Fiber_kill();
}

void fat_pread() {
    uint64_t *args = Fiber_GetArgs();
    uint64_t fd = args[0];
    void* data = (void *) args[1];
    uint64_t btr = args[2];
    uint64_t offset = args[3];
    
    // Maybe add validation check of file descriptor here
    FIL* file = &(Files[fd]);
    FRESULT RET = FR_TIMEOUT;

    RET = f_lseek(file, offset);

    if (RET != FR_OK) {
        Function_Fill_Response(args, RET, 0, 0);
        Fiber_kill();
    }
    
    uint32_t br = 0;

    RET = f_read(file, data, btr, &br);

    Function_Fill_Response(args, RET, br, 0);
    Fiber_kill();
}

void fat_close() {
    uint64_t *args = Fiber_GetArgs();
    uint64_t fd = args[0];

    if (fd < 0 || fd >= MAX_OPENED_FILENUM) {
        Function_Fill_Response(args, FR_INVALID_PARAMETER, 0, 0);
        Fiber_kill();
    }

    FRESULT RET = f_close(&(Files[fd]));
    if (RET == FR_OK) {
        File_FreeList[fd] = 0;
    }
    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

// Mode attribute
#define Directory 0040000
#define Regularfile 0100000
#define Blockdevice 0060000
#define Socket 0140000

void fat_stat() {
    uint64_t *args = Fiber_GetArgs();
    
    const void* filename = (void*) args[0];

    struct sddf_fs_stat_64* file_stat = (void*)args[2];
    FILINFO fileinfo;

    FRESULT RET = f_stat(filename, &fileinfo);
    if (RET != FR_OK) {
        Function_Fill_Response(args, RET, 0, 0);
        Fiber_kill();
    }
    
    memset(file_stat, 0, sizeof(struct sddf_fs_stat_64));
    file_stat->atime = fileinfo.ftime;
    file_stat->ctime = fileinfo.ftime;
    file_stat->mtime = fileinfo.ftime;
    
    file_stat->size = fileinfo.fsize;

// Now we have only one fat volume, so we can hard code it here
    file_stat->blksize = Fatfs[0].ssize;

// Study how is the structure of the mode, just leave it for now
    file_stat->mode = 0;
    if (fileinfo.fattrib & AM_DIR) {
        file_stat->mode |= 040755; // Directory with rwx for owner, rx for group and others
    } else {
        // Assume regular file, apply read-only attribute
        file_stat->mode |= 0444; // Readable by everyone
    }
    // Adjust for AM_RDO, if applicable
    if (fileinfo.fattrib & AM_RDO) {
        // If read-only and it's not a directory, remove write permissions.
        // Note: For directories, AM_RDO doesn't make sense to apply as "write"
        // because directories need to be writable for creating/removing files.
        if (!(fileinfo.fattrib & AM_DIR)) {
            file_stat->mode &= ~0222; // Remove write permissions
        }
    }

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_rename() {
    uint64_t *args = Fiber_GetArgs();
    uint64_t fd = args[0];

    const void* oldpath = (void*)args[0];
    const void* newpath = (void*)args[2];
    
    FRESULT RET = f_rename(oldpath, newpath);
   
    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_unlink() {
    uint64_t *args = Fiber_GetArgs();

    const void* path = (void*)(args[0]);

    FRESULT RET = f_unlink(path);

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_mkdir() {
    uint64_t *args = Fiber_GetArgs();

    const void* path = (void*)(args[0]);

    FRESULT RET = f_mkdir(path);

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_rmdir() {
    uint64_t *args = Fiber_GetArgs();
    const void* path = (void*)(args[0]);

    FRESULT RET = f_rmdir(path);

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_opendir() {
    uint64_t *args = Fiber_GetArgs();
    uint32_t fd = Find_FreeDir();
    if (fd == MAX_OPENED_DIRNUM) {
        Function_Fill_Response(args, FR_TOO_MANY_OPEN_FILES, 0, 0);
        Fiber_kill();
    }
    DIR* dir = &(Dirs[fd]);

    FRESULT RET = f_opendir(dir, (void *)args[0]);
    
    // Error handling
    if (RET != FR_OK) {
        Function_Fill_Response(args, RET, 0, 0);
        Fiber_kill();
    }
    
    // Set the position to 1 to indicate this file structure is in use
    Dirs_FreeList[fd] = 1;

    Function_Fill_Response(args, RET, fd, 0);
    Fiber_kill();
}

void fat_readdir() {
    uint64_t *args = Fiber_GetArgs();
    
    // Maybe add validation check of file descriptor here
    uint64_t fd = args[0];
    void* name = (void*)args[1];
    
    FILINFO fno;
    FRESULT RET = f_readdir(&Dirs[fd], &fno);
    
    if (RET == FR_OK) {
        strcpy(name, fno.fname);
    }

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();    
}

void fat_rewinddir() {
    uint64_t *args = Fiber_GetArgs();
    
    // Maybe add validation check of file descriptor here
    uint64_t fd = args[0];

    FRESULT RET = f_readdir(&Dirs[fd], 0);

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_sync() {
    uint64_t *args = Fiber_GetArgs();

    // Maybe add validation check of file descriptor here
    uint64_t fd = args[0];

    FRESULT RET = f_sync(&(Files[fd]));

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}

void fat_closedir() {
    uint64_t *args = Fiber_GetArgs();

    // Maybe add validation check of file descriptor here
    uint64_t fd = args[0];

    FRESULT RET = f_closedir(&Dirs[fd]);

    Function_Fill_Response(args, RET, 0, 0);
    Fiber_kill();
}