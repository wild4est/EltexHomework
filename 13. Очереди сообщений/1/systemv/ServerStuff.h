#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_QUEUE "/queue"
#define PROJ_ID 15
#define SIZE_BUFF 10

struct Msg_t{
        long mtype;
        char msg[SIZE_BUFF];
};

