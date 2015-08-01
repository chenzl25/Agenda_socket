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
#define SERVER_PORT 4000
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
int s;
int len;
char buffer[2048];
ostringstream os;
istringstream is;
struct sockaddr_in servAddr;
string op;
int read() {
    return recvfrom(s, buffer, 2048, 0, NULL, NULL);
}
int send() {
    sendto(s, buffer, strlen(buffer) , 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
}
int main() {

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(4000);
    inet_pton(AF_INET, "172.18.68.129", &servAddr.sin_addr);
    if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error:socket failed!");
        exit(1);
    }
    while(1) {
        getline(cin, op);
        strcpy(buffer, op.c_str());
        send();
        read();
        fputs(buffer, stdout);
    };
    close(s);
}
