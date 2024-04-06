#include"superheader.h"

struct task_linkedlist* file2ll();
int send_name(int sock,char * name);
int end_tasck(char * name,int client_socket);
int ll2file(struct task_linkedlist *head);
int printAll(int client_socket);
int CprintAll(int client_socket);
int print1task(int client_socket,char * name);
struct task_linkedlist* dell_task(int num,struct task_linkedlist* head);
int cleanFolder(int client_socket);
int task_file(char* name);