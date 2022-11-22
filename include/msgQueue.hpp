// This class manages POSIX mqueue creation and destruction

#ifndef MSG_QUEUE
#define MSG_QUEUE

#include <stdio.h>
#include <cstdlib>
#include <mqueue.h>
#include <errno.h>

class msgQueue
{
private:
    mqd_t MQ_;
    const char *qName_;

public:
    msgQueue(const char *, int, int, int);
    const mqd_t &getMQ() { return MQ_; }
    ~msgQueue();
};

#endif