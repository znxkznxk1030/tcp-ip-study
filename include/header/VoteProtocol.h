#ifndef _VOTE_PROTOCOL_
#define _VOTE_PROTOCOL_

#include <stdbool.h>
#include <stdint.h>

struct VoteInfo {
        uint64_t count;
        int candidate;
        bool isInquiry;
        bool isResponse;
};

typedef struct VoteInfo VoteInfo;

enum {
        MAX_CANDIDATE = 1000,
        MAX_WIRE_SIZE = 500
};

#endif
