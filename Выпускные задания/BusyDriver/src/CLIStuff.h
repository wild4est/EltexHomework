#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DriverManager.h"

#define GETSTR_SIZE_BUFF 255
#define WORDS_COUNT 5

#define COL_COM 32
#define COL_VAL 33
#define COL_BD 31

/*!
 * \brief Функция, заменяющая первый перенос на нулевой символ в строке
 * \param char* str - строка, подвергающаяся обработке
 * \detail Используется, чтобы убрать лишний перенос в сроке, принятой через fgets
 */
void DeleteEnter(char* str);

/*!
 * \brief Функция, проверяющая является ли полученная строка числом
 * \param char* str - строка, которая будет проверяться
 * \param size_t len - количество байтов в строке 
 * \return Число, обозначающее успех проверки. 1 - строка является числом. 0 - не является.
 */
int WordIsNumberCheck(char* str, size_t len);

/*!
 * \brief Функция, отвечающая за разделение строки на подстроки по символу
 * \param char*** words - указатель на массив строк, куда будут записаны подстроки
 * \param int* count - указатель на число, куда будет записано итоговое количество слов
 * \param char* str - строка, которая будет подвергаться разделению
 * \param const char* restrict sym - символ, по которому будет происходить разделение
 */
void Split(char*** words, int* count, char* str, const char* restrict sym);

/*!
 * \brief Функция, выводящая список команд в терминал
 */
void PrintCommands();

/*!
 * \brief Фнукция, выводящая приветсвие в терминал
 */
void PrintGreeting();

/*!
 * \brief Функция, выводящая машинку в терминал
 */
void PrintCar();

/*!
 * \brief Функция, запускающая основной цикл, где будет происходить ввод команд, их обработка и выполнение
 */
void MainCLI();
