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


/*!
 * \brief Структура водителя (дочернего процесса)
 * \param pid_t pid - pid процесса
 * \param int status - состояние водителя, может равняться STATUS_BUSY (занят) или STATUS_FREE (свободен)
 * \param struct PairOfPipes pair_of_pipes - пара каналов для комуникации между водителем и менеджером
 */
struct Driver {
	pid_t pid;
	int status;
	struct PairOfPipes pair_of_pipes;
};

/*!
 * \brief Структура менеджера водителей (главного/родительского процесса)
 * \param struct Driver** drivers - массив указателей на водителей
 * \param int length - колличество водителей
 */
struct DriverManager {
	struct Driver** drivers;
	int length;
};

/*!
 * \brief Функция, инициализирующая водителя и пораждающая дочерний процесс водителя
 * \param struct Driver** driver - Двойной указатель на водителя, куда будут инициализироваться данные
 * \detail После пораждения, водителю (дочернему процессу) через InitSignalProcessing присваивается обрабочик сигналов,
 * который ждёт сигнал SIGALRM. После в бесконечном цикле процесс ожидает, когда менеджер (главный поток) пришлёт
 * ему сообщение. В соотвествии с полученным сообщением, водитель выполняет одну из соотвествующих команд:
 *   - EXIT_MSG - выход. Водитель заканчивает работу.
 *   - SENDTASK_MSG - поступило задание. После получения водитель сообщает менеджеру свой статус. Если статус STATUS_BUSY,
 *     дочерний процесс на текущем шаге больше ничего не делает. Если статус STATUS_FREE, дочерний процесс меняет статус
 *     на STATUS_BUSY, ожидает от менеджера время и выполняет alarm() с указаным временем. После того, как водитель
 *     поймает SIGALRM, снова поменяет своё состояние на STATUS_FREE.
 *   - GETSTATUS_MSG - запрос статуса. Водитель возвращает свой статус. 
 */
void InitDriver(struct Driver** driver);

/*!
 * \brief Функция-обработчик, вызывающаяся во время отслеживания сигнала SIGALRM
 * \detail Меняет статус водятеля под номером driver_manager->length на STATUS_FREE.
 * Так как переменная driver_manager->length увеличивается после инициализации водителя в функции CreateDriver (то есть
 * после того, как был вызван fork() и задано поведение для дочернего процесса), то это означает, что для дочернего
 * процесса данная переменная не изменилась. Следовательно, на момент вызова данного обработчика в дочернем процессе
 * переменная driver_manager->length будет отражать текущий id водителя в массиве.
 */
void AlarmHandler();

/*!
 * \brief Функция, инициализирующая обратку сигнала SIGALRM.
 */
void InitSignalProcessing();

/*!
 * \brief Функция инициализации менеджера водителей
 * \detail Менеджер водителей является глобальной переменной, которая создаётся с момента запуска программы и
 * инициализируется единожды.
 */
void InitDriverManager();

/*!
 * \brief Функция создания водителя
 * \detail Если не был привышет лимит водителей (DRIVER_COUNT), инициализирует последний элемент массива с помощью
 * функции InitDriver()
 */
int CreateDriver();

/*!
 * \brief Функция, отвечающая за отправку статуса водителя менеджеру
 * \param struct Driver* driver - водитель, чей статус будет посылаться
 */
void SendStatus(struct Driver* driver);

/*!
 * \brief Функция, отвечающая за завершение работы менеджера
 * \detail Перед удалением менеджер пошлёт всем водителям сообщение EXIT_MSG. После вызывает FreeDriverManager.
 */
void CloseDriverManager();

/*!
 * \brief Функция удаления массива водителей и менеджера 
 */
void FreeDriverManager();

/*!
 * \brief Функция, выполняющая бинарный поиск водителя по его pid
 * \param pid_t pid - pid водителя
 * \return В случае успеха возвращается id элемента массива, у которого есть соотвествующий pid.
 * В случае провала вернётся -1.
 */
int BinarySearchByPid(pid_t pid);

/*!
 * \brief Функция, отвечающая за отправку сообщений в случае выполнения команды send_task
 * \param pid_t pid - pid водителя, которому будет отправлено задание
 * \param char* task_timer - время, на которое водитель будет занят
 * \return В случае если водитель не был найден или на каком-то этапе возникла ошибка, возвращается -1.
 * В ином случае вернётся статус водителя.
 */
int SendTask(pid_t pid, char* task_timer);

/*!
 * \brief Функция, запрашивающая статус у водителя по pid
 * \param pid_t pid - pid водителя, по которому его будут искать
 * \return  В случае если водитель не был найден или на каком-то этапе возникла ошибка, возвращается -1.
 * В ином случае вернётся статус водителя.
 */
int GetStatusByPid(pid_t pid);

/*!
 * \brief Функция, запрашивающая статус у водителя по его id в массиве
 * \param int id - id водителя, по которому его будут искать
 * \return  В случае если водитель не был найден или на каком-то этапе возникла ошибка, возвращается -1.
 * В ином случае вернётся статус водителя.
 */
int GetStatusById(int id);

/*!
 * \brief Функция, запрашивающая статус всех водителей по очереди
 */
void GetDrivers();
