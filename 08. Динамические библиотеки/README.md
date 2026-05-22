```bash
#Сборка библиотеки и проекта:
make all

#Запуск программы:
LD_LIBRARY_PATH="$PWD/lib" ./main

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
./run_test

#Очиста тестов:
make clean_test
