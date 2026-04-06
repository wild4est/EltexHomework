#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

#define QUEUE_NEW_CLIENT "/queue_new_client"
#define QUEUE_MSG "/queue_msg"
#define QUEUE_BROADCAST "/queue_broadcast"

#define SIZE_NAME_BUFF 30
#define SIZE_MSG_BUFF 256
//#define SIZE_BUFF 10
//#define SIZE_NAME 10
