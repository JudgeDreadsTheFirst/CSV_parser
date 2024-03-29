# CSV parser
---
## Компоненты репозитория
В репозитории находятся все необходимые для компиляции проекта файлы, а так же папка **test** с подготовленными мной файлами для тестирования разных сценариев.
Далее будут представлены указания к компиляции проекта.

---
## Компиляция проекта
### Windows 
Сначала необходимо получить объектные файлы
```
	g++ -c table.cpp
	g++ -c csv_processing.cpp
	g++ -c main.cpp
```
После необходимо ввести команду
```
        g++ main.o csv_processing.o table.o -o yadro_test.exe
```        
После этого можно призводить запуск исполняемого файла в формате `$programm_name.exe file_name.csv`.  
Так же можно создать простой файл `CMakeLists.txt`.
Добавить в него строки.
```
        cmake_minimum_required(VERSION 3.16)
        project(yadro_test LANGUAGES CXX)

        set(CMAKE_CXX_STANDARD 17)

        add_executable(yadro_test main.cpp csv_processing.cpp table.cpp)
``` 
Далее использовать команду.
```
	cmake --build
```
### Linux
Для компиляции программы на линукс можно производить те же самые дествия. Только необходимо учитывать что исполняемый файл не будет иметь расщирения `.exe`.
Так же можно упростить компиляцию использовав команду(применимо и в windows).
```
    g++ *.cpp -o yadro_test
```
И далее произвести запуск `$./programm_name.out file_name.csv`
