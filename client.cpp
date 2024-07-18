#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"



int main()
{

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &cli_addr.sin_addr.s_addr);
    errif(connect(socket_fd, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) == -1, "socket connect err");

    while(true)
    {
        char buf[1024];
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(socket_fd, buf, sizeof(buf));
        if (write_bytes == -1)
        {
            printf("server already disconnect, cant write any more!\n");
            break;
        }

        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(socket_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            close(socket_fd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(socket_fd);
            errif(true, "socket read error");
        }
    }
    

    return 0;
}