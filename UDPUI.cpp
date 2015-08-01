#include "AgendaUI.h"
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define SERVER_PORT 4000
using std::istringstream;
using std::ostringstream;
using std::stringstream;
using std::string;
using std::list;
using std::cin;
using std::cout;
using std::endl;
using std::left;
using std::right;
using std::setw;
//
char buffer[2048];
void clear_buffer(){
    memset(buffer, 0, sizeof(buffer));
}
string res;
ostringstream os;
istringstream is;
void __read();
void __send();


//
AgendaUI::AgendaUI() {
    userName_ = "NONE";
    userPassword_ = "NONE";
    agendaService_ = AgendaService();
}
void AgendaUI::OperationLoop(void) {
    startAgenda();
}
// task functions
void AgendaUI::startAgenda(void) {
    cout  << endl << "****************" << endl << "startAgenda"  << endl << "***************" << endl;
    agendaService_.startAgenda();
    os << "--------------------------------Agenda--------------------------------" << endl;
    os << "Action :" << endl;
    os << "l   - log in Agenda by user name and password" << endl;
    os << "r   - register an Agenda account" << endl;
    os << "q   - quit Agenda" << endl;
    os << "help- show the operation list" << endl;
    os << "----------------------------------------------------------------------" << endl;
    os << endl;
    os << "Agenda : ~$ ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    start_input();
}
std::string AgendaUI::getOperation() {
    __read();
    string op;
    op = buffer;
    return op;
}
bool AgendaUI::executeOperation(std::string op) {
    if (op == "l") userLogIn();
    if (op == "r") userRegister();
    if (op == "q") quitAgenda();
    if (op == "o") userLogOut();
    if (op == "de") deleteUser();
    if (op == "lu") listAllUsers();
    if (op == "cm") createMeeting();
    if (op == "la") listAllMeetings();
    if (op == "las") listAllSponsorMeetings();
    if (op == "lap") listAllParticipateMeetings();
    if (op == "qm") queryMeetingByTitle();
    if (op == "qt") queryMeetingByTimeInterval();
    if (op == "dm") deleteMeetingByTitle();
    if (op == "da") deleteAllMeetings();
    if (op == "help") help();
    if (op == "help_start") help_start();
}
void AgendaUI::userLogIn(void) {
    cout  << endl << "****************" << endl << "login"  << endl << "***************" << endl;
    list<User> l = agendaService_.listAllUsers();
    if (l.empty()) {
        os << "[error] No User" << endl;
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
        start_input();
        return;
    }
    os << endl;
    os << "[log in] [user name] [password]" << endl;
    os << "[log in] ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send(); __read();
    istringstream ss(is.str());
    ss >> userName_ >> userPassword_;

    // is >> userName_ >> userPassword_;
    cout << is.str() << endl;
    cout << "!!!!!!!!!" << userName_ << "   " << userPassword_ << endl; 
    if (agendaService_.userLogIn(userName_, userPassword_)) {
        os << "[log in] succeed!" << endl;
        os << endl;
        help();
    } else {
        os << "[error] log in fail!" << endl;
        res = os.str();
        strcpy(buffer, res.c_str());
        os.str("");
        userLogIn();
    }
}
void AgendaUI::userRegister(void) {
    cout  << endl << "****************" << endl << "register"  << endl << "***************" << endl;
    os << endl;
    os << "[register] [user name] [password] [email] [phone]" << endl;
    os << "[register] ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send(); __read();
    string name, ps, email, phone;
    is >> name >> ps >> email >> phone;
    if (agendaService_.userRegister(name, ps, email, phone)) {
        os << "[register] succeed!" << endl;
        os << endl;
        os << "Agenda : ~$ ";
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    } else {
        os << "[error] register fail!" << endl;
        os << endl;
        os << "Agenda : ~$ ";
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
        userRegister();
    }
    start_input();
}
void AgendaUI::quitAgenda(void) {
    os << "quit";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    agendaService_.quitAgenda();
}
void AgendaUI::userLogOut(void) {
    cout  << endl << "****************" << endl << "log out"  << endl << "***************" << endl;
    os << endl;
    startAgenda();
}
void AgendaUI::deleteUser(void) {
    cout  << endl << "****************" << endl << "deleteUser"  << endl << "***************" << endl;
    if (agendaService_.deleteUser(userName_, userPassword_)) {
        os << "[delete agenda account] succeed!" << endl;
        os << "--------------------------------Agenda--------------------------------" << endl;
        os << "Action :" << endl;
        os << "l   - log in Agenda by user name and password" << endl;
        os << "r   - register an Agenda account" << endl;
        os << "q   - quit Agenda" << endl;
        os << "help- show the operation list" << endl;
        os << "----------------------------------------------------------------------" << endl;
        os << endl;
        os << "Agenda : ~$ ";
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
        start_input();
    } else {
        os << "[error] delete agenda acount fail!" << endl;
        os << endl;
        os << "Agenda@" << userName_ << " : # ";
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
        input();
    }
}
void AgendaUI::listAllUsers(void) {
    cout  << endl << "****************" << endl << "listAllUsers"  << endl << "***************" << endl;
    std::list<User> l = agendaService_.listAllUsers();
    os << left << setw(25) << "name";
    os << left << setw(25) << "email";
    os << left << setw(25) << "phone" << endl;
    for (list<User>::iterator it = l.begin(); it != l.end(); it++) {
        os << left << setw(25) << it->getName();
        os << left << setw(25) << it->getEmail();
        os << left << setw(25) << it->getPhone() << endl;
    }
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::createMeeting(void) {
    cout  << endl << "****************" << endl << "CreateMeeting"  << endl << "***************" << endl;
    os << "[create meeting] [title] [participator] [start time(yyyy-mm-dd/hh:mm)] [end time(yyy-mm-dd/hh:mm)]" << endl;
    os << "[create meeting] ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send(); __read();
    string title, participator, startDate, endDate;
    istringstream ss(is.str());
    ss >> title >> participator >> startDate >> endDate;

    cout << "!!!!!!!!" <<  title << participator << startDate << endDate << endl;
    if (agendaService_.createMeeting(userName_, title, participator, startDate, endDate)) {
        os << "[create meeting] succeed!" << endl;
        os << endl;
        os << "Agenda@" << userName_ << " : # ";
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
        input();
    } else {
        os << "[error] create meeting fail!" << endl;
        os << endl;
        os << "Agenda@" << userName_ << " : # ";
        res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
        input();
    }
}
void AgendaUI::listAllMeetings(void) {
    cout  << endl << "****************" << endl << "listAllMeeting"  << endl << "***************" << endl;
    os << endl;
    os << "[list all meetings]" << endl;
    os << endl;
    os << left << setw(17) << "title";
    os << left << setw(17) << "sponsor";
    os << left << setw(17) << "participator";
    os << left << setw(17) << "start time";
    os << left << setw(17) << "end time" << endl;
    list<Meeting> l = agendaService_.listAllMeetings(userName_);
    for (list<Meeting>::iterator it = l.begin(); it != l.end(); it++) {
        os << left << setw(17) << it->getTitle();
        os << left << setw(17) << it->getSponsor();
        os << left << setw(17) << it->getParticipator();
        os << left << setw(17) << Date::dateToString(it->getStartDate());
        os << left << setw(17) << Date::dateToString(it->getEndDate()) << endl;
    }
    os << endl;
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::listAllSponsorMeetings(void) {
    cout  << endl << "****************" << endl << "listAllSponsorMeetings"  << endl << "***************" << endl;
    os << endl;
    os << "[list all sponsor meetings]" << endl;
    os << endl;
    os << left << setw(17) << "title";
    os << left << setw(17) << "sponsor";
    os << left << setw(17) << "participator";
    os << left << setw(17) << "start time";
    os << left << setw(17) << "end time" << endl;
    list<Meeting> l = agendaService_.listAllSponsorMeetings(userName_);
    for (list<Meeting>::iterator it = l.begin(); it != l.end(); it++) {
        os << left << setw(17) << it->getTitle();
        os << left << setw(17) << it->getSponsor();
        os << left << setw(17) << it->getParticipator();
        os << left << setw(17) << Date::dateToString(it->getStartDate());
        os << left << setw(17) << Date::dateToString(it->getEndDate()) << endl;
    }
    os << endl;
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::listAllParticipateMeetings(void) {
    cout  << endl << "****************" << endl << "listAllParticipateMeetings"  << endl << "***************" << endl;
    os << endl;
    os << "[list all participator meetings]" << endl;
    os << endl;
    os << left << setw(17) << "title";
    os << left << setw(17) << "sponsor";
    os << left << setw(17) << "participator";
    os << left << setw(17) << "start time";
    os << left << setw(17) << "end time" << endl;
    list<Meeting> l = agendaService_.listAllParticipateMeetings(userName_);
    for (list<Meeting>::iterator it = l.begin(); it != l.end(); it++) {
        os << left << setw(17) << it->getTitle();
        os << left << setw(17) << it->getSponsor();
        os << left << setw(17) << it->getParticipator();
        os << left << setw(17) << Date::dateToString(it->getStartDate());
        os << left << setw(17) << Date::dateToString(it->getEndDate()) << endl;
    }
    os << endl;
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::queryMeetingByTitle(void) {
 cout  << endl << "****************" << endl << "queryMeetingByTitle"  << endl << "***************" << endl;
    os << endl;
    os << "[query meeting] [title]:" << endl;
    os << "[query meeting] ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send(); __read();
    string title;
    istringstream ss(is.str());
    // is >> title;
    ss >> title;
    cout << "!!!!!!!!" << title << endl;
    os << endl;
    os << left << setw(17) << "sponsor";
    os << left << setw(17) << "participator";
    os << left << setw(17) << "start time";
    os << left << setw(17) << "end time" << endl;
    list<Meeting> l = agendaService_.meetingQuery(userName_, title);
    for (list<Meeting>::iterator it = l.begin(); it != l.end(); it++) {
        os << left << setw(17) << it->getSponsor();
        os << left << setw(17) << it->getParticipator();
        os << left << setw(17) << Date::dateToString(it->getStartDate());
        os << left << setw(17) << Date::dateToString(it->getEndDate()) << endl;
    }
    os << endl;
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::queryMeetingByTimeInterval(void) {
 cout  << endl << "****************" << endl << "queryMeetingByTimeInterval"  << endl << "***************" << endl;
    os << endl;
    os << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    os << "[query meeting] ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send(),  __read();;
    std::string startDate, endDate;
    // is >> startDate >> endDate;
    istringstream ss(is.str());
    ss >> startDate >> endDate;
    cout << "!!!!!" << startDate << "   " << endDate << endl;
    os << endl;
    os << left << setw(17) << "title";
    os << left << setw(17) << "sponsor";
    os << left << setw(17) << "participator";
    os << left << setw(17) << "start time";
    os << left << setw(17) << "end time" << endl;
    list<Meeting> l = agendaService_.meetingQuery(userName_, startDate, endDate);
    for (list<Meeting>::iterator it = l.begin(); it != l.end(); it++) {
        os << left << setw(17) << it->getTitle();
        os << left << setw(17) << it->getSponsor();
        os << left << setw(17) << it->getParticipator();
        os << left << setw(17) << Date::dateToString(it->getStartDate());
        os << left << setw(17) << Date::dateToString(it->getEndDate()) << endl;
    }
    os << endl;
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::deleteMeetingByTitle(void) {
cout  << endl << "****************" << endl << "deleteMeetingByTitle"  << endl << "***************" << endl;
    os << "[delete meeting] [title]" << endl;
    os << "[delete meeting] ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    string title;
    is >> title; __read();
    if(agendaService_.deleteMeeting(userName_, title)) {
        os << "[delete meeting by title] succeed!" << endl;
    } else {
        os << "{error} delete meeting fail!" << endl;
    }
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    input();
}
void AgendaUI::deleteAllMeetings(void) {
cout  << endl << "****************" << endl << "deleteAllMeetings"  << endl << "***************" << endl;
    if (agendaService_.deleteAllMeetings(userName_)) {
        os << "[delete all meetings] succeed!" << endl;
    } else {
        os << "[error] fail!" << endl;
    }
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    input();
}
void AgendaUI::printMeetings(std::list<Meeting> meetings) {

}
void AgendaUI::help_start() {
cout  << endl << "****************" << endl << "start_help"  << endl << "***************" << endl;
    os << endl;
    os << "--------------------------------Agenda--------------------------------" << endl;
    os << "Action :" << endl;
    os << "l   - log in Agenda by user name and password" << endl;
    os << "r   - register an Agenda account" << endl;
    os << "q   - quit Agenda" << endl;
    os << "help- show the operation list" << endl;
    os << "----------------------------------------------------------------------" << endl;
    os << endl;
    os << "Agenda : ~$ ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    start_input();
}
void AgendaUI::help() {
cout  << endl << "****************" << endl << "help"  << endl << "***************" << endl;
    os << endl;
    os << "--------------------------------Agenda--------------------------------" << endl;
    os << "Action :" << endl;
    os << "o   - log out Agenda" << endl;
    os << "de  - delete Agenda account" << endl;
    os << "lu  - list all Agenda user" << endl;
    os << "cm  - create a meeting" << endl;
    os << "la  - list all meetings" << endl;
    os << "las - list all sponspor meetings" << endl;
    os << "lap - list all participator meetings" << endl;
    os << "qm  - query meeting by title" << endl;
    os << "qt  - query meeting by time interval" << endl;
    os << "dm  - delete meeting by title" << endl;
    os << "da  - delete all meetings" << endl;
    os << "help- show the operation list" << endl;
    os << "----------------------------------------------------------------------" << endl;
    os << endl;
    os << "Agenda@" << userName_ << " : # ";
    res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    string op = getOperation();
    while(1) {
        if (op == "o" || op == "de" || op == "lu" || op == "cm" || op == "la" || op == "las" || op == "lap" || op == "qm" || op == "qt" || op == "dm" || op == "da" || op == "help") {
            executeOperation(op);
            break;
        } else {
            os << "your enter is wrong please enter again" << endl;
            os << endl;
            os << "Agenda@" << userName_ << " : # ";
            res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
            input();
            break;
        }
    }
}
void AgendaUI::input() {
cout  << endl << "****************" << endl << "input"  << endl << "***************" << endl;
    // os << endl;
    // os << "Agenda@" << userName_ << " : # ";
    // res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    string op = getOperation();
    while(1) {
        if (op == "o" || op == "de" || op == "lu" || op == "cm" || op == "la" || op == "las" || op == "lap" || op == "qm" || op == "qt" || op == "dm" || op == "da" || op == "help") {
            executeOperation(op);
            break;
        } else {
            os << "your enter is wrong please enter again" << endl;
            os << endl;
            os << "Agenda@" << userName_ << " : # ";
            res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
            input();
            break;
        }
    }
}
void AgendaUI::start_input() {
cout  << endl << "****************" << endl << "start_input"  << endl << "***************" << endl;
    // os << endl;
    // os << "Agenda : ~$ ";
    // res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
    while(1) {
        string op = getOperation();
        if(op == "l" || op == "r" || op == "q" || op == "help") {
            if (op == "help") {
                executeOperation("help_start");
            } else {
                executeOperation(op);
            }
            break;
        } else {
            os << "your enter is wrong please enter again" << endl;
            os << endl;
            os << "Agenda : ~$ ";
            res = os.str(); strcpy(buffer, res.c_str()); os.str(""); __send();
            start_input();
            break;
        }
    }
}
AgendaUI aui;
int sock;
//sendto中使用的对方地址
struct sockaddr_in toAddr;
//在recvfrom中使用的对方主机地址
struct sockaddr_in fromAddr;
int recvLen;
unsigned int addrLen;
char recvBuffer[128];


int s;
int len;

struct sockaddr_in servAddr;
struct sockaddr_in clntAddr;
// socklen_t clntAddrLen;
unsigned int clntAddrLen;

void ___read() {
    if((recvLen = recvfrom(sock,recvBuffer,128,0,(struct sockaddr*)&toAddr,&addrLen))<0)
    {
        printf("()recvfrom()函数使用失败了.\r\n");
        close(sock);
        exit(1);
    }
    printf("__read from %s with string %s\n", inet_ntoa(toAddr.sin_addr), recvBuffer);
    cout << recvBuffer << endl;
}

void ___send() {
    string ot;
    cin >> ot;
    sendto(sock, ot.c_str(), strlen(ot.c_str()), 0,(struct sockaddr*)&toAddr,sizeof(toAddr));
}

void __read() {
//    clntAddr.sin_family = AF_INET;
//    clntAddr.sin_port = htons(6666);
//    inet_pton(AF_INET, "20.1.0.60", &clntAddr.sin_addr);
    addrLen = sizeof(toAddr);
    clear_buffer();
    is.str("");
    cout << "__reading ...." << endl;
    if ((len = recvfrom(s, buffer, 2048, 0, (struct sockaddr*)&toAddr,&addrLen))<0) {
        cout << "shit" << endl;
    }
    printf("__read from  %s with the string:\n%s\n",inet_ntoa(toAddr.sin_addr), buffer);
    cout << "__read finished" << endl;
    cout << "the client enter : " << buffer << endl;
    string tem = buffer;
    cout << "the tem is " << tem << endl; 
    is.str(tem);
}
void __send() {
//    clntAddr.sin_family = AF_INET;
//    clntAddr.sin_port = htons(6666);
//    inet_pton(AF_INET, "20.1.0.60", &clntAddr.sin_addr);
    cout << "__sending..." << endl;
    sendto(s, buffer, sizeof(buffer) , 0, (struct sockaddr*)&toAddr,sizeof(toAddr));
    printf("__send to %s with the string:\n%s\n",inet_ntoa(toAddr.sin_addr), buffer);
    cout << "__send finished" << endl;
}

int main(int argc, char *argv[])
{

    sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock < 0)
    {
        printf("创建套接字失败了.\r\n");
        exit(0);
    }
    memset(&fromAddr,0,sizeof(fromAddr));
    fromAddr.sin_family=AF_INET;
    fromAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    fromAddr.sin_port = htons(4000);


    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error:socket failed!");
        exit(1);
    }
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(4000);

    if (bind(s, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Error: bind failed!");
        exit(1);
    }


    // if(bind(sock,(struct sockaddr*)&fromAddr,sizeof(fromAddr))<0)
    // {
    //     printf("bind() 函数使用失败了.\r\n");
    //     close(sock);
    //     exit(1);
    // }

    // while(1){
    //     addrLen = sizeof(toAddr);
    //     __read();
    //     __send();
    // }

    cout << "----------------------------------------------------------------------" << endl;

  //   clntAddr.sin_family = AF_INET;
  //   clntAddr.sin_port = htons(4000);
  //   inet_pton(AF_INET, "172.18.182.152", &clntAddr.sin_addr);

//    int on=1;
//    if((setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
//    {
//        perror("setsockopt failed");
//        exit(EXIT_FAILURE);
//    }


    while(1) {

        __read();
//         char tmp[] = "asdfasdf";
//         sendto(s, tmp, sizeof(tmp), 0, (struct sockaddr*)&clntAddr, clntAddrLen);
        string op = buffer;
        cout << op << endl;
        if (op == "root") {

            aui.OperationLoop();
        }
        cout << "the client has quited." << endl;
    }
}
