#include "tests.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__);
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

// static inline void assertion_failure(){
// 	/* Use exception #15 for assertions, otherwise
// 	   reserved by Intel */
// 	asm volatile("int $15");
// }

// /* Checkpoint 1 tests */

// /* IDT Test - Example
//  *
//  * Asserts that first 10 IDT entries are not NULL
//  * Inputs: None
//  * Outputs: PASS/FAIL
//  * Side Effects: None
//  * Coverage: Load IDT, IDT definition
//  * Files: x86_desc.h/S
//  */
// int idt_test(){
// 	TEST_HEADER;

// 	int i;
// 	int result = PASS;
// 	for (i = 0; i < 10; ++i){
// 		if ((idt[i].offset_15_00 == NULL) &&
// 			(idt[i].offset_31_16 == NULL)){
// 			assertion_failure();
// 			result = FAIL;
// 		}
// 	}
// 	return result;
// }

// /* exception_hanlder
//  *
//  * Asserts certain exception will raise when it occurs
//  * Inputs: None
//  * Outputs: PASS or print the error by the exception handler
//  * Side Effects: the exception handler should be raised
//  * Coverage: initialize idt and exception handler
//  * Files: idt.c/h
//  */
// static inline void exception_hanlder(){
// 	/* Use exception #1 for assertions, otherwise
// 	   reserved by Intel */
// 	asm volatile("int $1");
// }

// /* idt_test_0
//  *
//  * Asserts divison by zero handler will raise when it occurs
//  * Inputs: None
//  * Outputs: PASS or print the error by exception
//  * Side Effects: the division by zero exception shoud be raised
//  * Coverage: initialize idt and interrupt handler
//  * Files: idt.c/h
//  */
// int idt_test_0(){
// 	TEST_HEADER;
// 	int result = PASS;
// 	// raise error divison by zero
// 	// int i = 10 / 0;
// 	return result;
// }
// /* Test suite entry point */

// /* page_video_mem_test_1
//  *
//  * Asserts video memory is paged correctly
//  * Inputs: None
//  * Outputs: PASS or print the page fault error on the screen
//  * Side Effects: should pass the test
//  * Coverage: initailize paging
//  * Files: paging.c/h
//  */
// int page_video_mem_test_1(){
//  TEST_HEADER;
// 	// try to access all video memories, which start at 0xB8000,
// 	// which has size 4KB, 1024 entries
// 	unsigned int * video_memory = (unsigned int *)0x0B8000;
// 	int i;
// 	int tmp;
// 	for(i = 0; i < 1024; i++){	// whatever it is, so magic
// 		tmp = *(video_memory + i);
// 	}
//  return PASS;
// }

// /* page_video_mem_test_2
//  *
//  * Asserts pagefault handler will raise when it occurs
//  * Inputs: None
//  * Outputs: PASS or print the page falt error
//  * Side Effects: should raise the page fault exception handler
//  * Coverage: initailize paging
//  * Files: paging.c/h
//  */
// int page_video_mem_test_2(){
//  TEST_HEADER;
// 	// try to access memory that should be NOT Present at 0xB5000
// 	int tmp;
// 	unsigned int * invalid = (unsigned int *)0x0B5000;	// whatever it is, so magic
// 	tmp = *invalid;
//  return FAIL;
// }

// /* page_video_mem_test_3
//  *
//  * Asserts the pagefault handler will raise when it occurs
//  * Inputs: None
//  * Outputs: PASS or print the error by the exception
//  * Side Effects: should raise the page fault exception handler
//  * Coverage: initailize paging
//  * Files: paging.c/h
//  */
// int page_video_mem_test_3(){
//  TEST_HEADER;
// 	// try to access memory that should be NOT Present at 0xB9000
// 	int tmp;
// 	unsigned int * invalid = (unsigned int *)0x0B9000;	// whatever it is, so magic
// 	tmp = *invalid;
//  return FAIL;
// }

// /* page_dereference_null
//  *
//  * Asserts the page fualt handler will raise when dereference
//  * NULL pinter occurs
//  *
//  * Inputs: None
//  * Outputs: PASS or print the page fault error on the screen
//  * Side Effects: should raise the page fault exception handler
//  * Coverage: initailize paging
//  * Files: paging.c/h
//  */
// int page_dereference_null(){
// 	TEST_HEADER;
// 	// try to dereference NULL pointer
// 	int tmp;
// 	unsigned int * null_ptr = NULL;
// 	tmp = *null_ptr;
//  return FAIL;
// }

// /* page_kernel_test
//  *
//  * Asserts page fault handler will raise when it occurs
//  * 
//  * Inputs: None
//  * Outputs: PASS or print the page fault error on the screen
//  * Side Effects: should PASS the test
//  * Coverage: initailize paging
//  * Files: paging.c/h
//  */
// int page_kernel_test(){
//  	TEST_HEADER;
// 	// try to access kernel memory at 0x400000
// 	int tmp;
// 	unsigned int *  kernel = (unsigned int *)0x400000;	// whatever it is, so magic
// 	tmp = *kernel;
//  return PASS;
// }



// /* Checkpoint 2 tests */
// /* test_read_directory
//  *
//  * Asserts the directory read can print all of the files' names
//  *
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: should print all the files' name on the screen
//  * Coverage: dir_read, read_dentry_by_index
//  * Files: file_system.c/h
//  */
// int test_read_directory(){
// 	// intialize a useless variable, a large buffer and its length.
// 	int32_t fd = 0;
// 	uint8_t buf[4096]; // whatever it is, so magic
// 	int32_t length = 4096;	// whatever it is, so magic
// 	int32_t check;

// 	// check whether the dir is read successfully
// 	check = dir_read(fd, buf, length);
// 	if (check == READ_FAIL){
// 		return FAIL;
// 	}
// 	return PASS;
// }

// /* test_file_read_general
//  *
//  * Asserts read file can print the content of the file
//  *
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print the contents of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_general(){
// 	// inilialize a big buffer and it length offset reads from begining
// 	uint8_t filename[] = "frame1.txt";
// 	uint8_t buf[10000];	// whatever it is, so magic
// 	uint32_t length = 10000;	// whatever it is, so magic
// 	int32_t check;
// 	pcb_t cur;
// 	int32_t fd = 1;
// 	dentry_t dentry;
// 	read_dentry_by_name(filename, &dentry);
	
// 	cur.descriptor[fd].inode = (inode_t*)((uint32_t)inodes_start + dentry.inodes * BLOCK_SIZE);
// 	cur.descriptor[fd].file_position = 0;
	
// 	current_pcd = &cur;

// 	check = file_open((uint8_t*)filename);
// 	if (check == READ_FAIL){
// 		return FAIL;
// 	}
// 	file_read(fd, (uint8_t*)buf, length);

// 	return PASS;
// }

// /* test_file_read_general_short_length
//  *
//  * Asserts read file can read only part of the file
//  *
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print part of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_general_short_length(){
// 	// inilialize a big buffer and short length offset reads from begining
// 	uint8_t filename[] = "frame1.txt";
// 	uint8_t buf[4096];	// whatever it is, so magic
// 	uint32_t length = 200;	// whatever it is, so magic
// 	int32_t check;
// 	uint32_t offset = 0;	// whatever it is, so magic

// 	check = file_open((uint8_t*)filename);
// 	if (check == READ_FAIL){
// 		return FAIL;
// 	}
// 	file_read(buf, length, offset);

// 	return PASS;
// }

// /* test_file_read_general_offset
//  *
//  * Asserts read file can read only part of the file
//  * that not starting from begining
//  * 
//  *
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print part of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_general_offset(){
// 	// initialize a large buffer and some short length and offset
// 	uint8_t filename[] = "frame0.txt";
// 	uint8_t buf[4096];	// whatever it is, so magic
// 	uint32_t length = 7;	// whatever it is, so magic
// 	int32_t check;
// 	uint32_t offset = 7;	// whatever it is, so magic

// 	check = file_open((uint8_t*)filename);
// 	if (check == READ_FAIL){
// 		return FAIL;
// 	}
// 	file_read(buf, length, offset);

// 	return PASS;
// }

// /* test_file_read_large
//  *
//  * Asserts read file can read large file
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print the content of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_large(){
// 	// inilialize a big buffer and it length offset reads from begining
// 	uint8_t filename[] = "verylargetextwithverylongname.tx";
// 	uint8_t buf[50000];	// whatever it is, so magic
// 	uint32_t length = 5300;	// whatever it is, so magic
// 	int32_t check;
// 	uint32_t offset = 0;	// whatever it is, so magic

// 	check = file_open((uint8_t*)filename);
// 	if (check == READ_FAIL){
// 		return FAIL;
// 	}
// 	file_read(buf, length, offset);

// 	return PASS;
// }

// /* test_file_read_none_file_type
//  *
//  * Asserts read file can read none text file
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print the content of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_none_file_type(){
// 	// inilialize a big buffer and it length offset reads from begining
// 	uint8_t filename[] = "pingpong";
// 	uint8_t buf[50000];	// whatever it is, so magic
// 	uint32_t length = 50000;	// whatever it is, so magic
// 	int32_t check;
// 	uint32_t offset = 0;	// whatever it is, so magic

// 	check = file_open((uint8_t*)filename);
// 	if (check == READ_FAIL){
// 		return FAIL;
// 	}

// 	file_read(buf, length, offset);

// 	return PASS;
// }

// /* test_file_read_none_exit_file
//  *
//  * Asserts read file can handle none exist file
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print the content of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_none_exit_file(){
// 	uint8_t filename[] = "asidfgaysdg";
// 	int check;

// 	check = file_open((uint8_t*)filename);
// 	printf("What are you babbling?\n");
// 	if (check == READ_FAIL){
// 		return PASS;
// 	}

// 	return FAIL;
// }

// /* test_file_write
//  *
//  * Asserts file write do nothing
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: none
//  * Coverage: file_write
//  * Files: file_system.c/h
//  */
// int test_file_write(){
// 	int check;

// 	check = file_write();
// 	if (check == READ_FAIL){
// 		return PASS;
// 	}

// 	return FAIL;
// }

// /* test_dir_write
//  *
//  * Asserts dir write do nothing
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: none
//  * Coverage: dir_write
//  * Files: file_system.c/h
//  */
// int test_dir_write(){
// 	int check;

// 	check = file_write();
// 	if (check == READ_FAIL){
// 		return PASS;
// 	}

// 	return FAIL;
// }

// /* test_file_read_print_all_file
//  *
//  * Asserts read file can all files
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: print the content of the file on the screen
//  * Coverage: file_open, file_read, read_dentry_by_name
//  * Files: file_system.c/h
//  */
// int test_file_read_print_all_file(){
// 	//initialize a big buffer and its length
// 	int i;
// 	uint8_t buf[50000];	// whatever it is, so magic
// 	uint32_t length = 50000;	// whatever it is, so magic
// 	int32_t check;
// 	uint32_t offset = 0;	// whatever it is, so magic
// 	uint8_t* filename[14];	// whatever it is, so magic
// 	filename[0] =(uint8_t*)"cat";	// whatever it is, so magic
// 	filename[1] =(uint8_t*)"counter";	// whatever it is, so magic
// 	filename[2] =(uint8_t*)"frame0.txt";	// whatever it is, so magic
// 	filename[3] =(uint8_t*)"frame1.txt";	// whatever it is, so magic
// 	filename[4] =(uint8_t*)"grep";	// whatever it is, so magic
// 	filename[5] =(uint8_t*)"hello";	// whatever it is, so magic
// 	filename[6] =(uint8_t*)"ls";	// whatever it is, so magic
// 	filename[7] =(uint8_t*)"pingpong";	// whatever it is, so magic
// 	filename[8] =(uint8_t*)"shell";	// whatever it is, so magic
// 	filename[9] =(uint8_t*)"sigtest";	// whatever it is, so magic
// 	filename[10] =(uint8_t*)"syserr";	// whatever it is, so magic
// 	filename[11] =(uint8_t*)"testprint";	// whatever it is, so magic
// 	filename[12] =(uint8_t*)"verylargetextwithverylongname.txt";	// whatever it is, so magic
// 	// loop over each file and print it on the screen	
// 	for (i=0; i<13; i++){	// whatever it is, so magic
// 		check = file_open((uint8_t*)filename[i]);
// 		if (check == READ_FAIL && i != 12){	// whatever it is, so magic
// 			return FAIL;
// 		}

// 	file_read(buf, length, offset);
// 	}
// 	return PASS;
	
// }
// /* test_rtc_rate_change
//  *
//  * Asserts rtc works 
//  * 
//  * Inputs: None
//  * Outputs: PASS or FAIL
//  * Side Effects: numbers
//  * Coverage: rtc_handler
//  * Files: rtc_handler.c/h
//  */
// int test_rtc_rate_change(){
// 	int32_t rate[10]; // test all possible rates
// 	int i,j,ret;
// 	clear();
// 	for(i=0; i < 10 ; i++){	// whatever it is, so magic
// 		rate[i] = 2 << i;	// whatever it is, so magic
// 		ret = rtc_write(0,rate+i,4);	// whatever it is, so magic
// 		printf("current rate = %d\n",rate[i]);
// 		for(j=1;j<10*(i+1);j++){
// 			// print stuff when rtc interrupt happen
// 			ret = rtc_read(0,0,0);	// whatever it is, so magic
// 			putc('1');	// whatever it is, so magic
// 		}
// 		clear();
// 	}
// 	return PASS;
// }

// /* 
//  * read_write_test2
//  * description: 
//  * call terminal_read and terminal_write to test functionality
//  * input: none
//  * output: none 
//  * side effect: should both return -1 for SUCCESS
//  */
// int read_write_test2(){
// 	// test for garbage inputs
//     if(terminal_read(0, NULL, 200) != -1)return FAIL;	// whatever it is, so magic
//     if(terminal_write(0, NULL, 200) != -1)return FAIL;	// whatever it is, so magic
//  return PASS;
// }

// /* Checkpoint 3 tests */
/* 
 * test_execute_testprint
 * description: 
 * raise system call testprint
 * input: none
 * output: none 
 * side effect: start executing testprint and print a sentence on the screen
 */
int test_execute_testprint(){
	uint8_t arr[] = "testprint";
	execute(arr);
	return PASS;
}

/* 
 * test_execute_testprint
 * description: 
 * raise system call shell
 * input: none
 * output: none 
 * side effect: start executing shell
 */
int test_execute_shell(){
	uint8_t arr[] = "shell";
	execute(arr);
	return PASS;
}
/* 
 * test_execute_open
 * description: 
 * raise system call open
 * input: none
 * output: none 
 * side effect: should pass if open return 2
 */
int test_execute_open(){
	int i;
	uint8_t fn[] = "testprint";
	i = open(fn);
	// 2 should be teh first program
	if (i == 2){
		return PASS;
	}
	return FAIL;
}

/* 
 * test_execute_open_fail
 * description: 
 * raise system call open
 * input: none
 * output: none 
 * side effect: should pass if open return -1
 */
int test_execute_open_fail(){
	int i;
	uint8_t fn[] = "qwertyuiop";
	i = open(fn);
	// -1 for wrong input for close
	if (i == -1){
		return PASS;
	}
	return FAIL;
}

/* 
 * test_execute_close
 * description: 
 * raise system call close
 * input: none
 * output: none 
 * side effect: should pass if open return 2
 */
int test_execute_close(){
	int i;
	i = close(2);
	// 2 should be teh first program
	if (i == 2){
		return PASS;
	}
	return FAIL;
}

/* 
 * test_execute_close_fail
 * description: 
 * raise system call close
 * input: none
 * output: none 
 * side effect: should pass if open return -1
 */
int test_execute_close_fail(){
	int i;
	i = close(8);
	// -1 for wrong input for close
	if (i == -1){
		return PASS;
	}
	return FAIL;
}
// /* Checkpoint 4 tests */
/* 
 * test_getargs_fail
 * description: 
 * test garbage inputs for getargs
 * input: none
 * output: none 
 * side effect: should pass if getarges return -1
 */
int test_getargs_fail(){

	int32_t i, j;
	int8_t* buf;
	int8_t buf1[1];

	buf = NULL;
	buf1[0] = '\0';

	i = getargs(buf, 1);
	j = getargs(buf1, -1);

	// -1 for wrong input for getargs
	if (i == -1 && j == -1){
		return PASS;
	}
	return FAIL;
}

/* 
 * test_vidmap_fail
 * description: 
 * test garbage inputs for vidmap
 * input: none
 * output: none 
 * side effect: should pass if getarges return -1
 */
int test_vidmap_fail(){

	int32_t i, j, k, l;
	uint8_t* buf;
	uint8_t** buf1;


	buf = NULL;
	buf1 = &buf;
	i = vidmap(buf1);
	buf1 = NULL;
	j = vidmap(buf1);
	buf1 = (uint8_t**)(0x07000000);		// put invalide address
	k = vidmap(buf1);
	buf1 = (uint8_t**)(0x08500000); 	// put invalide address
	l = vidmap(buf1);
	// -1 for wrong input, 0 for success
	if (i == -1 && j == -1 && k == -1 && l == -1){
		return PASS;
	}
	return FAIL;
}

// /* Checkpoint 5 tests */	

// // launch the test
void launch_tests(){
	// printf("launching test\n");
	// check point 1
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("page_video_mem_test_1", page_video_mem_test_1());
	//TEST_OUTPUT("page_video_mem_test_2", page_video_mem_test_2());
	//TEST_OUTPUT("page_video_mem_test_3", page_video_mem_test_3());
	//TEST_OUTPUT("page_dereference_null", page_dereference_null());
	//TEST_OUTPUT("page_kernel_test", page_kernel_test());
	//TEST_OUTPUT("idt_test_0", idt_test_0());
	//TEST_OUTPUT("paging", test_interrupts());
	
	// check point 2
	//TEST_OUTPUT("test rtc rate change", test_rtc_rate_change());
	//TEST_OUTPUT("dirtory read", test_read_directory());
	//TEST_OUTPUT("file read general", test_file_read_general());
	//TEST_OUTPUT("read file with short length", test_file_read_general_short_length());
	//TEST_OUTPUT("read file with offset", test_file_read_general_offset());
	//TEST_OUTPUT("read file large", test_file_read_large());
	//TEST_OUTPUT("read file that does not exit", test_file_read_none_exit_file());
	//TEST_OUTPUT("read file that is not file type", test_file_read_none_file_type());
	//TEST_OUTPUT("read all files", test_file_read_print_all_file());
	//TEST_OUTPUT("write to files", test_file_write());
	//TEST_OUTPUT("write to dir", test_dir_write());
	//TEST_OUTPUT("test read and wirte",read_write_test2());

	// check point 3
	//TEST_OUTPUT("test system call testprint",test_execute_testprint());
	//TEST_OUTPUT("test system call shell",test_execute_shell());
	//TEST_OUTPUT("test system call open",test_execute_open());
	//TEST_OUTPUT("test system call open fail",test_execute_open_fail());
	//TEST_OUTPUT("test system call close",test_execute_close());
	//TEST_OUTPUT("test system call close fail",test_execute_close_fail());

	// check point 4
	//TEST_OUTPUT("test system call getarges",test_getargs_fail());
	//TEST_OUTPUT("test system call vidmap",test_vidmap_fail());
 }
