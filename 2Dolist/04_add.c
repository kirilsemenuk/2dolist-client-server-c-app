#include"04_add.h"





int send_name(int sock,char * name){
    char buff[BUFFSIZE];
    strcpy(buff,name);
    send(sock,buff,BUFFSIZE,0);
    fprintf(stdout,"h1\n");
    return 0;
}
int end_tasck(char * name,int client_socket)
{
    struct task_linkedlist *alltsk=file2ll();
    struct task_linkedlist *head=alltsk;
    int i=0;
    char  buff[254];
    while(alltsk!=NULL)
    {
        if(strcmp(name,alltsk->x.name)==0)
        {
            if(alltsk->x.end_time!=0)
            {
                printf("task all-ready ended\n");
                send(client_socket,"task all-ready ended",20,0);
                return 0;
            }
            alltsk->x.end_time=time(NULL);
            task_file(alltsk->x.name);
            sprintf( buff,"task: %s\n\t number of sec:%ld\n",alltsk->x.name,alltsk->x.end_time-alltsk->x.start_time);
            send(client_socket,buff,254,0);
            break;;
        }
        i++;
        alltsk=alltsk->next;
    }
    ll2file(head);
    return 0;
}

int printAll(int client_socket)
{
    struct task_linkedlist *alltsk=file2ll();
    struct tm *ptm,*ptm1;
    char  buff[254],line[1024]={0},path[1000],name[25];
    FILE* F1;
    int i=1;

    while(alltsk->next!=NULL)
    {   
        //line[1000]={0};
        strcpy(name,alltsk->x.name);
        printf("name:%s\n%ld",alltsk->x.name,alltsk->x.start_time);
        alltsk=alltsk->next;
        printf("name:%s\n%ld",alltsk->x.name,alltsk->x.start_time);
        sprintf( path, "DataBase/%s/README.txt",name);
        F1 = fopen(path,"r");
        fscanf(F1,"%[^\n]",line);
        printf("%2d.%s\n",i,line);
        fclose(F1);
        
        //sprintf( buff, "%s\n ",line);

        printf("%d. end\n",i);
        send(client_socket,line,1024,0);
        strcpy(line,"");
        i++;
    }
    printf("*__*");
    send(client_socket,"*__*",1024,0);
    printf("end\n");
    return 0;
}
int ll2file(struct task_linkedlist *head){

    FILE*f1;
    remove ("DataBase/task.bin");
    f1=fopen("DataBase/task.bin","wb");
    struct task_linkedlist* temp=head,*temp2;
    while (head!=NULL)
    {
        fwrite(&head->x,1,sizeof(struct task),f1);
        temp=head;
        head=head->next;
        if(temp!=NULL)
        {
            free(temp);
        }
    }
    fclose(f1);
    return 0;
}
int CprintAll(int client_socket)
{
    char printTask[BUFFSIZE];
    while(true)
    {
        recv(client_socket,printTask,BUFFSIZE,0);

        if(strcmp(printTask,"*__*")==0){
            return 0;
        }
        printf("%s\n",printTask);
    }
}
int print1task(int client_socket,char * name)
{
    struct task_linkedlist *alltsk=file2ll();
    struct tm *ptm,*ptm1;
    char  buff[254],line[]={0},path[254];
    FILE* F1;
    int i=1;

    while(alltsk->next!=NULL)
    {   
        if(strcmp(name,alltsk->x.name)==0)
        {    
        //line[1000]={0};
        sprintf( path, "DataBase/%s/README.txt",alltsk->x.name);
        F1 = fopen(path,"r");
        fscanf(F1,"%[^\n]",line);
        fclose(F1);
        printf("%s",line);
        send(client_socket,line,1024,0);        
        break;
        }
        if(alltsk->next==NULL)break;
        alltsk=alltsk->next;
        i++;
        break;
    }
    send(client_socket,"*__*",5,0);
    return 0;
}
struct task_linkedlist* dell_task(int num,struct task_linkedlist* head)
{
    struct task_linkedlist *one=head;
    struct task_linkedlist *two=head->next;
    int i=1;
    while (two!=NULL)
    {
        if(i==num && num==1)
        {
            one=one->next;
            free(head);
            return one;
        }
        if(i==num && num!=1)
        {
            one->next=two->next;
            free(two);
            return head;
        }
        if(i>num)
        {
            printf("number task dont found\n");
            return NULL;//or head
        }
        if(i!=1)
        {
        one=one->next;
        two=two->next;
        }
        i++ ;  
    }
    return NULL;
}
int cleanFolder(int client_socket)
{
    struct task_linkedlist *alltsk=file2ll();
    struct task_linkedlist *head;
    char def[254],defnew[254];
    int i=1,j=0;
    head=alltsk;
    while(true)
    {   printf("%s time: %ld\n",alltsk->x.name,alltsk->x.end_time);
        if(alltsk->x.end_time!=0)
        {
            head=dell_task(i,head);
            ll2file(head);
            sprintf( def, "./DataBase/%s",alltsk->x.name);
            sprintf( defnew, "./DataBase/%s-finished",alltsk->x.name);
            printf("def: %s\n\n",def);
            //remove(def);
            rename(def,defnew);
            j++;
        }
        alltsk=alltsk->next;
        i++;
    }
    if(j!=0)
    {
        send(client_socket,"remove files sucsessfuly!",1024,0);
    }
    else
    {
        send(client_socket,"dont faund finished files!",1024,0);
    }
}



int task_file(char* name){

    char buff[50],path[30],line[1000],date[50];
    struct task_linkedlist *alltsk=file2ll();
    struct tm *ptm;
    sprintf( path, "DataBase/%s/README.txt",name);
    FILE* F2 = fopen(path,"r");
    fscanf(F2,"%[^\n]",line);
    fclose(F2);
    FILE* f1=fopen(path,"w");
    if(f1==NULL)return 1;
    while (alltsk!=NULL)
    {
        if(strcmp(alltsk->x.name,name)==0)
        {
            break;
        }
        alltsk=alltsk->next;
    }
    
    ptm = localtime(&alltsk->x.start_time);
    sprintf( date, "end at:%2d/%2d/%d\n",ptm->tm_mday,ptm->tm_mon+1,ptm->tm_year+1900);
    fprintf(f1,"%s:\t%s \n---------------------------------------\n",line,date);
    fclose(f1);
    ///chdir("/.."); 
    return 0;
}