#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"


int main(int argc, char *argv[]){
    
        if(argc < 3 || argc > 4)
        {
                DieWithUserMessage("Parameter (s)", "<Server Address> <Echo Word> [ <Server Port>]");
        }

        char *server = argv[1];
        char *echoString = argv[2];

        //in_port_t servPort = (argc == 4)? atoi(argv[3]) : 7;
        char *service = (argc == 4) ? argv[3] : "echo";

        int sock = SetupTCPClientSocket(server, service);
        if (sock < 0)
                DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");

        size_t echoStringLen = strlen(echoString);
        
        /*
         *  에코 문자열을 서버로 전송
         */
        ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
        if(numBytes < 0)
                DieWithSystemMessage("send() failed");
        else if (numBytes != echoStringLen)
                DieWithUserMessage("send()", "sent unexpected number of bytes");

        /*
         *  서버로부터 에코된 문자열을 수신
         */
        unsigned int totalBytesRcvd = 0;
        fputs("Received: ", stdout);
        while(totalBytesRcvd < echoStringLen)
        {
            char buffer[BUFSIZE];

            numBytes = recv(sock, buffer, BUFSIZE - 1, 0);

            if (numBytes < 0)
            {
                DieWithSystemMessage("recv() failed");
            }else if (numBytes == 0)
            {
                DieWithUserMessage("revc()", "connection closed prematurely");
            }
            totalBytesRcvd += numBytes;
            buffer[numBytes] = '\0';
            fputs(buffer, stdout);
        }

        fputc('\n', stdout);
        
        close(sock);
        exit(0);
}
