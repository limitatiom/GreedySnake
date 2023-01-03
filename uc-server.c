//
// Created by 21050 on 2023/1/3.
//
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_64.lib")
int main(){
    const char* NAME = NULL;//IP (hostname)//主机名
    const char* SERVICER = "8080";//端口号
    const struct addrinfo hints = {
        .ai_family = AF_INET,//只关心ipv4地址，ipv6地址：AF_INET6
        .ai_socktype = SOCK_STREAM,//TCP(not UDP)
        .ai_flags = AI_PASSIVE,//说明，主机要被动地接受,使用0.0.0.0ip地址，该ip地址只用于绑定
    }; //设定对于ip地址的条件,(不初始化其他成员，默认设置为0)
    struct addrinfo *bind_addr;//被绑定的ip地址，在getaddrinfo的函数作返回值
    if(getaddrinfo(NAME,SERVICER,&hints,&bind_addr)){//返回ip地址链表的头指针
        fprintf(stderr,"getaddrinfo() failed\n");
        exit(EXIT_FAILURE);
    }
    //创建socket
    //服务器和客户端之间的连接可以当作一个文件来抽象理解，socket_listen:相当于一个文件的标识符
    int socket_listen = socket(bind_addr->ai_family,
                               bind_addr->ai_socktype,
                               bind_addr->ai_protocol);
    if(socket_listen == -1){
        fprintf(stderr,"socket() failed/n");
        exit(EXIT_FAILURE);
    }
    if(bind(socket_listen,bind_addr->ai_addr,bind_addr->ai_addrlen) == -1){//分别是名称和大小
        fprintf(stderr,"socket() failed/n");
        exit(EXIT_FAILURE);
    }
    //listen:
    if(listen(socket_listen,32) != 0) {//前面的表示监听对象，后面的表示可以等待的进程数
        fprintf(stderr,"listen() failed\n");
        exit(EXIT_FAILURE);
    }
    //accept:直到有新的请求：
    //TODO: the last two parameters
    // accept : 成功 ： fd >= 0 失败 -1
    int socket_client = accept(socket_listen,NULL,NULL);
    if(socket_client == -1){
        fprintf(stderr,"accept() failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connection Established\n");
    return EXIT_SUCCESS;
}