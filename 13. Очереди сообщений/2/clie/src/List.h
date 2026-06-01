#pragma once

#include <malloc.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

#include "../../ServerStuff.h"

/*!
 * \brief Структура вершины списка
 * \param char data[SIZE_BUFF_MSG] - данные
 * \param struct Node* next - указатель на следующую вершину
 */
struct Node {
	char data[SIZE_BUFF_MSG];
	struct Node* next;
};

/*!
 * \brief Структура списка
 * \param int length - текущий размер списка
 * \param struct Node* start - указатель на начало списка
 */
struct List {
	int length;
	struct Node* start;
};

/*!
 * \brief Функция, отвечающая за инициализацию списка
 * \return Указатель на структуру списка
 */
struct List* InitList();

/*!
 * \brief Функция, отвечающая за добавление новой вершины в список
 * \param struct List* list - указатель на список, куда будет добавлена новая
 * вершина
 * \param char* data - данные для новой вершины
 */
void AddNode(struct List* list, char* data);

/*!
 * \brief Функция, отвчающая за удаление вершины из списка
 * \param struct List* list - указатель на список, откуда будет удалена вершина
 * \param char* data - данные, по которым будет искаться удаляемая вершина
 */
void DeleteNode(struct List* list, char* data);

/*!
 * \brief Функция, отвечающая за очищение списка
 * \param struct List* list - указатель на список, который необходимо очистить
 */
void FreeList(struct List* list);
