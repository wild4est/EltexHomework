#pragma once

#include <errno.h>
#include <fcntl.h>
#include <ncurses.h>
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

#define MSG_COUNT 10

/*!
 * \brief Структура разделяемой памяти, эмулирующая поведение очереди, состоящей
 * из одного сообщения \param int shm - файловый дескриптор разделяемой памяти
 * \param void* ptr - указатель на область разделяемой памяти
 * \param sem_t* sem_read - семафор на чтение разделяемой памяти
 * \param sem_t* sem_write - семафор на запись разделяемой памяти
 */
struct SharedMemory {
	int shm;
	void* ptr;
	sem_t* sem_read;
	sem_t* sem_write;
};

/*!
 * \brief Структура клиента
 * \param char* name - имя клиента
 * \param struct SharedMemory shared_mem_messaging - структура разделяемой
 * памяти для отправки сообщений \param struct SharedMemory shared_mem_personal
 * - структура разделяемой памяти для получения индивидуальных сообщений \param
 * struct SharedMemory shared_mem_exit - структура разделяемой памяти для
 * отправки сообщений о выходе \param struct List* list_clients - указатель на
 * список имён пользователей \param struct List* list_msg - указатель на список
 * последних сообщений
 */
struct Client {
	char* name;

	struct SharedMemory shared_mem_messaging;
	struct SharedMemory shared_mem_personal;
	struct SharedMemory shared_mem_exit;

	struct List* list_clients;
	struct List* list_msgs;
};

/*!
 * \brief Функция инициализации разделяемой памяти
 * \param int* shm_fd - указатель на файловый дескриптор
 * \param void** ptr - указатель на участок разделяемой памяти
 * \param const char* name - название раздляемой памяти
 * \param int oflag - флаги
 * \param size_t size - размер памяти
 */
int InitShm(int* shm_fd, void** ptr, const char* name, int oflag, size_t size);

/*!
 * \brief Функция закрытия разделяемой памяти
 * \param struct SharedMemory* shared_mem - структура разделяемой памяти
 */

void CloseSharedMemory(struct SharedMemory* shared_mem);

/*!
 * \brief Функция удаление разделяемой памяти
 * \param char* name_shm - имя разделяемой памяти, которую необходимо удалить
 * \param char* name_sem_rd - имя семафора на чтение, который необходимо удалить
 * \param char* name_sem_wr - имя семафора на запись, который необходимо удалить
 */
void UnlinkSharedMemory(char* name_shm, char* name_sem_rd, char* name_sem_wr);

/*!
 * \brief Функция инициализации структуры клиента
 * \param char* name - имя клиента
 */
struct Client* InitClient(char* name);

/*!
 * \brief Функция отправки сообщения серверу
 * \param struct SharedMemory* shared_mem - указатель на структуру разделяемой
 * памяти, по которой необходимо отправить сообщение \param char* name - имя
 * клиета \param char* msg - сообщение
 */
int SendMsgToServer(struct SharedMemory* shared_mem, char* name, char* msg);

/*!
 * \brief Функция принятия сообщения от сервера
 * \param struct SharedMemory* shared_mem - указатель на структуру разделяемой
 * памяти, по которой необходиом получить сообщение \param struct List**
 * list_msg - список сообщений \param struct List** list_clients - список
 * клиентов \detail Если принятое сообщение начинается на '+', то клиент
 * считает, что получил имя только что присоединившего пользователя, и добавляет
 * его в список клиентов. Если принятое сообщение начинается на '-', то клиент
 * считает, что он получил имя пользователя, который покинул чат, и удаляет его
 * из списка клиентов. Во всех остальных случаях клиент считает, что он получил
 * обычное сообщение и записывает его в список сообщений.
 */
int GetMsgFromServer(struct SharedMemory* shared_mem, struct List** list_msgs,
		     struct List** list_clients);

/*!
 * \brief Функция принятия данных от сервера
 * \param struct Client* client - структура клиента
 * \detail В данной функции клиент получает данные от сервера о других клиентах
 * и уже имеющихся сообщениях на сервере
 */
int GetDataFromServer(struct Client* client);
