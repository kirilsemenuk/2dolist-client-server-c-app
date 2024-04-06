//the file must be in the folder

#include"02_add.h"
int map_and_send(int soc,char *path,char * task){
 //send distantion
    char task_name[FOLDER_MAX_SIZE]={0},file_name[FOLDER_MAX_SIZE]={0};
    strcpy(task_name,task);
    strcpy(file_name,path);

    //maping the file
    int fd=open(path,O_RDWR,(mode_t)0600);
    if(fd==-1){
        return -1;
        puts("file did not exsist\n");}
    struct stat s;
    size_t status = fstat (fd, & s);// need to cheack the error !
    int file_size=s.st_size;
    char myfile[file_size];



    char *ptr=mmap(0,file_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
   // memcpy(ptr, myfile, file_size);
    //msync(ptr, file_size, MS_SYNC);

    // the sending
    //send task name
    send(soc,task_name,FOLDER_MAX_SIZE,0);
    //send file name
    send(soc,file_name,FOLDER_MAX_SIZE,0);

    //file size
    send(soc,&file_size,sizeof(size_t),0);
    //send the file
    send(soc,ptr,file_size,0);
    puts(ptr);
    munmap(ptr,file_size);
    close(fd);
    return 0;
}


int get_file(int soc){
    char task_name[FOLDER_MAX_SIZE]={0},file_name[FOLDER_MAX_SIZE]={0};
    size_t file_size;
    recv(soc,task_name,FOLDER_MAX_SIZE,0);

    recv(soc,file_name,FOLDER_MAX_SIZE,0);
    char buff[SIZE]={0};
    sprintf(buff,"DataBase/%s/%s",task_name,file_name);
    recv(soc,&file_size,sizeof(size_t),0);
    char myfile[file_size];


    int fd=open(buff,O_RDWR|O_CREAT,(mode_t)0600);
    if(!fd){
        puts("OPEN PROBLAM \n");
        return -1;

    }
    char *ptr=mmap(0,file_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    recv(soc,myfile,file_size,0);
    puts(myfile);
    ftruncate(fd,file_size);
    memcpy(ptr,myfile,file_size);
    munmap(ptr,file_size);
    close(fd);
    
    return 0;
}

