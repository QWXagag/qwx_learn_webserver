#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "Buffer.h"

class Buffer;

int main()
{

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &cli_addr.sin_addr.s_addr);
    errif(connect(socket_fd, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) == -1, "socket connect err");

    Buffer* read_buf = new Buffer();  
    Buffer* send_buf = new Buffer();
    while(true)
    {
        send_buf->getline();
        ssize_t write_bytes = write(socket_fd, send_buf->c_str(), send_buf->size());
        if (write_bytes == -1)
        {
            printf("server already disconnect, cant write any more!\n");
            break;
        }

        char buf[1024];
        bzero(&buf, sizeof(buf));
        int already_read = 0;
        while (true)
        {
            ssize_t read_bytes = read(socket_fd, buf, sizeof(buf));
            if (read_bytes > 0)
            {
                read_buf->append(buf, read_bytes);
                already_read += read_bytes;
            }
            else if (read_bytes == 0)
            {
                printf("server socket disconnected!\n");
                close(socket_fd);
                exit(EXIT_SUCCESS);
            }
            else if (read_bytes == -1)
            {
                close(socket_fd);
                errif(true, "socket read error");
            }
            if (already_read >= send_buf->size())
            {
                printf("message from server: %s\n", read_buf->c_str());
                break;
            }
        }
        read_buf->clear();
        
    }
    
    return 0;
}