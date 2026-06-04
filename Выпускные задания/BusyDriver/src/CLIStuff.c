#include "CLIStuff.h"

void DeleteEnter(char* str) {
	for (int i = 0; i < GETSTR_SIZE_BUFF; i++) {
		if (str[i] == '\n') {
			str[i] = '\0';
			return;
		}
	}
}

void Split(char*** words, int* count, char* str, const char* restrict sym) {
	DeleteEnter(str);

	char* token = strtok(str, sym);
	int i = 0;
	while (token != NULL) {
		if (i >= WORDS_COUNT) {
			break;
		}
		(*words)[i] = malloc(sizeof(char) * GETSTR_SIZE_BUFF);
		strcpy((*words)[i], token);
		i++;
		token = strtok(NULL, sym);
	}
	(*words)[i] = NULL;
	*count = i;
}

void PrintCommands() {
	printf(" \e[1;%dmcreate_driver\e[0m - создать новый процесс\n",
	       COL_COM);
	printf(
	    " \e[1;%dmsend_task \e[%dm<pid> <task_timer>\e[0m - создать задачу "
	    "для водителя с номером"
	    " \e[1;%dm<pid>\e[0m, чтобы занять его на "
	    "\e[1;%dm<task_timer>\e[0m секунд\n",
	    COL_COM, COL_VAL, COL_VAL, COL_VAL);
	printf(
	    " \e[1;%dmget_status \e[%dm<pid>\e[0m - показывает статус водителя "
	    "с номером \e[1;%dm<pid>\e[0m\n",
	    COL_COM, COL_VAL, COL_VAL);
	printf(
	    " \e[1;%dmget_drivers\e[0m - показывает \e[1;%dm<pid>\e[0m и "
	    "статус всех водителей\n",
	    COL_COM, COL_VAL);
	printf(" \e[1;%dmhelp\e[0m - вывести все имеющиеся команды\n", COL_COM);
	printf(" \e[1;%dmexit\e[0m - выйти\n", COL_COM);
}

void PrintCar() {
	printf(
	    "\e[1;31m___________________________________\e[0m     "
	    "__________\n");
	printf(
	    "\e[1;33m___________________________________\e[0m\\___/"
	    "_____|____\\___\n");
	printf(
	    "\e[1;32m___________________________________\e[0m|         |       "
	    " \\\n");
	printf(
	    "\e[1;34m___________________________________\e[0m|_/О|__________/"
	    "О|_|\n");
}

void PrintGreeting() {
	printf("\n");

	PrintCar();

	printf("\n	Добро пожаловать в \e[1;%dmBusyDriver\e[0m!\n", COL_BD);
	printf(
	    "Для того, чтобы начать ввидите одну из нижеперечисленных "
	    "комманд:\n\n");
	PrintCommands();
	printf("\n");
}

int WordIsNumberCheck(char* str, int len) {
	for (int i = 0; i < len; i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}
	return 1;
}

void MainCLI() {
	InitDriverManager();
	PrintGreeting();
	char** words = malloc(sizeof(char*) * (WORDS_COUNT + 1));
	// struct Driver* driver;

	while (1) {
		char get_str[GETSTR_SIZE_BUFF];
		printf("\e[1;%dm>>\e[0m ", COL_VAL);
		fgets(get_str, GETSTR_SIZE_BUFF, stdin);
		int count_words = 0;
		Split(&words, &count_words, get_str, " ");

		if (words[0] == NULL) {
			continue;
		}
		if (strcmp(words[0], "create_driver") == 0) {
			if (CreateDriver() == -1) {
				printf("Вы привысили лимит водителей\n");
			}
		} else if (strcmp(words[0], "send_task") == 0) {
			if (count_words != 3) {
				printf("Неверное количество аргументов\n");
				printf(
				    "Введите комманду \e[1;%dmhelp\e[0m для "
				    "дополнительной информации\n\n",
				    COL_COM);
				continue;
			}
			if (!WordIsNumberCheck(words[1], strlen(words[1])) ||
			    !WordIsNumberCheck(words[2], strlen(words[2]))) {
				printf(
				    "Один из аргументов не является "
				    "числом\n");
				printf(
				    "Введите комманду "
				    "\e[1;%dmhelp\e[0m для "
				    "дополнительной информации\n\n",
				    COL_COM);
				continue;
			}

			int pid = atoi(words[1]);
			SendTask(pid, words[2]);

		} else if (strcmp(words[0], "get_status") == 0) {
			if (count_words != 2) {
				printf("Неверное количество аргументов\n");
				printf(
				    "Введите комманду \e[1;%dmhelp\e[0m для "
				    "дополнительной информации\n\n",
				    COL_COM);
				continue;
			}
			if (!WordIsNumberCheck(words[1], strlen(words[1]))) {
				printf("Аргумент не является числом\n");
				printf(
				    "Введите комманду \e[1;%dmhelp\e[0m для "
				    "дополнительной информации\n\n",
				    COL_COM);
				continue;
			}
			int pid = atoi(words[1]);
			int status = GetStatusByPid(pid);
			if (status != -1) {
				printf("Водитель<%d>: %s\n", pid,
				       ((status == STATUS_BUSY) ? "занят"
								: "свободен"));
			}
		} else if (strcmp(words[0], "get_drivers") == 0) {
			GetDrivers();
		} else if (strcmp(words[0], "help") == 0) {
			PrintCommands();
		} else if (strcmp(words[0], "exit") == 0) {
			CloseDriverManager();
			break;
		}
		printf("\n");
	}

	free(words);
}

