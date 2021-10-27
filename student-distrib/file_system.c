#include "file_system.h"

/*
 * init_file_system
 *   DESCRIPTION: initialize the file system
 *   INPUTS: module -- module to initialize the system
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void init_file_system(module_t *module)
{
    if (module == NULL)
    {
        return;
    }

    /************************************/
    /* Greatest TA Benny's lecture note */
    /* inode_t* ptr = 0x100000;         */
    /* ptr++;                           */
    /* ptr = 0x101000 not 0x100001;     */
    /************************************/

    // initialize the file system
    fs_start = (uint32_t *)module->mod_start;
    boot_block = (boot_block_t *)module->mod_start;
    inodes_start = (inode_t *)((uint32_t)fs_start + BLOCKS_SIZE);
    data_blocks_start = (uint32_t *)((uint32_t)fs_start + (boot_block->num_inodes) * BLOCKS_SIZE + BLOCKS_SIZE);
}

/*
 * read_dentry_by_name
 *   DESCRIPTION: find the corresponding dentry by the given name
 *   INPUTS: const uint8_t *fname -- the name to find the dentry
 *           dentry_t *dentry -- the place storing all dentries
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *                 -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry)
{
    int i;             // loop counter
    int cmp_str_check; // 0 for same string
    dentry_t *cur_dentry;

    if (fname == NULL || dentry == NULL)
    {
        return -1;
    }

    for (i = 0; i < FILE_NAME_LEN; i++)
    {
        if (fname[i] == '\0')
        { // "\ 0" is the last character of string
            break;
        }
    }

    if (i == (FILE_NAME_LEN) && fname[i] != '\0')
    { // "\ 0" is the last character of string
        return -1;
    }

    // loop over all the dir and find the match dir
    for (i = 0; i < boot_block->num_dir_entries; i++)
    {
        cur_dentry = &(boot_block->dentries[i]);
        cmp_str_check = strncmp((int8_t *)fname, (int8_t *)cur_dentry->file_name, FILE_NAME_LEN);
        // if find a match, copy the data
        if (cmp_str_check == SAME_STR)
        {
            memcpy(dentry, cur_dentry, DENTRY_ALL);
            return 0;
        }
    }

    return -1;
}

/*
 * read_dentry_by_index
 *   DESCRIPTION: find the corresponding dentry by the given index
 *   INPUTS: const uint32_t index -- the index to find the dentry
 *           dentry_t *dentry -- the place storing all dentries
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *                 -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t *dentry)
{
    dentry_t *cur_dentry;

    if (dentry == NULL)
    {
        return -1;
    }

    // check if the index is valid
    if (index >= boot_block->num_dir_entries || index < 0) // index cannot be negative
    {
        return -1;
    }

    // if the index is valid, copy the directory entry info to dentry
    cur_dentry = &(boot_block->dentries[index]);
    memcpy(dentry, cur_dentry, DENTRY_ALL);
    return 0;
}

/*
 * read_data
 *   DESCRIPTION: read the data in the corresponding inode of length,
 *                and write into buf
 *   INPUTS: uint32_t inode -- inodes to read
 *           uint32_t offset -- jump to the needed place
 *           uint8_t *buf -- place to write the data
 *           uint32_t length -- length of data
 *   OUTPUTS: none
 *   RETURN VALUE: the length wrote in buf
 *                 -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t length)
{
    int i;
    inode_t *cur_node;
    int cur_block_index = 0;        // initialize to 0
    int first_block_data_read = 0;  // initialize to 0
    int num_data_block_to_read = 0; // initialize to 0
    int offset_off = 0;             // initialize to 0
    uint32_t *cur_data;
    uint32_t buf_offset = 0; // initialize to 0
    uint32_t origin_length = length;

    // sanity check
    /* make sure that length, offset and inode are not negative values */
    if (buf == NULL || length < 0 || offset < 0 || inode < 0 || inode >= boot_block->num_inodes)
    {
        return -1;
    }

    // get the current inode pointer
    cur_node = (inode_t *)((uint32_t)inodes_start + inode * BLOCKS_SIZE);

    if (offset > cur_node->length)
    {
        return 0;
    }

    // // check if the given inode is a file type
    // for (i=0; i<boot_block->num_dir_entries; i++){
    //     if((boot_block->dentries[i]).inodes == inode){
    //         if (boot_block->dentries[i].file_type != TYPE_FILE)
    //             return -1;
    //     }
    // }

    // if the current node length is zero, return directly
    if (cur_node->length == 0 || offset == cur_node->length)
    {
        return 0;
    }

    // check if the length is larger than the valid length
    if (length > (cur_node->length - offset))
    {
        length = cur_node->length - offset;
    }

    origin_length = length;

    offset_off = offset / BLOCKS_SIZE;

    // calculate how many bytes to read in the first block
    first_block_data_read = BLOCKS_SIZE - (offset % BLOCKS_SIZE);

    // calculate how many data blocks to read except the first and the last

    if (length >= BLOCKS_SIZE)
    {
        num_data_block_to_read = (length - first_block_data_read) / BLOCKS_SIZE;
    }

    if ((length - first_block_data_read) % BLOCKS_SIZE == 0)
    {
        num_data_block_to_read--;
    }

    // copy the first block data to temp buffer
    cur_block_index = cur_node->file_blocks[offset_off];

    if (cur_block_index < 0 || cur_block_index >= boot_block->num_data_blocks)
    {
        return -1;
    }

    cur_data = (uint32_t *)((uint32_t)data_blocks_start + cur_block_index * BLOCKS_SIZE);
    cur_data = (uint32_t *)((uint32_t)cur_data + BLOCKS_SIZE - first_block_data_read);
    if (length > first_block_data_read)
    {
        memcpy(buf, cur_data, first_block_data_read);
        buf_offset = first_block_data_read;
        length = length - first_block_data_read;
    }
    else
    {
        memcpy(buf, cur_data, length);
        buf_offset = first_block_data_read;
        length = 0; // prevent to read the next data
    }

    if (length == 0) // if nothing to read directly return
    {
        return origin_length;
    }

    for (i = 0; i < num_data_block_to_read; i++)
    {
        cur_block_index = cur_node->file_blocks[i + offset_off + 1];               // skip the boot block
        if (cur_block_index < 0 || cur_block_index >= boot_block->num_data_blocks) // index cannot be negative
        {
            return -1;
        }
        cur_data = (uint32_t *)((uint32_t)data_blocks_start + cur_block_index * BLOCKS_SIZE);
        memcpy((uint8_t *)((uint32_t)buf + buf_offset), cur_data, BLOCKS_SIZE);
        buf_offset = buf_offset + BLOCKS_SIZE;
        length = length - BLOCKS_SIZE;
    }

    // copy the last block
    cur_block_index = cur_node->file_blocks[num_data_block_to_read + offset_off + 1];
    if (cur_block_index < 0 || cur_block_index >= boot_block->num_data_blocks) // index cannot be negative
    {
        return -1;
    }
    cur_data = (uint32_t *)((uint32_t)data_blocks_start + cur_block_index * BLOCKS_SIZE);
    memcpy((uint32_t *)((uint32_t)buf + buf_offset), cur_data, length);

    return origin_length;
    //memcpy(void* dest, const void* src, uint32_t n)
}

/*
 * file_open
 *   DESCRIPTION: open the corresponding file by the name
 *   INPUTS: uint8_t *fname -- name of the file to be open
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *                 -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t file_open(const uint8_t *filename)
{
    int check;
    check = read_dentry_by_name(filename, &file_open_dentry);

    if (check == -1) // if read name failed
    {
        return -1;
    }

    return 0;
}

/*
 * file_close
 *   DESCRIPTION: close the file
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: none
 */
int32_t file_close(int32_t fd)
{
    return 0;
}

/*
 * file_read
 *   DESCRIPTION: read files into buf of length
 *   INPUTS: uint8_t *buf -- buf to write the file
 *           uint32_t length -- length of file to read
 *           uint32_t offset -- jump to the needed place
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *                 -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t file_read(int32_t fd, void *buf, int32_t length)
{
    uint32_t inode1;
    inode_t *inode2;
    int32_t check;
    pcb_t *current_pcb = get_pcb();
    if (length < 0 || current_pcb->descriptors[fd].f_flag == UNUSE) // if nothing to read
    {
        return -1;
    }
    inode2 = (inode_t *)((current_pcb->descriptors[fd]).f_inode);
    if (inode2 == NULL) return -1;
    inode1 = ((uint32_t)inode2 - (uint32_t)(inodes_start)) / BLOCKS_SIZE;

    check = read_data(inode1, current_pcb->descriptors[fd].f_file_position, (uint8_t *)buf, (uint32_t)length);


    if (check == -1) return -1;

    current_pcb->descriptors[fd].f_file_position += check;
    //puts((int8_t *)buf);

    return check;
}

/*
 * file_write
 *   DESCRIPTION: not implemented this checkpoint
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t file_write(int32_t fd, const void *buf, int32_t length)
{
    // do nothing and return -1
    return -1;
}

/*
 * dir_open
 *   DESCRIPTION: open the corresponding dir
 *   INPUTS: uint8_t *fname -- name of the dir to be opem
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *                 -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t dir_open(const uint8_t *filename)
{
    int check;
    check = read_dentry_by_name(filename, &file_open_dentry);

    if (check == -1) // if read dentry failed
    {
        return -1;
    }

    return 0;
}

/*
 * dir_close
 *   DESCRIPTION: close the dir
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: none
 */
int32_t dir_close(int32_t fd)
{
    // do nothing and return 0
    return 0;
}

/*
 * dir_read
 *   DESCRIPTION: read the files in the directory
 *   INPUTS: int32_t fd -- passed in parameter
 *           uint8_t *buf -- buf to write names of files
 *           int32_t length -- length of files to read
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *                 -1 for failure
 *   SIDE EFFECTS: print all the names on the screen
 */
// int32_t dir_read(int32_t fd, void *buf, int32_t length)
// {
//     uint8_t index;
//     uint8_t dentry[DENTRY_ALL];
//     uint32_t size;
//     char buf1[] = "\n";
//     char buf2[] = " ";
//     char buf3[] = "\0";
//     int8_t temp_buf[10];
//     uint8_t *ori_buf = (uint8_t *)buf;
//     if (length < 0 || buf == NULL)  // if nothing to read
//     {
//         return -1;
//     }

//     for (index = 0; index < boot_block->num_dir_entries; index++)
//     {
//         length--;

//         if (length < 0) // if nothing to read
//         {
//             break;
//         }

//         read_dentry_by_index(index, (dentry_t *)dentry);

//         size = ((inode_t *)((boot_block->dentries[index].inodes) * BLOCKS_SIZE + (uint32_t)inodes_start))->length;

//         // copy file type to buffer
//         itoa((boot_block->dentries[index].file_type), temp_buf, 10);
//         strncpy((int8_t *)((uint8_t *)ori_buf),  temp_buf, strlen(temp_buf));
//         ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen(temp_buf));
//         // make a blank space
//         strncpy((int8_t *)((uint8_t *)ori_buf), buf2, strlen((int8_t*)buf2));
//         ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen((int8_t*)buf2));
//         // copy file length to buffer
//         itoa(size, temp_buf, 10);
//         strncpy((int8_t *)((uint8_t *)ori_buf),  temp_buf, strlen(temp_buf));
//         ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen(temp_buf));
//         // make a blank space
//         strncpy((int8_t *)((uint8_t *)ori_buf), buf2, strlen((int8_t*)buf2));
//         ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen((int8_t*)buf2));
//         // copy file name to buffer
//         if (strlen((int8_t *)(boot_block->dentries[index].file_name)) < FILE_NAME_LEN){
//             strncpy((int8_t *)((uint8_t *)ori_buf), (int8_t *)(boot_block->dentries[index].file_name), strlen((int8_t *)(boot_block->dentries[index].file_name)));
//             ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen((int8_t *)(boot_block->dentries[index].file_name)));
//         }
//         else{
//             strncpy((int8_t *)((uint8_t *)ori_buf), (int8_t *)(boot_block->dentries[index].file_name),FILE_NAME_LEN);
//             ori_buf = (uint8_t*)((uint32_t)ori_buf + FILE_NAME_LEN);
//         }
//         // make a new line
//         strncpy((int8_t *)((uint8_t *)ori_buf), buf1, strlen((int8_t*)buf1));
//         ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen((int8_t*)buf1));
//     }
//     strncpy((int8_t *)((uint8_t *)ori_buf), buf3, strlen((int8_t*)buf3));
//     ori_buf = (uint8_t*)((uint32_t)ori_buf + strlen((int8_t*)buf3));
//     //puts((int8_t *)buf);
//     return 0;
// }

int32_t dir_read(int32_t fd, void *buf, int32_t length)
{
    dentry_t dentry;
    pcb_t *cur_pcb = get_pcb();
    int32_t s_len = 0;

    if (length < 0 || buf == NULL || cur_pcb->descriptors[fd].f_flag == UNUSE) // if nothing to read
    {
        return -1;
    }
    // check if all files are read
    if (cur_pcb->descriptors[fd].f_file_position == boot_block->num_dir_entries)
    {
        cur_pcb->descriptors[fd].f_file_position = 0;
        return 0;
    }

    // get the current denrty
    read_dentry_by_index(cur_pcb->descriptors[fd].f_file_position, &dentry);

    //copy the file name to the buffer
    if (strlen((int8_t *)dentry.file_name) >= FILE_NAME_LEN)
    {
        strncpy((int8_t *)buf, (int8_t *)dentry.file_name, FILE_NAME_LEN);
    }
    else
    {
        strcpy((int8_t *)buf, (int8_t *)dentry.file_name);
    }

    // increament the counter
    cur_pcb->descriptors[fd].f_file_position += 1;

    // return the length of the file
    s_len = (int32_t)strlen((int8_t *)dentry.file_name);
    if (s_len >= FILE_NAME_LEN)
    {
        s_len = FILE_NAME_LEN;
    }
    return s_len;
}

uint32_t get_length(uint32_t inodes)
{
    inode_t *temp;
    temp = (inode_t *)((uint32_t)fs_start + (inodes + 1) * BLOCKS_SIZE);
    return temp->length;
}
/*
 * dir_write
 *   DESCRIPTION: no inplemented this checkpoint
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t dir_write(int32_t fd, const void *buf, int32_t length)
{
    // do nothing and return -1
    return -1;
}
