#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define SIZE_BUFF 255

/*!
 * \brief Структура вершины списка
 */
struct Node{
	char msg[SIZE_BUFF];
	struct Node* next;
};

/*!
 * \brief Структура списка
 */
struct List{
	int length;
	int max_length;
	struct Node* start;
};

/*!
 * \brief Функция инициализации списка
 * \param int max_length - максимальный размер списка
 * \return Указатель на инициализированный список
 */
struct List* InitList(int max_length);

/*!
 * \brief Функция, отвечающая за добавление новой вершины в список
 * \param struct List* list - указатель на список, куда будет добавлена вершина
 * \param char* msg - данные, которые будут записаны в список
 */
void AddNode(struct List* list, char* msg);

/*!
 * \brief Функция, отвечающая за удаление первой вершины в списке
 * \param struct List* list - список, из которого будет удалён первый элемент
 */
void DeleteFirstMsg(struct List* list);

/*!
 * \brief Функция, отвечающая за удаление списка
 * \param struct List* list - список, который будет удалён
 */
void FreeList(struct List* list);
//void DeleteNodeById(struct List* list, int id);
