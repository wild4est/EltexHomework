#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "../../ServerStuff.h"

/*!
 * \brief Структура вершины списка
 * \param char data[SIZE_BUFF_MSG] - данные
 * \param struct Node* next - указатель на следующую структуру
 */
struct Node{
	char data[SIZE_BUFF_MSG];
	struct Node* next;
};

/*!
 * \brief Структура списка
 * \param int length - текущая длина списка
 * \param sturct Node* start - указатель на начало списка
 */
struct List{
	int length;
	struct Node* start;
};

/*!
 * \brief Функция, отвечающая за инициализацию списка
 * \raturn Указатель на инциализированую структуру списка
 */
struct List* InitList();

/*!
 * \brief Функция, отвечающая за добавление новой вершины
 * \param struct List* list - указатель на список, куда будет добавлена вершина
 * \param char* data - данные
 */
void AddNode(struct List* list, char* data);

/*!
 * \brief Функция, отвечающая за удаление вершины
 * \param struct List* list - указатель на список, откуда будет удалена вершина
 * \param char* data - данные, по которым будет искаться вершина 
 */
void DeleteNode(struct List* list, char* data);

/*!
 * \brief Функция, отвечающая за очищение списка
 * \param struct List* list - указатель на список, который необходимо очистить
 */
void FreeList(struct List* list);
