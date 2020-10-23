#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

int main(int argc, char const *argv[]) 
{
    int valread, new_socket;
    const char *hello;
    char buffer[1024] = {0};
    printf("In newly created child with uid %d \n", getuid());
    if (argc != 3)
    {
        perror("Not correct number of arguments!");
        exit(EXIT_FAILURE);
    }
    else
    {
        new_socket = *argv[1];
	    hello = argv[2];
    }
    // Send the message from exec-ed child with lower privileges than parent
    valread = read( new_socket , buffer, 1024); 
    printf("%s\n", buffer); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent from execed child\n");
    return 0;
} 
