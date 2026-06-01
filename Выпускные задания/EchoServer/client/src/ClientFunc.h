#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "List.h"
#include "RawIPV4.h"

#define EXIT_CODE "exit"
#define SIZE_MSG_SEND 245

/*!
 * \brief Структура клиента. Здесь хранится всё, связанное с backend
 * составляющей.
 */
struct Client {
	struct List* list_msg;
	int fd;
	struct sockaddr_in addr;
	uint16_t port;
	uint32_t ip_addr;
};

/*!
 * \brief Функция, отвечающая за инфициализацию структуры клиента
 * \param struct Client* client - указатель на структуру клиента, которая будет
 * инициализирована \param int port_client - порт клиента \param int port_server
 * - порт сервера \param const char* ip_addr_client - ipv4 адрес клиента \param
 * const char* ip_addr_server - ipv4 адрес сервера \return Число, обозначающее
 * успех операции. 0 - всё прошло успешно. -1 - возникла ошибка.
 */
int InitClient(struct Client* client, int port_client, int port_server,
	       const char* ip_addr_client, const char* ip_addr_server);

/*!
 * \brief Функция отправки сообщений
 * \param struct Client* client - указатель на структуру клиента
 * \param char* msg - сообщение, которое необходимо отправить
 * \return Число, обозначающее успех операции. 0 - всё прошло успешно. -1 -
 * возникла ошибка.
 */
int SendMsg(struct Client* client, char* msg);

/*!
 * \brief Функция принятие сообщений
 * \param struct Client* client - указатель на структуру клиента
 * \return Число, обозначающее успех операции. 0 - всё прошло успешно. -1 -
 * возникла ошибка. 1 - сервер прислал сообщение о завершении работы. \detail
 * Полученное сообщение будет добавлено в список сообщений (sturct List*
 * list_msg) в структуре клиента
 */
int GetMsg(struct Client* client);

/*!
 * \brief Функция, проверяющая, что полученное сообщение является кодом выхода.
 * \param char* msg - сообщение, которое будет проверяться
 * \return Число, обозначающее успех проверки. 0 - не является. 1 - является.
 */
int IsItExitMsg(char* msg);

/*!
 * \brief Функция, отвечающая за удаление структуры клиента
 * \param struct Client* client - указатель на структуру клиента, которая будет
 * удалена
 */
void FreeClient(struct Client* client);
