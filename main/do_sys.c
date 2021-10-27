#include "do_sys.h"

/* function pointers for file, directory and rtc and the elf format */
uint8_t elf[] = {0x7f, 0x45, 0x4c, 0x46};
func_ptr file_funcs[FUNCTION_PTR_SIZE] = {file_read, file_write, file_open, file_close};
func_ptr dir_funcs[FUNCTION_PTR_SIZE] = {dir_read, dir_write, dir_open, dir_close};
func_ptr rtc_funcs[FUNCTION_PTR_SIZE] = {rtc_read, rtc_write, rtc_open, rtc_close};

//An array that stores the process status
int32_t process[MAX_PROCESS_NUM] = {PROCESS_OFF, PROCESS_OFF, PROCESS_OFF, PROCESS_OFF, PROCESS_OFF, PROCESS_OFF};

static int8_t args[NAME_BUFFER];

/*
 * get_next_pid
 *   DESCRIPTION: get the next pid
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1 for fail, or the next pid
 *   SIDE EFFECTS: none
 */
extern int32_t get_next_pid(){
  int i;
  // loop over the array and return the next available
  for(i = 0; i < MAX_PROCESS_NUM; i++){
    if(process[i] == PROCESS_OFF){
      process[i] = PROCESS_ON;
      return i;
    }
  }
  return -1;
}

/*
 * halt
 *   DESCRIPTION: halt the current process
 *   INPUTS: uint8_t status -- pass in parameters
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: none
 */
extern int32_t halt(uint8_t status)
{
  // todo : chekpoint 3

  // pop kernel stack of process
  // pop page for process
  // close files used by process
  // return control
  int32_t i, esp, ebp;
  pcb_t *current_pcb = get_pcb();
  for (i = 0; i < MAX_FILE; i++)
  {
    if (current_pcb->descriptors[i].f_flag == INUSE)
    {
      current_pcb->descriptors[i].file_operations_table_ptr[3](0); // 3 CALL CLOSE, 0 IS PASS IN ANYTHING
    }
  }

  // reset the process flag
  process[current_pcb->pid] = PROCESS_OFF;

  // if try to halt the first three, relaunch
  uint8_t jb[] = "shell";
  if (current_pcb -> pid == PROCESS_ZERO || current_pcb -> pid == PROCESS_ONE || current_pcb -> pid == PROCESS_TWO) {
    execute(jb);
  }
  
  // reset the current terminal vid_mem flag
  if (terminals[current_running_terminal].user_vid_mem == HIGH){
    terminals[current_running_terminal].user_vid_mem = LOW;
    delete_user_video_page();
  }

  // reset the paing back to the parent process
  reset_paging(_8MB + _4MB * (current_pcb->parent_pid));

  // reset tss to parent process
  tss.esp0 = current_pcb->parent_esp0;
  tss.ss0 = KERNEL_DS;

  // update current pcb, child space discarded
  esp = current_pcb->parent_esp;
  ebp = current_pcb->parent_ebp;
  current_pcb = (pcb_t *)(PCB_BASE + (MAX_PCB - current_pcb->parent_pid) * _8KB);
  int32_t sb = (int32_t)status;
  sb &= SB_MASK;
  asm volatile(
      "movl %0, %%esp \n \
       movl %1, %%ebp \n \
       movl %2, %%eax \n \
       jmp return_from_child"
        :
        : "r"(esp), "r"(ebp), "r"(sb)
        : "eax", "esp", "ebp");
  status = (uint8_t)sb;

  return 0;
}

/*
 * execute
 *   DESCRIPTION: Error function to return -1
 *   INPUTS: const uint8_t *command -- command buffer passed in
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success, -1 for failure
 *   SIDE EFFECTS: none
 */
extern int32_t execute(const uint8_t *command)
{
  int32_t flags;
  cli_and_save(flags);
  int32_t next_pid;

  // sanity check
  if((next_pid = get_next_pid()) == -1) return -1;

  int i, j, k;
  int32_t test;
  uint32_t entry;
  dentry_t dentry;
  uint8_t realname[NAME_BUFFER];
  uint32_t physical_addr;
  uint8_t sanity_buffer[FUNCTION_PTR_SIZE];
  uint32_t *buffer;
  uint32_t *user_stack;

  // clear the buffer
  for (i = 0; i < NAME_BUFFER; i++)
  {
    realname[i] = SET_ZERO;
    args[i] = SET_ZERO;
  }

  //parse the arguments
  for (i = 0; command[i] == ' '; i++);
  for (j = i; command[j] != ' ' && command[j] != '\0' && command[j] != '\n'; j++);
  for (k = i; k < j; k++) realname[k - i] = command[k];
  realname[k - i] = '\0';

  //realname now holds the correct command string, i points to start of the string and j points the end of string
  for (i = j; command[i] == ' '; i++);
  for (j = i; command[j] != ' ' && command[j] != '\0' && command[j] != '\n'; j++);

  //args now holds the correct argument
  for (k = i; k < j; k++) args[k - i] = command[k];
  args[k - i] = '\0';

  //check file validity
  if (read_dentry_by_name(realname, &dentry) == -1) {
    process[next_pid] = LOW;
    return -1; // if read fails
  }

  // read the elf from the given inode
  read_data(dentry.inodes, SET_ZERO, sanity_buffer, sizeof(sanity_buffer));

  // check if executable
  if (strncmp((int8_t *)sanity_buffer, (int8_t *)elf, sizeof(sanity_buffer) != SET_ZERO)){
    process[next_pid] = LOW;
    return -1; // if string compare fails
  }

  // read data from the file system
  read_data(dentry.inodes, READ_DATA_OFFSET, sanity_buffer, sizeof(sanity_buffer));

  //assign value for the entry point
  entry = *((uint32_t *)sanity_buffer);

  //setup paging
  physical_addr = _8MB + _4MB * next_pid;
  reset_paging(physical_addr);

  //load the program to the page;
  buffer = (uint32_t *)(_128MB + PROGRAM_OFFSET);
  user_stack = (uint32_t *)(_128MB + _4MB);
  test = read_data(dentry.inodes, SET_ZERO, (uint8_t *)buffer, _4MB);

  //configuring pcb
  pcb_t *pcb = (pcb_t *)(PCB_BASE + (MAX_PCB - next_pid) * _8KB);
  pcb_init(pcb, next_pid);

  // set the tss for the process
  pcb->parent_esp0 = tss.esp0;
  tss.esp0 = (uint32_t)(PCB_BASE + (MAX_PCB - next_pid) * _8KB + _8KB - PCB_OFFSET);
  tss.ss0 = KERNEL_DS;

  //context switch
  // PUSH ORDER:
  // USER_DS
  // ESP
  // EFLAG
  // CS
  // EIP
  // 0x0202 = Eflags
  // EIP = return address iret to = user code start

  /* 0X0202 is IF flag with IF on */
  /* others are just pushing parameters */
  asm volatile(
      "movl %%esp, %0 \n \
      movl %%ebp, %1 \n\
      pushl %5 \n\
      pushl %4 \n\
      pushl $0x0202\n\
      pushl %3 \n\
      pushl %2 \n\
      iret  \n\
      return_from_child: \n\
      leave \n\
      ret"
      : "=m"(pcb->parent_esp), "=m"(pcb->parent_ebp)
      : "r"(entry), "r"(USER_CS), "r"(user_stack), "r"(USER_DS)
      : "memory", "esp", "ebp");

  restore_flags(flags);

  return 0;
}

/*
 * terminate_by_exception
 *   DESCRIPTION: terminate the programs
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: squash the programs if exception happens
 */
extern int32_t terminate_by_exception()
{
  uint32_t i, esp, ebp;

  // reset paing
  pcb_t *current_pcb = get_pcb();
  reset_paging(_4MB + _4MB * (current_pcb->parent_pid));

  // set the process flag
  process[current_pcb->pid] = LOW;

  //close files in the pcb
  for (i = 0; i < MAX_FILE; i++)
  {
    if (current_pcb->descriptors[i].f_flag == INUSE)
    {
      // 3 call close and 0 pass in anything
      current_pcb->descriptors[i].file_operations_table_ptr[3](0); 
    }
  }

  // reset tss to parent 
  tss.esp0 = current_pcb->parent_esp0;

  // update current pcb, child space discarded
  current_pcb = (pcb_t *)(PCB_BASE + (MAX_PCB - current_pcb->parent_pid) * _8KB);

  /*error code not get, put -1 in eax*/
  asm volatile(
        "movl %0, %%esp \n\
         movl %1, %%ebp \n\
         movl $-1, %%eax \n\
         jmp return_from_child"
          :
          : "r"(esp), "r"(ebp)
          : "eax", "esp", "ebp");
  return 0;
}

/*
 * open
 *   DESCRIPTION: open the corresponding file
 *   INPUTS: const uint8_t *filename -- filename of file to be open
 *   OUTPUTS: none
 *   RETURN VALUE: the position of the file in fd
 *   SIDE EFFECTS: none
 */
extern int32_t open(const uint8_t *filename)
{
  // todo : chekpoint 3
  int i = SET_ZERO;
  fd_t *fd;
  dentry_t dentry;
  struct inode_t *inode;
  pcb_t *current_pcb = get_pcb();

  // check if there is a free descriptor
  for (i = SKIP_INOUT; i < MAX_FILE; i++)
  {
    if ((current_pcb->descriptors[i]).f_flag == UNUSE)  // not in use
    {
      break;
    }
  }

  if (i == MAX_FILE) return -1;
  // check if the file name is valid

  if (read_dentry_by_name(filename, &dentry) == -1) return -1; // if read name fails

  // allocate a new fd
  fd = &(current_pcb->descriptors[i]);

  inode = (struct inode_t *)((uint32_t)fs_start + (dentry.inodes + 1) * BLOCKS_SIZE); // SKIP BOOTBLOCK

  if (dentry.file_type == TYPE_DIR)
  {
    fd->file_operations_table_ptr = dir_funcs;
    fd->f_inode = inode;
    fd->f_file_position = SET_ZERO;
    fd->f_flag = INUSE;
  }
  if (dentry.file_type == TYPE_FILE)
  {
    fd->file_operations_table_ptr = file_funcs;
    fd->f_inode = inode;
    fd->f_file_position = SET_ZERO;
    fd->f_flag = INUSE;
  }
  if (dentry.file_type == TYPE_RTC)
  {
    fd->file_operations_table_ptr = rtc_funcs;
    fd->f_inode = inode;
    fd->f_file_position = SET_ZERO;
    fd->f_flag = INUSE;
  }
  return i;
}

/*
 * close
 *   DESCRIPTION: close the corresponding file
 *   INPUTS: int32_t fd -- file to be closed
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: none
 */
extern int32_t close(int32_t fd)
{
  pcb_t *current_pcb = get_pcb();
  if (fd < 0 || fd > MAX_PCB || fd == 1 || fd == 0 || current_pcb->descriptors[fd].f_flag == UNUSE)
    return -1;
  current_pcb->descriptors[fd].f_flag = UNUSE;
  current_pcb->descriptors[fd].f_file_position = -1; // set to unuse
  current_pcb->descriptors[fd].f_inode = NULL;
  current_pcb->descriptors[fd].file_operations_table_ptr = NULL;
  return 0;
}

/*
 * read
 *   DESCRIPTION: call the read function
 *   INPUTS: int32_t fd -- file to be read
 *           void *buf -- buffer to be written
 *           int32_t nbytes -- how many bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: what the read function return
 *   SIDE EFFECTS: none
 */
extern int32_t read(int32_t fd, void *buf, int32_t nbytes)
{
  // todo : chekpoint 3
  pcb_t *current_pcb = get_pcb();
  if (fd < 0 || fd > MAX_PCB || buf == NULL || nbytes < 0) return -1;
  if (current_pcb->descriptors[fd].f_flag == UNUSE) return -1;

  return current_pcb->descriptors[fd].file_operations_table_ptr[0](fd, buf, nbytes);
}

/*
 * write
 *   DESCRIPTION: write the corresponding file
 *   INPUTS: int32_t fd -- file to be written
 *           const void *buf -- buffer to be written
 *           int32_t nbytes -- how many bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: none
 */
extern int32_t write(int32_t fd, const void *buf, int32_t nbytes)
{
  // todo : chekpoint 3
  pcb_t *current_pcb = get_pcb();
  if (fd < 0 || fd > MAX_PCB || buf == NULL || nbytes < 0 || current_pcb->descriptors[fd].f_flag == UNUSE) return -1;

  return current_pcb->descriptors[fd].file_operations_table_ptr[1](fd, buf, nbytes);
}

/*
 * getargs
 *   DESCRIPTION: get the arguments
 *   INPUTS: uint8_t *buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
extern int32_t getargs(int8_t *buf, int32_t nbytes)
{
  // sanity check
  if (nbytes <= 0) return -1;
  if (buf == NULL) return -1;
  if (args[0] == '\0')return -1;

  // get the arguments to the buffer
  strcpy(buf, args);
  return 0;
}

/*
 * vidmap
 *   DESCRIPTION: map the virtual video memory for the user
 *   INPUTS: uint8_t **screen_start
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
extern int32_t vidmap(uint8_t **screen_start)
{
  // senity check
  if (screen_start == NULL) return -1;

  terminals[get_current_running_terminal()].user_vid_mem = HIGH;
  // check the validity of screen start and map the video memory
  if(screen_start >= (uint8_t **)_128MB && screen_start < (uint8_t **)(_128MB + _4MB)){
      reset_video_page(VID_MEM_ADDR);
      *screen_start = (uint8_t *)USER_VID_MEM;
      return 0;
  }
  return -1;
}

/*
 * set_handler
 *   DESCRIPTION: extra for this mp
 *   INPUTS: int32_t signum, void *handler_address
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
extern int32_t set_handler(int32_t signum, void *handler_address)
{
  return -1;
}

/*
 * sig_return
 *   DESCRIPTION: extra for this mp
 *   INPUTS: void
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
extern int32_t sig_return(void)
{
  return -1;
}
