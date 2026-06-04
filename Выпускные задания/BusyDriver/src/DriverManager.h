#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "PairOfPipes.h"

#define SIZE_BUFF 10
#define DRIVER_COUNT 10

#define EXIT_MSG "exit"
#define SENDTASK_MSG "sendtask"
#define GETSTATUS_MSG "getstatus"
#define BUSY_MSG "busy"
#define FREE_MSG "free"

#define STATUS_BUSY 0
#define STATUS_FREE 1

struct Driver {
	pid_t pid;
	int status;
	struct PairOfPipes pair_of_pipes;
};

struct DriverManager {
	struct Driver** drivers;
	int length;
};

void InitDriver(struct Driver** driver);
void AlarmHandler();
void InitSignalProcessing();
void InitDriverManager();
int CreateDriver();
void SendStatus(struct Driver* driver);
void CloseDriverManager();
void FreeDriverManager();
int BinarySearchByPid(pid_t pid);
int SendTask(pid_t pid, char* task_timer);
int GetStatusByPid(pid_t pid);
int GetStatusById(int id);
void GetDrivers();
