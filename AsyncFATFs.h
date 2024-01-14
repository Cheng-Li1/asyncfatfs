#include "ff15/source/ff.h"

struct f_open_s{
    FIL* fp;
    const TCHAR* path;
    BYTE mode;
    FRESULT RET;
};
void f_open_async();

struct f_close_s{
    FIL* fp;
    FRESULT RET;
};
void f_close_async();

struct f_read_s{
    FIL* fp; 
    void* buff;
    UINT btr;
    UINT* br;
    FRESULT RET;
};
void f_read_async();

struct f_write_s{
    FIL* fp;
    const void* buff; 
    UINT btw;
    UINT* bw;
    FRESULT RET;
};
void f_write_async();

struct f_lseek_s{
    FIL* fp;
    FSIZE_t ofs;
    FRESULT RET;
};
void f_lseek_async ();

struct f_truncate_s{
    FIL* fp;
    FRESULT RET;
};
void f_truncate_async ();

struct f_sync_s {
    FIL* fp;
    FRESULT RET; // Field to store the return value
};
// Wrapper function for f_sync
void f_sync_async();

struct f_opendir_s {
    DIR* dp;
    const TCHAR* path;
    FRESULT RET;
};
void f_opendir_async();

struct f_closedir_s {
    DIR* dp;
    FRESULT RET;
};
void f_closedir_async();

struct f_readdir_s {
    DIR* dp;
    FILINFO* fno;
    FRESULT RET;
};
void f_readdir_async();

struct f_findfirst_s {
    DIR* dp;
    FILINFO* fno;
    const TCHAR* path;
    const TCHAR* pattern;
    FRESULT RET;
};
void f_findfirst_async();

struct f_findnext_s {
    DIR* dp;
    FILINFO* fno;
    FRESULT RET;
};
void f_findnext_async();

struct f_mkdir_s {
    const TCHAR* path;
    FRESULT RET;
};
void f_mkdir_async();

struct f_unlink_s {
    const TCHAR* path;
    FRESULT RET;
};
void f_unlink_async();

struct f_rename_s {
    const TCHAR* path_old;
    const TCHAR* path_new;
    FRESULT RET;
};
void f_rename_async();

struct f_stat_s {
    const TCHAR* path;
    FILINFO* fno;
    FRESULT RET;
};
void f_stat_async();

struct f_chmod_s {
    const TCHAR* path;
    BYTE attr;
    BYTE mask;
    FRESULT RET;
};
void f_chmod_async();

struct f_utime_s {
    const TCHAR* path;
    const FILINFO* fno;
    FRESULT RET;
};
void f_utime_async();

struct f_chdir_s {
    const TCHAR* path;
    FRESULT RET;
};
void f_chdir_async();

struct f_chdrive_s {
    const TCHAR* path;
    FRESULT RET;
};
void f_chdrive_async();

struct f_getcwd_s {
    TCHAR* buff;
    UINT len;
    FRESULT RET;
};
void f_getcwd_async();

struct f_getfree_s {
    const TCHAR* path;
    DWORD* nclst;
    FATFS** fatfs;
    FRESULT RET;
};
void f_getfree_async();

