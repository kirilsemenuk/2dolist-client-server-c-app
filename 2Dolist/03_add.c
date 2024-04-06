#include"03_add.h"
int send_full_task(int soc){
    char task[SIZE]={0};
    char buff[SIZE]={0};
    recv(soc,task,SIZE,0);
    if(cheack_if_folder_exsist(task)==0){
        strcpy(task,"TASK DIDM'T EXSIST\n");
        send(soc,task,SIZE,0);
        return 1;
    }
    int filesnum=my_num_of_files(task);
    send(soc,&filesnum,sizeof(int),0);
    if(!filesnum)
        return 1;   
    sprintf(buff,"DataBase/%s",task);
    DIR* d=opendir(buff);
    if(d){
        ;
    }else
        return 0 ;
        char buff2[20];
    struct dirent * entry;
    while ((entry = readdir(d)) != NULL) {
        sprintf(buff2,"%s",entry->d_name);
    if (strcmp(buff2,".")==0||strcmp(buff2,"..")==0) 
        continue;
        send_one_file(soc,buff2);
        
    }

    closedir(d);


    return 0;
}

int get_full_task(int soc,char* task){
    char buff[SIZE]={0};
    sprintf(buff,"%s",task);
    send(soc,buff,SIZE,0);
    if(0==strcmp(buff,"TASK DIDM'T EXSIST\n")){
        return -1;
    }
    mkdir(task,0770);
    chdir(task);
    int filenum=0;
    recv(soc,&filenum,SIZE,0);
    if(!filenum){

        return -1;
    }
    int i=0;
    while (i<filenum)
    {
        get_one_file(soc);
        i++;
    }
    
    chdir("..");

   return 0;
}

int cheack_if_folder_exsist(char* task){
    char buff[SIZE]={0};
    sprintf(buff,"DataBase/%s",task);
    DIR* d=opendir(buff);
    if(d){
        closedir(d);
        return 1;
    }



    return 0;
}

int my_num_of_files(char* task){
char buff[SIZE]={0};
int file_count = 0;
    sprintf(buff,"DataBase/%s",task);
    DIR* d=opendir(buff);
    if(d){
        ;
    }else
        return 0 ;
    struct dirent * entry;
    while ((entry = readdir(d)) != NULL) {
    if (strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0) 
        continue;
    
         file_count++;
    }
    closedir(d);

    return file_count;
}
int send_one_file(int soc,char*filename){
    //sending file name 
    //file size
    //file

    char buff[SIZE]={0};
    sprintf(buff,"%s",filename);
    send(soc,buff,SIZE,0);//sendin ok
    sprintf(buff,"DataBase/%s",filename);
    struct stat st;
    stat(filename,&st);
    int filesize=st.st_size;///problem
    send(soc,&filesize,sizeof(int),0);
    int fd=open(filename,O_RDWR,(mode_t)0600);
    
    if(!fd){

        return 1;
    }
    char *ptr=mmap(0,filesize,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==NULL){
        return 2;
    }
    send(soc,ptr,filesize,0);
    munmap(ptr,filesize);
    close(fd);

    
    return 0;
}

int get_one_file(int soc){

    char buff[SIZE]={0};
    read(soc,buff,SIZE);//problem
    int filesize=0;
    recv(soc,&filesize,sizeof(int),0);
 

    int fd=open(buff,O_RDWR|O_CREAT,(mode_t)0600);

    if(!fd){

        return 1;
    }
    char *ptr=mmap(0,filesize,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==NULL){
        return 2;
    }
    ftruncate(fd,filesize);
    recv(soc,ptr,filesize,0);
    munmap(ptr,filesize);
    close(fd);

    

    return 0;
}