#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*!
 * \brief Структура вершины дерева процессов.
 * \param pid_t pid - pid текущего процесса
 * \param struct NodeProcess* Left - укзаатель на структуру левой вершины.
 * \param struct NodeProcess* Right - укзаатель на структуру правой вершины.
 */
struct NodeProcess{
	pid_t pid;
	struct NodeProcess* Left;
	struct NodeProcess* Right;
};

/*!
 * \brief Функция, отвечающая за инициализацию вершины.
 * \return Указатель на вершину дерева.
 * \details Во время инициализации зануляет указатели на левую и правую вершину.
 * pid оставляет пустым, так как он заполняется автоматически при запуске процессов в функции void StartFork().
 */
struct NodeProcess* InitNodeProcess();

/*!
 * \brief Функция, отвечающая за освобождение памяти дерева.
 * Рекурсивно высвобождает память, выделенную под каждую вершину дерева.
 * \param struct NodeProcess* np - указатель на вершину дерева.
 */
void FreeNodeProcess(struct NodeProcess* np);

/*!
 * \brief Функция, отвечающая за запуск процессов в соответствии с указаным деревом.
 * \param struct NodeProcess* np - указатель на вершину дерева.
 * \detail Функция рекурсивно проходится по дереву и порождает новые процессы. Все процессы выводят свой текущий
 * pid и ppid. В случае если процесс является дочерним, то он рекурсивно вызывает функцию void StartFork()
 * для соотвествуюзей ветки дерева, а затем завершает работу. В случае если процесс является родительским, то он
 * ждёт завершения дочернего процесса и выводит соотвествующее сообщение.
 */
void StartFork(struct NodeProcess* np);

