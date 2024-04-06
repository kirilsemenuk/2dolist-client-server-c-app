#include"superheader.h"
#include"01_add.h"

#include"02_add.h"
#include"03_add.h"
#include"04_add.h"


bool stop_con =false;

//signal func
void handle_sigtstp(int sig){
    fprintf(stdout,"signal \n" ); 
    stop_con=true;
    //stop=1;
}
//thread function
void * handle_connection(void * p_client_socket){
    int client_socket=*(int*)p_client_socket;
    int MODE=0;
    char buffer[BUFFSIZE];
    MODE=get_mode_from_client(client_socket);
    fprintf(stdout,"\t--->%d\n",MODE);
    switch (MODE)
    {
    case 1:
        /* display a task */
        fprintf(stdout,"/* display a task */");
        char tname[BUFFSIZE]={0};
        read(client_socket,tname,BUFFSIZE);
        pthread_mutex_lock(&mutex);
        print1task(client_socket,tname);
        pthread_mutex_unlock(&mutex);
        
        break;
    case 2:
        /* add a task */
         fprintf(stdout,"/* add a task */");
        char name[BUFFSIZE]={0};
        char dis[BUFFSIZE]={0};
        recv(client_socket,name,BUFFSIZE,0);///need to work with szie
        recv(client_socket,dis,BUFFSIZE,0);///need to work with szie
        
        pthread_mutex_lock(&mutex);
        name2struct2file(name);
        
        new_task_folder(name,dis);
        pthread_mutex_unlock(&mutex);
        
        //printf(" %s \t %s \n ",name,dis);
        break;    
    case 3:
        /* add file to task */
         pthread_mutex_lock(&mutex); 
         fprintf(stdout,"/* add file to task */");
         get_file(client_socket);
         pthread_mutex_unlock(&mutex);

         
        break;
    case 4:
        /* finsh an task */
         fprintf(stdout,"/* display a task */");
         ///cheack if task exsist 
         //cenge end time
        char end_task[BUFFSIZE]={0};
        recv(client_socket,end_task,BUFFSIZE,0); 
        pthread_mutex_lock(&mutex); 
        end_tasck(end_task,client_socket);
        pthread_mutex_unlock(&mutex);
        break;
    case 5:
        /*  get an task */
         pthread_mutex_lock(&mutex); 
         fprintf(stdout," /* finsh an task */");
         send_full_task(client_socket);
         pthread_mutex_unlock(&mutex);
        break;  
    case 6:
        /*   close the server */
         fprintf(stdout,"/ /*   close the server */");
         kill(getppid(),SIGUSR2);

        break;          
    default:
        pthread_mutex_lock(&mutex);
        printAll(client_socket);
        pthread_mutex_unlock(&mutex);

        break;
    }


    fprintf(stdout,"\nconnection end \n");

    close(client_socket);
    return NULL;
    
}

int main(){

    //signals

    signal(SIGUSR2,handle_sigtstp);
    //struct sigaction sa;
    //sa.sa_handler = &handle_sigtstp;
    // sigaction(SIGUSR2,&sa,NULL);
    

    //socket creation
    int server_socket, client_socket;
    struct sockaddr_in serv_addr ;
    check((server_socket=socket(AF_INET,SOCK_STREAM,0)),
    "failed to creatr socket\n");
    serv_addr.sin_family =AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port =htons(PORT);
    check((bind(server_socket,(struct sockaddr*)&serv_addr,sizeof(serv_addr))),
    "failed to bind\n");
    check(listen(server_socket,5),"failed to listen \n");


//base functoin create folder and task file
server_begin_function();
//the loop
    
    pthread_t t1[10];
    while (!stop_con)
    {
        int i=0;
        //connecting
        fprintf(stdout,"whaiting to connect \n");
        check(client_socket=accept(server_socket,NULL,NULL)
        ,"failed to accept\n");
        
        
        //trading

        void *p_client_socket=(void*)&client_socket;
        pthread_create(&t1[i],NULL,handle_connection,p_client_socket);
        i++;
        //usleep(4000000);
    }
    //after loop
    ///pthread_join(t1,NULL);
    printf("good bay ,server go to sleep \n");
    close(server_socket);
    
    return 0;
}