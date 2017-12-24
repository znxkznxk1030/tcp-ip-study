#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
#include "Framer.h"
#include "VoteEncoding.h"


int main(int argc, char *argv[]){

    if (argc < 4 || argc > 5)
        DieWithUserMessage("Parameter (s)", "<Server> <Port/Service> <Candidate> [I]");

    char *server = argv[1];
    char *service = argv[2];

    int candi = atoi(argv[3]);
    if (candi < 0 || candi > MAX_CANDIDATE)
        DieWithUserMessage("Candidate # not valid", argv[3]);

    bool inq = argc > 4 && strcmp(argv[4], "I") == 0;

    int sock = SetupTCPClientSocket(server, service);
    if (sock < 0)
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");

    FILE *str = fdopen(sock, "r+");
    if (str == NULL)
        DieWithSystemMessage("fdopen(0 failed");

    VoteInfo vi;
    memset(&vi, 0, sizeof(vi));

    vi.isInquiry = inq;
    vi.candidate = candi;

    uint8_t outbuf[MAX_WIRE_SIZE];
    size_t reqSize = Encode(&vi, outbuf, MAX_WIRE_SIZE);

    printf("Sending %zu-byte %s for candidate %d...\n", reqSize, (inq ? "inquiry" : "vote"), candi);

    if (PutMsg(outbuf, reqSize, str) < 0)
        DieWithSystemMessage("PutMsg() failed");

    uint8_t inbuf[MAX_WIRE_SIZE];
    size_t respSize = GetNextMsg(str, inbuf, MAX_WIRE_SIZE);
    
    if (Decode(inbuf, respSize, &vi)) {
        printf("Received:\n");

        if (vi.isResponse)
            printf(" Response to ");
        if (vi.isInquiry)
            printf("inquiry ");
        else
            printf("vote ");

        printf("for candidate %d\n", vi.candidate);
        if (vi.isResponse)
            printf(" count = %llu\n", vi.count);
        
    }

    fclose(str);

    exit(0);
}
