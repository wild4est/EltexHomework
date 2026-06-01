#pragma once

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../ServerStuff.h"
#include "List.h"

#define CLIENT_NAME "Cow"
#define CLIENT_MSG "Moo"

#define MSG_COUNT 10

/*!
 * \brief Структура клиента
 * \param char* name - имя клиента
 * \param mqd_t mqd_messaging - очередь для отправки сообщений сообщений
 * \param mqd_t mqd_personal - очередь для получения индивидуальных сообщений
 * \param mqd_t mqd_exit - очередь для отправки сообщений о выходе
 * \param struct List* list_clients - список клиентов
 * \param struct List* list_msgs - список сообщений
 */
struct Client {
	char* name;
	mqd_t mqd_messaging;
	mqd_t mqd_personal;
	mqd_t mqd_exit;
	struct List* list_clients;
	struct List* list_msgs;
};

/*!
 * \brief Функция, отвечающая за инициализацию очереди
 * \param mqd_t* mqd - указатель на очередь сообщений
 * \param const char* path - название очереди
 * \param long maxmsg - максимальное количество сообщений
 * \param long mssize - размер сообщения
 * \param oflag - флаги
 */
int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize,
	      int oflag);

/*!
 * \brief Функция, отвечающая за инициализанию структуру клиента
 * \param char* name - имя клиента
 * \return Указатель на структуру клиента
 */
struct Client* InitClient(char* name);

/*!
 * \brief Функция, отвечающая за отправку сообщения серверу
 * \param mqd_t mqd_messaging - очередь для оправки сообщения
 * \param char* name - имя клиента
 * \param char* msg - сообщение
 */
int SendMsgToServer(mqd_t mqd_messaging, char* name, char* msg);

/*!
 * \brief Функция, отвечающая за получение сообщений от сервера
 * \param mqd_t mqd_personal - очередь, по которой будут приниматься сообщения
 * \param struct List** list_msg - указатель на список сообщений
 * \param struct List** list_clients - указатель на список клиентов
 * \detail Если принятое сообщение начинается на '+', то клиент считает, что
 * получил имя только что присоединившего пользователя, и добавляет его в список
 * клиентов. Если принятое сообщение начинается на '-', то клиент считает, что
 * он получил имя пользователя, который покинул чат, и удаляет его из списка
 * клиентов. Во всех остальных случаях клиент считает, что он получил обычное
 * сообщение и записывает его в список сообщений.
 */
int GetMsgFromServer(mqd_t mqd_personal, struct List** list_msgs,
		     struct List** list_clients);

/*!
 * \brief Функция, отвечающая за получение данных о пользователях и сообщениях
 * от сервера \param struct Client* client - указатель на структуру клиента
 */
int GetDataFromServer(struct Client* client);
