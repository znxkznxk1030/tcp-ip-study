#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]){
        if (argc != 2)
            DieWithUserMessage("Parameter(s)", "<Server Port/Service>");

        char *service = argv[1];

        struct addrinfo addrCriteria;
        memset(&addrCriteria, 0, sizeof(addrCriteria));
        addrCriteria.ai_family = AF_UNSPEC;
        addrCriteria.ai_flags = AI_PASSIVE;
        addrCriteria.ai_socktype = SOCK_DGRAM;
        addrCriteria.ai_protocol = IPPROTO_UDP;

        struct addrinfo *servAddr;
        int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
        if (rtnVal != 0)
            DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

        int sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
        if (sock < 0)
            DieWithSystemMessage("socket() failed");

        if (bind(sock, servAddr->ai_addr, servAddr->ai_addrlen) < 0)
            DieWithSystemMessage("bind() failed");

        freeaddrinfo(servAddr);

        for(;;){
                struct sockaddr_storage clntAddr;
                socklen_t clntAddrLen = sizeof(clntAddr);

                char buffer[MAXSTRINGLENGTH];

                ssize_t numBytesRcvd = recvfrom(sock, buffer, MAXSTRINGLENGTH, 0, (struct sockaddr *) &clntAddr, &clntAddrLen);

                printf("clntAddrLen : %d\n", clntAddrLen);

                if (numBytesRcvd < 0)
                    DieWithSystemMessage("recvfrom() failed");

                fputs("Handling client ", stdout);
                PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);

                fputc('\n', stdout);
                
                printf("sock: %d\n", sock);
                printf("numBytesRcvd : %zd\n", numBytesRcvd);
                printf("%s\n", buffer);

                printf("%d\n", ((struct sockaddr *) &clntAddr)->sa_family); 
                printf("%s\n", ((struct sockaddr *) &clntAddr)->sa_data);
                printf("%lu\n", sizeof(clntAddr));

                char addr6_str[20];
                inet_ntop(clntAddr.ss_family, (void *) ((struct sockaddr *) &clntAddr), addr6_str, sizeof(addr6_str));

                printf("ipv6_str : %s\n", addr6_str);

                struct sockaddr testAddr;
                testAddr.sa_family = AF_INET6;
                strcpy(testAddr.sa_data, "::1");

                ssize_t numBytesSent = sendto(sock, "123", numBytesRcvd, MSG_EOR,
                        ((struct sockaddr *) &clntAddr), sizeof(clntAddr));

                printf("numBytesSent : %d\n", numBytesSent);

                if (numBytesSent < 0)
                    DieWithSystemMessage("sendto() failed");
                else if (numBytesSent != numBytesRcvd)
                    DieWithUserMessage("sendto()", "sent unexpected number of bytes");
        }
}
