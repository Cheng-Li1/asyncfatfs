#include "ff15/source/ff.h"

typedef enum {
	ASYNCFR_OK = 0,				/* (0) Succeeded */
	ASYNCFR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	ASYNCFR_INT_ERR,				/* (2) Assertion failed */
	ASYNCFR_NOT_READY,			/* (3) The physical drive cannot work */
	ASYNCFR_NO_FILE,				/* (4) Could not find the file */
	ASYNCFR_NO_PATH,				/* (5) Could not find the path */
	ASYNCFR_INVALID_NAME,		/* (6) The path name format is invalid */
	ASYNCFR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	ASYNCFR_EXIST,				/* (8) Access denied due to prohibited access */
	ASYNCFR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	ASYNCFR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	ASYNCFR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	ASYNCFR_NOT_ENABLED,			/* (12) The volume has no work area */
	ASYNCFR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	ASYNCFR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any problem */
	ASYNCFR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	ASYNCFR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	ASYNCFR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	ASYNCFR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > FF_FS_LOCK */
	ASYNCFR_INVALID_PARAMETER,	    /* (19) Given parameter is invalid */
    ASYNCFR_REQUEST_IN_POOL,        /* (20) Extended, Request successfully queued */
    ASYNCFR_TOO_MANY_REQUESTS,      /* (21) Extended, Request Pool is full */
} ASYNCFRESULT;

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