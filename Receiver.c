#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>

#define Size 1024

void write_file(int socket){
    int n;
    FILE *fp;
    char *file_name ="arrived.txt";
    char buffer[Size];

    fp = fopen(file_name, "w");
    if(fp == NULL){
    perror("Error in writing to file\n");
    exit(1);
    }

    while(1){
        n = recv(socket, buffer, Size, 0);
        if(n <= 0){
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, Size);
    }
    return;
}

int main(){

char *loopback_ip = "127.0.0.1";
int port = 9999;

__socklen_t addr_size;

//char receiver_massage[256] = "you have reached the server";

int main_receiver_socket, sub_receeiver_socket;
if((main_receiver_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("problem with initialzing socket");
    exit(1);
}
printf("Socket has been initiazed succesfully\n");


struct sockaddr_in Server_Address, New_Address;
Server_Address.sin_family = AF_INET;
Server_Address.sin_port = htons(port);
Server_Address.sin_addr.s_addr = inet_addr(loopback_ip);

int bind_check;
bind_check = bind(main_receiver_socket, (struct sockaddr*) &Server_Address, sizeof(Server_Address));
if(bind_check == -1){
    perror("there is problem with binding");
    exit(1);
}
printf("Binding has been made succesfully\n");

int listen_check;
listen_check = listen(main_receiver_socket, 1);
if(listen_check == 0){
printf("Listening...\n");
}
else{
    perror("there is problem with listening");
    exit(1);
}

addr_size = sizeof(New_Address);
sub_receeiver_socket = accept(main_receiver_socket, (struct sockaddr *) &New_Address, &addr_size);
if(sub_receeiver_socket == -1){ // dont need this if
    perror("accept doesn't work");
    exit(1);
}
printf("accept works\n");

write_file(sub_receeiver_socket);
printf("data has been written succesfully to the file\n");




/*
if((sub_receeiver_socket = accept(main_receiver_socket, NULL, NULL)) == -1){
    perror("problem with initializing sub socket");
    exit(1);
}
printf("Socket has been initiazed succesfully\n");

send(sub_receeiver_socket, receiver_massage, sizeof(receiver_massage), 0);

close(sub_receeiver_socket);
printf("sub_socket closedsuccesfully\n");
*/
    return 0;
}

