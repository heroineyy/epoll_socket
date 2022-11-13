#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h>

void sys_error(const char *str) 
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    char buf[BUFSIZ];
    
    int cfd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7777);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    int ret = connect(cfd, (struct sockaddr*)&server_addr,sizeof server_addr);
    if(ret == -1 ) sys_error("connet error");

    int flag = fcntl(STDIN_FILENO, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flag);

    flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);

    while(1)
    {
        ret = read(STDIN_FILENO, buf, sizeof buf);
        if(ret > 0) write(cfd, buf, ret);
        if(!strncmp(buf,"exit",4)) break;
        int ret = read(cfd, buf, sizeof buf);
        if(ret > 0) write(STDOUT_FILENO, buf, ret);
    }
    
    close(cfd);
    return 0;
}
