#include "DriverManager.h"

struct DriverManager* driver_manager = NULL;

void InitDriver(struct Driver** driver) {
	*driver = malloc(sizeof(struct Driver));
	(*driver)->status = STATUS_FREE;
	InitPairOfPipes(&((*driver)->pair_of_pipes));

	(*driver)->pid = fork();
	if ((*driver)->pid == -1) {
		printf("\e[31m[!]\e[0m Error: fork\n");
		exit(EXIT_FAILURE);
	}

	if ((*driver)->pid == 0) {
		if (ClosePairForChild(&((*driver)->pair_of_pipes)) == -1) {
			printf("\e[31m[!]\e[0m Error: ClosePairForChild\n");
			exit(EXIT_FAILURE);
		}
		InitSignalProcessing();

		while (1) {
			char msg[SIZE_BUFF];
			int recv_getmsg = GetMsg(
			    (*driver)->pair_of_pipes.pipe_from_parent, msg);
			if (recv_getmsg == -1) {
				printf("\e[31m[!]\e[0m Error: InitDriver\n");
				exit(EXIT_FAILURE);
			} else if (recv_getmsg == 1) {
				continue;
			}

			if (strcmp(msg, EXIT_MSG) == 0) {
				exit(EXIT_SUCCESS);
			} else if (strcmp(msg, SENDTASK_MSG) == 0) {
				SendStatus(*driver);
				if ((*driver)->status == STATUS_BUSY) {
					continue;
				}

				(*driver)->status = STATUS_BUSY;

				char task_timer_str[SIZE_BUFF];
				int recv_getmsg2 = GetMsg(
				    (*driver)->pair_of_pipes.pipe_from_parent,
				    task_timer_str);
				if (recv_getmsg2 == -1) {
					exit(EXIT_FAILURE);
				}

				int task_timer_int = atoi(task_timer_str);

				alarm(task_timer_int);
			} else if (strcmp(msg, GETSTATUS_MSG) == 0) {
				SendStatus(*driver);
			}
		}

	} else {
		if (ClosePairForParent(&((*driver)->pair_of_pipes)) == -1) {
			printf("\e[31m[!]\e[0m Error: ClosePairForParent\n");
			exit(EXIT_FAILURE);
		}
	}
}

void AlarmHandler() {
	driver_manager->drivers[driver_manager->length]->status = STATUS_FREE;
}

void InitSignalProcessing() {
	struct sigaction act;
	act.sa_handler = AlarmHandler;
	sigaction(SIGALRM, &act, NULL);
}

void SendStatus(struct Driver* driver) {
	int recv_sendmsg;
	if (driver->status == STATUS_BUSY) {
		recv_sendmsg =
		    SendMsg(driver->pair_of_pipes.pipe_from_child, BUSY_MSG);
		if (recv_sendmsg == -1) {
			exit(EXIT_FAILURE);
		}
	} else {
		recv_sendmsg =
		    SendMsg(driver->pair_of_pipes.pipe_from_child, FREE_MSG);
		if (recv_sendmsg == -1) {
			exit(EXIT_FAILURE);
		}
	}
}

void CloseDriverManager() {
	for (int i = 0; i < driver_manager->length; i++) {
		int recv_sendmsg = SendMsg(
		    driver_manager->drivers[i]->pair_of_pipes.pipe_from_parent,
		    EXIT_MSG);
		int status;
		wait(&status);
		pid_t pid_driver = driver_manager->drivers[i]->pid;
		printf("Водитель<%d> завершил работу со статусом %d\n",
		       pid_driver, status);
	}
	FreeDriverManager();
}

void FreeDriverManager() {
	for (int i = 0; i < driver_manager->length; i++) {
		free(driver_manager->drivers[i]);
	}
	free(driver_manager->drivers);
	free(driver_manager);
}

void GetDrivers() {
	if (driver_manager->length == 0) {
		printf("Перечень водителей пуст\n");
	}

	for (int i = 0; i < driver_manager->length; i++) {
		int status = GetStatusById(i);
		int pid = driver_manager->drivers[i]->pid;
		printf("Водитель<%d>: %s\n", pid,
		       ((status == STATUS_BUSY) ? "занят" : "свободен"));
	}
}

void InitDriverManager() {
	driver_manager = malloc(sizeof(struct DriverManager));
	driver_manager->drivers =
	    malloc(sizeof(struct Driver*) * (DRIVER_COUNT + 1));
	driver_manager->drivers[0] = NULL;
	driver_manager->length = 0;
}

int CreateDriver() {
	if (driver_manager->length >= DRIVER_COUNT) {
		return -1;
	}
	InitDriver(&(driver_manager->drivers[driver_manager->length]));
	printf("Водитель<%d> готов к работе!\n",
	       driver_manager->drivers[driver_manager->length]->pid);
	driver_manager->length++;
	driver_manager->drivers[driver_manager->length] = NULL;

	return 0;
}

int BinarySearchByPid(pid_t pid) {
	int L = 0;
	int R = driver_manager->length;
	if (R <= 0) {
		return -1;
	}

	int M;
	while (L < (R - 1)) {
		M = (L + R) / 2;

		if (driver_manager->drivers[M]->pid <= pid) {
			L = M;
		} else if (driver_manager->drivers[M]->pid > pid) {
			R = M;
		}
	}

	if (driver_manager->drivers[L]->pid == pid) {
		return L;
	}

	return -1;
}

int SendTask(pid_t pid, char* task_timer) {
	int id = BinarySearchByPid(pid);
	if (id == -1) {
		printf("Водитель<%d> не найден\n", pid);
		return -1;
	}
	int recv_sendmsg =
	    SendMsg(driver_manager->drivers[id]->pair_of_pipes.pipe_from_parent,
		    SENDTASK_MSG);
	if (recv_sendmsg == -1) {
		int err = errno;
		printf("\e[31m[!]\e[0m Error: SendTask %d\n", err);
		return -1;
	}

	char msg[SIZE_BUFF];
	int recv_getmsg = GetMsg(
	    driver_manager->drivers[id]->pair_of_pipes.pipe_from_child, msg);
	if (recv_getmsg == -1) {
		int err = errno;
		printf("\e[31m[!]\e[0m Error: GetTask %d\n", err);
		return -1;
	}
	if (strcmp(msg, BUSY_MSG) == 0) {
		printf("Водитель<%d> в данный момент времени занят\n", pid);
		return STATUS_BUSY;
	} else {
		printf("Водитель<%d> свободен\n", pid);
	}

	recv_sendmsg =
	    SendMsg(driver_manager->drivers[id]->pair_of_pipes.pipe_from_parent,
		    task_timer);
	if (recv_sendmsg == -1) {
		int err = errno;
		printf("\e[31m[!]\e[0m Error: SendTask %d\n", err);
		return -1;
	}
	printf("Водитель<%d> принял задание (%s)\n", pid, task_timer);
	return STATUS_FREE;
}

int GetStatusById(int id) {
	int recv_sendmsg =
	    SendMsg(driver_manager->drivers[id]->pair_of_pipes.pipe_from_parent,
		    GETSTATUS_MSG);
	if (recv_sendmsg == -1) {
		int err = errno;
		printf("\e[31m[!]\e[0m Error: SendTask %d\n", err);
		return -1;
	}

	char msg[SIZE_BUFF];
	int recv_getmsg = GetMsg(
	    driver_manager->drivers[id]->pair_of_pipes.pipe_from_child, msg);
	if (recv_getmsg == -1) {
		int err = errno;
		printf("\e[31m[!]\e[0m Error: GetTask %d\n", err);
		return -1;
	}
	if (strcmp(msg, BUSY_MSG) == 0) {
		return STATUS_BUSY;
	}
	return STATUS_FREE;
}

int GetStatusByPid(pid_t pid) {
	int id = BinarySearchByPid(pid);
	if (id == -1) {
		printf("Водитель<%d> не найден\n", pid);
		return -1;
	}
	int status = GetStatusById(id);
	if (status == -1) {
		return -1;
	}
	return status;
}
