/* 
 * File:   main.cpp
 * Author: spas
 *
 * Created on 7 Октябрь 2011 г., 14:04
 */

#include <cstdlib>

using namespace std;

/*
 * Функция для выполнения загрузки данных из CSV файла.
 */
char** read_from_csv(char* file_url) {
    FILE* p_file;
    char** result;
    char* file_string;

    p_file = fopen(file_url, "r");
    if (p_file == NULL) {
        perror("Error opening file " + file_url);
    } else {
        if (fgets(file_string, 100)) {
            
        }
    }
    return null;
}

/*
 * Функция для записи результатов в CSV файлы.
 */
bool write_to_csv(char** data) {

    return false;
}

int main(int argc, char** argv) {
    // Загрузка данных из CSV файла

    /* Реализация алгоритма RELIEF
     * В том числе:
     *  1) Функция оценки расстояния между элементами.
     *  2) Функция поиска ближайшего элемента этого же класса.
     *  3) Функция поиска ближайшего элемента из другого класса.
     *  4) Функция вычисления изменения веса параметров.
     *** В результате выполняется отсеивание параметров с отрицательным весом.
     */

    // Вывод результата в CSV файл


    return 0;
}

