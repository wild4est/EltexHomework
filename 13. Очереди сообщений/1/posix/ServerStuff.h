#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

#define QUEUE_FCTS "/queue_fcts" //from client to server
#define QUEUE_FSTC "/queue_fstc" //from server to client
#define SIZE_BUFF 10

