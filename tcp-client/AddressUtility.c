#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

void PrintSocketAddress(const struct sockaddr *address, FILE *stream){
        if (address == NULL || stream == NULL)
            return;

        void *numbericAddress; //Pointer to binary address
        char addrBuffer[INET6_ADDRSTRLEN];
        in_port_t port;

        switch (address->sa_family){
                case AF_INET:
                    numbericAddress = &((struct sockaddr_in *) address)->sin_addr;
                    port = ntohs(((struct sockaddr_in *) address)->sin_port);
                    break;

                case AF_INET6:
                    numbericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
                    port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
                    break;
                    
                default :
                    fputs("[unknown type]", stream);
                    return ;
        }

        if (inet_ntop(address->sa_family, numbericAddress, addrBuffer, sizeof(addrBuffer)) == NULL){
          fputs("[invaild address]", stream);      
        } else {
          fprintf(stream, "%s", addrBuffer);
          if (port != 0){
                fprintf(stream, "-%u", port);
          }
        }
}
