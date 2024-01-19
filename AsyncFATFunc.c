#include "AsyncFATFs.h"
#include "ff15/source/ff.h"
#include "FiberPool/FiberPool.h"

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

void f_findfirst_async() {
    struct f_findfirst_s* args = Fiber_GetArgs();
    args->RET = f_findfirst(args->dp, args->fno, args->path, args->pattern);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_findnext_async() {
    struct f_findnext_s* args = Fiber_GetArgs();
    args->RET = f_findnext(args->dp, args->fno);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

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

void f_chmod_async() {
    struct f_chmod_s* args = Fiber_GetArgs();
    args->RET = f_chmod(args->path, args->attr, args->mask);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_utime_async() {
    struct f_utime_s* args = Fiber_GetArgs();
    args->RET = f_utime(args->path, args->fno);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_chdir_async() {
    struct f_chdir_s* args = Fiber_GetArgs();
    args->RET = f_chdir(args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_chdrive_async() {
    struct f_chdrive_s* args = Fiber_GetArgs();
    args->RET = f_chdrive(args->path);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_getcwd_async() {
    struct f_getcwd_s* args = Fiber_GetArgs();
    args->RET = f_getcwd(args->buff, args->len);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}

void f_getfree_async() {
    struct f_getfree_s* args = Fiber_GetArgs();
    args->RET = f_getfree(args->path, args->nclst, args->fatfs);
    Function_Fill_Response(args, args->RET, 0, 0);
    Fiber_kill();
}