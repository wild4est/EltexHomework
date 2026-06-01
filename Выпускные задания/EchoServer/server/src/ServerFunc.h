#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#include "ClientList.h"
#include "RawIPV4.h"

#define EXIT_CODE "exit"
#define SIZE_BUFF 245

/*!
 * \brief Структура сервера
 */
struct Server{
	struct List* list_clients;
	struct sockaddr_in addr;	
	int fd;
	uint32_t ip_addr;
};

/*!
 * \brief Функция, отвечающая за выведение лого
 */
void PrintLogo();

/*!
 * \brief Функция инициализации сервера
 * \param struct Server* server - указатель на струтуру сервера, которая будет инициализирована
 * \param int port - порт сервера
 * \param const char* ip_addr - сетевой адрес сервера
 */
int InitServer(struct Server* server, int port, const char* ip_addr);


/*!
 * \brief Функция, отвечающая за получение сообщения
 * \param struct Server* server - указатель на структуру сервера
 * \param char* msg - строка, куда будет возвращено полученное сообщение
 * \param struct sockaddr_in* addr - адресс, по которому будет полученно сообщение
 * \return Число, обозначающее успех операции. -1 - произошла ошибка. 0 - всё прошло успешно. 1 - от сервера было
 * получено сообщение о завершении работы.
 */
int GetMsg(struct Server* server, char* msg, struct sockaddr_in* addr);

/*!
 * \brief Функция, отвечающая за отправку сообщения
 * \param struct Client* client - указатель на стрктуру клиента, которому будет отпралвено сообщение
 * \param int fd_server - файловый дескриптор сервера
 * \param uint16_t server_port - порт сервера
 * \param uint32_t server_ip_addr - сетевой адрес сервера
 * \param char* buff - сообщение, которое будет отправлено
 * \return Число, обозначающее успех операции. -1 - произошла ошибка. 1 - всё прошло успешно.
 * \detail Во время отправки к концу сообщения добвляется количество сообщений, которые клиент отправил
 */
int SendMsg(struct Client* client, int fd_server, uint16_t server_port, uint32_t server_ip_addr, char* buff);

/*!
 * \brief Функция, отвечающая за отправку кода
 * \param struct Client* client - указатель на стрктуру клиента, которому будет отпралвено сообщение
 * \param int fd_server - файловый дескриптор сервера
 * \param uint16_t server_port - порт сервера
 * \param uint32_t server_ip_addr - сетевой адрес сервера
 * \param char* code - код, который будет отправлен
 * \return Число, обозначающее успех операции. -1 - произошла ошибка. 1 - всё прошло успешно.
 */
int SendCode(struct Client* client, int fd_server, uint16_t server_port, uint32_t server_ip_addr, char* code);

/*!
 * \brief Функция удаления структуры сервера
 * \param struct Server* server - указатель на структуру сервера, которая будет удалена
 */
void FreeServer(struct Server* server);

/*!
 * \brief Функция, запускающая процесс прослушивания и отправки сообщений
 * \param struct Server* server - указатель на структуру сервера
 */
void StartListeningMsg(struct Server* server);
