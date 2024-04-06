#ifndef HEADER_FILE1
#define HEADER_FILE1

#include"superheader.h"


pthread_mutex_t mutex ; 



#include <unistd.h>
#define SIZE 1024
#define PORT 12344
#define MODESIZE 2

int arg_to_int(char* word);
void check(int a,char * mess);
int get_mode_from_client(int sock);
int server_begin_function();//create loge file ,and data base folder
//int requast_to_add_new_task ;
int send_name_and_discription(int sock,char * name ,char *dis);
int get_name_and_discription(int sock,char * name ,char *dis);
int name2struct2file(char* name);
int new_task_folder(char* name,char* dis);
int is_name_exist(char* name);
struct task_linkedlist* file2ll();


#endif