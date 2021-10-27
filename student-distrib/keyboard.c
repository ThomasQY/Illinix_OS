#include "keyboard.h"
#include "paging.h"
/* Reference from http://www.osdever.net/bkerndev/Docs/keyboard.htm */
// map the typed possible 128 keyboard keys to the human readable characters

static unsigned char left_shift_flag = 0;
static unsigned char right_shift_flag = 0;
static unsigned char caps_flag = 0;
static unsigned char ctl_flag = 0;
static unsigned char alt_flag = 0;
static int buffer_ptr = 0;

unsigned char scan_table[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

/* modified Scancode table from above to display shifted chars*/
unsigned char scan_table_shift[128] =
    {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*',    /* 9 */
        '(', ')', '_', '+', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'Q', 'W', 'E', 'R',                               /* 19 */
        'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
        '"', '~', 0,                                      /* Left shift */
        '|', 'Z', 'X', 'C', 'V', 'B', 'N',                /* 49 */
        'M', '<', '>', '?', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

/* 
 * keyboard_input
 * description: 
 * handle the keyboard input by getting the data from port
 * input: none
 * output: none
 * side effect: print the typed input to the screen, update the cursor
 */
void keyboard_input()
{
    // clear the input for the interrupt
    cli();

    // initialize variables
    int key_pressed;
    uint8_t key_in_buffer;
    uint32_t old_pte = get_old_pte();

    // set the pte mapping to video memory
    set_pte(VID_INDEX,VID_ADDR);

    // update the x, y coordinates
    set_x(terminals[get_current_looking_terminal()].x_pos);
    set_y(terminals[get_current_looking_terminal()].y_pos);

    // check where a key is pressed
    if ((key_pressed = inb(KEYBOARD_PORT)))
    {
        // get the input from the port
        key_in_buffer = (uint8_t)key_pressed;
        /* 
         * check whether the input is in the keyboard mapping table.
         * If so, read from the table which key is been pressed and 
         * print it to the screen
         */
        int check = KEYBOARD_MAP_TABLE - key_pressed;
        switch (key_pressed)
        {
        case RIGHT_SHIFT_PRESS:
            right_shift_flag = ONE;
            break;
        case LEFT_SHIFT_PRESS:
            left_shift_flag = ONE;
            break;
        case TAB:
            break;
        case RIGHT_SHIFT_RELEASE:
            right_shift_flag = ZERO;
            break;
        case LEFT_SHIFT_RELEASE:
            left_shift_flag = ZERO;
            break;
        case CTRL_PRESS:
            ctl_flag = ONE;
            break;
        case CTRL_RELEASE:
            ctl_flag = ZERO;
            break;
        case ALT_PRESS:
            alt_flag = ONE;
            break;
        case ALT_RELEASE:
            alt_flag = ZERO;
            break;
        case CAPS_PRESS:
            caps_flag = (caps_flag == ONE) ? ZERO : ONE;
            break;
        case CAPS_RELEASE:
            break;
        case BACK_SPACE:
            if (buffer_ptr > ZERO)
            {
                buffer_ptr--;
                keyboard_buffer[buffer_ptr] = '\0';
                putc('\b');
            }

            break;
        case ENTER:
            if (terminals[get_current_looking_terminal()].enter_flag == LOW){
                 key_in_buffer = scan_table[key_pressed];
                 if (buffer_ptr < KEY_BUFFER_SIZE - 1)
                     keyboard_buffer[buffer_ptr++] = key_in_buffer;
                putc('\n');
                terminals[get_current_looking_terminal()].enter_flag = HIGH;
            }
            //calls the test to echo the buffer on the screen
            //read_write_test();
            //memset(keyboard_buffer, '\0', KEY_BUFFER_SIZE);
            //buffer_ptr = ZERO;
            //if no test here, should clear the key_board_buffer here.
            break;
        case F1_PRESS:
            
            if (alt_flag)
            {
                switch_screen(TERM_ZERO);
                break;
            }
            break;
        case F2_PRESS:
            
            if (alt_flag)
            {
                switch_screen(TERM_ONE);
                break;
            }
            break;
        case F3_PRESS:
            
            if (alt_flag)
            {
                switch_screen(TERM_TWO);
                break;
            }
            break;
        case L_PRESS:
            if (ctl_flag)
            {
                clear();
                printf("391OS> ");
                puts(keyboard_buffer);
                break;
            }
        default:
            if (check <= ZERO)
                break;
            if (ctl_flag)
                break;
            if ((!left_shift_flag) && (!caps_flag) && (!right_shift_flag))
            {
                key_in_buffer = scan_table[key_pressed];
                //buffer ptr points to 127 means it is pointing to the end of the buffer
                if (key_in_buffer == ZERO)
                    break;
                if (buffer_ptr < KEY_BUFFER_SIZE - ONE)
                {
                    keyboard_buffer[buffer_ptr++] = key_in_buffer;
                    putc(key_in_buffer);
                }
            }
            else if ((left_shift_flag || right_shift_flag) && (caps_flag && is_letter(key_pressed)))
            {
                //at least one shift was pressed and capslock was pressed
                key_in_buffer = scan_table[key_pressed];
                if (key_in_buffer == ZERO)
                    break;
                if (buffer_ptr < KEY_BUFFER_SIZE - ONE)
                {
                    keyboard_buffer[buffer_ptr++] = key_in_buffer;
                    putc(key_in_buffer);
                }
            }
            else if ((!(left_shift_flag || right_shift_flag)) && caps_flag && !is_letter(key_pressed)) //only caps flag is pressed but not the shift
            {
                key_in_buffer = scan_table[key_pressed];
                if (key_in_buffer == ZERO)
                    break;
                if (buffer_ptr < KEY_BUFFER_SIZE - ONE)
                {
                    keyboard_buffer[buffer_ptr++] = key_in_buffer;
                    putc(key_in_buffer);
                }
            }
            else
            {
                key_in_buffer = scan_table_shift[key_pressed];
                if (key_in_buffer == ZERO)
                    break;
                if (buffer_ptr < KEY_BUFFER_SIZE - ONE)
                {
                    keyboard_buffer[buffer_ptr++] = key_in_buffer;
                    putc(key_in_buffer);
                }
            }
        }
    }
    // input status ready, read from keyboard
    // udpayey the x, y coordinates and the cursor position
    terminals[get_current_looking_terminal()].x_pos = get_x();
    terminals[get_current_looking_terminal()].y_pos = get_y();
    update_cursor(get_x(), get_y());

    // send EOI and map the pte back to original address
    send_eoi(IRQ_NUM_ONE);
    set_pte(VID_INDEX,old_pte);
}

/* 
 * terminal_write
 * description: 
 * write the buffer value to the screen.
 * input: 
 * int32_t fd
 * buf: The buffer to write from
 * nbytes: the number of bytes needs to write.
 * output: The number of bytes actually write. -1 if buffer is NULL
 * side effect: none
 */
int32_t terminal_write(int32_t fd, const void *buf, int32_t nbytes)
{
    cli();
    int i;
    int8_t *tmp;

    // sanity check
    if (!buf)
        return -1;

    // if the current looking is not the current running, write to the buffer
    if (get_current_running_terminal() != get_current_looking_terminal()) {
        // calculate the current running buffer addr and remap the video buffer
        uint32_t running_pos = VID_ADDR + (get_current_running_terminal() + ONE) * _4KB;
        set_pte(VID_INDEX, running_pos);
        // update the x,y coordinates
        set_x(terminals[get_current_running_terminal()].x_pos);
        set_y(terminals[get_current_running_terminal()].y_pos);
    }

    // write the contents to the video memory and update the x,y coordinates
    tmp = (int8_t *)buf;
    for (i = 0; i < nbytes; i++)
    {
        putc(tmp[i]);
    }
    terminals[get_current_running_terminal()].x_pos = get_x();
    terminals[get_current_running_terminal()].y_pos = get_y();

    // if the current looking is the same as the current running, only update the cursor
    // otherwise map pte back to the video memory, save the x,y coordinates and update the cursor
    if (get_current_looking_terminal() == get_current_running_terminal()){
        update_cursor(get_x(), get_y());
    }else{
        set_pte(VID_INDEX, VID_ADDR);
        set_x(terminals[get_current_looking_terminal()].x_pos);
        set_y(terminals[get_current_looking_terminal()].y_pos);
        update_cursor(get_x(), get_y());
    }
    return nbytes;
}

/* 
 * terminal_read
 * description: 
 * read the keyborad buffer to the buf parameter
 * input: 
 * int32_t fd
 * buf: The buffer to read to from the keyboard_buffer
 * nbytes: the number of bytes needs to read.
 * output: The number of bytes actually read.
 * side effect: fill buf with the values in keyboard_buffer
 */
int32_t terminal_read(int32_t fd, void *buf, int32_t nbytes)
{
    int8_t *tmp;
    // reset the enter flag to low to clear the flag
    terminals[get_current_running_terminal()].enter_flag = LOW;

    // sanity check
    if (!buf)
        return -1;
    if (nbytes > KEY_BUFFER_SIZE)
        nbytes = KEY_BUFFER_SIZE;

    // wait for the current running terminal enter "Enter"
    while (1)
    {
        if (terminals[get_current_running_terminal()].enter_flag == HIGH){
            break;
        }
    }

    // after receive the enter, clear the flag
    terminals[get_current_running_terminal()].enter_flag = LOW;

    //copy from the keyboard_buffer to tmp buffer(buf), fill the all the rest of the buffer with 0
    tmp = (int8_t *)buf;
    strncpy(tmp, keyboard_buffer, nbytes);

    //clear keyboard buffer and reset the buffer_ptr
    memset(keyboard_buffer, '\0', KEY_BUFFER_SIZE);
    
    // reset the current running terminal's buffer pointer
    terminals[get_current_running_terminal()].curr_buffer_ptr = 0;

    // reset the buffer pointer
    buffer_ptr = 0;
    
    //return the number in one line terminates by \n or nbytes
    if (tmp[nbytes - 1] != '\0')
        return nbytes;
    return strlen(tmp);
    
}

/* 
 * terminal_open
 * description: 
 * corresponds to one system call
 * input: const uint8_t* filename
 * output: 0
 * side effect: clear the keyboard_buffer
 */
int32_t terminal_open(const uint8_t *filename)
{
    memset(keyboard_buffer, '\0', KEY_BUFFER_SIZE);
    return 0;
}

/* 
 * terminal_close
 * description: 
 * corresponds to one system call, do nothing
 * input: int32_t fd
 * output: 0
 * side effect: none
 */
int32_t terminal_close(int32_t fd)
{
    return 0;
}

/* 
 * read_write_test
 * description: 
 * call terminal_read and terminal_write to test functionality
 * input: none
 * output: none
 * side effect: print what ever is on keyboard_buffer on the screen, 
 * clear key_board_buffer_2
 */
void read_write_test()
{
    //clear the buffer
    memset(keyboard_buffer_2, '\0', KEY_BUFFER_SIZE);
    int32_t fd = 0;
    void *buf = (void *)keyboard_buffer_2;
    //test number greater than 128
    int32_t nbytes = 200;
    //read the keyboard buffer to keyboard_buffer_2
    terminal_read(fd, buf, nbytes);
    //write the buffer to the screen
    terminal_write(fd, buf, nbytes);
    memset(keyboard_buffer_2, '\0', KEY_BUFFER_SIZE);
}

/*
 * is_letter
 *   DESCRIPTION: check if the key pressed is a letter
 *   INPUTS: intt key -- key pressed
 *   OUTPUTS: none
 *   RETURN VALUE: 1 for success
 *                 0 for failure
 *   SIDE EFFECTS:  none
 */
int is_letter(int key)
{
    if ((key >= Q_PRESSED && key <= P_PRESSED) ||
        (key >= A_PRESSED && key <= L_PRESSED) ||
        (key >= Z_PRESSED && key <= M_PRESSED))
        return ONE;
    return ZERO;
}

/*
 * get_keyboard_buffer
 *   DESCRIPTION: get the current keyboard buffer
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: keyboard buffer
 *   SIDE EFFECTS:  none
 */
int8_t *get_keyboard_buffer()
{
    return keyboard_buffer;
}

/*
 * set_buffer_ptr
 *   DESCRIPTION: set the keyboard buffer to the given value
 *   INPUTS: new_ptr -- the next keyboard buffer index
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:  none
 */
void set_buffer_ptr(int new_ptr){
    buffer_ptr = new_ptr;
}

/*
 * get_buffer_ptr
 *   DESCRIPTION: get the keyboard buffer index
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:  none
 */
int get_buffer_ptr(){
    return buffer_ptr;
}
