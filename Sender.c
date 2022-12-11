#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>

#define Size 1024

void send_file(FILE *fp, int socket){
    char data[Size] = {0};
    while(fgets(data, Size, fp) != NULL){//when we want to send half massege we uselength/2
        if(send(socket, data, sizeof(data), 0) == -1){
            perror("problem with sending data");
            exit(1); 
        } 
        bzero(data, Size);
   }
}




int main(){

char *loopback_ip = "127.0.0.1";
int port = 9999;

FILE *fp;
char *file_name = "send_me.txt";

// initializing socket
int sender_socket;
sender_socket = socket(AF_INET, SOCK_STREAM, 0);
if(sender_socket == -1){
    perror("problem with initialzing socket");
    exit(1);
}
printf("Socket has been initiazed succesfully\n");

struct sockaddr_in Server_Address;
Server_Address.sin_family = AF_INET;
Server_Address.sin_port = htons(port);
Server_Address.sin_addr.s_addr = inet_addr(loopback_ip);;

int connection_status;
connection_status = connect(sender_socket, (struct sockaddr *) &Server_Address, sizeof(Server_Address));
if(connection_status == -1){
    perror("there is a problem in the connection\n");
    exit(1);
}
printf("Connection has been made successfully\n");

fp = fopen(file_name, "r");
if(fp == NULL){
    perror("Error in reading file\n");
    exit(1);
}
printf("file has been read successfully\n");

send_file(fp, sender_socket);















/*
char reciever_response[256];
recv(sender_socket, &reciever_response, sizeof(reciever_response), 0);

printf("the server sent the data.\n%s\n", reciever_response);

close(sender_socket);
printf("sub_socket closedsuccesfully\n");
*/

return 0;
}