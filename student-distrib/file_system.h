#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include "types.h"
#include "multiboot.h"
#include "lib.h"
#include "pcb.h"
#include "do_sys.h"


#define BOOT_BLOCK_RESERVED_TOTAL    52
#define DENTRY_RESERVED_TOTAL        24
#define FILE_NAME_LEN                32
#define DENTRY_TOTAL                 63
#define DENTRY_ALL                   64
#define BLOCKS_SIZE                  4096
#define BLOCKS_TOTAL_MINUS           1023
#define BLOCK_NUM                    1024
#define FOUR_BYTE                    4
#define SAME_STR                     0
#define TYPE_RTC                     0
#define TYPE_DIR                     1
#define TYPE_FILE                    2



typedef struct dentry
{
    uint8_t file_name[FILE_NAME_LEN];
    uint32_t file_type;
    uint32_t inodes;
    uint8_t dentry_reserved[DENTRY_RESERVED_TOTAL];
} dentry_t;

typedef struct inode
{
    uint32_t length;
    uint32_t file_blocks[BLOCKS_TOTAL_MINUS];
} inode_t;

typedef struct boot_block
{
    uint32_t num_dir_entries;
    uint32_t num_inodes;
    uint32_t num_data_blocks;
    uint8_t boot_block_reserved[BOOT_BLOCK_RESERVED_TOTAL];
    dentry_t dentries[DENTRY_TOTAL];
} boot_block_t;

int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t *dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t length);

void init_file_system(module_t* module);

int32_t file_open(const uint8_t * filename);
int32_t file_close(int32_t fd);
int32_t file_read(int32_t fd, void *buf, int32_t length);
int32_t file_write(int32_t fd, const void *buf, int32_t length);

int32_t dir_open(const uint8_t * filename);
int32_t dir_close(int32_t fd);
int32_t dir_read(int32_t fd, void * buf, int32_t length);
int32_t dir_write(int32_t fd, const void *buf, int32_t length);
uint32_t get_length(uint32_t inodes);

/* create local variables */
 uint32_t *fs_start;
 inode_t *inodes_start;
 uint32_t *data_blocks_start;
 struct boot_block *boot_block;
 dentry_t file_open_dentry;

#endif




