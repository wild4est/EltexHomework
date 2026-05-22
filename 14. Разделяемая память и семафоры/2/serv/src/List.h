#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#include "../../ServerStuff.h"

/*!
 * \brief Структура вершины для списка сообщений
 * \param char msg[SIZE_BUFF_MSG] - сообщение
 * \param struct NodeMsg* next - указатель на следующую вершину
 */
struct NodeMsg{
	char msg[SIZE_BUFF_MSG];
	struct NodeMsg* next;
};

/*!
 * \brief Структура списка сообщений
 * \param int length - текущий размер списка
 * \param struct NodeMsg* start - указатель на начало списка
 */
struct ListMsg{
	int length;
	struct NodeMsg* start;
};

/*!
 * \brief Структура разделяемой памяти, эмулирующая поведение очереди, состоящей из одного сообщения
 * \param int shm - файловый дескриптор разделяемой памяти
 * \param void* ptr - указатель на область разделяемой памяти
 * \param sem_t* sem_read - семафор на чтение разделяемой памяти
 * \param sem_t* sem_write - семафор на запись разделяемой памяти
 */
struct SharedMemory{
	int shm;
	void* ptr;
	sem_t* sem_read;
	sem_t* sem_write;
};

/*!
 * \brief Структура вершины для списка клиентов
 * \param char name[SIZE_BUFF_NAME] - имя пользователя
 * \param struct SharedMemory shared_mem_per - структура разделяемой памяти
 * \param struct NodeClient* next - указатель на следующую вершину
 */
struct NodeClient{
	char name[SIZE_BUFF_NAME];
	struct SharedMemory shared_mem_per;
	struct NodeClient* next;
};

/*!
 * \brief Структура списка клиентов
 * \param int length - текущий размер списка
 * \param struct NodeClient* start - указатель на начало списка
 */
struct ListClient{
	int length;
	struct NodeClient* start;
};

/*!
 * \brief Функция инициализации структуры разделяемой памяти
 * \param struct SharedMemory* shared_mem - структура разделяемой памяти
 * \param int shm_fd - файловый дескриптор разделяемой памяти
 * \param void* ptr - указатель на область разделяемой памяти
 * \param sem_t* sem_read - семафор на чтение разделяемой памяти
 * \param sem_t* sem_write - семафор на запись разделяемой памяти
 */
void InitSharedMemory(struct SharedMemory* shared_mem, int shm_fd, void* ptr, sem_t* sem_read, sem_t* sem_write);

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
 * \brief Функция инициализации списка сообщений
 * \return Указатель на список сообщений
 */
struct ListMsg* InitListMsg();

/*!
 * \brief Функция добавления вершины в список сообщений
 * \param struct ListMsg* list_msg - указатель на список сообщений, куда будет добавлена вершина
 * \param char* msg - сообщение, которое необходимо добавить
 */
void AddNodeMsg(struct ListMsg* list_msg, char* msg);

/*!
 * \brief Функция удаления вершины из списка сообщений
 * \param struct ListMsg* list_msg - указатель на список сообщений, откуда будет удалена вершина
 * \param char* msg - сообщение, которое будет удалено
 */
void DeleteNodeMsg(struct ListMsg* list_msg, char* msg);

/*!
 * \brief Функция высвобождения списка сообщений
 * \param struct ListMsg* list_msg - список сообщений
 */
void FreeListMsg(struct ListMsg* list_msg);

/*!
 * \brief Функция инициализации списка клиентов
 * \return Указатель на список клиентов
 */
struct ListClient* InitListClient();

/*!
 * \brief Функция добавления новой вершины в список клиентов
 * \param struct ListClient* list_client - список клиентов, куда будет добавлена вершина
 * \param char* name - имя клиента
 * \param struct SharedMemory shared_mem - структура индивидуальной разделяемой памяти для клиента
 */
void AddNodeClient(struct ListClient* list_client, char* name, struct SharedMemory shared_mem);

/*!
 * \brief Функция удаления вершины из списка клиентов
 * \param struct ListClient* list_client - список клиетов, откуда будет удалена вершина
 * \param char* name - имя клиента, который будет удалён
 */
void DeleteNodeClient(struct ListClient* list_client, char* name);

/*!
 * \brief Функция высвобождения списка клиентов
 * \param struct ListClient* list_client - список клиентов
 */
void FreeListClient(struct ListClient* list_client);
