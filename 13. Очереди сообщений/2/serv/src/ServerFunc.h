#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../ServerStuff.h"
#include "List.h"

#define MAX_COUNT_CLIENTS 5
#define MAX_COUNT_MSGS 10

/*!
 * \brief Скрутура сервера
 * \param struct ListClient* list_clients - список клиентов
 * \param struct ListMsg* list_msg - список сообщений
 * \param mqd_t mqd_new_cleints - очередь сообщений для принятия запросов на
 * принятие в чат \param mqd_t mqd_messaging - очередь для принятия сообщений от
 * клиентов \param mqd_t mqd_exit - очередь для принятия запросов от клиентов,
 * которые желают покинуть чат
 */
struct Server {
	struct ListClient* list_clients;
	struct ListMsg* list_msgs;
	mqd_t mqd_new_clients;
	mqd_t mqd_messaging;
	mqd_t mqd_exit;
};

/*!
 * \brief Функция, отвечающая за инициализацию очереди
 * \param mqd_t* mqd - указатель на очередь
 * \param const char* path - название очереди
 * \param long maxmsg - максимальное количество сообщений
 * \param long msgsize - размер сообщений
 * \param int oflag - флаги
 */
int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize,
	      int oflag);

/*!
 * \brief Функция инициализации сервера
 * \return Указатель на структуру сервера
 */
struct Server* InitServer();

/*!
 * \brief Функция открытия индивидуальной очереди для клиента
 * \param char* name - имя клиента
 * \param mqd_t* mqd_client - указатель на очередь клиента
 */
int OpenQueueToClient(char* name, mqd_t* mqd_client);

/*!
 * \brief Функция, отвечающая за отправку сообщения всем клиентам
 * \param struct ListClient* list - список клиентов
 * \param char* answer - сообщение, которое необходимо отправить
 */
void SendMsgToAllClients(struct ListClient* list, char* answer);

/*!
 * \brief Функция, отвечающая за отправку сообщения клиенту
 * \param mqd_t mqd_client - очередь клиента, по которой будет отправляться
 * сообщение \param char* answer - сообщение
 */
void SendMsgToClient(mqd_t mqd_client, char* answer);

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
 * соотвествующего потока. \param void* args - указатель на структуру сервера
 * \detail Функция ждёт, пока кто-то из пользователей не пришлёт сообщение.
 * Затем сервер рассылает данное сообщение всем пользователям, включая того
 * пользователя, который отправил сообщение.
 */
void* MessagingListener(void* args);

/*!
 * \brief Функция, прослушивающая запросы на выход. Используется во время
 * создания соотвествующего потока. \param void* args - указатель на структуру
 * сервера \detail Функция ждёт, пока кто-то из пользователей не пришлёт своё
 * имя. Затем сервер удаляет данного пользователя из списка клиентов и сообщает
 * всем оставшимся пользователям, что данный пользователь покинул чат.
 */
void* ExitListener(void* args);
