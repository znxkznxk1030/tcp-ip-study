#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"

static const int MAXPENDING = 5;

void HandleTCPClient(int clntSocket){
        char buffer[BUFSIZE];

        ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
        if (numBytesRcvd < 0)
            DieWithSystemMessage("recv() failed");

        while (numBytesRcvd > 0){
                ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
                if (numBytesSent < 0)
                    DieWithSystemMessage("send() failed");
                else if (numBytesSent != numBytesRcvd)
                    DieWithUserMessage("send()", "sent unexpected number of bytes");

                numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
                if (numBytesRcvd < 0)
                    DieWithSystemMessage("recv() failed");
        }

        close(clntSocket);

}

int SetupTCPServerSocket(const char *service){
      struct addrinfo addrCriteria;
      memset(&addrCriteria, 0, sizeof(addrCriteria));
      addrCriteria.ai_family = AF_UNSPEC;
      addrCriteria.ai_flags = AI_PASSIVE;
      addrCriteria.ai_socktype = SOCK_STREAM;
      addrCriteria.ai_protocol = IPPROTO_TCP;

      struct addrinfo *servAddr;
      int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
      if (rtnVal != 0)
          DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

      int servSock = -1;
      for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next){
                servSock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);

                if (servSock < 0)
                    continue;

                if ((bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) == 0)
                        && (listen(servSock, MAXPENDING) == 0)){
                        struct sockaddr_storage localAddr;
                        socklen_t addrSize = sizeof(localAddr);
                        if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
                            DieWithSystemMessage("getsockname() failed");
                        fputs("Binding to ", stdout);
                        PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
                        fputc('\n', stdout);
                        break;
                }

                close(servSock);
                servSock = -1;
      }
      freeaddrinfo(servAddr);
        
        return servSock;     
}

int AcceptTCPConnection(int servSock) {
        struct sockaddr_storage clntAddr;
        
        socklen_t clntAddrLen = sizeof(clntAddr);

        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);

        if (clntSock < 0)
            DieWithSystemMessage("accept () failed");

        fputs("Handling client ", stdout);
        PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
        fputc('\n', stdout);

        return clntSock;

}














