#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_LEN 2048
#define PORT 10023

int main(int argc, char* argv[])
{
    int serv_sock = -1, client_sock = -1;
    socklen_t addr_len = 0;
    struct sockaddr_in6 local_addr = { 0 }, client_addr = { 0 };
    char buf[BUF_LEN] = { 0 };
    int err = -1;

    /* ����socket */
    serv_sock = socket(PF_INET6, SOCK_STREAM, 0);
    if (-1 == serv_sock)
    {
        perror("socket error: ");
        return -1;
    }

    /* ����ַ�ṹ */
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(PORT);
    local_addr.sin6_addr = in6addr_any;

    /* �󶨵�ַ */
    err = bind(serv_sock, (struct sockaddr*) & local_addr, sizeof(struct sockaddr_in6));
    if (-1 == err)
    {
        perror("bind error: ");
        close(serv_sock);
        return -1;
    }

    /* ���� */
    err = listen(serv_sock, 5);
    if (-1 == err)
    {
        perror("listen error: ");
        close(serv_sock);
        return -1;
    }

    /* ѭ���ȴ��ͻ��������� */
    while (1)
    {
        memset(&client_addr, 0x0, sizeof(client_addr));
        addr_len = sizeof(struct sockaddr_in6);
        client_sock = accept(serv_sock, (struct sockaddr*) & client_addr, &addr_len);
        if (-1 == client_sock)
        {
            perror("accept error:");
            close(serv_sock);
            return -1;
        }

        /* ת��client��ַΪ�ַ�������ӡ */
        inet_ntop(AF_INET6, &client_addr.sin6_addr, buf, BUF_LEN);
        printf("A clinet connected, ip: %s, port %d\n", buf, ntohs(client_addr.sin6_port));

        /* ������Ϣ */
        memset(buf, 0x0, BUF_LEN);
        err = recv(client_sock, buf, BUF_LEN, 0);
        if (err < 0)
        {
            perror("recv error:");
            close(serv_sock);
            close(client_sock);
            return -1;
        }
        printf("recv %d bytes: %s\n", err, buf);

        /* ������Ϣ */
        err = send(client_sock, buf, strlen(buf), 0);
        if (err < 0)
        {
            perror("send error:");
            close(serv_sock);
            close(client_sock);
            return -1;
        }

        /* �ر����client���� */
        close(client_sock);
    }
    return 0;
}
