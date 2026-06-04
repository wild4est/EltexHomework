#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE_BUFF 10

/*!
 * \brief Скруктура пары каналов
 * \param pipe_from_child[2] - канал, где сообщения будут отправляться от дочернего процесса родительскому
 * \param pipe_from_parant[2] - канал, где сообщения будут отправться от родительского процесса дочернему
 */
struct PairOfPipes {
	int pipe_from_child[2];
	int pipe_from_parent[2];
};

/*!
 * \brief Функция инициализация пары каналов
 * \param struct PairOfPipes* pair_of_pipes - указатель на пару каналов, которая будет инициализирована
 * \return Успех инициализации. 0 - всё прошло успешно. 1 - произошла ошибка.
 */
int InitPairOfPipes(struct PairOfPipes* pair_of_pipes);

/*!
 * \brief Функция закрытия файловых дескрипторов со стороны дочернего процесса
 * \param struct PairOfPipes* pair_of_pipes - указатель на пару каналов дочернего процесса
 * \return Успех инициализации. 0 - всё прошло успешно. 1 - произошла ошибка.
 */
int ClosePairForChild(struct PairOfPipes* pair_of_pipes);

/*!
 * \brief Функция закрытия файловых дескрипторов со стороны родительского процесса
 * \param struct PairOfPipes* pair_of_pipes - указатель на пару каналов родительского процесса
 * \return Успех инициализации. 0 - всё прошло успешно. 1 - произошла ошибка.
 */
int ClosePairForParent(struct PairOfPipes* pair_of_pipes);

/*!
 * \brief Функция, принимающая сообщение по каналу
 * \int pipefd[2] - канал, по которому будет приниматься сообщение
 * \char* msg - указатель на строку, куда будет записано сообщение
 * \return Успех инициализации. 0 - всё прошло успешно. 1 - произошла ошибка.
 */
int GetMsg(int pipefd[2], char* msg);

/*!
* \brief Функция, отправляющая сообщение по каналу
* \int pipefd[2] - канал, по которому будет отправлено сообщение
* \char* msg - указатель на строку, которая будет отправлено в качестве сообщения
* \return Успех инициализации. 0 - всё прошло успешно. 1 - произошла ошибка.
*/
int SendMsg(int pipefd[2], char* msg);
