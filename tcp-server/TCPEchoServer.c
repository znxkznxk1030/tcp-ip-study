#include <stdio.h>
#include "Practical.h"
#include <unistd.h>

int main(int argc, char *argv[]){

    if (argc != 2)
        DieWithUserMessage("Parameter (s)", "<Server Port/Service>");

    char *service = argv[1];

    int servSock = SetupTCPServerSocket(service);
    if(servSock < 0)
        DieWithUserMessage("SetupTCPServerSocket() failed", service);

    for(;;){
        int clntSock = AcceptTCPConnection(servSock);

        HandleTCPClient(clntSock);
        close(clntSock);
    }
}

