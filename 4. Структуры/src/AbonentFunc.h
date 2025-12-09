#include <stdio.h>
#include <string.h>

/*! Размер строчного буффера.
 * Применяется в структуре abonent.
 */
#define SIZE_CHAR_ARRAY 10

/*! Размер массива абонентов.
 * Применяется в структуре vector.
 */
#define SIZE_ABONENTS 100

/*!
 * \brief Структура абонента.
 * Отвечает за хранение информации об абоненте.
 * \param char name[SIZE_CHAR_ARRAY] - имя абонента.
 * \param char second_name[SIZE_CHAR_ARRAY] - фамилия абонента.
 * \param char tel[SIZE_CHAR_ARRAY] - телефон абонента.
 */
struct abonent {
	char name[SIZE_CHAR_ARRAY];
	char second_name[SIZE_CHAR_ARRAY];
	char tel[SIZE_CHAR_ARRAY];
};

/*!
 * \brief Структура вектора абонентов.
 * Отвечает за хранение массива абонентов и информации о заполненности массива.
 * \param int capacity - максимальное колличество элементов, которое может быть в массиве.
 * \param int current_size - колличество элементов, которое присуствует в массиве в данный момент врмени.
 * \param struct abonent abonents[SIZE_ABONENTS] - массив абонентов.
 */
struct vector {
	int capacity;
	int current_size;
	struct abonent abonents[SIZE_ABONENTS];
};

/*!
 * \brief Функция, отвечающая за вывод содержимого структуры abonent в консоль.
 * \param struct abonent *abo - указатель на структуру абонента.
 */
void PrintAbonent(struct abonent *abo);

/*!
 * \brief Функция, отвечающая за вывод всех элементов вектора в консоль.
 * \param struct vector *vec - указатель на структуру вектора.
 */
void PrintVector(struct vector *vec);

/*!
 * \brief Функция, отвечающая за вывод элементов вектора, находящихся в некотором диапозоне.
 * \param struct vecotr *vec - указатель на структуру вектор.
 * \param unsigned int L - левая граница.
 * \param unsigned int R - правая граница.
 * \details Выводит в консоль с L-ый по R-ый элемент массива abonents. 
 */
void PrintVectorInRange(struct vector *vec, unsigned int L, unsigned int R);

/*!
 * \brief Функция, отвечающая за заполнение переменных capacity и surrent_size в структуре vector.
 * \param struct vector *vec - указатель на структуру vector.
 * \warning Данную функцию рекомендуется всегда вызывать после создания перменной типа struct vector.
 */
void InitVector(struct vector *vec);

/*!
 * \brief Функция, отвечающая за добавление нового абонента в вектор.
 * \param struct vector *vec - указатель на структуру vector
 * \param char *aname - указатель на строчный массив (имя абонента)
 * \param char *asecond_name - указтель на строчный массив (фамилия абонента)
 * \param char *atel - указательный на строчный массив (телефон абонента)
 * \return Целочисленное значение, обозначающее успех операции. (0 - абонент добавился успешно, 1 - произошла ошибка).
 * \details Функция заполняет первый не пустой элемент массива abonents в структуре vector.
 * Первый не пустой элемент определяется при помощи переменной current_size, отражающей колличество имеющихся элементов
 * в массиве.
 */
int AddAbonent(struct vector *vec, char *aname, char *asecond_name, char *atel);

/*!
 * \brief Функция, отвечающая за удаление абонента из вектора по его индексу.
 * \param struct vector *vec - указатель на структуру vector
 * \param unsigned int id - индекс абонента
 * \return Целочисленное значение, обозначающее успех операции. (0 - абонент удалился успешно, 1 - произошла ошибка).
 * \details Начиная с элемента указанного по индексу и заканчивая предпоследним, i элемент перезаписывается
 * на i+1 элемент. После значение переменной current_size (которая отвечает за текущий размер массива) уменьшается
 * на единицу.
 */
int DeleteAbonentId(struct vector *vec, unsigned int id);

/*!
 * \brief Функция, отвечающая за поиск абонента по имени.
 * \param struct vector *vec - указатель на структуру vector
 * \param char *aname - указатель на строчный массив (имя абонента)
 * \return В случае успеха возвращается казатель на структуру абонента, в случае если искомый абонент отсуствует или
 * во время поиска произошли какие-то проблемы возвращается NULL.
 * \details Поиск осуществляется перебором.
 */
struct abonent* SearchAbonentName(struct vector *vec, char *aname);
