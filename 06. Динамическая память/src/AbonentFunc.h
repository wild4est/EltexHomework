#include <stdio.h>
#include <string.h>
#include <malloc.h>

/*! Размер строчного буффера.
 * Применяется в структуре Abonent.
 */
#define SIZE_CHAR_ARRAY 10

/*! Начало отсчёта индификаторов абонентов.
 * Применяется при инициализации начала списка.
 */
#define START_ID 0

/*!
 * \brief Структура абонента.
 * Отвечает за хранение информации об абоненте.
 * \param char name[SIZE_CHAR_ARRAY] - имя абонента.
 * \param char second_name[SIZE_CHAR_ARRAY] - фамилия абонента.
 * \param char tel[SIZE_CHAR_ARRAY] - телефон абонента.
 */
struct Abonent {
	char name[SIZE_CHAR_ARRAY];
	char second_name[SIZE_CHAR_ARRAY];
	char tel[SIZE_CHAR_ARRAY];
};

/*!
 * \brief Узел для двусвязного списка абонентов.
 * \param struct Abonent data - структура абонента.
 * \param struct ListNode *prev - указатель на предыдущий узел.
 * \param struct ListNode *next - указатель на следующий узел.
 */
struct ListNode {
	int id;
	struct Abonent *data;
	struct ListNode *prev;
	struct ListNode *next;
};

/*!
 * \brief Двусвязный список абонентов.
 * \param struct ListNode *start - указатель на начало списка.
 * \param struct ListNode *end - указатель на конец списка. 
 */
struct List {
	int current_size;
	struct ListNode *start;
	struct ListNode *end;
};

/*!
 * \brief Функция, отвечающая за вывод содержимого структуры abonent в консоль.
 * \param struct Abonent *abo - указатель на структуру абонента.
 */
void PrintAbonent(struct Abonent *abo);

/*!
 * \brief Функция, отвечающая за вывод в консоль всех элементов списка, начиная с указанного узла.
 * \param struct ListNode* node - указатель на узел.
 */
void PrintListSince(struct ListNode* node);

/*!
 * \brief Функция, отвечающая за вывод в консоль определённого колличества элементов списка, начиная с указанного узла.
 * \param struct ListNode* node - указатель на структуру узла, с которой начнётся вывод.
 * \param unsigned int range - колличество элементов, которые нужно вывести. 
 */
void PrintListInRange(struct ListNode* node, unsigned int range);

/*!
 * \brief Функция, отвечающая за инициализацию списка.
 * \return Указатель на структуру List.
 * \warning Данную функцию рекомендуется всегда вызывать после создания перменной типа struct vector.
 */
struct List* InitList();

/* !
 * \brief Функция, отвечающая за инициализацию стартого узла списка.
 * \param struct Abonent* abonent - указатель на струкруту абонента, который будет находится первый в списке.
 * \return Указатель на структуру стартого узла.
 */
struct ListNode* InitStartOfList(struct Abonent* abonent);

/* !
 * \brief Функция, отвечающая за инициализацию последующих узлов списка
 * \param struct ListNode* cur_node - указатель на текущий узел.
 * \param struct Abonent* abonent - указатель на структуру абонента, которого необходимо добавить.
 * \return Указатель но новую структуру узла.
 */
struct ListNode* InitNextNode(struct ListNode* cur_node, struct Abonent* abonent);

/* !
 * \breif Функция, отвечающая за инициализацию структуры абонента.
 * \param char *aname - указатель на строчный массив (имя абонента)
 * \param char *asecond_name - указтель на строчный массив (фамилия абонента)
 * \param char *atel - указательный на строчный массив (телефон абонента)
 * \return Указатель на полученную структуру абонента.
 */
struct Abonent* InitAbonent(char* aname, char* asecond_name, char* atel);

/*!
 * \brief Функция, отвечающая за добавление нового абонента в список.
 * \param struct List* list - указатель на двусвязный список
 * \param struct Abonent* abonent - указатель на структуру абонента, которого необходимо добавить.
 * \return Целочисленное значение, обозначающее успех операции. (0 - абонент добавился успешно, 1 - произошла ошибка).
 * \details В данной функции обрабатывается две ситуации: добавление первого элемента и добавление последующего элемета.
 * В случае добавления первого элемента, в структуре списка задаётся указатель на стартовый узел и на конечный
 * узел, а так же задаётся размер, равный единице. В случае добавления последующего элемента к текущему размеру списка
 * добавляется единица, а так изменяется указатель на конец списка.
 */
int AddAbonent(struct List* list, struct Abonent* abonent);

/*!
 * \brief Функция, отвечающая за удаление узла из памяти.
 * \param struct ListNode* node - указатель на узел, память которого необходимо освободить.
 */
void FreeListNode(struct ListNode* node);

/*!
 * \brief Функция, отвечающая за удаление абонента из списка по его индексу.
 * \param struct vector *vec - указатель на структуру списка
 * \param unsigned int id - индекс абонента
 * \return Целочисленное значение, обозначающее успех операции. (0 - абонент удалился успешно, 1 - указатель на список
 * равен NULL, 2 - указанный индекс выходит за рамки имеющихся индексов).
 * \details Сначала через цикл while, перебором, находится узел с искомым индексом. Затем происходит ряд проверок и
 * дествий, в ходе которых искомый узел отсоединяется от списка. После узел удаляется.
 */
int DeleteAbonentId(struct List *list, unsigned int id);

/*!
 * \brief Функция, отвечающая за поиск абонента по имени.
 * \param struct List* list - указатель на структуру списка
 * \param char *aname - указатель на строчный массив (имя абонента)
 * \return В случае успеха возвращается казатель на структуру абонента, в случае если искомый абонент отсуствует или
 * во время поиска произошли какие-то проблемы возвращается NULL.
 * \details Поиск осуществляется перебором.
 */
struct Abonent* SearchAbonentName(struct List *list, char *aname);
