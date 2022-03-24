// Implementation of msgQueue class member functions

#include "msgQueue.hpp"
#include <errno.h>

msgQueue::msgQueue(const char *name, int flags, int maxmsg, int msgsize) : qName_{name}
{
    struct mq_attr attr;
    attr.mq_flags = 0; /* Flags (ignored for mq_open()) */
    attr.mq_maxmsg = maxmsg;
    attr.mq_msgsize = msgsize;

    MQ_ = mq_open(qName_, flags, 0664, &attr);

    if (this->MQ_ < 0)
    {
        printf("%s mq_open failed\n", qName_);
        int err = errno;
        printf("%i", err); exit(1);
    }
}

msgQueue::~msgQueue()
{
    if (mq_close(MQ_) < 0)
    {
        printf("%s mq_close failed\n", qName_);
        int err = errno;
        printf("%i", err); exit(1);
    }
    if (mq_unlink(qName_) < 0)
    {
        printf("%s mq_unlink failed\n", qName_);
        int err = errno;
        printf("%i", err); exit(1);
    }
}
