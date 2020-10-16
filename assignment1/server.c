// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";

    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // modified to resolve setsockopt: Protocol not available issue in OS X
    if (
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ||
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))
    ) {
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //Privilege Seperation Code 

    // split is made here to seperate socket setup and data processing

    int pid = fork();
    
    
    if (pid < 0)
    {
        // Error if process id is negative, exit gracefully
        printf("Error while creating child process! Fork Failed!");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // If process id is zero, in child process
        printf("Printing from Child process\n");
        // Try to set uid.
        if (setuid(-2) < 0)
        {
            // in case of error gracefully exit
            perror("Error while setting uid!");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Send the message from the child with lower privileges than parent
            valread = read( new_socket , buffer, 1024);
            printf("%s\n", buffer);
            send(new_socket , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
        }
    }
    else
    {
        //If process id is greater than zero, in parent process
        printf("Printing from parent\nWait for child process\n");
        wait(NULL);
    } 
    return 0;
}

