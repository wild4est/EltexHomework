#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RawIPV4.h"

/*!
 * \brief Структура клиента. Содержит в себе общую информацию о клиенте
 */
struct Client {
	struct sockaddr_in addr;
	int msg_count;
};

/*!
 * \brief Структура вершины списка
 */
struct Node {
	struct Client client;
	struct Node* next;
};

/*!
 * \brief Структура списка
 */
struct List {
	int length;
	struct Node* start;
};

/*!
 * \brief Функция инициализации списка
 * \return Указатель на инициализированный список
 */
struct List* InitList();

/*!
 * \brief Функция инициализации клиента
 * \param struct Client* client - указатель на структуру клиента, которая будет
 * инициализирована
 * \param struct sockaddr_in* addr - указатель на структуру
 * адресса клиента
 */
void InitClient(struct Client* client, struct sockaddr_in* addr);

/*!
 * \brief Функция, отвечающая за подсчёт размера списка
 * \param struct List* list - указатель на список
 * \return Количество вершин в списке
 */
int CountLength(struct List* list);

/*!
 * \brief Функция, отвечающая за добавление новой вершины в список
 * \param struct List* list - указатель на список, куда будет добавлена вершина
 * \param struct Client* client - указатель на вершину клиента, которая будет
 * добавлена в список
 */
void AddNode(struct List* list, struct Client* client);

/*!
 * \brief Функция, отвечающая за поиск клиента по адресу
 * \param struct List* list - указатель на список, где будет производится поиск
 * \param struct sockaddr_in* new_addr - указатель адрес клиента, по которому
 * его будут искать
 * \return В случае, если клиент был найден вернётся указатель
 * на его структуру. В ином случае возвращается NULL
 */
struct Client* SearchClient(struct List* list, struct sockaddr_in* new_addr);

/*!
 * \brief Функция, отвечающая за удаление вершины по адресу клиента
 * \param struct List* list - указатель на список, где будет удалена вершина
 * \param struct sockaddr_in* addr - указатель на адрес клиента
 */
void DeleteNodeByAddr(struct List* list, struct sockaddr_in* addr);

/*!
 * \brief Функция удаления списка
 * \param struct List* list - указатель на список, который необходимо очистить
 */
void FreeList(struct List* list);
// void DeleteNodeById(struct List* list, int id);
