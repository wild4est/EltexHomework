#include <malloc.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

#include "../../ServerStuff.h"

/*!
 * \brief Структура вершины для списка сообщений
 * \param char msg[SIZE_BUFF_MSG] - сообщение
 * \param struct NodeMsg* next - указатель на следующую вершину
 */
struct NodeMsg {
	char msg[SIZE_BUFF_MSG];
	struct NodeMsg* next;
};

/*!
 * \brief Структура списка сообщений
 * \param int length - размер списка
 * \param struct NodeMsg* start - начало списка
 */
struct ListMsg {
	int length;
	struct NodeMsg* start;
};

/*!
 * \brief Структура вершины списка клиентов
 * \param char name[SIZE_BUFF_NAME] - имя клиента
 * \param mqd_t mqd_personal - индивидуальная очередь для клиента
 * \param struct NodeClient* next - указатель на следующую вершину
 */
struct NodeClient {
	char name[SIZE_BUFF_NAME];
	mqd_t mqd_personal;
	struct NodeClient* next;
};

/*!
 * \brief Структура списка клиентов
 * \param int length - текущий резмер списка
 * \param strcut NodeClient* start - указатель на начало списка
 */
struct ListClient {
	int length;
	struct NodeClient* start;
};

/*!
 * \brief Функция, отвечающая за иницализацию списка сообщений
 * \return Указатель на список сообщений
 */
struct ListMsg* InitListMsg();

/*!
 * \brief Функция, отвечающая за добавление новой вершины в список сообщений
 * \param struct ListMsg* list_msg - указатель на список сообщейни, куда будет
 * добавлена вершина
 * \param char* msg - сообщение
 */
void AddNodeMsg(struct ListMsg* list_msg, char* msg);

/*!
 * \brief Функция, отвечающая за удаление вершины из списка сообщений
 * \param struct ListMsg* list_msg - указатель на список сообщений, откуда будет
 * удалено сообщение
 * \param char* msg - сообщение
 */
void DeleteNodeMsg(struct ListMsg* list_msg, char* msg);

/*!
 * \brief Функция, отвечающая за удаление списка сообщений
 * \param struct ListMsg* list_msg - указатель на список сообщений, который
 * необходимо удалить
 */
void FreeListMsg(struct ListMsg* list_msg);

/*!
 * \brief Функция, отвечающая за инициализацию списка клиентов
 * \return Указатель на спмсок клиентов
 */
struct ListClient* InitListClient();

/*!
 * \brief Функция, отвечающая за добавление вершины в список клиентов
 * \param struct ListClient* listclient - указатель на список клиентов, куда
 * будет добавлена вершина
 * \param char* name - имя клиента
 * \param mqd_t mqd - индивидуальная очередь сообщений
 */
void AddNodeClient(struct ListClient* list_client, char* name, mqd_t mqd);

/*!
 * \brief Функция, отвечающая за удаление вершины из списка клиентов
 * \param struct ListCLient list_client - список клиентов, откуда будет удалена
 * вершина
 * \param char* name - имя клиента
 */
void DeleteNodeClient(struct ListClient* list_client, char* name);

/*!
 * \brief Функция, отвечающая за удаление списка клиентов
 * \param struct ListClient* list_client - список клиентов, который необходимо
 * удалить
 */
void FreeListClient(struct ListClient* list_client);
