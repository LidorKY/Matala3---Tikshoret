#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "string.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#define SIZE 2097152
#define id1 5230
#define id2 9714

int send_file1(FILE *fp, int sender_socket){

    char data [SIZE]={0};//We have has been asked for 1MB file size. Therefore, we need 1024KB and 1KB=1024B. Furtheremore each char is 8B. 
    size_t position = ftell(fp);//current position
    fseek(fp,0,SEEK_END);//Go to end
    size_t length = ftell(fp);//the position is the size
    fseek(fp,position,SEEK_SET);//restore original position

    while (fgets(data, length ,fp))
    {
        if(send(sender_socket,data,length/2,0)==-1){
            perror("error in sending data 1.\n");
            exit(1);
        }  
    }
    return 0;
}

int send_file2(FILE *fp, int sender_socket){

    char data [SIZE]={0};//We have has been asked for 1MB file size. Therefore, we need 1024KB and 1KB=1024B. Furtheremore each char is 8B. 
    size_t position = ftell(fp);//current position
    fseek(fp,0,SEEK_END);//Go to end
    size_t length = ftell(fp);//the position is the size
    fseek(fp,position,SEEK_SET);//restore original position

    while (fgets(data, length ,fp))
    {
        if(send(sender_socket,&data[(length/2)],sizeof(data),0)==-1){
            perror("error in sending data 2.\n");
            exit(1);
        }
       bzero(data,SIZE);
    }
    return 0;
}

int main(){
    //creating a socket
    int sender_socket;
    sender_socket=socket(AF_INET, SOCK_STREAM, 0);//because we are in linux the default cc is cubic.
    if(sender_socket==-1){
        printf("there is a problem with initializing sender.\n");
    }
    else{
        printf("-initialize successfully.\n");
    }

struct sockaddr_in Receiver_address;
FILE *fp;
char *filename ="Sender_massege.txt";

Receiver_address.sin_family = AF_INET;
Receiver_address.sin_port = htons(9999);
Receiver_address.sin_addr.s_addr = INADDR_ANY;

int connection_status = connect(sender_socket,(struct sockaddr *) &Receiver_address,sizeof(Receiver_address));
if(connection_status==-1){
    printf("there is an error with the connection.\n");
}
else{
printf("-connected.\n");
}
//char server_response[256];
//recv(sender_socket,&server_response, sizeof(server_response),0);

//printf("The server sent the data: %s .\n", server_response);

fp = fopen(filename, "r");
if(fp==NULL){
    perror("Error in reading file.");
    exit(1);
}

//send first half of the file
if(send_file1(fp,sender_socket)==0){
printf("-File data 1 has been send successfully.\n");
}

//receive xor
char xor[256];
if(recv(sender_socket, &xor,sizeof(xor),0)<=0){
    perror("Sender didn't get the xor");
    exit(1);
}
printf("The server sent the xor: %s .\n", xor);
bzero(xor, 256);//like memset- delete the first n characters in thr String.


//change algorithm
char *Reno = "reno";
socklen_t Reno_len = strlen(Reno);
if (setsockopt(sender_socket, IPPROTO_TCP,TCP_CONGESTION,Reno,Reno_len) != 0){//the change in CC from Cubic to Reno 
    perror("error in changing algorithm");
    exit(1);
}

//send second half of the file
if(send_file2(fp,sender_socket)==0){
printf("-File data 2 has been send successfully.\n");
}

close(sender_socket);
printf("-closing...\n");

return 0;
}
