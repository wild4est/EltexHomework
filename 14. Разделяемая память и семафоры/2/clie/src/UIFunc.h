#pragma once

#include <errno.h>
#include <malloc.h>
#include <ncurses.h>
#include <stdio.h>

#include "ClientFunc.h"

#define START_X 1
#define START_Y 1

#define WIN_MSGS_H 12
#define WIN_MSGS_W 40

#define WIN_TEXT_H 10

#define WIN_CLIENTS_H 12
#define WIN_CLIENTS_W 40

/*!
 * \brief Структура рабочего простраства
 * \param WINDOW* win_msgs - окно со списком сообщений
 * \param WINDOW* win_text - окно для ввода сообщения
 * \param WINDOW* win_clients - окно со списком клиентов
 */
struct Workspace {
	WINDOW* win_msgs;
	WINDOW* win_text;
	WINDOW* win_clients;
};

/*!
 * \brief Функция, инициализирующая настройки окна ncurses
 */
void InitNcursesStuff();

/*!
 * \brief Функция, инициализующая рабочее простраство
 */
void InitWorkspace();

/*!
 * \brief Функция, обновляющая все окна
 */
void RefreshAllWin();

/*!
 * \brief Функция, выводящая содержимой списка в окно
 * \param WINDOW* win - окно, где будет выводится список
 * \param struct List* list - список
 */
void PrintList(WINDOW* win, struct List* list);

/*!
 * \brief Функция, прослушивающая сообщения на отправку. Используется при
 * создании соотвествующего потока. \param void* args - указатель на структуру
 * клиента \detail Функция ждёт пока пользователь не введёт вообщение в окне
 * win_text. После получения сообщения отправляет его серверу
 */
void* MessagingListener(void* args);

/*!
 * \brief Функция, прослушивающая сообщения от сервера. Используется при
 * создании соотвествующего потока. \param void* args - указатель на структуру
 * клиента \detail Функция ждёт пока не поуступит сообщение от сервера и
 * обрабатывает его.
 */
void* BroadcastListener(void* args);
