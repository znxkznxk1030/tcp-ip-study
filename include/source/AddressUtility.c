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


bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2) {
      if (addr1 == NULL || addr2 == NULL)
              return addr1 == addr2;
      else if (addr1->sa_family != addr2->sa_family)
              return false;
      else if (addr1->sa_family == AF_INET) {
              struct sockaddr_in *ipv4Addr1 = (struct sockaddr_in *) addr1;
              struct sockaddr_in *ipv4Addr2 = (struct sockaddr_in *) addr2;
              return ipv4Addr1->sin_addr.s_addr == ipv4Addr2->sin_addr.s_addr && ipv4Addr1->sin_port == ipv4Addr2->sin_port;
      } else if (addr1->sa_family == AF_INET6) {
              struct sockaddr_in6 *ipv6Addr1 = (struct sockaddr_in6 *) addr1;
              struct sockaddr_in6 *ipv6Addr2 = (struct sockaddr_in6 *) addr2;
              return memcmp(&ipv6Addr1->sin6_addr, &ipv6Addr2->sin6_addr,
              sizeof(struct in6_addr)) == 0 && ipv6Addr1->sin6_port == ipv6Addr2->sin6_port;
      } else
              return false;
}
