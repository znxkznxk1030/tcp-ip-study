#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int SetupTCPClientSocket(const char *host, const char *service) {
        struct addrinfo addrCriteria;
        memset(&addrCriteria, 0, sizeof(addrCriteria));
        addrCriteria.ai_family = AF_UNSPEC;
        addrCriteria.ai_socktype = SOCK_STREAM;
        addrCriteria.ai_protocol = IPPROTO_TCP;

        struct addrinfo *servAddr;
        int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
        if (rtnVal != 0)
            DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

        int sock = -1;
        for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
                sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
                if (sock < 0)
                    continue;

                if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
                    break;

                close(sock);
                sock = -1;
        }

        freeaddrinfo(servAddr);
        return sock;
}
