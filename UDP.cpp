#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<iostream>
#include <iomanip>
#include <sstream>
#define SERVER_PORT 6666
using namespace std;
    int s;
    int len;
    char buffer[1024];
    ostringstream os;
    istringstream is;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    socklen_t clntAddrLen;
int read() {
    return recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr*)&clntAddr, &clntAddrLen);
}
int send() {
    sendto(s, buffer, sizeof(buffer) , 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr));
}
int main() {

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6666);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error:socket failed!");
        exit(1);
    }
    if (bind(s, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Error: bind failed!");
        exit(1);
    }
    while(1) {
        len = read();
        printf("%s\n", buffer);
        string op = buffer;
    };
}
