#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../ServerStuff.h"
#include "List.h"

#define MAX_COUNT_CLIENTS 5
#define MAX_COUNT_MSGS 10

/*!
 * \brief Стуктура сервера
 * \param struct ListClient* list_clients - список клиентов
 * \param struct ListMsg* list_msg - список сообщений
 * \param struct SharedMemory shared_mem_new_client - структура разделяеой
 * памяти для принятия запросов на принятие в чат
 * \param struct SharedMemory shared_mem_messaging - структура разделяемой
 * памяти для принятия сообщений в чате
 * \param struct SharedMemory shared_mem_exit - структура разделяемой
 * памяти для принятия запросов от клиентов, которые желают покинуть чат
 */
struct Server {
	struct ListClient* list_clients;
	struct ListMsg* list_msgs;

	struct SharedMemory shared_mem_new_client;
	struct SharedMemory shared_mem_messaging;
	struct SharedMemory shared_mem_exit;
};

/*!
 * \breif Функция инициализации разделяемой памяти
 * \param int* shm_fd - указатель на файловый дескриптор разделяемой памяти
 * \param voif** ptr - указатель на область разделяемой памяти
 * \param const char* name - название разделяемой памяти
 * \param size_t size - размер разделяемой памяти
 */
int InitShm(int* shm_fd, void** ptr, const char* name, size_t size);

/*!
 * \breif Функция инициализации сервера
 * \return Указатель на структуру сервера
 */
struct Server* InitServer();

/*!
 * \brief Функция создания индивидуальной разделяемой памяти для клиента
 * \param char* name - имя клиента
 * \param struct SharedMemory* shared_mem - указатель на структуру разделяемой
 * памяти
 */
int OpenSharedMemoryToClient(char* name, struct SharedMemory* shared_mem);

/*!
 * \brief Функция отправки сообщения всем клиентам
 * \param struct ListClient* list - список клиентов
 * \param char* answer - сообщение, которое необходимо отправить
 */
void SendMsgToAllClients(struct ListClient* list, char* answer);

/*!
 * \brief Функция отправки сообщения одному клиенту
 * \param struct SharedMemory* shared_mem - разделяемая память клиента, по
 * которой будет отправлено сообщение \param char* answer - сообщение
 */
void SendMsgToClient(struct SharedMemory* shared_mem, char* answer);

/*!
 * \brief Функция, прослушивающая новых клиентов. Используется во время создания
 * соответствующего потока. \param void* args - указатель на структуру сервера
 * \detail Функция ждёт, пока новый пользователь не сообщит своё имя серверу.
 * В слечае, если пользователь может быть принят на сервер пользователю
 * отправляется ответ о том, что он принят и все данные о пользователях и
 * сообщениях. Так же в таком случае сервер оповещает всех пользователей о новом
 * учатснике. В ином случае пользователю отправляется ответ, что он был
 * отклонён.
 */
void* NewClientListener(void* args);

/*!
 * \brief Функция, прослушиващая сообщения. Используется во время создания
 * соотвествующего потока.
 * \param void* args - указатель на структуру сервера
 * \detail Функция ждёт, пока кто-то из пользователей не пришлёт сообщение.
 * Затем сервер рассылает данное сообщение всем пользователям, включая того
 * пользователя, который отправил сообщение.
 */
void* MessagingListener(void* args);

/*!
 * \brief Функция, прослушивающая запросы на выход. Используется во время
 * создания соотвествующего потока.
 * \param void* args - указатель на структуру сервера
 * \detail Функция ждёт, пока кто-то из пользователей не пришлёт своё
 * имя. Затем сервер удаляет данного пользователя из списка клиентов и сообщает
 * всем оставшимся пользователям, что данный пользователь покинул чат.
 */
void* ExitListener(void* args);
