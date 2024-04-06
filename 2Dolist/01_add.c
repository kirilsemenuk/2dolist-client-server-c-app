#include"01_add.h"


void check(int a,char * mess){
    if(a==-1){
    fprintf(stdout,"%s\n",mess);
    exit(0);
    }

}
int get_mode_from_client(int sock){
    char M_buff[MODESIZE]={0};
    int mode=0;
    recv(sock,M_buff,MODESIZE,0);
    mode=atoi(M_buff);
    return mode;
}
int arg_to_int(char* word){

    if(strcmp(word,"-d")==0){
        return 1;
    }
    else if (strcmp(word,"-n")==0)
    {
       return 2;
    }
    else if (strcmp(word,"-u")==0)
    {
       return 3;
    }else if (strcmp(word,"-x")==0)
    {
       return 4;
    }
    else if (strcmp(word,"-g")==0)
    {
       return 5;
    }
    else if (strcmp(word,"-xxx")==0)
    {
       return 6;
    }
    else
         return -1;
}
int server_begin_function(){
    if(mkdir("DataBase",0777)==-1)
    perror("Error");
    FILE* f1=fopen("DataBase/task.bin","r");
    int x=0;
    if(f1==NULL){
        f1=fopen("DataBase/task.bin","w");
    }
    fclose(f1);
    return 0;
}
int send_name_and_discription(int sock,char * name ,char *dis){
    char buff[BUFFSIZE];
    strcpy(buff,name);
    send(sock,buff,BUFFSIZE,0);
    fprintf(stdout,"h1\n");
    strcpy(buff,dis);
    send(sock,buff,BUFFSIZE,0);
    fprintf(stdout,"h2\n");
    return 0;
}


int get_name_and_discription(int sock,char* name ,char * dis){
    char buff[BUFFSIZE];
    recv(sock,buff,BUFFSIZE,0);////need to work with the size
    strcpy(name,buff);
    recv(sock,buff,BUFFSIZE,0);
    strcpy(dis,buff);
    return 0;
}
int new_task_folder(char* name,char* dis){
    char buff[50],line[1024];
    FILE* f1;
    struct tm *ptm;
    time_t now;
    time(&now);
    sprintf(buff,"./DataBase/%s",name);
    puts("hello\n");
    if(mkdir(buff,0777)==-1){
    perror("Error");
    return 1;
    }
  //  puts("hello2\n");
    //chdir(buff);
    sprintf(buff,"./DataBase/%s/README.txt",name);
    f1=fopen(buff,"w");
    if(f1==NULL)
    {
        printf("--exit\n");
        return 1;
    }
    ptm = localtime(&now);
    sprintf(line,"%s- %2d/%2d/%d -\t%s \n---------------------------------------\n",name,ptm->tm_mday,ptm->tm_mon+1,ptm->tm_year+1900,dis);
    fprintf(f1,"%s\n",line);
    fclose(f1);
    ///chdir("/..");
    return 0;
}

int is_name_exist(char* name){

    struct task_linkedlist *alltsk=file2ll();
    struct task_linkedlist *head=alltsk;
    //head=alltsk;
    int lior=0;
    while(alltsk!=NULL)
    {   
        printf("%s!=%s\n",name,alltsk->x.name);
        //printf(typeof)
        if(strcmp(name,alltsk->x.name)==0)
            {
                printf("exist\n");
                lior=1;
                break;
            }
        alltsk=alltsk->next;
    }
    if(lior==0)
        printf("not exist\n");
    struct task_linkedlist *temp;
    alltsk=head;
    while (alltsk!=NULL)
    {   temp=alltsk;
        alltsk=alltsk->next;
        free(temp);//-----
    }
    
    return lior;
}
struct task_linkedlist* file2ll(){
    FILE *f1=fopen("DataBase/task.bin","rb");
    if(f1==NULL){
        printf("fille didn't opend!!!!\n");
        return NULL;
    }
    struct task_linkedlist *head=malloc(sizeof(struct task_linkedlist));
    
    struct task_linkedlist *temp=head;

    while (fread(&head->x,sizeof(struct task),1,f1)==1)
    {
        
       
        head->next=malloc(sizeof(struct task_linkedlist));
        head=head->next;
        head->next=NULL;
        }
    


    return temp;
}

int name2struct2file(char* name){
 
    if(is_name_exist(name))//lior freee the ll
        return 1;

    struct task t1;
    sprintf(t1.name,"%s",name);
    t1.start_time=time(NULL);
    t1.end_time=0;
   // pthread_mutex_lock(&mutex);
    FILE* f1=fopen("DataBase/task.bin","ab");//errno cheack
   /// pthread_mutex_unlock(&mutex);
    if(f1==NULL)
        return -1;
    fwrite(&t1,1,sizeof(struct task),f1);
    fclose(f1);
    return 0;
}