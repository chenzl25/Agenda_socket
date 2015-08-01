#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main() {
    ifstream in("UDPclient.cpp");
    ofstream out("UDPclient.tran");
    string tem;
    while(getline(in, tem)) {
        for (int i = 0; i< tem.length(); i++) {
            tem[i] += 10;
        }
        out << tem << endl;
    }
}
