#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <malloc.h>
#include <errno.h>

#include "List.h"
#include "ClientFunc.h"

/*!
 * \brief Структура рабочего простраства
 */
struct Workspace{
	WINDOW* win_msg;
	WINDOW* win_text;
};

/*!
 * \brief Функция, отвечающая за настройку ncurses
 */
void InitNcursesStuff();

/*!
 * \brief Функция, отвечающая за инициализацию рабочего простраства и отображения всех окон
 * \param struct Workspace* worpspace - рабочее пространство
 * \param int h - высота рабочего пространства
 * \param int w - ширина рабочего пространства
 * \param int y - расположение рабочего пространства по координате y
 * \param int x - расположение рабочего пространства по координате x
 */
void InitWorkspace(struct Workspace* workspace, int h, int w, int y, int x);

/*!
 * \brief Функция, отвечающая за выведение содержимого списка в окне
 * \param WINDOW* win - окно, в котором будет выведен список
 * \param struct List* list_msg - список, содержимое которого будет выводиться
 */
void PrintMsgs(WINDOW* win, struct List* list_msg);

/*!
 * \brief Функция, отвечающая за обноваление окон в рабочем пространстве
 * \param struct Workpace* workspace - рабочее простраство, окна которого будут обновлены
 */
void RefreshAllWin(struct Workspace* workspace);

/*!
 * \brief Функция, которая запускает процесс принятия и отправки сообщений
 * \param struct Client* client - указатель на структуру клиента
 * \param struct Workspace* workspace - указатель на структуру рабочего простраства
 */
int StartWorkspace(struct Client* client, struct Workspace* workspace);
