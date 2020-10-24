// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    // Dev.to Struct (reference to pwnam usage) https://dev.to/0xbf/use-getpwname-to-get-uid-by-login-name-c-lang-4bc2
    char name[30] = {"nobody"};
    struct passwd* pwd;

    printf("execve=0x%p\n", execve);

    pwd = getpwnam(name);
    if(pwd == NULL) {
       printf("pwd empty");   
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
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
    // This is when the application listens to the socket (processes data)
    // After the socket is created then is attached to the 8080 port, we let socket address struct be set and then 
    // once the bind is complete we know the root process is attached to the port
    // At this point we know that the the root privileges can be dropped by
    // forking the root process and then dropping privileges of the child to the nobody user
    // since the connection is already established the read and write data processing using the new_socket
    // can be done with droppped privileges

    // We store the forked process (child) into pid_t variable
    pid_t child = fork();
    int returnstatus;
    // first if the forked value is larger than 0 means we are in the parent process
    if(child > 0) {
        // Call waitpid to wait for the child process to exit
        while(waitpid(child, &returnstatus,0)>0);
        // exit parent
        exit(0); 
    }
    else if (child == 0) {
        // print statement to show the nobody name using pwnam 
        printf("pwd name = %d \n",pwd->pw_name);
        // Here change child priveleges to the pw_name we received from pwnam (drop priviliges)
        setuid(pwd->pw_name);
        // now server will listen through the nobody process (data processing)
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
        
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        // After child is done data processing it will exit the child process
        exit(0);
    } else {
        printf("ERROR");
        exit(child);
    }
    return 0;
}
