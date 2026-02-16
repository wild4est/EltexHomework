```bash
#Сборка библиотеки и проекта:
make all

#Запуск программы:
./main

#Очистка библиотеки и проекта:
make clean


#Сборка только библиотеки:
make compile_lib

#Очистка только библиотеки:
make clean_lib


#Сборка только проекта:
make compile_proj

#Очиста только проекта:
make clean_proj


#Сборка тестов:
make run_test

#Запуск тестов:
LD_LIBRARY_PATH=plugins ./run_test

#Очиста тестов:
make clean_test
