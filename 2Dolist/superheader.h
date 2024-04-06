#ifndef HEADER_FILE
#define HEADER_FILE

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<sys/mman.h>
#include<pthread.h>
#include<signal.h>
#include"string.h"
#include<errno.h>
#include<time.h>
#define BUFFSIZE 1024


#include<assert.h>

#include<strings.h>


#include<sys/mman.h>
#include <unistd.h>


#define FOLDER_MAX_SIZE 20
typedef enum { false, true } bool;
struct task
{
    char name[25];
    time_t start_time;
    time_t end_time;
}task;
struct task_linkedlist
{
    struct task x;
    struct task_linkedlist * next;
}task_linkedlist;





#define SIZE 1024
#endif