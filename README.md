# Illinix_OS
A compact operating system as the final project of ECE391 at UIUC

## Description
This os support basic functions of an operating system:\
file system,system calls, hardware api and multi-process scheduling

## File organization
* Main - key kernel code folder, some of the most important ones are:
     * kernel.c -- initial bootup code and initialization 
     * do_sys.c -- systemcall prosedures 
     * idt.c -- interrupt descriptor table and interrupt handlers  
* Syscalls/fish - testfunctions and library functions in c
* fsdir - directory copied to the virtual machine running the os 
